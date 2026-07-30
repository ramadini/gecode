#include <gecode/list.hh>
#include <gecode/search.hh>

#include <cassert>
#include <cstddef>
#include <iostream>
#include <initializer_list>
#include <utility>
#include <variant>
#include <vector>

namespace {

using Gecode::BoolVar;
using Gecode::IntVar;
using Gecode::ListVar;
using Gecode::Space;

Gecode::List::Domain fixed(std::initializer_list<int> values) {
  return Gecode::List::Domain::fixed(std::vector<int>(values));
}

Gecode::List::Domain lists(
    int lower,
    int upper,
    unsigned int min_length,
    unsigned int max_length) {
  return Gecode::List::Domain::top(
      Gecode::List::ValueSet(lower, upper),
      min_length,
      max_length);
}

std::vector<int> patterned_values(std::size_t size) {
  std::vector<int> values(size);
  for (std::size_t index = 0; index < size; ++index) {
    values[index] =
        static_cast<int>((index * 31U) % 127U) - 63;
  }
  return values;
}

class LiteralCloneSpace final : public Space {
 public:
  ListVar x;

  explicit LiteralCloneSpace(std::vector<int> values)
      : Space(),
        x(*this, Gecode::List::Domain::fixed(std::move(values))) {}

  LiteralCloneSpace(LiteralCloneSpace& other)
      : Space(other),
        x() {
    x.update(*this, other.x);
  }

  Space* copy() override {
    return new LiteralCloneSpace(*this);
  }
};

class EqualityFailureSpace final : public Space {
 public:
  ListVar x;
  ListVar y;

  EqualityFailureSpace()
      : Space(),
        x(*this, fixed({1})),
        y(*this, fixed({2})) {
    Gecode::rel(*this, x, Gecode::IRT_EQ, y);
  }

  EqualityFailureSpace(EqualityFailureSpace& other)
      : Space(other),
        x(),
        y() {
    x.update(*this, other.x);
    y.update(*this, other.y);
  }

  Space* copy() override {
    return new EqualityFailureSpace(*this);
  }
};

class ConcatFailureSpace final : public Space {
 public:
  ListVar x;
  ListVar y;
  ListVar z;

  ConcatFailureSpace()
      : Space(),
        x(*this, fixed({1})),
        y(*this, fixed({3})),
        z(*this, fixed({1, 2})) {
    Gecode::concat(*this, x, y, z);
  }

  ConcatFailureSpace(ConcatFailureSpace& other)
      : Space(other),
        x(),
        y(),
        z() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    z.update(*this, other.z);
  }

  Space* copy() override {
    return new ConcatFailureSpace(*this);
  }
};

class LengthFailureSpace final : public Space {
 public:
  ListVar x;
  IntVar n;

  LengthFailureSpace()
      : Space(),
        x(*this, lists(0, 2, 0, 4)),
        n(*this, 5, 5) {
    Gecode::length(*this, x, n);
  }

  LengthFailureSpace(LengthFailureSpace& other)
      : Space(other),
        x(),
        n() {
    x.update(*this, other.x);
    n.update(*this, other.n);
  }

  Space* copy() override {
    return new LengthFailureSpace(*this);
  }
};

class ReifiedFailureSpace final : public Space {
 public:
  ListVar x;
  ListVar y;
  BoolVar truth;

  ReifiedFailureSpace()
      : Space(),
        x(*this, fixed({7})),
        y(*this, fixed({8})),
        truth(*this, 1, 1) {
    Gecode::rel(
        *this,
        x,
        Gecode::IRT_EQ,
        y,
        Gecode::Reify(truth, Gecode::RM_EQV));
  }

  ReifiedFailureSpace(ReifiedFailureSpace& other)
      : Space(other),
        x(),
        y(),
        truth() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    truth.update(*this, other.truth);
  }

  Space* copy() override {
    return new ReifiedFailureSpace(*this);
  }
};

class SubsumptionSpace final : public Space {
 public:
  ListVar x;
  ListVar y;
  ListVar z;
  IntVar n;
  BoolVar truth;

  SubsumptionSpace()
      : Space(),
        x(*this, fixed({4})),
        y(*this, fixed({4})),
        z(*this, fixed({4, 4})),
        n(*this, 1, 1),
        truth(*this, 1, 1) {
    Gecode::rel(*this, x, Gecode::IRT_EQ, y);
    Gecode::concat(*this, x, y, z);
    Gecode::length(*this, x, n);
    Gecode::rel(
        *this,
        x,
        Gecode::IRT_EQ,
        y,
        Gecode::Reify(truth, Gecode::RM_EQV));
  }

  SubsumptionSpace(SubsumptionSpace& other)
      : Space(other),
        x(),
        y(),
        z(),
        n(),
        truth() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    z.update(*this, other.z);
    n.update(*this, other.n);
    truth.update(*this, other.truth);
  }

  Space* copy() override {
    return new SubsumptionSpace(*this);
  }
};

class SearchSpace final : public Space {
 public:
  ListVar x;

  SearchSpace()
      : Space(),
        x(
            *this,
            Gecode::List::Domain::repeat(
                Gecode::List::ValueSet(-1, 1),
                0,
                3)) {
    Gecode::branch_exact(*this, x);
  }

  SearchSpace(SearchSpace& other)
      : Space(other),
        x() {
    x.update(*this, other.x);
  }

  Space* copy() override {
    return new SearchSpace(*this);
  }
};

class NGLSpace final : public Space {
 public:
  ListVar x;
  Gecode::NGL* literal;

  NGLSpace()
      : Space(),
        x(
            *this,
            Gecode::List::Domain::repeat(
                Gecode::List::ValueSet(-2, 2),
                1,
                1)),
        literal(nullptr) {
    Gecode::branch_exact(*this, x);
  }

  NGLSpace(NGLSpace& other)
      : Space(other),
        x(),
        literal(nullptr) {
    x.update(*this, other.x);
    if (other.literal != nullptr)
      literal = other.literal->copy(*this);
  }

  Space* copy() override {
    return new NGLSpace(*this);
  }

  void capture(unsigned int alternative) {
    assert(literal == nullptr);
    assert(status() == Gecode::SS_BRANCH);
    const Gecode::Choice* selected = choice();
    literal = ngl(*selected, alternative);
    delete selected;
    assert(literal != nullptr);
  }
};

const Gecode::List::LiteralSlice& assigned_literal(
    const ListVar& variable) {
  assert(variable.assigned());
  assert(variable.domain().segment_count() == 1);
  return std::get<Gecode::List::LiteralSegment>(
             variable.domain().segments().front())
      .literal;
}

void test_shared_literal_lifetime_across_space_clones() {
  constexpr std::size_t literal_size = 50000;
  constexpr int clone_count = 128;

  auto* root =
      new LiteralCloneSpace(patterned_values(literal_size));
  const void* storage_id =
      assigned_literal(root->x).storage_id();
  assert(storage_id != nullptr);

  std::vector<LiteralCloneSpace*> clones;
  clones.reserve(clone_count);
  for (int index = 0; index < clone_count; ++index) {
    auto* clone = static_cast<LiteralCloneSpace*>(root->clone());
    assert(assigned_literal(clone->x).storage_id() == storage_id);
    clones.push_back(clone);
  }

  delete root;

  for (LiteralCloneSpace* clone : clones) {
    const auto span = assigned_literal(clone->x).span();
    assert(span.size() == literal_size);
    assert(span[0] == -63);
    assert(span[literal_size - 1U] ==
           static_cast<int>(((literal_size - 1U) * 31U) % 127U) - 63);
    assert(assigned_literal(clone->x).storage_id() == storage_id);
    delete clone;
  }
}

template<class FailureSpace>
void exercise_failed_spaces(int iterations) {
  for (int iteration = 0; iteration < iterations; ++iteration) {
    auto* failed = new FailureSpace();
    assert(failed->status() == Gecode::SS_FAILED);
    delete failed;
  }
}

void test_failed_propagation_cleanup() {
  constexpr int iterations = 500;
  exercise_failed_spaces<EqualityFailureSpace>(iterations);
  exercise_failed_spaces<ConcatFailureSpace>(iterations);
  exercise_failed_spaces<LengthFailureSpace>(iterations);
  exercise_failed_spaces<ReifiedFailureSpace>(iterations);
}

void test_subsumption_and_clone_cleanup() {
  for (int iteration = 0; iteration < 750; ++iteration) {
    auto* root = new SubsumptionSpace();
    assert(root->status() == Gecode::SS_SOLVED);

    auto* clone = static_cast<SubsumptionSpace*>(root->clone());
    assert(clone->status() == Gecode::SS_SOLVED);

    delete root;
    delete clone;
  }
}

void test_dfs_recomputation_lifecycle() {
  for (int iteration = 0; iteration < 40; ++iteration) {
    Gecode::Search::Options options;
    options.c_d = 1;

    auto* root = new SearchSpace();
    Gecode::DFS<SearchSpace> search(root, options);
    delete root;
    int solutions = 0;

    while (SearchSpace* solution = search.next()) {
      assert(solution->x.assigned());
      ++solutions;
      delete solution;
    }

    // Sum_{length=0..3} 3^length.
    assert(solutions == 40);
  }
}

void test_brancher_and_ngl_clone_cleanup() {
  for (int iteration = 0; iteration < 750; ++iteration) {
    auto* root = new NGLSpace();
    root->capture(static_cast<unsigned int>(iteration % 2));

    assert(root->literal->status(*root) == Gecode::NGL::NONE);

    auto* clone = static_cast<NGLSpace*>(root->clone());
    assert(clone->literal != nullptr);
    assert(clone->literal->status(*clone) == Gecode::NGL::NONE);
    assert(clone->literal->prune(*clone) == Gecode::ES_OK);
    assert(clone->status() != Gecode::SS_FAILED);
    assert(clone->literal->status(*clone) == Gecode::NGL::FAILED);

    delete root;
    delete clone;
  }
}

}  // namespace

int main() {
  test_shared_literal_lifetime_across_space_clones();
  test_failed_propagation_cleanup();
  test_subsumption_and_clone_cleanup();
  test_dfs_recomputation_lifecycle();
  test_brancher_and_ngl_clone_cleanup();

  std::cout << "List native sanitizer lifecycle tests passed\n";
  return 0;
}
