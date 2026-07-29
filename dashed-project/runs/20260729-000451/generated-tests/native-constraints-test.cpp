#include <gecode/dashed.hh>

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

namespace {

using dashed::Domain;
using dashed::ValueSet;

class BinarySpace final : public Gecode::Space {
public:
  Gecode::ListVar x;
  Gecode::ListVar y;

  BinarySpace(Domain dx, Domain dy, Gecode::IntRelType relation)
      : Gecode::Space(),
        x(*this, std::move(dx)),
        y(*this, std::move(dy)) {
    Gecode::rel(*this, x, relation, y);
  }

  BinarySpace(BinarySpace& other)
      : Gecode::Space(other), x(), y() {
    x.update(*this, other.x);
    y.update(*this, other.y);
  }

  Gecode::Space* copy() override {
    return new BinarySpace(*this);
  }
};

class SelfBinarySpace final : public Gecode::Space {
public:
  Gecode::ListVar x;

  explicit SelfBinarySpace(Gecode::IntRelType relation)
      : Gecode::Space(),
        x(*this, Domain::top(ValueSet(1, 3), 0, 3)) {
    Gecode::rel(*this, x, relation, x);
  }

  SelfBinarySpace(SelfBinarySpace& other)
      : Gecode::Space(other), x() {
    x.update(*this, other.x);
  }

  Gecode::Space* copy() override {
    return new SelfBinarySpace(*this);
  }
};

class ReifiedSpace final : public Gecode::Space {
public:
  Gecode::ListVar x;
  Gecode::ListVar y;
  Gecode::BoolVar b;

  ReifiedSpace(Domain dx, Domain dy, Gecode::IntRelType relation,
               int bmin = 0, int bmax = 1,
               Gecode::ReifyMode mode = Gecode::RM_EQV)
      : Gecode::Space(),
        x(*this, std::move(dx)),
        y(*this, std::move(dy)),
        b(*this, bmin, bmax) {
    Gecode::rel(*this, x, relation, y,
                Gecode::Reify(b, mode));
  }

  ReifiedSpace(ReifiedSpace& other)
      : Gecode::Space(other), x(), y(), b() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    b.update(*this, other.b);
  }

  Gecode::Space* copy() override {
    return new ReifiedSpace(*this);
  }
};

class ConcatSpace final : public Gecode::Space {
public:
  Gecode::ListVar x;
  Gecode::ListVar y;
  Gecode::ListVar z;

  ConcatSpace(Domain dx, Domain dy, Domain dz)
      : Gecode::Space(),
        x(*this, std::move(dx)),
        y(*this, std::move(dy)),
        z(*this, std::move(dz)) {
    Gecode::concat(*this, x, y, z);
  }

  ConcatSpace(ConcatSpace& other)
      : Gecode::Space(other), x(), y(), z() {
    x.update(*this, other.x);
    y.update(*this, other.y);
    z.update(*this, other.z);
  }

  Gecode::Space* copy() override {
    return new ConcatSpace(*this);
  }
};

class AliasConcatSpace final : public Gecode::Space {
public:
  Gecode::ListVar x;
  Gecode::ListVar z;

  AliasConcatSpace()
      : Gecode::Space(),
        x(*this, Domain::top(ValueSet(1, 2), 0, 2)),
        z(*this, Domain::top(ValueSet(1, 2), 0, 4)) {
    Gecode::concat(*this, x, x, z);
  }

  AliasConcatSpace(AliasConcatSpace& other)
      : Gecode::Space(other), x(), z() {
    x.update(*this, other.x);
    z.update(*this, other.z);
  }

  Gecode::Space* copy() override {
    return new AliasConcatSpace(*this);
  }
};

class LengthSpace final : public Gecode::Space {
public:
  Gecode::ListVar x;
  Gecode::IntVar n;

  LengthSpace(Domain domain, int nmin, int nmax)
      : Gecode::Space(),
        x(*this, std::move(domain)),
        n(*this, nmin, nmax) {
    Gecode::length(*this, x, n);
  }

  LengthSpace(LengthSpace& other)
      : Gecode::Space(other), x(), n() {
    x.update(*this, other.x);
    n.update(*this, other.n);
  }

  Gecode::Space* copy() override {
    return new LengthSpace(*this);
  }
};

void test_equality_assignment() {
  auto* s = new BinarySpace(
      Domain::top(ValueSet(1, 3), 2, 2),
      Domain::fixed(std::vector<int>{2, 3}),
      Gecode::IRT_EQ);

  assert(s->status() != Gecode::SS_FAILED);
  assert(s->x.assigned());
  assert(s->x.val() == std::vector<int>({2, 3}));
  delete s;
}

void test_equality_same_shape_intersection() {
  auto* s = new BinarySpace(
      Domain::repeat(ValueSet(1, 3), 2, 2),
      Domain::repeat(ValueSet(2, 4), 2, 2),
      Gecode::IRT_EQ);

  assert(s->status() != Gecode::SS_FAILED);
  assert(s->x.domain().accepts(Domain::fixed(std::vector<int>{2, 3})));
  assert(s->y.domain().accepts(Domain::fixed(std::vector<int>{3, 2})));
  assert(!s->x.domain().accepts(Domain::fixed(std::vector<int>{1, 1})));
  assert(!s->y.domain().accepts(Domain::fixed(std::vector<int>{4, 4})));
  delete s;
}

void test_equality_failure_and_aliases() {
  auto* failed = new BinarySpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{2}),
      Gecode::IRT_EQ);
  assert(failed->status() == Gecode::SS_FAILED);
  delete failed;

  auto* equal_self = new SelfBinarySpace(Gecode::IRT_EQ);
  assert(equal_self->status() != Gecode::SS_FAILED);
  delete equal_self;

  auto* unequal_self = new SelfBinarySpace(Gecode::IRT_NQ);
  assert(unequal_self->status() == Gecode::SS_FAILED);
  delete unequal_self;
}

void test_equality_clone_with_active_propagator() {
  auto* root = new BinarySpace(
      Domain::top(ValueSet(1, 3), 1, 3),
      Domain::top(ValueSet(1, 3), 1, 3),
      Gecode::IRT_EQ);

  assert(root->status() != Gecode::SS_FAILED);
  assert(!root->x.assigned());
  assert(!root->y.assigned());

  auto* clone = static_cast<BinarySpace*>(root->clone());
  Gecode::Dashed::ListView clone_y(clone->y);
  Gecode::ModEvent me = clone_y.replace(
      *clone, Domain::fixed(std::vector<int>{3, 2}));
  if (Gecode::me_failed(me))
    clone->fail();

  assert(clone->status() != Gecode::SS_FAILED);
  assert(clone->x.assigned());
  assert(clone->x.val() == std::vector<int>({3, 2}));

  assert(!root->x.assigned());
  assert(!root->y.assigned());

  delete root;
  delete clone;
}

void test_disequality() {
  auto* disjoint_lengths = new BinarySpace(
      Domain::top(ValueSet(1, 2), 1, 1),
      Domain::top(ValueSet(1, 2), 2, 2),
      Gecode::IRT_NQ);
  assert(disjoint_lengths->status() != Gecode::SS_FAILED);
  delete disjoint_lengths;

  auto* different = new BinarySpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{2}),
      Gecode::IRT_NQ);
  assert(different->status() != Gecode::SS_FAILED);
  delete different;

  auto* same = new BinarySpace(
      Domain::fixed(std::vector<int>{1, 2}),
      Domain::fixed(std::vector<int>{1, 2}),
      Gecode::IRT_NQ);
  assert(same->status() == Gecode::SS_FAILED);
  delete same;
}

void test_reified_equality() {
  auto* equal = new ReifiedSpace(
      Domain::fixed(std::vector<int>{1, 2}),
      Domain::fixed(std::vector<int>{1, 2}),
      Gecode::IRT_EQ);
  assert(equal->status() != Gecode::SS_FAILED);
  assert(equal->b.assigned() && equal->b.val() == 1);
  delete equal;

  auto* different = new ReifiedSpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{2}),
      Gecode::IRT_EQ);
  assert(different->status() != Gecode::SS_FAILED);
  assert(different->b.assigned() && different->b.val() == 0);
  delete different;

  auto* forced_true = new ReifiedSpace(
      Domain::top(ValueSet(1, 3), 2, 2),
      Domain::fixed(std::vector<int>{2, 3}),
      Gecode::IRT_EQ, 1, 1);
  assert(forced_true->status() != Gecode::SS_FAILED);
  assert(forced_true->x.assigned());
  assert(forced_true->x.val() == std::vector<int>({2, 3}));
  delete forced_true;

  auto* contradictory_false = new ReifiedSpace(
      Domain::fixed(std::vector<int>{2}),
      Domain::fixed(std::vector<int>{2}),
      Gecode::IRT_EQ, 0, 0);
  assert(contradictory_false->status() == Gecode::SS_FAILED);
  delete contradictory_false;
}

void test_reified_disequality() {
  auto* equal = new ReifiedSpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{1}),
      Gecode::IRT_NQ);
  assert(equal->status() != Gecode::SS_FAILED);
  assert(equal->b.assigned() && equal->b.val() == 0);
  delete equal;

  auto* different = new ReifiedSpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{2}),
      Gecode::IRT_NQ);
  assert(different->status() != Gecode::SS_FAILED);
  assert(different->b.assigned() && different->b.val() == 1);
  delete different;
}

void test_half_reification_rejected() {
  bool caught = false;
  try {
    auto* unsupported = new ReifiedSpace(
        Domain::top(ValueSet(1, 2), 0, 2),
        Domain::top(ValueSet(1, 2), 0, 2),
        Gecode::IRT_EQ, 0, 1, Gecode::RM_IMP);
    delete unsupported;
  } catch (const Gecode::Int::UnknownReifyMode&) {
    caught = true;
  }
  assert(caught);
}

void test_concat_forward() {
  auto* s = new ConcatSpace(
      Domain::fixed(std::vector<int>{1, 2}),
      Domain::fixed(std::vector<int>{3}),
      Domain::top(ValueSet(1, 3), 0, 5));

  assert(s->status() != Gecode::SS_FAILED);
  assert(s->z.assigned());
  assert(s->z.val() == std::vector<int>({1, 2, 3}));
  delete s;
}

void test_concat_backward_suffix() {
  auto* s = new ConcatSpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::top(ValueSet(1, 4), 0, 4),
      Domain::fixed(std::vector<int>{1, 2, 3}));

  assert(s->status() != Gecode::SS_FAILED);
  assert(s->y.assigned());
  assert(s->y.val() == std::vector<int>({2, 3}));
  delete s;
}

void test_concat_lengths_and_failure() {
  auto* lengths = new ConcatSpace(
      Domain::top(ValueSet(1, 2), 1, 2),
      Domain::top(ValueSet(1, 2), 2, 3),
      Domain::top(ValueSet(1, 2), 0, 10));

  assert(lengths->status() != Gecode::SS_FAILED);
  assert(lengths->z.min_length() == 3);
  assert(lengths->z.max_length() == 5);
  delete lengths;

  auto* failed = new ConcatSpace(
      Domain::fixed(std::vector<int>{1}),
      Domain::fixed(std::vector<int>{2}),
      Domain::fixed(std::vector<int>{1, 3}));
  assert(failed->status() == Gecode::SS_FAILED);
  delete failed;
}

void test_concat_alias_rejected() {
  bool caught = false;
  try {
    auto* unsupported = new AliasConcatSpace();
    delete unsupported;
  } catch (const Gecode::Int::ArgumentSame&) {
    caught = true;
  }
  assert(caught);
}

void test_length_still_works() {
  auto* s = new LengthSpace(
      Domain::top(ValueSet(1, 3), 1, 5), 2, 4);

  assert(s->status() != Gecode::SS_FAILED);
  assert(s->x.min_length() == 2);
  assert(s->x.max_length() == 4);
  assert(s->n.min() == 2);
  assert(s->n.max() == 4);
  delete s;
}

} // namespace

int main() {
  test_equality_assignment();
  test_equality_same_shape_intersection();
  test_equality_failure_and_aliases();
  test_equality_clone_with_active_propagator();
  test_disequality();
  test_reified_equality();
  test_reified_disequality();
  test_half_reification_rejected();
  test_concat_forward();
  test_concat_backward_suffix();
  test_concat_lengths_and_failure();
  test_concat_alias_rejected();
  test_length_still_works();

  std::cout << "Dashed native constraint tests passed\n";
  return 0;
}
