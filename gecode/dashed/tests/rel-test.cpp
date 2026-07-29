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



class LengthSpace final : public Space {
public:
  ListVar x;
  IntVar n;

  LengthSpace(
      dashed::Domain domain,
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
      dashed::Domain dx,
      dashed::Domain dy,
      dashed::Domain dz)
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

  const dashed::Domain expected_x(
      {
          dashed::RepeatSegment{
              dashed::ValueSet(10),
              1,
              1},
          dashed::RepeatSegment{
              dashed::ValueSet(20),
              0,
              1},
      },
      1,
      2);

  const dashed::Domain expected_y(
      {
          dashed::RepeatSegment{
              dashed::ValueSet(20),
              0,
              1},
          dashed::LiteralSegment{
              dashed::LiteralSlice(
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

  const dashed::Domain expected_x(
      {
          dashed::RepeatSegment{
              dashed::ValueSet(10),
              1,
              1},
          dashed::RepeatSegment{
              dashed::ValueSet(20),
              0,
              1},
          dashed::RepeatSegment{
              dashed::ValueSet(30),
              0,
              1},
      },
      1,
      3);

  const dashed::Domain expected_y(
      {
          dashed::RepeatSegment{
              dashed::ValueSet(20),
              0,
              1},
          dashed::RepeatSegment{
              dashed::ValueSet(30),
              0,
              1},
          dashed::LiteralSegment{
              dashed::LiteralSlice(
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

    const dashed::Domain expected_x(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(7),
                0,
                1},
        },
        0,
        1);

    const dashed::Domain expected_y(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(7),
                0,
                1},
            dashed::RepeatSegment{
                dashed::ValueSet(8),
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
        dashed::Domain::repeat(
            dashed::ValueSet(30, 40),
            1,
            3),
        lists(-100, 100, 0, 10));

    assert(space->status() != Gecode::SS_FAILED);

    const dashed::Domain expected_z(
        {
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        10, 20})},
            dashed::RepeatSegment{
                dashed::ValueSet(30, 40),
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
        dashed::Domain::repeat(
            dashed::ValueSet(2),
            1,
            2),
        dashed::Domain::repeat(
            dashed::ValueSet(9),
            2,
            3));

    assert(space->status() == Gecode::SS_FAILED);

    delete space;
  }

  {
    auto* space = new ConcatSpace(
        fixed({-50000}),
        dashed::Domain::repeat(
            dashed::ValueSet(0, 250000),
            1,
            2),
        lists(-1000000, 1000000, 0, 10));

    assert(space->status() != Gecode::SS_FAILED);

    const dashed::Domain expected_z(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(-50000),
                1,
                1},
            dashed::RepeatSegment{
                dashed::ValueSet(0, 250000),
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
    const dashed::Domain result_domain(
        {
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        10, 20})},
            dashed::RepeatSegment{
                dashed::ValueSet(30, 40),
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

    const dashed::Domain expected_y =
        dashed::Domain::repeat(
            dashed::ValueSet(30, 40),
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(10, 20),
                1,
                3},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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

    const dashed::Domain expected_x =
        dashed::Domain::repeat(
            dashed::ValueSet(10, 20),
            1,
            3);

    assert(
        space->x.domain() ==
        expected_x);

    delete space;
  }

  {
    const dashed::Domain result_domain(
        {
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        -50000, 0})},
            dashed::RepeatSegment{
                dashed::ValueSet(
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

    const dashed::Domain expected_y =
        dashed::Domain::repeat(
            dashed::ValueSet(
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
    const dashed::Domain result_domain(
        {
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        10, 20})},
            dashed::RepeatSegment{
                dashed::ValueSet(30, 40),
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
    const dashed::Domain result_domain(
        {
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        10, 20, 30, 40})},
            dashed::RepeatSegment{
                dashed::ValueSet(50, 60),
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

    const dashed::Domain expected_y(
        {
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        30, 40})},
            dashed::RepeatSegment{
                dashed::ValueSet(50, 60),
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(7),
                5,
                5},
            dashed::RepeatSegment{
                dashed::ValueSet(10, 20),
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

    const dashed::Domain expected_y(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(7),
                3,
                3},
            dashed::RepeatSegment{
                dashed::ValueSet(10, 20),
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(10, 20),
                1,
                2},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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

    const dashed::Domain expected_x(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(10, 20),
                1,
                2},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(
                    -50000,
                    250000),
                1,
                2},
            dashed::RepeatSegment{
                dashed::ValueSet(70000),
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

    const dashed::Domain expected_x(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(
                    -50000,
                    250000),
                1,
                2},
            dashed::RepeatSegment{
                dashed::ValueSet(70000),
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
    const dashed::Domain result_domain(
        {
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        10, 20, 30, 40})},
            dashed::RepeatSegment{
                dashed::ValueSet(50, 60),
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(10, 20),
                1,
                2},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(
                    -50000,
                    250000),
                3,
                5},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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

    const dashed::Domain expected_y(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(
                    -50000,
                    250000),
                1,
                3},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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
    const dashed::Domain result_domain(
        {
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        -90000, 0})},
            dashed::RepeatSegment{
                dashed::ValueSet(
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

    const dashed::Domain expected_x(
        {
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        -90000, 0})},
            dashed::RepeatSegment{
                dashed::ValueSet(
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(7),
                1,
                3},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(7, 9),
                3,
                5},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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
    const dashed::Domain result_domain(
        {
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        20, 30})},
            dashed::RepeatSegment{
                dashed::ValueSet(7, 9),
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(10),
                1,
                1},
            dashed::RepeatSegment{
                dashed::ValueSet(30, 40),
                1,
                3},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    const dashed::Domain right_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(35, 45),
                2,
                4},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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

    const dashed::Domain expected_y(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(35, 40),
                2,
                3},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(30, 40),
                1,
                3},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        50, 51, 90})},
        },
        4,
        6);

    const dashed::Domain left_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(35, 45),
                2,
                4},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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

    const dashed::Domain expected_x(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(35, 40),
                2,
                3},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(10),
                1,
                1},
            dashed::RepeatSegment{
                dashed::ValueSet(30, 40),
                1,
                3},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        50, 51})},
        },
        4,
        6);

    const dashed::Domain disjoint_right(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(60, 70),
                2,
                4},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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
    const dashed::Domain result_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(
                    -50000,
                    250000),
                1,
                5},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>{
                        70000, 80000, 90000})},
        },
        4,
        8);

    const dashed::Domain left_domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(
                    150000,
                    300000),
                3,
                8},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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

    const dashed::Domain expected_x(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(
                    150000,
                    250000),
                3,
                5},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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
        dashed::Domain::repeat(
            dashed::ValueSet(10, 20),
            1,
            2),
        dashed::Domain::repeat(
            dashed::ValueSet(30),
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
        dashed::Domain::repeat(
            dashed::ValueSet(1),
            1,
            2),
        dashed::Domain::repeat(
            dashed::ValueSet(3),
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
        dashed::Domain::repeat(
            dashed::ValueSet(
                -50000,
                0),
            1,
            2),
        dashed::Domain::repeat(
            dashed::ValueSet(250000),
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
    const dashed::Domain domain(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(1, 3),
                0,
                3},
            dashed::LiteralSegment{
                dashed::LiteralSlice(
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

    const dashed::Domain expected(
        {
            dashed::RepeatSegment{
                dashed::ValueSet(1, 3),
                1,
                1},
            dashed::RepeatSegment{
                dashed::ValueSet(9),
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
        dashed::Domain::top(
            dashed::ValueSet(
                -1000000,
                1000000),
            2,
            dashed::kUnboundedLength),
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

} // namespace


int main() {
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
