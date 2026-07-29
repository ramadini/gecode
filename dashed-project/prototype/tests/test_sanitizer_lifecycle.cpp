#include "dashed/dashed.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>

namespace {

using dashed::BoolDomain;
using dashed::Domain;
using dashed::IntBounds;
using dashed::LiteralSegment;
using dashed::LiteralSlice;
using dashed::RepeatSegment;
using dashed::Segment;
using dashed::ValueSet;

std::vector<int> patterned_values(std::size_t size) {
  std::vector<int> values(size);
  for (std::size_t index = 0; index < size; ++index) {
    values[index] = static_cast<int>((index * 37U) % 101U) - 50;
  }
  return values;
}

void test_shared_literal_survives_source_destruction() {
  constexpr std::size_t total_size = 20000;
  constexpr std::size_t suffix_size = 7000;

  Domain survivor;
  const void* storage_id = nullptr;

  {
    Domain source =
        Domain::fixed(patterned_values(total_size));
    assert(source.assigned());
    assert(source.segment_count() == 1);

    const auto& source_literal =
        std::get<LiteralSegment>(
            source.segments().front())
            .literal;
    storage_id = source_literal.storage_id();
    assert(storage_id != nullptr);

    survivor = source.assigned_suffix(
        static_cast<dashed::Length>(suffix_size));
    assert(survivor.assigned());
    assert(survivor.segment_count() == 1);

    const auto& survivor_literal =
        std::get<LiteralSegment>(
            survivor.segments().front())
            .literal;
    assert(survivor_literal.storage_id() == storage_id);

    std::vector<Domain> clones(256, survivor);
    for (const Domain& clone : clones) {
      assert(clone == survivor);
      const auto& clone_literal =
          std::get<LiteralSegment>(
              clone.segments().front())
              .literal;
      assert(clone_literal.storage_id() == storage_id);
    }
  }

  const std::vector<int> value = survivor.value();
  const std::vector<int> expected =
      patterned_values(total_size);
  assert(value.size() == suffix_size);
  assert(std::equal(
      value.begin(),
      value.end(),
      expected.end() -
          static_cast<std::ptrdiff_t>(suffix_size)));

  const auto& literal =
      std::get<LiteralSegment>(
          survivor.segments().front())
          .literal;
  assert(literal.storage_id() == storage_id);
}

void test_copy_narrowing_isolation() {
  const Domain original =
      Domain::repeat(ValueSet(-3, 3), 0, 8);

  for (int iteration = 0; iteration < 5000; ++iteration) {
    Domain clone = original;
    const dashed::Length exact =
        static_cast<dashed::Length>(iteration % 9);

    const dashed::Change change =
        clone.tighten_length(exact, exact);
    assert(!dashed::failed(change));
    assert(clone.min_length() == exact);
    assert(clone.max_length() == exact);
    assert(original.min_length() == 0);
    assert(original.max_length() == 8);

    Domain sibling = original;
    const dashed::Length lower =
        static_cast<dashed::Length>(iteration % 4);
    const dashed::Change sibling_change =
        sibling.tighten_length(lower, 8);
    assert(!dashed::failed(sibling_change));
    assert(sibling.min_length() == lower);
    assert(original.min_length() == 0);
  }
}

void test_failed_propagation_lifecycle() {
  for (int iteration = 0; iteration < 4000; ++iteration) {
    {
      Domain left = Domain::fixed({1, iteration});
      Domain right = Domain::fixed({2, iteration});
      const auto result =
          dashed::propagate_equal(left, right);
      assert(result.failed());
      assert(left.failed() || right.failed());
    }

    {
      Domain result = Domain::fixed({1, 2});
      Domain left = Domain::fixed({1});
      Domain right = Domain::fixed({3});
      const auto propagation =
          dashed::propagate_concat(result, left, right);
      assert(propagation.failed());
      assert(result.failed() || left.failed() || right.failed());
    }

    {
      Domain list = Domain::repeat(ValueSet(0, 2), 0, 4);
      IntBounds length{5, 6, false};
      const auto propagation =
          dashed::propagate_length(list, length);
      assert(propagation.failed());
      assert(list.failed() || length.failed);
    }

    {
      Domain left = Domain::fixed({7});
      Domain right = Domain::fixed({8});
      BoolDomain truth{false, true};
      const auto propagation =
          dashed::propagate_reified_equal(
              left,
              right,
              truth);
      assert(propagation.failed());
    }
  }
}

void test_many_segment_normalization_lifecycle() {
  for (int iteration = 0; iteration < 300; ++iteration) {
    std::vector<Segment> segments;
    segments.reserve(600);

    for (int index = 0; index < 600; ++index) {
      if ((index % 11) == 0) {
        segments.push_back(
            RepeatSegment{ValueSet(-2, 2), 0, 1});
      } else {
        segments.push_back(
            RepeatSegment{
                ValueSet((index + iteration) % 17),
                1,
                1});
      }
    }

    Domain domain(
        std::move(segments),
        545,
        600);
    assert(!domain.failed());

    Domain copy = domain;
    copy.normalize();
    assert(copy == domain);

    const dashed::Change change =
        copy.tighten_length(560, 580);
    assert(!dashed::failed(change));
    assert(copy.min_length() == 560);
    assert(copy.max_length() == 580);
    assert(domain.min_length() == 545);
    assert(domain.max_length() == 600);
  }
}

void test_repeated_concat_and_slice_lifecycle() {
  const Domain source =
      Domain::fixed(patterned_values(4096));

  for (int iteration = 0; iteration < 2000; ++iteration) {
    const dashed::Length split =
        static_cast<dashed::Length>(
            1 + (iteration % 4095));

    Domain prefix = source.assigned_prefix(split);
    Domain suffix = source.assigned_suffix(
        static_cast<dashed::Length>(4096 - split));
    Domain recombined = prefix.concatenated(suffix);

    assert(recombined.assigned());
    assert(recombined.assigned_equal(source));

    Domain result = source;
    const auto propagation =
        dashed::propagate_concat(
            result,
            prefix,
            suffix);
    assert(!propagation.failed());
    assert(prefix.assigned());
    assert(suffix.assigned());
  }
}

}  // namespace

int main() {
  test_shared_literal_survives_source_destruction();
  test_copy_narrowing_isolation();
  test_failed_propagation_lifecycle();
  test_many_segment_normalization_lifecycle();
  test_repeated_concat_and_slice_lifecycle();

  std::cout << "Dashed sanitizer lifecycle tests passed\n";
  return 0;
}
