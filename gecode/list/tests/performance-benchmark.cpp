#include <gecode/list.hh>
#include <gecode/search.hh>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;
using Gecode::IntVar;
using Gecode::ListVar;
using Gecode::Space;

volatile std::uint64_t benchmark_sink = 0;

struct Options {
  std::string scenario;
  std::string profile = "custom";
  std::string report;
  int samples = 5;
  int iterations = 100;
};

long parse_positive(const char* text, const char* option) {
  char* end = nullptr;
  const long value = std::strtol(text, &end, 10);
  if (end == text || *end != '\0' || value <= 0)
    throw std::invalid_argument(std::string("invalid value for ") + option);
  return value;
}

Options parse_options(int argc, char* argv[]) {
  Options options;
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    auto require_value = [&](const char* option) -> const char* {
      if (++index >= argc)
        throw std::invalid_argument(std::string("missing value for ") + option);
      return argv[index];
    };

    if (argument == "--scenario") {
      options.scenario = require_value("--scenario");
    } else if (argument == "--profile") {
      options.profile = require_value("--profile");
    } else if (argument == "--report") {
      options.report = require_value("--report");
    } else if (argument == "--samples") {
      options.samples = static_cast<int>(
          parse_positive(require_value("--samples"), "--samples"));
    } else if (argument == "--iterations") {
      options.iterations = static_cast<int>(
          parse_positive(require_value("--iterations"), "--iterations"));
    } else if (argument == "--help" || argument == "-h") {
      std::cout
          << "Usage: list-performance-benchmark "
             "--scenario propagate|clone|dfs [--profile NAME] "
             "[--report PATH] [--samples N] [--iterations N]\n";
      std::exit(0);
    } else {
      throw std::invalid_argument("unknown argument: " + argument);
    }
  }

  if (options.scenario != "propagate" && options.scenario != "clone" &&
      options.scenario != "dfs") {
    throw std::invalid_argument(
        "--scenario must be propagate, clone, or dfs");
  }
  if (options.samples < 3)
    throw std::invalid_argument("--samples must be at least 3");
  return options;
}

Gecode::List::Domain fixed(std::initializer_list<int> values) {
  return Gecode::List::Domain::fixed(std::vector<int>(values));
}

std::vector<int> patterned_values(std::size_t size) {
  std::vector<int> values(size);
  for (std::size_t index = 0; index < size; ++index)
    values[index] = static_cast<int>((index * 31U) % 127U) - 63;
  return values;
}

class PropagationSpace final : public Space {
 public:
  ListVar x;
  ListVar y;
  ListVar z;
  IntVar length;

  PropagationSpace()
      : Space(),
        x(*this, Gecode::List::Domain::repeat(
                     Gecode::List::ValueSet(-2, 2), 0, 4)),
        y(*this, fixed({3, 4})),
        z(*this, Gecode::List::Domain::repeat(
                     Gecode::List::ValueSet(-2, 4), 2, 6)),
        length(*this, 2, 6) {
    Gecode::concat(*this, x, y, z);
    Gecode::length(*this, z, length);
    Gecode::rel(*this, x, Gecode::IRT_NQ, y);
  }

  PropagationSpace(PropagationSpace& other)
      : Space(other), x(), y(), z(), length() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    z.update(*this, other.z);
    length.update(*this, other.length);
  }

  Space* copy() override { return new PropagationSpace(*this); }
};

class LiteralCloneSpace final : public Space {
 public:
  ListVar x;

  LiteralCloneSpace()
      : Space(),
        x(*this, Gecode::List::Domain::fixed(patterned_values(16384))) {}

  LiteralCloneSpace(LiteralCloneSpace& other)
      : Space(other), x() {
    x.update(*this, other.x);
  }

  Space* copy() override { return new LiteralCloneSpace(*this); }
};

class SearchSpace final : public Space {
 public:
  ListVar x;

  SearchSpace()
      : Space(),
        x(*this, Gecode::List::Domain::repeat(
                     Gecode::List::ValueSet(-1, 1), 0, 3)) {
    Gecode::branch_exact(*this, x);
  }

  SearchSpace(SearchSpace& other)
      : Space(other), x() {
    x.update(*this, other.x);
  }

  Space* copy() override { return new SearchSpace(*this); }
};

struct WorkResult {
  std::uint64_t operations = 0;
  std::uint64_t solutions = 0;
  std::uint64_t checksum = 0;
};

WorkResult run_propagate(int iterations) {
  WorkResult result;
  for (int iteration = 0; iteration < iterations; ++iteration) {
    auto* space = new PropagationSpace();
    const Gecode::SpaceStatus status = space->status();
    if (status == Gecode::SS_FAILED) {
      delete space;
      throw std::runtime_error("propagation benchmark space failed");
    }
    result.checksum += static_cast<std::uint64_t>(status) +
                       static_cast<std::uint64_t>(iteration + 1);
    ++result.operations;
    delete space;
  }
  return result;
}

WorkResult run_clone(int iterations) {
  WorkResult result;
  auto* root = new LiteralCloneSpace();
  if (root->status() != Gecode::SS_SOLVED) {
    delete root;
    throw std::runtime_error("clone benchmark root was not solved");
  }
  for (int iteration = 0; iteration < iterations; ++iteration) {
    auto* clone = static_cast<LiteralCloneSpace*>(root->clone());
    if (clone->status() != Gecode::SS_SOLVED) {
      delete clone;
      delete root;
      throw std::runtime_error("clone benchmark clone was not solved");
    }
    result.checksum += static_cast<std::uint64_t>(clone->x.assigned()) +
                       static_cast<std::uint64_t>(iteration + 1);
    ++result.operations;
    delete clone;
  }
  delete root;
  return result;
}

WorkResult run_dfs(int iterations) {
  WorkResult result;
  for (int iteration = 0; iteration < iterations; ++iteration) {
    Gecode::Search::Options options;
    options.c_d = 1;
    auto* root = new SearchSpace();
    Gecode::DFS<SearchSpace> search(root, options);
    delete root;

    int iteration_solutions = 0;
    while (SearchSpace* solution = search.next()) {
      if (!solution->x.assigned()) {
        delete solution;
        throw std::runtime_error("DFS returned an unassigned solution");
      }
      ++iteration_solutions;
      delete solution;
    }
    if (iteration_solutions != 40)
      throw std::runtime_error("DFS benchmark expected 40 solutions");
    ++result.operations;
    result.solutions += static_cast<std::uint64_t>(iteration_solutions);
    result.checksum += static_cast<std::uint64_t>(iteration_solutions) *
                       static_cast<std::uint64_t>(iteration + 1);
  }
  return result;
}

WorkResult run_work(const std::string& scenario, int iterations) {
  if (scenario == "propagate")
    return run_propagate(iterations);
  if (scenario == "clone")
    return run_clone(iterations);
  return run_dfs(iterations);
}

std::uint64_t percentile(const std::vector<std::uint64_t>& sorted,
                         double fraction) {
  const double rank = std::ceil(fraction * static_cast<double>(sorted.size()));
  const std::size_t index = static_cast<std::size_t>(
      std::max(1.0, rank) - 1.0);
  return sorted[std::min(index, sorted.size() - 1)];
}

void append_report(const Options& options,
                   const std::vector<std::uint64_t>& ns_per_operation,
                   const WorkResult& totals) {
  if (options.report.empty())
    return;

  bool write_header = true;
  {
    std::ifstream existing(options.report, std::ios::binary | std::ios::ate);
    write_header = !existing || existing.tellg() == 0;
  }

  std::ofstream report(options.report, std::ios::app);
  if (!report)
    throw std::runtime_error("cannot open report: " + options.report);
  if (write_header) {
    report << "scenario\tprofile\tsamples\titerations_per_sample"
              "\toperations\tsolutions\tmedian_ns_per_op\tp95_ns_per_op"
              "\tmin_ns_per_op\tmax_ns_per_op\tchecksum\tresult\n";
  }

  std::vector<std::uint64_t> sorted = ns_per_operation;
  std::sort(sorted.begin(), sorted.end());
  report << options.scenario << '\t' << options.profile << '\t'
         << options.samples << '\t' << options.iterations << '\t'
         << totals.operations << '\t' << totals.solutions << '\t'
         << percentile(sorted, 0.50) << '\t' << percentile(sorted, 0.95)
         << '\t' << sorted.front() << '\t' << sorted.back() << '\t'
         << totals.checksum << "\tPASS\n";
}

int run(const Options& options) {
  // Warm-up is deliberately outside the measured samples.
  benchmark_sink ^= run_work(options.scenario, options.iterations).checksum;
  benchmark_sink ^= run_work(options.scenario, options.iterations).checksum;

  std::vector<std::uint64_t> ns_per_operation;
  ns_per_operation.reserve(static_cast<std::size_t>(options.samples));
  WorkResult totals;

  for (int sample = 0; sample < options.samples; ++sample) {
    const auto start = Clock::now();
    const WorkResult result = run_work(options.scenario, options.iterations);
    const auto stop = Clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
        stop - start).count();
    if (result.operations == 0)
      throw std::runtime_error("benchmark sample performed no operations");

    const std::uint64_t per_operation = static_cast<std::uint64_t>(
        std::max<std::int64_t>(1, elapsed) /
        static_cast<std::int64_t>(result.operations));
    ns_per_operation.push_back(per_operation);
    totals.operations += result.operations;
    totals.solutions += result.solutions;
    totals.checksum ^= result.checksum + static_cast<std::uint64_t>(sample + 1);
    benchmark_sink ^= result.checksum;
  }

  if (options.scenario == "dfs" &&
      totals.solutions != totals.operations * 40U) {
    throw std::runtime_error("DFS aggregate solution count changed");
  }

  std::vector<std::uint64_t> sorted = ns_per_operation;
  std::sort(sorted.begin(), sorted.end());
  append_report(options, ns_per_operation, totals);

  std::cout << std::left << std::setw(10) << options.scenario
            << " samples=" << options.samples
            << " iterations=" << options.iterations
            << " operations=" << totals.operations
            << " solutions=" << totals.solutions
            << " median_ns_per_op=" << percentile(sorted, 0.50)
            << " p95_ns_per_op=" << percentile(sorted, 0.95)
            << " checksum=" << totals.checksum
            << " result=PASS\n";
  return 0;
}

}  // namespace

int main(int argc, char* argv[]) {
  try {
    return run(parse_options(argc, argv));
  } catch (const std::exception& error) {
    std::cerr << "list-performance-benchmark: " << error.what() << '\n';
    return 2;
  }
}
