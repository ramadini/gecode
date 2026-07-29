#include <gecode/dashed.hh>

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

namespace {

using Gecode::BoolVar;
using Gecode::IntVar;
using Gecode::ListVar;
using Gecode::Space;

dashed::Domain fixed(std::initializer_list<int> values) {
  return dashed::Domain::fixed(std::vector<int>(values));
}

dashed::Domain lists(int lo, int hi,
                     unsigned int min_length,
                     unsigned int max_length) {
  return dashed::Domain::top(
      dashed::ValueSet(lo, hi),
      min_length,
      max_length);
}


class EqualitySpace final : public Space {
public:
  ListVar x;
  ListVar y;

  EqualitySpace(dashed::Domain dx, dashed::Domain dy)
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

  DisequalitySpace(dashed::Domain dx, dashed::Domain dy)
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

  ReifiedSpace(dashed::Domain dx,
               dashed::Domain dy,
               int b_min = 0,
               int b_max = 1,
               Gecode::IntRelType relation = Gecode::IRT_EQ)
      : Space(),
        x(*this, std::move(dx)),
        y(*this, std::move(dy)),
        b(*this, b_min, b_max) {
    Gecode::rel(
        *this,
        x,
        relation,
        y,
        Gecode::Reify(b, Gecode::RM_EQV));
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

  Gecode::Dashed::ListView clone_x(clone->x);

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
  dashed::Domain x(
      {
          dashed::RepeatSegment{
              dashed::ValueSet(1, 3),
              0,
              3},
          dashed::RepeatSegment{
              dashed::ValueSet(4),
              1,
              1},
          dashed::RepeatSegment{
              dashed::ValueSet(3, 6),
              0,
              2},
      },
      1,
      6);

  dashed::Domain y(
      {
          dashed::RepeatSegment{
              dashed::ValueSet(2, 4),
              0,
              3},
          dashed::RepeatSegment{
              dashed::ValueSet(6),
              1,
              1},
      },
      1,
      4);

  const dashed::Domain expected_x(
      {
          dashed::RepeatSegment{
              dashed::ValueSet(2, 3),
              0,
              2},
          dashed::RepeatSegment{
              dashed::ValueSet(4),
              1,
              1},
          dashed::RepeatSegment{
              dashed::ValueSet(3, 4),
              0,
              1},
          dashed::RepeatSegment{
              dashed::ValueSet(6),
              1,
              1},
      },
      2,
      4);

  const dashed::Domain expected_y(
      {
          dashed::RepeatSegment{
              dashed::ValueSet(2, 4),
              1,
              3},
          dashed::RepeatSegment{
              dashed::ValueSet(6),
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
    return dashed::Domain::repeat(
        dashed::ValueSet(1, 3),
        0,
        400);
  };

  const auto optional_disjoint_right = [] {
    return dashed::Domain::repeat(
        dashed::ValueSet(4),
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
    return dashed::Domain::repeat(
        dashed::ValueSet(1, 3),
        1,
        4);
  };

  const auto right = [] {
    return dashed::Domain::repeat(
        dashed::ValueSet(10, 12),
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

} // namespace


int main() {
  equality_assigns_unknown_side();
  equality_rejects_disjoint_lists();
  equality_clone_is_independent();
  segmented_equality_refines_mandatory_regions();

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
      << "Dashed relation regression tests passed\n";
  return 0;
}
