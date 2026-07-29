#include "dashed/dashed.hpp"

#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

namespace {

using dashed::Domain;
using dashed::IntRange;
using dashed::RepeatSegment;
using dashed::ValueSet;

void enumerate(std::vector<int>& value, std::size_t target,
               const std::vector<int>& alphabet,
               const std::function<void(const std::vector<int>&)>& visit) {
  if (value.size() == target) {
    visit(value);
    return;
  }
  for (int symbol : alphabet) {
    value.push_back(symbol);
    enumerate(value, target, alphabet, visit);
    value.pop_back();
  }
}

void test_normalization_preserves_language() {
  const ValueSet bit(0, 1);
  Domain split({RepeatSegment{bit, 0, 2}, RepeatSegment{bit, 1, 2}}, 1, 4);
  Domain merged({RepeatSegment{bit, 1, 4}}, 1, 4);
  assert(split == merged);

  const std::vector<int> alphabet{-1, 0, 1, 2};
  for (std::size_t length = 0; length <= 5; ++length) {
    std::vector<int> candidate;
    enumerate(candidate, length, alphabet, [&](const std::vector<int>& x) {
      assert(split.accepts(x) == merged.accepts(x));
    });
  }
}

void test_concat_language_on_fixed_inputs() {
  const std::vector<int> alphabet{-1, 0, 1};
  for (std::size_t left_length = 0; left_length <= 3; ++left_length) {
    std::vector<int> left;
    enumerate(left, left_length, alphabet, [&](const std::vector<int>& x) {
      for (std::size_t right_length = 0; right_length <= 3; ++right_length) {
        std::vector<int> right;
        enumerate(right, right_length, alphabet,
                  [&](const std::vector<int>& y) {
          Domain dx = Domain::fixed(x);
          Domain dy = Domain::fixed(y);
          Domain dz = dx.concatenated(dy);
          std::vector<int> expected = x;
          expected.insert(expected.end(), y.begin(), y.end());
          assert(dz.assigned());
          assert(dz.value() == expected);
        });
      }
    });
  }
}

void test_value_set_against_enumeration() {
  ValueSet lhs{{IntRange{-5, -3}, IntRange{0, 2}, IntRange{8, 10}}};
  ValueSet rhs{{IntRange{-4, 1}, IntRange{9, 12}}};
  ValueSet intersection = lhs.intersected(rhs);
  for (int value = -20; value <= 20; ++value) {
    assert(intersection.contains(value) ==
           (lhs.contains(value) && rhs.contains(value)));
  }
}

void test_literal_memory_accounting() {
  constexpr std::size_t n = 100000;

  // Use a genuinely non-uniform fixed sequence. A uniform sequence is now
  // normalized to one exact singleton repeat segment and intentionally has
  // no O(n) literal payload.
  std::vector<int> values;
  values.reserve(n);

  for (std::size_t i = 0; i < n; ++i) {
    values.push_back(
        static_cast<int>(i % 2));
  }

  dashed::Domain domain =
      dashed::Domain::fixed(std::move(values));

  assert(domain.segment_count() == 1);
  assert(std::holds_alternative<dashed::LiteralSegment>(
      domain.segments().front()));

  assert(
      domain.referenced_dynamic_bytes() >=
      n * sizeof(int));
}


std::vector<std::vector<int>> all_lists(std::size_t max_length) {
  const std::vector<int> alphabet{0, 1};
  std::vector<std::vector<int>> result;
  for (std::size_t length = 0; length <= max_length; ++length) {
    std::vector<int> value;
    enumerate(value, length, alphabet,
              [&](const std::vector<int>& x) { result.push_back(x); });
  }
  return result;
}

std::vector<Domain> sample_domains() {
  const ValueSet bit(0, 1);
  return {
      Domain::empty_list(),
      Domain::fixed({0}),
      Domain::fixed({1}),
      Domain::fixed({0, 1}),
      Domain::top(bit, 0, 2),
      Domain::repeat(ValueSet(0), 0, 2),
      Domain::repeat(ValueSet(1), 1, 2),
      Domain({RepeatSegment{bit, 1, 1},
              dashed::LiteralSegment{dashed::LiteralSlice({1})}},
             2, 2),
  };
}

void assert_subset(const Domain& restricted, const Domain& original,
                   const std::vector<std::vector<int>>& universe) {
  if (restricted.failed()) {
    return;
  }
  for (const auto& value : universe) {
    if (restricted.accepts(value)) {
      assert(original.accepts(value));
    }
  }
}

void test_equal_kernel_soundness() {
  const auto domains = sample_domains();
  const auto universe = all_lists(3);
  for (const Domain& original_x : domains) {
    for (const Domain& original_y : domains) {
      Domain x = original_x;
      Domain y = original_y;
      const auto result = dashed::propagate_equal(x, y);

      assert_subset(x, original_x, universe);
      assert_subset(y, original_y, universe);

      bool has_solution = false;
      for (const auto& value : universe) {
        const bool feasible = original_x.accepts(value) &&
                              original_y.accepts(value);
        has_solution = has_solution || feasible;
        if (feasible) {
          assert(!result.failed());
          assert(x.accepts(value));
          assert(y.accepts(value));
        }
      }
      if (result.failed()) {
        assert(!has_solution);
      }
      if (result.subsumed) {
        assert(x.assigned() && y.assigned() && x.assigned_equal(y));
      }
    }
  }
}

void test_not_equal_kernel_soundness() {
  const auto domains = sample_domains();
  const auto universe = all_lists(3);
  for (const Domain& original_x : domains) {
    for (const Domain& original_y : domains) {
      Domain x = original_x;
      Domain y = original_y;
      const auto result = dashed::propagate_not_equal(x, y);

      assert_subset(x, original_x, universe);
      assert_subset(y, original_y, universe);

      bool has_solution = false;
      for (const auto& xv : universe) {
        for (const auto& yv : universe) {
          const bool feasible = original_x.accepts(xv) &&
                                original_y.accepts(yv) && xv != yv;
          has_solution = has_solution || feasible;
          if (feasible) {
            assert(!result.failed());
            assert(x.accepts(xv));
            assert(y.accepts(yv));
          }
        }
      }
      if (result.failed()) {
        assert(!has_solution);
      }
    }
  }
}

void test_single_witness_disequality_exactness() {
  const auto universe =
      all_lists(3);

  for (const auto& forbidden :
       universe) {
    if (forbidden.size() != 3) {
      continue;
    }

    for (std::size_t witness = 0;
         witness < forbidden.size();
         ++witness) {
      std::vector<dashed::Segment>
          segments;

      if (witness != 0) {
        segments.push_back(
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>(
                        forbidden.begin(),
                        forbidden.begin() +
                            static_cast<
                                std::ptrdiff_t>(
                                witness)))});
      }

      segments.push_back(
          RepeatSegment{
              ValueSet(0, 1),
              1,
              1});

      if (witness + 1 <
          forbidden.size()) {
        segments.push_back(
            dashed::LiteralSegment{
                dashed::LiteralSlice(
                    std::vector<int>(
                        forbidden.begin() +
                            static_cast<
                                std::ptrdiff_t>(
                                witness + 1),
                        forbidden.end()))});
      }

      Domain assigned =
          Domain::fixed(forbidden);

      Domain candidate(
          std::move(segments),
          3,
          3);

      const Domain original =
          candidate;

      const auto result =
          dashed::propagate_not_equal(
              assigned,
              candidate);

      assert(!result.failed());
      assert(result.subsumed);

      for (const auto& value :
           universe) {
        const bool expected =
            original.accepts(value) &&
            value != forbidden;

        assert(
            candidate.accepts(value) ==
            expected);
      }
    }
  }
}


void test_half_reified_kernel_soundness() {
  using Kernel =
      dashed::PropagationResult (*)(
          Domain&,
          Domain&,
          dashed::BoolDomain&);

  const auto domains =
      sample_domains();

  const auto universe =
      all_lists(3);

  const std::vector<dashed::BoolDomain>
      bool_domains{
          {true, true},
          {true, false},
          {false, true},
      };

  const auto verify =
      [&](Kernel kernel,
          const auto& relation) {
        for (const Domain& original_x :
             domains) {
          for (const Domain& original_y :
               domains) {
            for (const auto original_b :
                 bool_domains) {
              Domain x = original_x;
              Domain y = original_y;
              auto b = original_b;

              const auto result =
                  kernel(
                      x,
                      y,
                      b);

              assert_subset(
                  x,
                  original_x,
                  universe);

              assert_subset(
                  y,
                  original_y,
                  universe);

              assert(
                  !b.may_be_false ||
                  original_b.may_be_false);

              assert(
                  !b.may_be_true ||
                  original_b.may_be_true);

              bool has_solution = false;

              for (const auto& xv :
                   universe) {
                if (!original_x.accepts(xv)) {
                  continue;
                }

                for (const auto& yv :
                     universe) {
                  if (!original_y.accepts(yv)) {
                    continue;
                  }

                  const bool equal =
                      xv == yv;

                  for (const bool bv :
                       {false, true}) {
                    const bool allowed =
                        bv
                            ? original_b.may_be_true
                            : original_b.may_be_false;

                    const bool feasible =
                        allowed &&
                        relation(
                            equal,
                            bv);

                    if (!feasible) {
                      continue;
                    }

                    has_solution = true;

                    assert(!result.failed());
                    assert(x.accepts(xv));
                    assert(y.accepts(yv));

                    assert(
                        bv
                            ? b.may_be_true
                            : b.may_be_false);
                  }
                }
              }

              if (result.failed()) {
                assert(!has_solution);
              }
            }
          }
        }
      };

  verify(
      dashed::propagate_implied_equal,
      [](bool equal, bool truth) {
        return !truth || equal;
      });

  verify(
      dashed::propagate_equal_implies,
      [](bool equal, bool truth) {
        return !equal || truth;
      });

  verify(
      dashed::propagate_implied_not_equal,
      [](bool equal, bool truth) {
        return !truth || !equal;
      });

  verify(
      dashed::propagate_not_equal_implies,
      [](bool equal, bool truth) {
        return equal || truth;
      });
}


void test_reified_kernel_soundness() {
  const auto domains = sample_domains();
  const auto universe = all_lists(3);
  const std::vector<dashed::BoolDomain> bool_domains{
      {true, true}, {false, true}, {true, false}};

  for (const Domain& original_x : domains) {
    for (const Domain& original_y : domains) {
      for (const auto original_b : bool_domains) {
        Domain x = original_x;
        Domain y = original_y;
        auto b = original_b;
        const auto result = dashed::propagate_reified_equal(x, y, b);

        assert_subset(x, original_x, universe);
        assert_subset(y, original_y, universe);
        assert((!b.may_be_false || original_b.may_be_false));
        assert((!b.may_be_true || original_b.may_be_true));

        bool has_solution = false;
        for (const auto& xv : universe) {
          for (const auto& yv : universe) {
            for (const bool bv : {false, true}) {
              const bool b_allowed = bv ? original_b.may_be_true
                                        : original_b.may_be_false;
              const bool feasible = original_x.accepts(xv) &&
                                    original_y.accepts(yv) && b_allowed &&
                                    (bv == (xv == yv));
              has_solution = has_solution || feasible;
              if (feasible) {
                assert(!result.failed());
                assert(x.accepts(xv));
                assert(y.accepts(yv));
                assert(bv ? b.may_be_true : b.may_be_false);
              }
            }
          }
        }
        if (result.failed()) {
          assert(!has_solution);
        }
      }
    }
  }
}

void test_assigned_concat_split_filtering() {
  const auto domains =
      sample_domains();

  const auto fixed_results =
      all_lists(4);

  for (const auto& result_value :
       fixed_results) {
    for (const Domain& original_x :
         domains) {
      for (const Domain& original_y :
           domains) {
        bool has_solution = false;

        for (std::size_t split = 0;
             split <= result_value.size();
             ++split) {
          const std::vector<int> prefix(
              result_value.begin(),
              result_value.begin() +
                  static_cast<std::ptrdiff_t>(
                      split));

          const std::vector<int> suffix(
              result_value.begin() +
                  static_cast<std::ptrdiff_t>(
                      split),
              result_value.end());

          if (original_x.accepts(prefix) &&
              original_y.accepts(suffix)) {
            has_solution = true;
          }
        }

        Domain z =
            Domain::fixed(result_value);

        Domain x = original_x;
        Domain y = original_y;

        const auto result =
            dashed::propagate_concat(
                z,
                x,
                y);

        assert(
            result.failed() ==
            !has_solution);

        if (!has_solution) {
          continue;
        }

        for (std::size_t split = 0;
             split <= result_value.size();
             ++split) {
          const std::vector<int> prefix(
              result_value.begin(),
              result_value.begin() +
                  static_cast<std::ptrdiff_t>(
                      split));

          const std::vector<int> suffix(
              result_value.begin() +
                  static_cast<std::ptrdiff_t>(
                      split),
              result_value.end());

          if (original_x.accepts(prefix) &&
              original_y.accepts(suffix)) {
            assert(x.accepts(prefix));
            assert(y.accepts(suffix));
            assert(z.accepts(result_value));
          }
        }
      }
    }
  }
}


void test_concat_kernel_soundness() {
  const auto domains = sample_domains();
  const auto small_values = all_lists(2);
  const auto z_universe = all_lists(4);

  for (const Domain& original_x : domains) {
    for (const Domain& original_y : domains) {
      for (const Domain& original_z : domains) {
        Domain x = original_x;
        Domain y = original_y;
        Domain z = original_z;
        const auto result = dashed::propagate_concat(z, x, y);

        assert_subset(x, original_x, z_universe);
        assert_subset(y, original_y, z_universe);
        assert_subset(z, original_z, z_universe);

        bool has_solution = false;
        for (const auto& xv : small_values) {
          if (!original_x.accepts(xv)) continue;
          for (const auto& yv : small_values) {
            if (!original_y.accepts(yv)) continue;
            std::vector<int> zv = xv;
            zv.insert(zv.end(), yv.begin(), yv.end());
            const bool feasible = original_z.accepts(zv);
            has_solution = has_solution || feasible;
            if (feasible) {
              assert(!result.failed());
              assert(x.accepts(xv));
              assert(y.accepts(yv));
              assert(z.accepts(zv));
            }
          }
        }
        if (result.failed()) {
          assert(!has_solution);
        }
      }
    }
  }
}

void test_length_kernel_signed_bounds() {
  const auto domains =
      sample_domains();

  const auto universe =
      all_lists(4);

  for (const Domain& original :
       domains) {
    for (int lower = -2;
         lower <= 4;
         ++lower) {
      for (int upper = -2;
           upper <= 4;
           ++upper) {
        if (lower > upper) {
          continue;
        }

        Domain x = original;

        dashed::IntBounds length{
            lower,
            upper};

        const auto result =
            dashed::propagate_length(
                x,
                length);

        bool has_solution = false;

        for (const auto& value :
             universe) {
          const int size =
              static_cast<int>(
                  value.size());

          const bool feasible =
              original.accepts(value) &&
              lower <= size &&
              size <= upper;

          if (!feasible) {
            continue;
          }

          has_solution = true;

          assert(!result.failed());
          assert(x.accepts(value));
          assert(length.lower <= size);
          assert(size <= length.upper);
        }

        assert(
            result.failed() ==
            !has_solution);

        if (!result.failed()) {
          assert(length.lower >= 0);
          assert(
              length.lower <=
              length.upper);
        }
      }
    }
  }

  {
    Domain x =
        Domain::top(
            ValueSet(-10, 10),
            3,
            dashed::kUnboundedLength);

    dashed::IntBounds length{
        -100,
        std::numeric_limits<
            std::int64_t>::max()};

    const auto first =
        dashed::propagate_length(
            x,
            length);

    assert(!first.failed());
    assert(length.lower == 3);

    assert(
        length.upper ==
        static_cast<std::int64_t>(
            dashed::kUnboundedLength));

    assert(x.min_length() == 3);
    assert(
        x.max_length() ==
        dashed::kUnboundedLength);

    const auto second =
        dashed::propagate_length(
            x,
            length);

    assert(!second.failed());
    assert(second.left == dashed::Change::none);
    assert(second.result == dashed::Change::none);
  }

  {
    Domain x =
        Domain::top(
            ValueSet(-10, 10),
            0,
            5);

    dashed::IntBounds invalid{
        4,
        3};

    const auto result =
        dashed::propagate_length(
            x,
            invalid);

    assert(result.failed());
    assert(x.failed());
    assert(invalid.failed);
  }
}


void test_length_kernel_soundness() {
  const auto domains = sample_domains();
  const auto universe = all_lists(3);
  for (const Domain& original : domains) {
    for (int lower = 0; lower <= 3; ++lower) {
      for (int upper = lower; upper <= 3; ++upper) {
        Domain x = original;
        dashed::IntBounds length{lower, upper};
        const auto result = dashed::propagate_length(x, length);
        assert_subset(x, original, universe);

        bool has_solution = false;
        for (const auto& value : universe) {
          const auto n = static_cast<int>(value.size());
          const bool feasible = original.accepts(value) && lower <= n && n <= upper;
          has_solution = has_solution || feasible;
          if (feasible) {
            assert(!result.failed());
            assert(x.accepts(value));
            assert(length.lower <= n && n <= length.upper);
          }
        }
        if (result.failed()) {
          assert(!has_solution);
        }
      }
    }
  }
}

}  // namespace

int main() {
  test_normalization_preserves_language();
  test_concat_language_on_fixed_inputs();
  test_value_set_against_enumeration();
  test_literal_memory_accounting();
  test_equal_kernel_soundness();
  test_not_equal_kernel_soundness();
  test_single_witness_disequality_exactness();
  test_half_reified_kernel_soundness();
  test_reified_kernel_soundness();
  test_assigned_concat_split_filtering();
  test_concat_kernel_soundness();
  test_length_kernel_signed_bounds();
  test_length_kernel_soundness();
  std::cout << "All Dashed property tests passed.\n";
  return 0;
}
