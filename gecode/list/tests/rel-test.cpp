#include <gecode/list.hh>
#include <gecode/search.hh>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

using Gecode::BoolVar;
using Gecode::IntVar;
using Gecode::ListVar;
using Gecode::Space;

Gecode::List::Domain fixed(std::initializer_list<int> values) {
  return Gecode::List::Domain::fixed(std::vector<int>(values));
}

Gecode::List::Domain lists(int lo, int hi,
                     unsigned int min_length,
                     unsigned int max_length) {
  return Gecode::List::Domain::top(
      Gecode::List::ValueSet(lo, hi),
      min_length,
      max_length);
}


class ListArraySpace final : public Space {
 public:
  Gecode::ListVarArray x;

  explicit ListArraySpace(bool construct_through_arguments)
      : Space(),
        x() {
    const Gecode::List::Domain initial =
        lists(-20, 20, 1, 4);

    if (construct_through_arguments) {
      Gecode::ListVarArgs arguments(
          *this,
          3,
          initial);
      x = Gecode::ListVarArray(
          *this,
          arguments);
    } else {
      x = Gecode::ListVarArray(
          *this,
          3,
          initial);
    }
  }

  ListArraySpace(ListArraySpace& other)
      : Space(other),
        x() {
    x.update(*this, other.x);
  }

  Space* copy() override {
    return new ListArraySpace(*this);
  }
};


class LengthBranchSpace final : public Space {
 public:
  ListVar x;

  LengthBranchSpace(
      Gecode::List::Domain domain,
      Gecode::IntValBranch values)
      : Space(),
        x(*this, std::move(domain)) {
    Gecode::branch_length(
        *this,
        x,
        values);
  }

  LengthBranchSpace(
      LengthBranchSpace& other)
      : Space(other),
        x() {
    x.update(*this, other.x);
  }

  Space* copy() override {
    return new LengthBranchSpace(*this);
  }
};


class LengthArrayBranchSpace final : public Space {
 public:
  Gecode::ListVarArray x;

  LengthArrayBranchSpace()
      : Space(),
        x(
            *this,
            2,
            lists(-1, 1, 0, 1)) {
    Gecode::ListVarArgs variables(x);
    Gecode::branch_length(
        *this,
        variables,
        Gecode::INT_VAR_SIZE_MIN(),
        Gecode::INT_VAL_MIN());
  }

  LengthArrayBranchSpace(
      LengthArrayBranchSpace& other)
      : Space(other),
        x() {
    x.update(*this, other.x);
  }

  Space* copy() override {
    return new LengthArrayBranchSpace(*this);
  }
};


class ExactDomainBranchSpace final : public Space {
 public:
  ListVar x;

  explicit ExactDomainBranchSpace(
      Gecode::List::Domain domain)
      : Space(),
        x(*this, std::move(domain)) {
    Gecode::branch_exact(*this, x);
  }

  ExactDomainBranchSpace(
      ExactDomainBranchSpace& other)
      : Space(other),
        x() {
    x.update(*this, other.x);
  }

  Space* copy() override {
    return new ExactDomainBranchSpace(*this);
  }
};


class ExactDomainNGLSpace final : public Space {
 public:
  ListVar x;
  Gecode::NGL* literal;

  explicit ExactDomainNGLSpace(
      Gecode::List::Domain domain)
      : Space(),
        x(*this, std::move(domain)),
        literal(nullptr) {
    Gecode::branch_exact(*this, x);
  }

  ExactDomainNGLSpace(
      ExactDomainNGLSpace& other)
      : Space(other),
        x(),
        literal(nullptr) {
    x.update(*this, other.x);

    if (other.literal != nullptr) {
      literal = other.literal->copy(*this);
    }
  }

  Space* copy() override {
    return new ExactDomainNGLSpace(*this);
  }

  void capture_literal(unsigned int alternative) {
    assert(literal == nullptr);
    assert(status() == Gecode::SS_BRANCH);

    const Gecode::Choice* selected = choice();
    literal = ngl(*selected, alternative);
    delete selected;

    assert(literal != nullptr);
  }

  void commit_current(unsigned int alternative) {
    assert(status() == Gecode::SS_BRANCH);
    const Gecode::Choice* selected = choice();
    commit(*selected, alternative);
    delete selected;
  }
};


class ExactDomainArrayBranchSpace final : public Space {
 public:
  Gecode::ListVarArray x;

  ExactDomainArrayBranchSpace()
      : Space(),
        x(
            *this,
            2,
            Gecode::List::Domain::repeat(
                Gecode::List::ValueSet(-1, 1),
                1,
                1)) {
    Gecode::ListVarArgs variables(x);
    Gecode::branch_exact(*this, variables);
  }

  ExactDomainArrayBranchSpace(
      ExactDomainArrayBranchSpace& other)
      : Space(other),
        x() {
    x.update(*this, other.x);
  }

  Space* copy() override {
    return new ExactDomainArrayBranchSpace(*this);
  }
};


class EqualitySpace final : public Space {
public:
  ListVar x;
  ListVar y;

  EqualitySpace(Gecode::List::Domain dx, Gecode::List::Domain dy)
      : Space(),
        x(*this, std::move(dx)),
        y(*this, std::move(dy)) {
    Gecode::rel(*this, x, Gecode::IRT_EQ, y);
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

  DisequalitySpace(Gecode::List::Domain dx, Gecode::List::Domain dy)
      : Space(),
        x(*this, std::move(dx)),
        y(*this, std::move(dy)) {
    Gecode::rel(*this, x, Gecode::IRT_NQ, y);
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


class ReifiedSpace final : public Space {
public:
  ListVar x;
  ListVar y;
  BoolVar b;

  ReifiedSpace(
      Gecode::List::Domain dx,
      Gecode::List::Domain dy,
      int b_min = 0,
      int b_max = 1,
      Gecode::IntRelType relation =
          Gecode::IRT_EQ,
      Gecode::ReifyMode mode =
          Gecode::RM_EQV)
      : Space(),
        x(*this, std::move(dx)),
        y(*this, std::move(dy)),
        b(*this, b_min, b_max) {
    Gecode::rel(
        *this,
        x,
        relation,
        y,
        Gecode::Reify(
            b,
            mode));
  }

  ReifiedSpace(ReifiedSpace& other)
      : Space(other),
        x(),
        y(),
        b() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    b.update(*this, other.b);
  }

  Space* copy() override {
    return new ReifiedSpace(*this);
  }
};


class AliasReifiedSpace final : public Space {
public:
  ListVar x;
  BoolVar b;

  AliasReifiedSpace(
      Gecode::IntRelType relation,
      Gecode::ReifyMode mode)
      : Space(),
        x(*this, lists(-10, 10, 0, 4)),
        b(*this, 0, 1) {
    Gecode::rel(
        *this,
        x,
        relation,
        x,
        Gecode::Reify(
            b,
            mode));
  }

  AliasReifiedSpace(
      AliasReifiedSpace& other)
      : Space(other),
        x(),
        b() {
    x.update(*this, other.x);
    b.update(*this, other.b);
  }

  Space* copy() override {
    return new AliasReifiedSpace(*this);
  }
};


class AliasEqualitySpace final : public Space {
public:
  ListVar x;

  explicit AliasEqualitySpace(bool equal)
      : Space(),
        x(*this, lists(-10, 10, 0, 4)) {
    Gecode::rel(
        *this,
        x,
        equal ? Gecode::IRT_EQ : Gecode::IRT_NQ,
        x);
  }

  AliasEqualitySpace(AliasEqualitySpace& other)
      : Space(other),
        x() {
    x.update(*this, other.x);
  }

  Space* copy() override {
    return new AliasEqualitySpace(*this);
  }
};



class LengthSpace final : public Space {
public:
  ListVar x;
  IntVar n;

  LengthSpace(
      Gecode::List::Domain domain,
      int minimum,
      int maximum)
      : Space(),
        x(*this, std::move(domain)),
        n(*this, minimum, maximum) {
    Gecode::length(*this, x, n);
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


class ConcatSpace final : public Space {
public:
  ListVar x;
  ListVar y;
  ListVar z;

  ConcatSpace(
      Gecode::List::Domain dx,
      Gecode::List::Domain dy,
      Gecode::List::Domain dz)
      : Space(),
        x(*this, std::move(dx)),
        y(*this, std::move(dy)),
        z(*this, std::move(dz)) {
    Gecode::concat(*this, x, y, z);
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


void equality_assigns_unknown_side() {
  auto* space = new EqualitySpace(
      lists(-200000, 200000, 0, 20),
      fixed({-8, 42, 100000}));

  assert(space->status() != Gecode::SS_FAILED);

  assert(space->x.assigned());
  assert(space->y.assigned());

  const std::vector<int> expected{-8, 42, 100000};
  assert(space->x.val() == expected);
  assert(space->y.val() == expected);

  delete space;
}


void equality_rejects_disjoint_lists() {
  auto* space = new EqualitySpace(
      fixed({1, 2, 3}),
      fixed({1, 2, 4}));

  assert(space->status() == Gecode::SS_FAILED);

  delete space;
}


void equality_clone_is_independent() {
  auto* root = new EqualitySpace(
      lists(-5, 5, 1, 3),
      lists(0, 10, 2, 4));

  assert(root->status() != Gecode::SS_FAILED);

  // Equality intersects both length ranges.
  assert(root->x.min_length() == 2);
  assert(root->x.max_length() == 3);
  assert(root->y.min_length() == 2);
  assert(root->y.max_length() == 3);

  auto* clone =
      static_cast<EqualitySpace*>(root->clone());

  Gecode::List::ListView clone_x(clone->x);

  Gecode::ModEvent me =
      clone_x.replace(*clone, fixed({2, 3}));

  if (Gecode::me_failed(me))
    clone->fail();

  assert(clone->status() != Gecode::SS_FAILED);

  assert(clone->x.assigned());
  assert(clone->y.assigned());

  assert(clone->x.val() ==
         std::vector<int>({2, 3}));
  assert(clone->y.val() ==
         std::vector<int>({2, 3}));

  // The root is unaffected.
  assert(!root->x.assigned());
  assert(!root->y.assigned());

  delete root;
  delete clone;
}


void segmented_equality_refines_mandatory_regions() {
  Gecode::List::Domain x(
      {
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(1, 3),
              0,
              3},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(4),
              1,
              1},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(3, 6),
              0,
              2},
      },
      1,
      6);

  Gecode::List::Domain y(
      {
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(2, 4),
              0,
              3},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(6),
              1,
              1},
      },
      1,
      4);

  const Gecode::List::Domain expected_x(
      {
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(2, 3),
              0,
              2},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(4),
              1,
              1},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(3, 4),
              0,
              1},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(6),
              1,
              1},
      },
      2,
      4);

  const Gecode::List::Domain expected_y(
      {
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(2, 4),
              1,
              3},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(6),
              1,
              1},
      },
      2,
      4);

  auto* space =
      new EqualitySpace(
          std::move(x),
          std::move(y));

  assert(space->status() != Gecode::SS_FAILED);

  assert(space->x.domain() == expected_x);
  assert(space->y.domain() == expected_y);

  assert(space->x.min_length() == 2);
  assert(space->x.max_length() == 4);
  assert(space->y.min_length() == 2);
  assert(space->y.max_length() == 4);

  // The domains are refined but still represent multiple concrete lists.
  assert(!space->x.assigned());
  assert(!space->y.assigned());

  delete space;
}


void disequality_prunes_single_witness_native() {
  const Gecode::List::Domain candidate(
      {
          Gecode::List::LiteralSegment{
              Gecode::List::LiteralSlice(
                  std::vector<int>{1})},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(2, 4),
              1,
              1},
          Gecode::List::LiteralSegment{
              Gecode::List::LiteralSlice(
                  std::vector<int>{3})},
      },
      3,
      3);

  const Gecode::List::Domain expected(
      {
          Gecode::List::LiteralSegment{
              Gecode::List::LiteralSlice(
                  std::vector<int>{1})},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(3, 4),
              1,
              1},
          Gecode::List::LiteralSegment{
              Gecode::List::LiteralSlice(
                  std::vector<int>{3})},
      },
      3,
      3);

  {
    auto* root = new DisequalitySpace(
        fixed({1, 2, 3}),
        candidate);

    assert(
        root->status() !=
        Gecode::SS_FAILED);

    assert(root->y.domain() == expected);
    assert(!root->y.assigned());

    assert(
        !root->y.domain().accepts(
            fixed({1, 2, 3})));

    auto* clone =
        static_cast<DisequalitySpace*>(
            root->clone());

    Gecode::List::ListView
        clone_y(clone->y);

    const Gecode::ModEvent me =
        clone_y.replace(
            *clone,
            fixed({1, 3, 3}));

    if (Gecode::me_failed(me)) {
      clone->fail();
    }

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(clone->y.assigned());
    assert(
        clone->y.val() ==
        std::vector<int>({1, 3, 3}));

    assert(!root->y.assigned());
    assert(root->y.domain() == expected);

    delete root;
    delete clone;
  }

  {
    // (x = y) -> false delegates to disequality and must expose the same
    // pruning through the half-reified adapter.
    auto* space = new ReifiedSpace(
        fixed({1, 2, 3}),
        candidate,
        0,
        0,
        Gecode::IRT_EQ,
        Gecode::RM_PMI);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->b.assigned());
    assert(space->b.val() == 0);
    assert(space->y.domain() == expected);

    delete space;
  }
}



void disequality_prunes_endpoint_count_native() {
  const Gecode::List::Domain candidate(
      {
          Gecode::List::LiteralSegment{
              Gecode::List::LiteralSlice(
                  std::vector<int>{9})},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(2),
              1,
              3},
          Gecode::List::LiteralSegment{
              Gecode::List::LiteralSlice(
                  std::vector<int>{8})},
      },
      3,
      5);

  const Gecode::List::Domain expected(
      {
          Gecode::List::LiteralSegment{
              Gecode::List::LiteralSlice(
                  std::vector<int>{9})},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(2),
              2,
              3},
          Gecode::List::LiteralSegment{
              Gecode::List::LiteralSlice(
                  std::vector<int>{8})},
      },
      4,
      5);

  {
    auto* root = new DisequalitySpace(
        fixed({9, 2, 8}),
        candidate);

    assert(
        root->status() !=
        Gecode::SS_FAILED);

    assert(root->y.domain() == expected);
    assert(root->y.min_length() == 4);
    assert(root->y.max_length() == 5);

    auto* clone =
        static_cast<DisequalitySpace*>(
            root->clone());

    Gecode::List::ListView
        clone_y(clone->y);

    const Gecode::ModEvent me =
        clone_y.replace(
            *clone,
            fixed({9, 2, 2, 8}));

    if (Gecode::me_failed(me)) {
      clone->fail();
    }

    assert(
        clone->status() !=
        Gecode::SS_FAILED);
    assert(clone->y.assigned());
    assert(
        clone->y.val() ==
        std::vector<int>({9, 2, 2, 8}));

    assert(!root->y.assigned());
    assert(root->y.domain() == expected);

    delete root;
    delete clone;
  }

  {
    // (x = y) -> false delegates to disequality and therefore exposes the
    // same endpoint-count pruning through reverse implication.
    auto* space = new ReifiedSpace(
        fixed({9, 2, 8}),
        candidate,
        0,
        0,
        Gecode::IRT_EQ,
        Gecode::RM_PMI);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->b.assigned());
    assert(space->b.val() == 0);
    assert(space->y.domain() == expected);

    delete space;
  }
}


void disequality_accepts_distinct_fixed_lists() {
  auto* space = new DisequalitySpace(
      fixed({-1, 0, 1}),
      fixed({-1, 0, 2}));

  assert(space->status() != Gecode::SS_FAILED);

  delete space;
}


void disequality_rejects_equal_fixed_lists() {
  auto* space = new DisequalitySpace(
      fixed({7, 8, 9}),
      fixed({7, 8, 9}));

  assert(space->status() == Gecode::SS_FAILED);

  delete space;
}


void alias_cases() {
  auto* equal = new AliasEqualitySpace(true);
  assert(equal->status() != Gecode::SS_FAILED);
  delete equal;

  auto* not_equal = new AliasEqualitySpace(false);
  assert(not_equal->status() == Gecode::SS_FAILED);
  delete not_equal;
}


void reified_empty_only_equality() {
  const auto optional_disjoint_left = [] {
    return Gecode::List::Domain::repeat(
        Gecode::List::ValueSet(1, 3),
        0,
        400);
  };

  const auto optional_disjoint_right = [] {
    return Gecode::List::Domain::repeat(
        Gecode::List::ValueSet(4),
        0,
        900);
  };

  {
    // Disjoint alphabets do not imply inequality: the empty list is
    // contained in both domains.
    auto* space = new ReifiedSpace(
        optional_disjoint_left(),
        optional_disjoint_right());

    assert(space->status() != Gecode::SS_FAILED);
    assert(!space->b.assigned());

    assert(!space->x.assigned());
    assert(!space->y.assigned());

    delete space;
  }

  {
    // When equality is required, the unique common value is [].
    auto* space = new ReifiedSpace(
        optional_disjoint_left(),
        optional_disjoint_right(),
        1,
        1);

    assert(space->status() != Gecode::SS_FAILED);

    assert(space->b.assigned());
    assert(space->b.val() == 1);

    assert(space->x.assigned());
    assert(space->y.assigned());

    assert(space->x.val().empty());
    assert(space->y.val().empty());

    delete space;
  }
}


void reified_equality_becomes_true() {
  auto* space = new ReifiedSpace(
      fixed({-100, 0, 50000}),
      fixed({-100, 0, 50000}));

  assert(space->status() != Gecode::SS_FAILED);
  assert(space->b.assigned());
  assert(space->b.val() == 1);

  delete space;
}


void reified_equality_becomes_false() {
  auto* space = new ReifiedSpace(
      fixed({1, 2, 3}),
      fixed({1, 2, 4}));

  assert(space->status() != Gecode::SS_FAILED);
  assert(space->b.assigned());
  assert(space->b.val() == 0);

  delete space;
}


void true_boolean_enforces_equality() {
  auto* space = new ReifiedSpace(
      lists(-10, 10, 0, 6),
      fixed({-4, 0, 9}),
      1,
      1);

  assert(space->status() != Gecode::SS_FAILED);

  assert(space->x.assigned());
  assert(space->y.assigned());
  assert(space->x.val() ==
         std::vector<int>({-4, 0, 9}));

  delete space;
}


void false_boolean_enforces_disequality() {
  auto* space = new ReifiedSpace(
      fixed({5, 6}),
      fixed({5, 6}),
      0,
      0);

  assert(space->status() == Gecode::SS_FAILED);

  delete space;
}


void reified_disequality() {
  auto* distinct = new ReifiedSpace(
      fixed({1}),
      fixed({2}),
      0,
      1,
      Gecode::IRT_NQ);

  assert(distinct->status() != Gecode::SS_FAILED);
  assert(distinct->b.assigned());
  assert(distinct->b.val() == 1);

  delete distinct;

  auto* equal = new ReifiedSpace(
      fixed({3, 4}),
      fixed({3, 4}),
      0,
      1,
      Gecode::IRT_NQ);

  assert(equal->status() != Gecode::SS_FAILED);
  assert(equal->b.assigned());
  assert(equal->b.val() == 0);

  delete equal;
}


void reified_mandatory_disjoint_languages() {
  const auto left = [] {
    return Gecode::List::Domain::repeat(
        Gecode::List::ValueSet(1, 3),
        1,
        4);
  };

  const auto right = [] {
    return Gecode::List::Domain::repeat(
        Gecode::List::ValueSet(10, 12),
        1,
        4);
  };

  {
    // Equality is impossible despite overlapping length intervals.
    auto* space = new ReifiedSpace(
        left(),
        right(),
        0,
        1,
        Gecode::IRT_EQ);

    assert(space->status() != Gecode::SS_FAILED);
    assert(space->b.assigned());
    assert(space->b.val() == 0);

    delete space;
  }

  {
    // The same language-disjointness entails disequality.
    auto* space = new ReifiedSpace(
        left(),
        right(),
        0,
        1,
        Gecode::IRT_NQ);

    assert(space->status() != Gecode::SS_FAILED);
    assert(space->b.assigned());
    assert(space->b.val() == 1);

    delete space;
  }

  {
    // Requiring equality must fail.
    auto* space = new ReifiedSpace(
        left(),
        right(),
        1,
        1,
        Gecode::IRT_EQ);

    assert(space->status() == Gecode::SS_FAILED);

    delete space;
  }

  {
    // Requiring disequality is consistent.
    auto* space = new ReifiedSpace(
        left(),
        right(),
        1,
        1,
        Gecode::IRT_NQ);

    assert(space->status() != Gecode::SS_FAILED);

    delete space;
  }
}


void concat_fixed_result_exact_split() {
  {
    auto* space = new ConcatSpace(
        lists(-100, 100, 2, 2),
        lists(-100, 100, 3, 3),
        fixed({10, 20, 30, 40, 50}));

    assert(space->status() != Gecode::SS_FAILED);

    assert(space->x.assigned());
    assert(space->y.assigned());
    assert(space->z.assigned());

    assert(
        space->x.val() ==
        std::vector<int>({10, 20}));

    assert(
        space->y.val() ==
        std::vector<int>({30, 40, 50}));

    assert(
        space->z.val() ==
        std::vector<int>(
            {10, 20, 30, 40, 50}));

    auto* clone =
        static_cast<ConcatSpace*>(
            space->clone());

    assert(clone->status() != Gecode::SS_FAILED);
    assert(
        clone->x.val() ==
        std::vector<int>({10, 20}));
    assert(
        clone->y.val() ==
        std::vector<int>({30, 40, 50}));

    delete space;
    delete clone;
  }

  {
    // The exact prefix is outside x's alphabet.
    auto* space = new ConcatSpace(
        lists(-5, 5, 1, 1),
        lists(-100, 100, 2, 2),
        fixed({10, 20, 30}));

    assert(space->status() == Gecode::SS_FAILED);

    delete space;
  }

  {
    // The exact suffix is outside y's alphabet.
    auto* space = new ConcatSpace(
        lists(-100, 100, 2, 2),
        lists(-5, 5, 1, 1),
        fixed({1, 2, 50}));

    assert(space->status() == Gecode::SS_FAILED);

    delete space;
  }

  {
    // Generic integer values are preserved, including negatives and
    // values outside character ranges.
    auto* space = new ConcatSpace(
        lists(-1000000, 1000000, 1, 1),
        lists(-1000000, 1000000, 2, 2),
        fixed({-50000, 0, 250000}));

    assert(space->status() != Gecode::SS_FAILED);

    assert(
        space->x.val() ==
        std::vector<int>({-50000}));

    assert(
        space->y.val() ==
        std::vector<int>({0, 250000}));

    delete space;
  }
}


void concat_fixed_result_split_interval_native() {
  auto* space = new ConcatSpace(
      lists(-100, 100, 1, 2),
      lists(-100, 100, 2, 3),
      fixed({10, 20, 30, 40}));

  assert(space->status() != Gecode::SS_FAILED);

  const Gecode::List::Domain expected_x(
      {
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(10),
              1,
              1},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(20),
              0,
              1},
      },
      1,
      2);

  const Gecode::List::Domain expected_y(
      {
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(20),
              0,
              1},
          Gecode::List::LiteralSegment{
              Gecode::List::LiteralSlice(
                  std::vector<int>{
                      30, 40})},
      },
      2,
      3);

  assert(space->x.domain() == expected_x);
  assert(space->y.domain() == expected_y);

  assert(!space->x.assigned());
  assert(!space->y.assigned());
  assert(space->z.assigned());

  delete space;
}


void concat_fixed_result_wide_split_interval_native() {
  auto* space = new ConcatSpace(
      lists(-100, 100, 1, 3),
      lists(-100, 100, 2, 4),
      fixed({10, 20, 30, 40, 50}));

  assert(space->status() != Gecode::SS_FAILED);

  const Gecode::List::Domain expected_x(
      {
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(10),
              1,
              1},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(20),
              0,
              1},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(30),
              0,
              1},
      },
      1,
      3);

  const Gecode::List::Domain expected_y(
      {
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(20),
              0,
              1},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(30),
              0,
              1},
          Gecode::List::LiteralSegment{
              Gecode::List::LiteralSlice(
                  std::vector<int>{
                      40, 50})},
      },
      2,
      4);

  assert(space->x.domain() == expected_x);
  assert(space->y.domain() == expected_y);

  auto* clone =
      static_cast<ConcatSpace*>(
          space->clone());

  assert(clone->status() != Gecode::SS_FAILED);
  assert(clone->x.domain() == expected_x);
  assert(clone->y.domain() == expected_y);

  delete space;
  delete clone;
}


void concat_fixed_result_empty_boundaries_native() {
  {
    auto* space = new ConcatSpace(
        lists(-100, 100, 0, 0),
        lists(-100, 100, 2, 2),
        fixed({7, 8}));

    assert(space->status() != Gecode::SS_FAILED);

    assert(space->x.assigned());
    assert(space->y.assigned());

    assert(space->x.val().empty());
    assert(
        space->y.val() ==
        std::vector<int>({7, 8}));

    delete space;
  }

  {
    auto* space = new ConcatSpace(
        lists(-100000, 100000, 2, 2),
        lists(-100, 100, 0, 0),
        fixed({-7, 80000}));

    assert(space->status() != Gecode::SS_FAILED);

    assert(space->x.assigned());
    assert(space->y.assigned());

    assert(
        space->x.val() ==
        std::vector<int>({-7, 80000}));

    assert(space->y.val().empty());

    delete space;
  }

  {
    // The split can be before or after the first value.
    auto* space = new ConcatSpace(
        lists(-100, 100, 0, 1),
        lists(-100, 100, 1, 2),
        fixed({7, 8}));

    assert(space->status() != Gecode::SS_FAILED);

    const Gecode::List::Domain expected_x(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(7),
                0,
                1},
        },
        0,
        1);

    const Gecode::List::Domain expected_y(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(7),
                0,
                1},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(8),
                1,
                1},
        },
        1,
        2);

    assert(space->x.domain() == expected_x);
    assert(space->y.domain() == expected_y);

    delete space;
  }
}


void concat_projects_result_structure_native() {
  {
    auto* space = new ConcatSpace(
        fixed({10, 20}),
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(30, 40),
            1,
            3),
        lists(-100, 100, 0, 10));

    assert(space->status() != Gecode::SS_FAILED);

    const Gecode::List::Domain expected_z(
        {
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        10, 20})},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(30, 40),
                1,
                3},
        },
        3,
        5);

    assert(space->z.domain() == expected_z);
    assert(space->x.assigned());
    assert(!space->y.assigned());
    assert(!space->z.assigned());

    auto* clone =
        static_cast<ConcatSpace*>(
            space->clone());

    assert(clone->status() != Gecode::SS_FAILED);
    assert(clone->z.domain() == expected_z);

    delete space;
    delete clone;
  }

  {
    auto* space = new ConcatSpace(
        fixed({1}),
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(2),
            1,
            2),
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(9),
            2,
            3));

    assert(space->status() == Gecode::SS_FAILED);

    delete space;
  }

  {
    auto* space = new ConcatSpace(
        fixed({-50000}),
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(0, 250000),
            1,
            2),
        lists(-1000000, 1000000, 0, 10));

    assert(space->status() != Gecode::SS_FAILED);

    const Gecode::List::Domain expected_z(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(-50000),
                1,
                1},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(0, 250000),
                1,
                2},
        },
        2,
        3);

    assert(space->z.domain() == expected_z);

    delete space;
  }
}


void concat_exact_structural_boundaries_native() {
  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        10, 20})},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(30, 40),
                1,
                3},
        },
        3,
        5);

    auto* space = new ConcatSpace(
        fixed({10, 20}),
        lists(-100, 100, 0, 10),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_y =
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(30, 40),
            1,
            3);

    assert(
        space->y.domain() ==
        expected_y);

    auto* clone =
        static_cast<ConcatSpace*>(
            space->clone());

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(
        clone->y.domain() ==
        expected_y);

    delete space;
    delete clone;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(10, 20),
                1,
                3},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        30, 40})},
        },
        3,
        5);

    auto* space = new ConcatSpace(
        lists(-100, 100, 0, 10),
        fixed({30, 40}),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_x =
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(10, 20),
            1,
            3);

    assert(
        space->x.domain() ==
        expected_x);

    delete space;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        -50000, 0})},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(
                    250000,
                    300000),
                1,
                2},
        },
        3,
        4);

    auto* space = new ConcatSpace(
        fixed({-50000, 0}),
        lists(
            -1000000,
            1000000,
            0,
            10),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_y =
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(
                250000,
                300000),
            1,
            2);

    assert(
        space->y.domain() ==
        expected_y);

    delete space;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        10, 20})},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(30, 40),
                1,
                3},
        },
        3,
        5);

    auto* space = new ConcatSpace(
        fixed({10, 99}),
        lists(-100, 100, 0, 10),
        result_domain);

    assert(
        space->status() ==
        Gecode::SS_FAILED);

    delete space;
  }
}


void concat_partial_exact_boundaries_native() {
  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        10, 20, 30, 40})},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(50, 60),
                1,
                2},
        },
        5,
        6);

    auto* space = new ConcatSpace(
        fixed({10, 20}),
        lists(-100, 100, 0, 10),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_y(
        {
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        30, 40})},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(50, 60),
                1,
                2},
        },
        3,
        4);

    assert(
        space->y.domain() ==
        expected_y);

    auto* clone =
        static_cast<ConcatSpace*>(
            space->clone());

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(
        clone->y.domain() ==
        expected_y);

    delete space;
    delete clone;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(7),
                5,
                5},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(10, 20),
                1,
                2},
        },
        6,
        7);

    auto* space = new ConcatSpace(
        fixed({7, 7}),
        lists(-100, 100, 0, 10),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_y(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(7),
                3,
                3},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(10, 20),
                1,
                2},
        },
        4,
        5);

    assert(
        space->y.domain() ==
        expected_y);

    delete space;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(10, 20),
                1,
                2},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        30, 40, 50, 60})},
        },
        5,
        6);

    auto* space = new ConcatSpace(
        lists(-100, 100, 0, 10),
        fixed({50, 60}),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_x(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(10, 20),
                1,
                2},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        30, 40})},
        },
        3,
        4);

    assert(
        space->x.domain() ==
        expected_x);

    delete space;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(
                    -50000,
                    250000),
                1,
                2},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(70000),
                5,
                5},
        },
        6,
        7);

    auto* space = new ConcatSpace(
        lists(
            -1000000,
            1000000,
            0,
            10),
        fixed(
            {70000, 70000, 70000}),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_x(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(
                    -50000,
                    250000),
                1,
                2},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(70000),
                2,
                2},
        },
        3,
        4);

    assert(
        space->x.domain() ==
        expected_x);

    auto* clone =
        static_cast<ConcatSpace*>(
            space->clone());

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(
        clone->x.domain() ==
        expected_x);

    delete space;
    delete clone;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        10, 20, 30, 40})},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(50, 60),
                1,
                2},
        },
        5,
        6);

    auto* space = new ConcatSpace(
        fixed({10, 99}),
        lists(-100, 100, 0, 10),
        result_domain);

    assert(
        space->status() ==
        Gecode::SS_FAILED);

    delete space;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(10, 20),
                1,
                2},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        30, 40, 50, 60})},
        },
        5,
        6);

    auto* space = new ConcatSpace(
        lists(-100, 100, 0, 10),
        fixed({99, 60}),
        result_domain);

    assert(
        space->status() ==
        Gecode::SS_FAILED);

    delete space;
  }
}


void concat_mandatory_repeat_boundaries_native() {
  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(
                    -50000,
                    250000),
                3,
                5},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        70000, 80000})},
        },
        5,
        7);

    auto* space = new ConcatSpace(
        fixed({-50000, 0}),
        lists(
            -1000000,
            1000000,
            0,
            10),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_y(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(
                    -50000,
                    250000),
                1,
                3},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        70000, 80000})},
        },
        3,
        5);

    assert(
        space->y.domain() ==
        expected_y);

    auto* clone =
        static_cast<ConcatSpace*>(
            space->clone());

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(
        clone->y.domain() ==
        expected_y);

    delete space;
    delete clone;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        -90000, 0})},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(
                    70000,
                    90000),
                3,
                5},
        },
        5,
        7);

    auto* space = new ConcatSpace(
        lists(
            -1000000,
            1000000,
            0,
            10),
        fixed({70000, 90000}),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_x(
        {
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        -90000, 0})},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(
                    70000,
                    90000),
                1,
                3},
        },
        3,
        5);

    assert(
        space->x.domain() ==
        expected_x);

    auto* clone =
        static_cast<ConcatSpace*>(
            space->clone());

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(
        clone->x.domain() ==
        expected_x);

    delete space;
    delete clone;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(7),
                1,
                3},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        7, 20})},
        },
        3,
        5);

    auto* space = new ConcatSpace(
        fixed({7, 7}),
        lists(-100, 100, 0, 10),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->y.min_length() == 1);
    assert(space->y.max_length() == 3);

    delete space;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(7, 9),
                3,
                5},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        20, 30})},
        },
        5,
        7);

    auto* space = new ConcatSpace(
        fixed({7, 99}),
        lists(-100, 100, 0, 10),
        result_domain);

    assert(
        space->status() ==
        Gecode::SS_FAILED);

    delete space;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        20, 30})},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(7, 9),
                3,
                5},
        },
        5,
        7);

    auto* space = new ConcatSpace(
        lists(-100, 100, 0, 10),
        fixed({99, 9}),
        result_domain);

    assert(
        space->status() ==
        Gecode::SS_FAILED);

    delete space;
  }
}


void concat_segmented_boundary_remainders_native() {
  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(10),
                1,
                1},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(30, 40),
                1,
                3},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    const Gecode::List::Domain right_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(35, 45),
                2,
                4},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    auto* space = new ConcatSpace(
        fixed({10}),
        right_domain,
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_y(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(35, 40),
                2,
                3},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        5);

    assert(
        space->y.domain() ==
        expected_y);

    auto* clone =
        static_cast<ConcatSpace*>(
            space->clone());

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(
        clone->y.domain() ==
        expected_y);

    delete space;
    delete clone;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(30, 40),
                1,
                3},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        50, 51, 90})},
        },
        4,
        6);

    const Gecode::List::Domain left_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(35, 45),
                2,
                4},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    auto* space = new ConcatSpace(
        left_domain,
        fixed({90}),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_x(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(35, 40),
                2,
                3},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        5);

    assert(
        space->x.domain() ==
        expected_x);

    auto* clone =
        static_cast<ConcatSpace*>(
            space->clone());

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(
        clone->x.domain() ==
        expected_x);

    delete space;
    delete clone;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(10),
                1,
                1},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(30, 40),
                1,
                3},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    const Gecode::List::Domain disjoint_right(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(60, 70),
                2,
                4},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    auto* space = new ConcatSpace(
        fixed({10}),
        disjoint_right,
        result_domain);

    assert(
        space->status() ==
        Gecode::SS_FAILED);

    delete space;
  }

  {
    const Gecode::List::Domain result_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(
                    -50000,
                    250000),
                1,
                5},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        70000, 80000, 90000})},
        },
        4,
        8);

    const Gecode::List::Domain left_domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(
                    150000,
                    300000),
                3,
                8},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        70000, 80000})},
        },
        5,
        10);

    auto* space = new ConcatSpace(
        left_domain,
        fixed({90000}),
        result_domain);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected_x(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(
                    150000,
                    250000),
                3,
                5},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{
                        70000, 80000})},
        },
        5,
        7);

    assert(
        space->x.domain() ==
        expected_x);

    delete space;
  }
}


void concat_assigned_result_split_filtering_native() {
  {
    auto* space = new ConcatSpace(
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(10, 20),
            1,
            2),
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(30),
            1,
            2),
        fixed({10, 20, 30}));

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->x.assigned());
    assert(space->y.assigned());
    assert(space->z.assigned());

    assert(
        space->x.val() ==
        std::vector<int>({10, 20}));

    assert(
        space->y.val() ==
        std::vector<int>({30}));

    auto* clone =
        static_cast<ConcatSpace*>(
            space->clone());

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(
        clone->x.val() ==
        std::vector<int>({10, 20}));

    assert(
        clone->y.val() ==
        std::vector<int>({30}));

    delete space;
    delete clone;
  }

  {
    auto* space = new ConcatSpace(
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(1),
            1,
            2),
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(3),
            1,
            2),
        fixed({1, 2, 3}));

    assert(
        space->status() ==
        Gecode::SS_FAILED);

    delete space;
  }

  {
    auto* space = new ConcatSpace(
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(
                -50000,
                0),
            1,
            2),
        Gecode::List::Domain::repeat(
            Gecode::List::ValueSet(250000),
            1,
            2),
        fixed(
            {-50000, 0, 250000}));

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(
        space->x.val() ==
        std::vector<int>(
            {-50000, 0}));

    assert(
        space->y.val() ==
        std::vector<int>(
            {250000}));

    delete space;
  }

  {
    auto* space = new ConcatSpace(
        fixed({}),
        lists(-100, 100, 0, 2),
        fixed({7, 8}));

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->y.assigned());

    assert(
        space->y.val() ==
        std::vector<int>({7, 8}));

    delete space;
  }
}


void length_bidirectional_native() {
  {
    auto* space = new LengthSpace(
        lists(-100, 100, 2, 6),
        0,
        10);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->x.min_length() == 2);
    assert(space->x.max_length() == 6);
    assert(space->n.min() == 2);
    assert(space->n.max() == 6);

    delete space;
  }

  {
    auto* space = new LengthSpace(
        lists(-100, 100, 0, 8),
        3,
        5);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->x.min_length() == 3);
    assert(space->x.max_length() == 5);
    assert(space->n.min() == 3);
    assert(space->n.max() == 5);

    delete space;
  }

  {
    const Gecode::List::Domain domain(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(1, 3),
                0,
                3},
            Gecode::List::LiteralSegment{
                Gecode::List::LiteralSlice(
                    std::vector<int>{9})},
        },
        1,
        4);

    auto* space = new LengthSpace(
        domain,
        2,
        2);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    const Gecode::List::Domain expected(
        {
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(1, 3),
                1,
                1},
            Gecode::List::RepeatSegment{
                Gecode::List::ValueSet(9),
                1,
                1},
        },
        2,
        2);

    assert(space->x.domain() == expected);
    assert(!space->x.assigned());

    assert(space->n.assigned());
    assert(space->n.val() == 2);

    delete space;
  }

  {
    auto* space = new LengthSpace(
        fixed({-50000, 0, 250000}),
        0,
        10);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->x.assigned());
    assert(space->n.assigned());
    assert(space->n.val() == 3);

    delete space;
  }
}


void length_failure_clone_and_unbounded_native() {
  {
    auto* space = new LengthSpace(
        lists(-100, 100, 0, 5),
        -5,
        -1);

    assert(
        space->status() ==
        Gecode::SS_FAILED);

    delete space;
  }

  {
    auto* space = new LengthSpace(
        lists(-100, 100, 4, 6),
        0,
        3);

    assert(
        space->status() ==
        Gecode::SS_FAILED);

    delete space;
  }

  {
    auto* root = new LengthSpace(
        lists(-100, 100, 1, 5),
        2,
        4);

    assert(
        root->status() !=
        Gecode::SS_FAILED);

    assert(root->x.min_length() == 2);
    assert(root->x.max_length() == 4);
    assert(root->n.min() == 2);
    assert(root->n.max() == 4);

    auto* clone =
        static_cast<LengthSpace*>(
            root->clone());

    Gecode::rel(
        *clone,
        clone->n,
        Gecode::IRT_EQ,
        3);

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(clone->n.assigned());
    assert(clone->n.val() == 3);
    assert(clone->x.min_length() == 3);
    assert(clone->x.max_length() == 3);

    assert(root->n.min() == 2);
    assert(root->n.max() == 4);
    assert(root->x.min_length() == 2);
    assert(root->x.max_length() == 4);

    delete root;
    delete clone;
  }

  {
    auto* space = new LengthSpace(
        Gecode::List::Domain::top(
            Gecode::List::ValueSet(
                -1000000,
                1000000),
            2,
            Gecode::List::kUnboundedLength),
        0,
        100);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->n.min() == 2);
    assert(space->n.max() == 100);
    assert(space->x.min_length() == 2);
    assert(space->x.max_length() == 100);

    delete space;
  }
}


void half_reification_modes_native() {
  {
    // b -> equality, with b=true.
    auto* space = new ReifiedSpace(
        lists(-100, 100, 0, 5),
        fixed({10, 20}),
        1,
        1,
        Gecode::IRT_EQ,
        Gecode::RM_IMP);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->x.assigned());
    assert(
        space->x.val() ==
        std::vector<int>({10, 20}));

    delete space;
  }

  {
    // Equality is impossible, so b must be false.
    auto* space = new ReifiedSpace(
        fixed({1}),
        fixed({2}),
        0,
        1,
        Gecode::IRT_EQ,
        Gecode::RM_IMP);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->b.assigned());
    assert(space->b.val() == 0);

    delete space;
  }

  {
    // b -> true imposes no value on b.
    auto* space = new ReifiedSpace(
        fixed({7, 8}),
        fixed({7, 8}),
        0,
        1,
        Gecode::IRT_EQ,
        Gecode::RM_IMP);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(!space->b.assigned());

    delete space;
  }

  {
    // Equality -> b.
    auto* space = new ReifiedSpace(
        fixed({-50000, 250000}),
        fixed({-50000, 250000}),
        0,
        1,
        Gecode::IRT_EQ,
        Gecode::RM_PMI);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->b.assigned());
    assert(space->b.val() == 1);

    delete space;
  }

  {
    // A false antecedent leaves b unconstrained.
    auto* space = new ReifiedSpace(
        fixed({1}),
        fixed({2}),
        0,
        1,
        Gecode::IRT_EQ,
        Gecode::RM_PMI);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(!space->b.assigned());

    delete space;
  }

  {
    // Equality -> false means disequality.
    auto* space = new ReifiedSpace(
        fixed({3, 4}),
        fixed({3, 4}),
        0,
        0,
        Gecode::IRT_EQ,
        Gecode::RM_PMI);

    assert(
        space->status() ==
        Gecode::SS_FAILED);

    delete space;
  }

  {
    // b -> disequality, but disequality is impossible.
    auto* space = new ReifiedSpace(
        fixed({9}),
        fixed({9}),
        0,
        1,
        Gecode::IRT_NQ,
        Gecode::RM_IMP);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->b.assigned());
    assert(space->b.val() == 0);

    delete space;
  }

  {
    // Disequality -> b.
    auto* space = new ReifiedSpace(
        fixed({9}),
        fixed({10}),
        0,
        1,
        Gecode::IRT_NQ,
        Gecode::RM_PMI);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->b.assigned());
    assert(space->b.val() == 1);

    delete space;
  }

  {
    // Disequality -> false enforces equality.
    auto* space = new ReifiedSpace(
        lists(-100, 100, 0, 5),
        fixed({30, 40}),
        0,
        0,
        Gecode::IRT_NQ,
        Gecode::RM_PMI);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->x.assigned());
    assert(
        space->x.val() ==
        std::vector<int>({30, 40}));

    delete space;
  }
}


void half_reification_alias_and_clone_native() {
  {
    auto* space = new AliasReifiedSpace(
        Gecode::IRT_EQ,
        Gecode::RM_IMP);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(!space->b.assigned());
    delete space;
  }

  {
    auto* space = new AliasReifiedSpace(
        Gecode::IRT_EQ,
        Gecode::RM_PMI);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->b.assigned());
    assert(space->b.val() == 1);
    delete space;
  }

  {
    auto* space = new AliasReifiedSpace(
        Gecode::IRT_NQ,
        Gecode::RM_IMP);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->b.assigned());
    assert(space->b.val() == 0);
    delete space;
  }

  {
    auto* space = new AliasReifiedSpace(
        Gecode::IRT_NQ,
        Gecode::RM_PMI);

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(!space->b.assigned());
    delete space;
  }

  {
    auto* root = new ReifiedSpace(
        lists(-100, 100, 0, 5),
        fixed({11, 12}),
        0,
        1,
        Gecode::IRT_EQ,
        Gecode::RM_IMP);

    assert(
        root->status() !=
        Gecode::SS_FAILED);

    assert(!root->b.assigned());
    assert(!root->x.assigned());

    auto* clone =
        static_cast<ReifiedSpace*>(
            root->clone());

    Gecode::rel(
        *clone,
        clone->b,
        Gecode::IRT_EQ,
        1);

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(clone->b.assigned());
    assert(clone->b.val() == 1);
    assert(clone->x.assigned());

    assert(
        clone->x.val() ==
        std::vector<int>({11, 12}));

    assert(!root->b.assigned());
    assert(!root->x.assigned());

    delete root;
    delete clone;
  }
}

void length_branching_bounds_native() {
  {
    auto* space = new LengthBranchSpace(
        Gecode::List::Domain::top(
            Gecode::List::ValueSet(-1, 1),
            2,
            Gecode::List::kUnboundedLength),
        Gecode::INT_VAL_MIN());

    assert(
        space->status() !=
        Gecode::SS_FAILED);

    assert(space->x.min_length() == 2);
    assert(
        space->x.max_length() ==
        static_cast<unsigned int>(
            Gecode::Int::Limits::max));

    delete space;
  }

  {
    const auto outside_integer_range =
        static_cast<Gecode::List::Length>(
            Gecode::Int::Limits::max) +
        1U;

    bool rejected = false;

    try {
      auto* space = new LengthBranchSpace(
          Gecode::List::Domain::top(
              Gecode::List::ValueSet(0),
              outside_integer_range,
              outside_integer_range),
          Gecode::INT_VAL_MIN());
      delete space;
    } catch (const Gecode::Int::OutOfLimits&) {
      rejected = true;
    }

    assert(rejected);
  }

  {
    Gecode::DFS<LengthBranchSpace> search(
        new LengthBranchSpace(
            lists(-2, 2, 2, 2),
            Gecode::INT_VAL_MIN()));

    int solutions = 0;

    while (LengthBranchSpace* solution = search.next()) {
      ++solutions;
      assert(solution->x.min_length() == 2);
      assert(solution->x.max_length() == 2);
      delete solution;
    }

    assert(solutions == 1);
  }
}


void length_branching_array_native() {
  Gecode::DFS<LengthArrayBranchSpace>
      search(new LengthArrayBranchSpace());

  std::vector<std::pair<unsigned int, unsigned int>>
      lengths;

  while (LengthArrayBranchSpace* solution = search.next()) {
    assert(
        solution->status() !=
        Gecode::SS_FAILED);

    assert(solution->x.size() == 2);

    for (int index = 0;
         index < solution->x.size();
         ++index) {
      assert(
          solution->x[index].min_length() ==
          solution->x[index].max_length());
    }

    lengths.emplace_back(
        solution->x[0].min_length(),
        solution->x[1].min_length());

    delete solution;
  }

  std::sort(
      lengths.begin(),
      lengths.end());

  const std::vector<
      std::pair<unsigned int, unsigned int>>
      expected{
          {0, 0},
          {0, 1},
          {1, 0},
          {1, 1},
      };

  assert(lengths == expected);
}


void exact_domain_branch_rejects_ambiguous_counts_native() {
  const Gecode::List::Domain ambiguous(
      {
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(0, 1),
              0,
              1},
          Gecode::List::RepeatSegment{
              Gecode::List::ValueSet(2, 3),
              0,
              1},
      },
      0,
      2);

  bool rejected = false;

  try {
    auto* space =
        new ExactDomainBranchSpace(ambiguous);
    delete space;
  } catch (const Gecode::Exception&) {
    rejected = true;
  }

  assert(rejected);
}


void exact_domain_ngl_length_native() {
  auto* root =
      new ExactDomainNGLSpace(
          Gecode::List::Domain::repeat(
              Gecode::List::ValueSet(0, 1),
              0,
              3));

  root->capture_literal(0);

  assert(
      root->literal->status(*root) ==
      Gecode::NGL::NONE);

  auto* clone =
      static_cast<ExactDomainNGLSpace*>(
          root->clone());

  assert(clone->literal != nullptr);
  assert(
      clone->literal->status(*clone) ==
      Gecode::NGL::NONE);

  assert(
      clone->literal->prune(*clone) ==
      Gecode::ES_OK);

  assert(
      clone->status() !=
      Gecode::SS_FAILED);
  assert(clone->x.min_length() == 2);
  assert(clone->x.max_length() == 3);
  assert(
      clone->literal->status(*clone) ==
      Gecode::NGL::FAILED);

  // Pruning the copied no-good literal must not affect its source space.
  assert(root->x.min_length() == 0);
  assert(root->x.max_length() == 3);

  root->commit_current(0);

  assert(
      root->status() !=
      Gecode::SS_FAILED);
  assert(root->x.min_length() == 0);
  assert(root->x.max_length() == 1);
  assert(
      root->literal->status(*root) ==
      Gecode::NGL::SUBSUMED);

  delete root;
  delete clone;
}


void exact_domain_ngl_value_native() {
  auto* root =
      new ExactDomainNGLSpace(
          Gecode::List::Domain::repeat(
              Gecode::List::ValueSet(-2, 2),
              1,
              1));

  root->capture_literal(0);

  assert(
      root->literal->status(*root) ==
      Gecode::NGL::NONE);

  assert(
      root->literal->prune(*root) ==
      Gecode::ES_OK);

  assert(
      root->status() !=
      Gecode::SS_FAILED);
  assert(
      root->literal->status(*root) ==
      Gecode::NGL::FAILED);

  assert(
      !root->x.domain().accepts(
          fixed({-2})));
  assert(
      !root->x.domain().accepts(
          fixed({-1})));
  assert(
      root->x.domain().accepts(
          fixed({0})));
  assert(
      root->x.domain().accepts(
          fixed({1})));
  assert(
      root->x.domain().accepts(
          fixed({2})));

  delete root;

  auto* sibling =
      new ExactDomainNGLSpace(
          Gecode::List::Domain::repeat(
              Gecode::List::ValueSet(-2, 2),
              1,
              1));

  sibling->capture_literal(1);
  assert(
      sibling->literal->prune(*sibling) ==
      Gecode::ES_OK);
  assert(
      sibling->literal->status(*sibling) ==
      Gecode::NGL::FAILED);

  assert(
      sibling->x.domain().accepts(
          fixed({-2})));
  assert(
      sibling->x.domain().accepts(
          fixed({-1})));
  assert(
      !sibling->x.domain().accepts(
          fixed({0})));

  delete sibling;
}


void exact_domain_branch_choice_archive_native() {
  auto* root =
      new ExactDomainBranchSpace(
          Gecode::List::Domain::repeat(
              Gecode::List::ValueSet(0, 1),
              0,
              3));

  assert(root->status() == Gecode::SS_BRANCH);

  const Gecode::Choice* original =
      root->choice();

  Gecode::Archive archive;
  original->archive(archive);

  std::ostringstream rendered;
  root->print(*original, 0, rendered);

  assert(
      rendered.str() ==
      "list[0] length <= 1");

  auto* replay =
      static_cast<ExactDomainBranchSpace*>(
          root->clone());

  const Gecode::Choice* restored =
      replay->choice(archive);

  replay->commit(*restored, 1);

  delete original;
  delete restored;

  assert(
      replay->status() !=
      Gecode::SS_FAILED);

  assert(replay->x.min_length() == 2);
  assert(replay->x.max_length() == 3);

  // The archived commit must affect only the clone used for replay.
  assert(root->x.min_length() == 0);
  assert(root->x.max_length() == 3);

  delete root;
  delete replay;
}


void exact_domain_branching_array_native() {
  Gecode::Search::Options options;
  options.c_d = 8;
  options.a_d = 2;

  Gecode::DFS<ExactDomainArrayBranchSpace> search(
      new ExactDomainArrayBranchSpace(),
      options);

  std::vector<std::vector<int>> pairs;

  while (ExactDomainArrayBranchSpace* solution = search.next()) {
    assert(solution->x[0].assigned());
    assert(solution->x[1].assigned());

    pairs.push_back(
        std::vector<int>{
            solution->x[0].val().front(),
            solution->x[1].val().front()});

    delete solution;
  }

  std::sort(pairs.begin(), pairs.end());

  assert(pairs.size() == 9);
  assert(
      std::adjacent_find(
          pairs.begin(),
          pairs.end()) ==
      pairs.end());

  std::vector<std::vector<int>> expected;
  for (int left = -1; left <= 1; ++left)
    for (int right = -1; right <= 1; ++right)
      expected.push_back({left, right});

  assert(pairs == expected);
}


void exact_domain_branching_single_native() {
  auto* root =
      new ExactDomainBranchSpace(
          Gecode::List::Domain::repeat(
              Gecode::List::ValueSet(0, 1),
              0,
              3));

  Gecode::DFS<ExactDomainBranchSpace> search(root);

  std::vector<std::vector<int>> solutions;

  while (ExactDomainBranchSpace* solution = search.next()) {
    assert(solution->x.assigned());
    solutions.push_back(solution->x.val());
    delete solution;
  }

  std::sort(solutions.begin(), solutions.end());

  assert(
      std::adjacent_find(
          solutions.begin(),
          solutions.end()) ==
      solutions.end());

  std::vector<std::vector<int>> expected;

  for (unsigned int length = 0;
       length <= 3;
       ++length) {
    const unsigned int count =
        1U << length;

    for (unsigned int bits = 0;
         bits < count;
         ++bits) {
      std::vector<int> value;
      value.reserve(length);

      for (unsigned int index = 0;
           index < length;
           ++index) {
        value.push_back(
            static_cast<int>(
                (bits >> index) & 1U));
      }

      expected.push_back(std::move(value));
    }
  }

  std::sort(expected.begin(), expected.end());

  assert(solutions == expected);
}


void length_branching_single_native() {
  auto* root = new LengthBranchSpace(
      lists(-2, 2, 0, 3),
      Gecode::INT_VAL_MIN());

  Gecode::DFS<LengthBranchSpace> search(root);

  std::vector<unsigned int> lengths;

  while (LengthBranchSpace* solution = search.next()) {
    assert(
        solution->status() !=
        Gecode::SS_FAILED);

    assert(
        solution->x.min_length() ==
        solution->x.max_length());

    lengths.push_back(
        solution->x.min_length());

    delete solution;
  }

  std::sort(
      lengths.begin(),
      lengths.end());

  assert(
      lengths ==
      std::vector<unsigned int>({0, 1, 2, 3}));
}


void list_var_arrays_clone_and_print_native() {
  static_assert(
      std::is_same_v<
          Gecode::ArrayTraits<
              Gecode::ListVarArgs>::ArgsType,
          Gecode::ListVarArgs>);

  static_assert(
      std::is_same_v<
          Gecode::ArrayTraits<
              Gecode::ListVarArray>::ArgsType,
          Gecode::ListVarArgs>);

  for (bool through_arguments : {false, true}) {
    auto* root =
        new ListArraySpace(
            through_arguments);

    assert(
        root->status() !=
        Gecode::SS_FAILED);

    assert(root->x.size() == 3);

    for (int i = 0;
         i < root->x.size();
         ++i) {
      assert(!root->x[i].assigned());
      assert(root->x[i].min_length() == 1);
      assert(root->x[i].max_length() == 4);
    }

    // Construction from VarArray and generic concatenation both depend on
    // the ListVar ArrayTraits specializations.
    Gecode::ListVarArgs arguments(
        root->x);

    assert(arguments.size() == 3);

    Gecode::ListVarArgs prefixed =
        root->x[0] + arguments;

    assert(prefixed.size() == 4);
    assert(
        prefixed[0].varimp() ==
        root->x[0].varimp());
    assert(
        prefixed[1].varimp() ==
        root->x[0].varimp());
    assert(
        prefixed[3].varimp() ==
        root->x[2].varimp());

    std::ostringstream rendered;
    rendered << root->x[0];

    assert(
        rendered.str() ==
        root->x[0].domain().to_string());

    std::ostringstream rendered_array;
    rendered_array << root->x;
    assert(!rendered_array.str().empty());

    auto* clone =
        static_cast<ListArraySpace*>(
            root->clone());

    Gecode::List::ListView
        clone_middle(clone->x[1]);

    const Gecode::ModEvent me =
        clone_middle.replace(
            *clone,
            fixed({7, -3, 11}));

    if (Gecode::me_failed(me))
      clone->fail();

    assert(
        clone->status() !=
        Gecode::SS_FAILED);

    assert(clone->x[1].assigned());
    assert(
        clone->x[1].val() ==
        std::vector<int>({7, -3, 11}));

    assert(!clone->x[0].assigned());
    assert(!clone->x[2].assigned());

    // Cloning the space must update every array element independently.
    assert(!root->x[1].assigned());
    assert(root->x[1].min_length() == 1);
    assert(root->x[1].max_length() == 4);

    delete root;
    delete clone;
  }
}


} // namespace


int main() {
  exact_domain_ngl_length_native();
  exact_domain_ngl_value_native();
  exact_domain_branch_choice_archive_native();
  exact_domain_branch_rejects_ambiguous_counts_native();
  exact_domain_branching_array_native();
  exact_domain_branching_single_native();
  length_branching_bounds_native();
  length_branching_array_native();
  length_branching_single_native();
  list_var_arrays_clone_and_print_native();
  half_reification_alias_and_clone_native();
  half_reification_modes_native();
  length_failure_clone_and_unbounded_native();
  length_bidirectional_native();
  concat_assigned_result_split_filtering_native();
  concat_segmented_boundary_remainders_native();
  concat_mandatory_repeat_boundaries_native();
  concat_partial_exact_boundaries_native();
  concat_exact_structural_boundaries_native();
  concat_projects_result_structure_native();
  concat_fixed_result_empty_boundaries_native();
  concat_fixed_result_wide_split_interval_native();
  concat_fixed_result_split_interval_native();
  concat_fixed_result_exact_split();
  equality_assigns_unknown_side();
  equality_rejects_disjoint_lists();
  equality_clone_is_independent();
  segmented_equality_refines_mandatory_regions();

  disequality_prunes_single_witness_native();
  disequality_prunes_endpoint_count_native();
  disequality_accepts_distinct_fixed_lists();
  disequality_rejects_equal_fixed_lists();
  alias_cases();

  reified_mandatory_disjoint_languages();
  reified_empty_only_equality();
  reified_equality_becomes_true();
  reified_equality_becomes_false();
  true_boolean_enforces_equality();
  false_boolean_enforces_disequality();
  reified_disequality();

  std::cout
      << "List relation regression tests passed\n";
  return 0;
}
