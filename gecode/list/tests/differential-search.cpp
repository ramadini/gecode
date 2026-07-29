#include <gecode/list.hh>
#include <gecode/search.hh>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace {

using Gecode::BoolVar;
using Gecode::IntVar;
using Gecode::ListVar;
using Gecode::Space;

Gecode::List::Domain binary_lists(
    unsigned int minimum,
    unsigned int maximum) {
  return Gecode::List::Domain::repeat(
      Gecode::List::ValueSet(1, 2),
      minimum,
      maximum);
}

std::string encode_list(const std::vector<int>& value) {
  std::ostringstream output;
  output << '[';
  for (std::size_t index = 0;
       index < value.size();
       ++index) {
    if (index != 0)
      output << ',';
    output << value[index];
  }
  output << ']';
  return output.str();
}

struct SolutionRow {
  std::string test_case;
  std::string solution;
};

struct StatisticsRow {
  std::string test_case;
  unsigned long int solutions;
  unsigned long int nodes;
  unsigned long int failures;
  unsigned long int depth;
};

class EqualitySpace final : public Space {
 public:
  ListVar x;
  ListVar y;

  EqualitySpace()
      : Space(),
        x(*this, binary_lists(0, 2)),
        y(*this, binary_lists(0, 2)) {
    Gecode::rel(*this, x, Gecode::IRT_EQ, y);
    Gecode::ListVarArgs variables(2);
    variables[0] = x;
    variables[1] = y;
    Gecode::branch_exact(*this, variables);
  }

  EqualitySpace(EqualitySpace& other)
      : Space(other),
        x(),
        y() {
    x.update(*this, other.x);
    y.update(*this, other.y);
  }

  Space* copy() override {
    return new EqualitySpace(*this);
  }
};

class DisequalitySpace final : public Space {
 public:
  ListVar x;
  ListVar y;

  DisequalitySpace()
      : Space(),
        x(*this, binary_lists(1, 1)),
        y(*this, binary_lists(1, 1)) {
    Gecode::rel(*this, x, Gecode::IRT_NQ, y);
    Gecode::ListVarArgs variables(2);
    variables[0] = x;
    variables[1] = y;
    Gecode::branch_exact(*this, variables);
  }

  DisequalitySpace(DisequalitySpace& other)
      : Space(other),
        x(),
        y() {
    x.update(*this, other.x);
    y.update(*this, other.y);
  }

  Space* copy() override {
    return new DisequalitySpace(*this);
  }
};

class ConcatSpace final : public Space {
 public:
  ListVar x;
  ListVar y;
  ListVar z;

  ConcatSpace()
      : Space(),
        x(*this, binary_lists(0, 1)),
        y(*this, binary_lists(0, 1)),
        z(*this, binary_lists(0, 2)) {
    Gecode::concat(*this, x, y, z);
    Gecode::ListVarArgs variables(3);
    variables[0] = x;
    variables[1] = y;
    variables[2] = z;
    Gecode::branch_exact(*this, variables);
  }

  ConcatSpace(ConcatSpace& other)
      : Space(other),
        x(),
        y(),
        z() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    z.update(*this, other.z);
  }

  Space* copy() override {
    return new ConcatSpace(*this);
  }
};

class LengthSpace final : public Space {
 public:
  ListVar x;
  IntVar n;

  LengthSpace()
      : Space(),
        x(*this, binary_lists(0, 2)),
        n(*this, 1, 1) {
    Gecode::length(*this, x, n);
    Gecode::branch_exact(*this, x);
  }

  LengthSpace(LengthSpace& other)
      : Space(other),
        x(),
        n() {
    x.update(*this, other.x);
    n.update(*this, other.n);
  }

  Space* copy() override {
    return new LengthSpace(*this);
  }
};

class ReifiedEqualitySpace final : public Space {
 public:
  ListVar x;
  ListVar y;
  BoolVar b;

  ReifiedEqualitySpace()
      : Space(),
        x(*this, binary_lists(1, 1)),
        y(*this, binary_lists(1, 1)),
        b(*this, 0, 1) {
    Gecode::rel(
        *this,
        x,
        Gecode::IRT_EQ,
        y,
        Gecode::Reify(b, Gecode::RM_EQV));
    Gecode::ListVarArgs variables(2);
    variables[0] = x;
    variables[1] = y;
    Gecode::branch_exact(*this, variables);
    Gecode::branch(*this, b, Gecode::BOOL_VAL_MIN());
  }

  ReifiedEqualitySpace(ReifiedEqualitySpace& other)
      : Space(other),
        x(),
        y(),
        b() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    b.update(*this, other.b);
  }

  Space* copy() override {
    return new ReifiedEqualitySpace(*this);
  }
};

template<class Model, class Encoder>
void collect(
    const std::string& identifier,
    Model* root,
    Encoder encoder,
    std::vector<SolutionRow>& solutions,
    std::vector<StatisticsRow>& statistics) {
  Gecode::Search::Options options;
  options.c_d = 4;
  options.a_d = 2;

  Gecode::DFS<Model> search(root, options);
  delete root;
  unsigned long int count = 0;

  while (Model* solution = search.next()) {
    ++count;
    solutions.push_back(
        SolutionRow{identifier, encoder(*solution)});
    delete solution;
  }

  const Gecode::Search::Statistics measured =
      search.statistics();

  statistics.push_back(
      StatisticsRow{
          identifier,
          count,
          measured.node,
          measured.fail,
          measured.depth});
}

void write_solutions(
    std::vector<SolutionRow> rows,
    std::ostream& output) {
  std::sort(
      rows.begin(),
      rows.end(),
      [](const SolutionRow& left,
         const SolutionRow& right) {
        return std::tie(left.test_case, left.solution) <
            std::tie(right.test_case, right.solution);
      });

  assert(
      std::adjacent_find(
          rows.begin(),
          rows.end(),
          [](const SolutionRow& left,
             const SolutionRow& right) {
            return left.test_case == right.test_case &&
                left.solution == right.solution;
          }) == rows.end());

  output << "case\tsolution\n";
  for (const SolutionRow& row : rows) {
    output << row.test_case << '\t'
           << row.solution << '\n';
  }
}

void write_statistics(
    std::vector<StatisticsRow> rows,
    std::ostream& output) {
  std::sort(
      rows.begin(),
      rows.end(),
      [](const StatisticsRow& left,
         const StatisticsRow& right) {
        return left.test_case < right.test_case;
      });

  output << "case\tsolutions\tnodes\tfailures\tdepth\n";
  for (const StatisticsRow& row : rows) {
    output << row.test_case << '\t'
           << row.solutions << '\t'
           << row.nodes << '\t'
           << row.failures << '\t'
           << row.depth << '\n';
  }
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 5 ||
      std::string(argv[1]) != "--report" ||
      std::string(argv[3]) != "--stats") {
    std::cerr << "usage: " << argv[0]
              << " --report PATH --stats PATH\n";
    return 2;
  }

  std::vector<SolutionRow> solutions;
  std::vector<StatisticsRow> statistics;

  collect(
      "search.eq.optional_binary",
      new EqualitySpace(),
      [](const EqualitySpace& solution) {
        return "x=" + encode_list(solution.x.val()) +
            ";y=" + encode_list(solution.y.val());
      },
      solutions,
      statistics);

  collect(
      "search.neq.binary_singleton",
      new DisequalitySpace(),
      [](const DisequalitySpace& solution) {
        return "x=" + encode_list(solution.x.val()) +
            ";y=" + encode_list(solution.y.val());
      },
      solutions,
      statistics);

  collect(
      "search.concat.optional_binary",
      new ConcatSpace(),
      [](const ConcatSpace& solution) {
        return "x=" + encode_list(solution.x.val()) +
            ";y=" + encode_list(solution.y.val()) +
            ";z=" + encode_list(solution.z.val());
      },
      solutions,
      statistics);

  collect(
      "search.length.binary_exact_one",
      new LengthSpace(),
      [](const LengthSpace& solution) {
        assert(solution.n.assigned());
        return "x=" + encode_list(solution.x.val()) +
            ";n=" + std::to_string(solution.n.val());
      },
      solutions,
      statistics);

  collect(
      "search.reified_eq.binary_singleton",
      new ReifiedEqualitySpace(),
      [](const ReifiedEqualitySpace& solution) {
        assert(solution.b.assigned());
        return "x=" + encode_list(solution.x.val()) +
            ";y=" + encode_list(solution.y.val()) +
            ";b=" + std::to_string(solution.b.val());
      },
      solutions,
      statistics);

  std::ofstream report(argv[2]);
  std::ofstream stats(argv[4]);
  if (!report.good() || !stats.good()) {
    std::cerr << "could not open differential output files\n";
    return 1;
  }

  write_solutions(std::move(solutions), report);
  write_statistics(std::move(statistics), stats);

  if (!report.good() || !stats.good()) {
    std::cerr << "could not write differential output files\n";
    return 1;
  }

  std::cerr << "Native List search differential cases passed\n";
  return 0;
}
