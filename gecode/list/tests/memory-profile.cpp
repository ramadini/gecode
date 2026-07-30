#include <gecode/list.hh>
#include <gecode/search.hh>

#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <string>
#include <sys/resource.h>
#include <utility>
#include <variant>
#include <vector>

#if defined(__GLIBC__)
#include <malloc.h>
#endif

namespace {

using Gecode::IntVar;
using Gecode::ListVar;
using Gecode::Space;

struct Options {
  std::string scenario;
  std::string profile = "custom";
  std::string report;
  int batches = 5;
  int iterations = 100;
  long max_growth_kb = 32768;
  double max_growth_percent = 25.0;
};

long parse_long(const char* text, const char* option) {
  errno = 0;
  char* end = nullptr;
  const long value = std::strtol(text, &end, 10);
  if (errno != 0 || end == text || *end != '\0' || value <= 0)
    throw std::invalid_argument(std::string("invalid value for ") + option);
  return value;
}

double parse_double(const char* text, const char* option) {
  errno = 0;
  char* end = nullptr;
  const double value = std::strtod(text, &end);
  if (errno != 0 || end == text || *end != '\0' || value <= 0.0)
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
    } else if (argument == "--batches") {
      options.batches = static_cast<int>(
          parse_long(require_value("--batches"), "--batches"));
    } else if (argument == "--iterations") {
      options.iterations = static_cast<int>(
          parse_long(require_value("--iterations"), "--iterations"));
    } else if (argument == "--max-growth-kb") {
      options.max_growth_kb =
          parse_long(require_value("--max-growth-kb"), "--max-growth-kb");
    } else if (argument == "--max-growth-percent") {
      options.max_growth_percent = parse_double(
          require_value("--max-growth-percent"),
          "--max-growth-percent");
    } else if (argument == "--help" || argument == "-h") {
      std::cout
          << "Usage: list-memory-profile --scenario spaces|clones|dfs "
             "[--profile NAME] [--report PATH] [--batches N] "
             "[--iterations N] [--max-growth-kb N] "
             "[--max-growth-percent P]\n";
      std::exit(0);
    } else {
      throw std::invalid_argument("unknown argument: " + argument);
    }
  }

  if (options.scenario != "spaces" &&
      options.scenario != "clones" &&
      options.scenario != "dfs") {
    throw std::invalid_argument(
        "--scenario must be spaces, clones, or dfs");
  }
  if (options.batches < 3)
    throw std::invalid_argument("--batches must be at least 3");
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

class LifecycleSpace final : public Space {
 public:
  ListVar x;
  ListVar y;
  ListVar z;
  IntVar length;

  LifecycleSpace()
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

  LifecycleSpace(LifecycleSpace& other)
      : Space(other), x(), y(), z(), length() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    z.update(*this, other.z);
    length.update(*this, other.length);
  }

  Space* copy() override { return new LifecycleSpace(*this); }
};

class LiteralCloneSpace final : public Space {
 public:
  ListVar x;

  LiteralCloneSpace()
      : Space(), x(*this, Gecode::List::Domain::fixed(
                            patterned_values(16384))) {}

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

long current_rss_kb() {
  std::ifstream status("/proc/self/status");
  std::string key;
  while (status >> key) {
    if (key == "VmRSS:") {
      long value = 0;
      std::string unit;
      status >> value >> unit;
      return value;
    }
    status.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return -1;
}

long peak_rss_kb() {
  struct rusage usage {};
  if (getrusage(RUSAGE_SELF, &usage) != 0)
    return -1;
  return usage.ru_maxrss;
}

void trim_heap() {
#if defined(__GLIBC__)
  malloc_trim(0);
#endif
}

struct WorkResult {
  long operations = 0;
  long solutions = 0;
};

WorkResult run_spaces(int iterations) {
  for (int iteration = 0; iteration < iterations; ++iteration) {
    auto* space = new LifecycleSpace();
    const Gecode::SpaceStatus status = space->status();
    assert(status != Gecode::SS_FAILED);
    delete space;
  }
  return {iterations, 0};
}

WorkResult run_clones(int iterations) {
  auto* root = new LiteralCloneSpace();
  assert(root->status() == Gecode::SS_SOLVED);
  for (int iteration = 0; iteration < iterations; ++iteration) {
    auto* clone = static_cast<LiteralCloneSpace*>(root->clone());
    assert(clone->status() == Gecode::SS_SOLVED);
    delete clone;
  }
  delete root;
  return {iterations, 0};
}

WorkResult run_dfs(int iterations) {
  long solutions = 0;
  for (int iteration = 0; iteration < iterations; ++iteration) {
    Gecode::Search::Options options;
    options.c_d = 1;
    auto* root = new SearchSpace();
    Gecode::DFS<SearchSpace> search(root, options);
    delete root;

    int iteration_solutions = 0;
    while (SearchSpace* solution = search.next()) {
      assert(solution->x.assigned());
      ++iteration_solutions;
      delete solution;
    }
    assert(iteration_solutions == 40);
    solutions += iteration_solutions;
  }
  return {iterations, solutions};
}

WorkResult run_batch(const std::string& scenario, int iterations) {
  if (scenario == "spaces")
    return run_spaces(iterations);
  if (scenario == "clones")
    return run_clones(iterations);
  return run_dfs(iterations);
}

double mean(const std::vector<long>& values, std::size_t begin,
            std::size_t end) {
  const long sum = std::accumulate(
      values.begin() + static_cast<std::ptrdiff_t>(begin),
      values.begin() + static_cast<std::ptrdiff_t>(end), 0L);
  return static_cast<double>(sum) / static_cast<double>(end - begin);
}

void append_report(const Options& options, long operations, long solutions,
                   long start_rss, long end_rss, long growth_kb,
                   long peak_rss, long growth_limit_kb, bool passed) {
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
    report << "scenario\tprofile\tbatches\titerations_per_batch\toperations"
              "\tsolutions\tstart_rss_kb\tend_rss_kb\tsteady_growth_kb"
              "\tpeak_rss_kb\tgrowth_limit_kb\tresult\n";
  }
  report << options.scenario << '\t' << options.profile << '\t'
         << options.batches << '\t' << options.iterations << '\t'
         << operations << '\t' << solutions << '\t' << start_rss << '\t'
         << end_rss << '\t' << growth_kb << '\t' << peak_rss << '\t'
         << growth_limit_kb << '\t' << (passed ? "PASS" : "FAIL") << '\n';
}

int run(const Options& options) {
  // Two unmeasured batches absorb one-time Gecode and allocator initialization.
  run_batch(options.scenario, options.iterations);
  run_batch(options.scenario, options.iterations);
  trim_heap();

  std::vector<long> samples;
  samples.reserve(static_cast<std::size_t>(options.batches));
  long operations = 0;
  long solutions = 0;
  for (int batch = 0; batch < options.batches; ++batch) {
    const WorkResult result = run_batch(options.scenario, options.iterations);
    operations += result.operations;
    solutions += result.solutions;
    trim_heap();
    const long rss = current_rss_kb();
    if (rss < 0)
      throw std::runtime_error("could not read VmRSS from /proc/self/status");
    samples.push_back(rss);
  }

  const std::size_t window = std::max<std::size_t>(1, samples.size() / 3);
  const double first_mean = mean(samples, 0, window);
  const double last_mean = mean(samples, samples.size() - window, samples.size());
  const long growth_kb = static_cast<long>(
      std::max(0.0, last_mean - first_mean));
  const long percent_limit = static_cast<long>(
      first_mean * options.max_growth_percent / 100.0);
  const long growth_limit_kb =
      std::max(options.max_growth_kb, percent_limit);
  const bool passed = growth_kb <= growth_limit_kb;

  append_report(options, operations, solutions, samples.front(),
                samples.back(), growth_kb, peak_rss_kb(), growth_limit_kb,
                passed);

  std::cout << std::left << std::setw(7) << options.scenario
            << " operations=" << operations
            << " solutions=" << solutions
            << " rss_start_kb=" << samples.front()
            << " rss_end_kb=" << samples.back()
            << " steady_growth_kb=" << growth_kb
            << " limit_kb=" << growth_limit_kb
            << " peak_rss_kb=" << peak_rss_kb()
            << " result=" << (passed ? "PASS" : "FAIL") << '\n';
  return passed ? 0 : 1;
}

}  // namespace

int main(int argc, char* argv[]) {
  try {
    return run(parse_options(argc, argv));
  } catch (const std::exception& error) {
    std::cerr << "list-memory-profile: " << error.what() << '\n';
    return 2;
  }
}
