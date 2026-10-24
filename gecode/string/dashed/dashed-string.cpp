#include <gecode/string/dashed/dashed-string.hh>

namespace Gecode { namespace String {

  OutOfLimitsDS::OutOfLimitsDS(const char* l)
    : Exception(l, "Dashed string out of limits") {}

  EmptyDS::EmptyDS(const char* l)
    : Exception(l, "Empty dashed string.") {}

  UnknownValDS::UnknownValDS(const char* l)
    : Exception(l, "Access to unknown value.") {}

}}


namespace Gecode { namespace String {

  DSIntSet::DSIntSet(void)
    : Gecode::Set::LUBndSet() {}

  DSIntSet::DSIntSet(Space& h, int a, int b)
    : Gecode::Set::LUBndSet(h,a,b) {
    if (a < 0 || b > DashedString::_MAX_STR_ALPHA)
      throw OutOfLimitsDS("DSIntSet::DSIntSet");
  }

  DSIntSet::DSIntSet(Space& h, const NSIntSet& s)
    : Gecode::Set::LUBndSet() {
    int n = s.size();
    _size = n;
    if (s.empty())
      return;
    n = s.length();
    RangeList* r = h.alloc<RangeList>(n);
    fst(r);
    lst(r + n - 1);
    NSRange* p = s.first();
    for (int i = 0; i < n; ++i) {
      r[i].min(p->l);
      r[i].max(p->u);
      r[i].next(&r[i + 1]);
      p = p->next;
    }
    r[n - 1].next(NULL);
  }

  DSIntSet::DSIntSet(Space& h, const DSIntSet& s)
    : Gecode::Set::LUBndSet() {
    this->init(h, s);
  }

}}

namespace Gecode { namespace String {

  DSBlock::DSBlock(void)
    : l(0), u(0), S() {}

  // Space/Heap reallocation moves only the wrapper; the ranges remain
  // allocator-owned.
  DSBlock::DSBlock(const DSBlock& b)
    : l(b.l), u(b.u), S() {
    static_cast<Set::BndSet&>(S) =
      static_cast<const Set::BndSet&>(b.S);
  }

  DSBlock&
  DSBlock::operator=(const DSBlock& b) {
    if (this != &b) {
      l = b.l;
      u = b.u;
      static_cast<Set::BndSet&>(S) =
        static_cast<const Set::BndSet&>(b.S);
    }
    return *this;
  }

  DSBlock::DSBlock(Space& h, const DSBlock& b)
    : l(0), u(0), S() {
    init(h, b);
  }

  DSBlock::DSBlock(Space& h, const NSBlock& b)
    : l(b.l), u(b.u), S(h,b.S) {}

  DSBlock::DSBlock(Space& h, const DSIntSet& s, int a, int b)
    : l(0), u(0), S() {
    if (b == 0 || a > b || s.size() == 0)
      return;
    if (a < 0)
      a = 0;
    if (b > DashedString::_MAX_STR_LENGTH)
      b = DashedString::_MAX_STR_LENGTH;
    l = a;
    u = b;
    S.init(h, s);
  }

  DSBlock::DSBlock(Space& h, int min, int max, int a, int b)
    : l(0), u(0), S() {
    if (b == 0 || a > b || min > max)
      return;
    if (a < 0)
      a = 0;
    if (b > DashedString::_MAX_STR_LENGTH)
      b = DashedString::_MAX_STR_LENGTH;
    l = a;
    u = b;
    S.init(h, min, max);
  }

}}

using Gecode::Support::DynamicArray;

namespace Gecode { namespace String {

  DSBlocks::DSBlocks(Space& h)
    : DynamicArray(h,1), _size(1) {
    x[0].init(h, DSBlock());
  }

  DSBlocks::DSBlocks(Space& h, int k)
    : DynamicArray(h,k), _size(k) {
    if (k < 0 || k > DashedString::_MAX_STR_LENGTH)
      throw OutOfLimitsDS("DSBlocks::DSBlocks");
    for (int i = 0; i < k; ++i)
      x[i].init(h, DSBlock());
  }

  DSBlocks::DSBlocks(Space& h, int k, const DSBlock& b)
    : DynamicArray(h,k), _size(k) {
    if (k < 0 || k > DashedString::_MAX_STR_LENGTH)
      throw OutOfLimitsDS("DSBlocks::DSBlocks");
    for (int i = 0; i < k; ++i)
      x[i].init(h, b);
  }

  DSBlocks::DSBlocks(Space& h, const DSBlocks& d)
    : DynamicArray(h,d._size), _size(d._size) {
    for (int i = 0; i < _size; ++i)
      x[i].init(h, d.at(i));
  }

  DSBlocks::DSBlocks(Space& h, const NSBlocks& v)
    : DynamicArray(h,(int) v.size()), _size(v.size()) {
    for (int i = 0; i < (int) v.size(); ++i)
      x[i].init(h, v[i]);
  }

}}

namespace Gecode { namespace String {

  // Default maximum length is 65535 = 2^16 - 1.
  int DashedString::_MAX_STR_LENGTH = 65535;
  // Default alphabet is extended ASCII, isomorphic to [0, 255].
  int DashedString::_MAX_STR_ALPHA = 255;
  // When possible, decompose a regular expression into basic constraints.
  bool DashedString::_DECOMP_REGEX = true;
  // Use reverse propagation for regular expressions.
  bool DashedString::_REVERSE_REGEX = true;
  // Use quadratic version of sweep algorithm.
  bool DashedString::_QUAD_SWEEP = false;

  DashedString::DashedString(Space& h)
    : _changed(false), _blocks(h), _min_length(0), _max_length(0) {}

  namespace {

    void
    check_length_bounds(int l, int u) {
      if (l < 0 || u < 0 || l > DashedString::_MAX_STR_LENGTH ||
          u > DashedString::_MAX_STR_LENGTH)
        throw OutOfLimitsDS("DashedString::DashedString");
      if (l > u)
        throw EmptyDS("DashedString::DashedString");
    }

  }

  DashedString::DashedString(Space& h, const DSBlock& b)
    : _changed(false), _blocks(h), _min_length(0), _max_length(0) {
    check_length_bounds(b.l, b.u);
    if (b.S.empty() && b.l > 0)
      throw EmptyDS("DashedString::DashedString");
    if (b.u == 0 || b.S.empty())
      return;
    _blocks.at(0).update(h, b);
    _min_length = b.l;
    _max_length = b.u;
  }

  DashedString::DashedString(Space& h, NSBlocks& v, int mil, int mal)
    : _changed(false), _blocks(h), _min_length(0), _max_length(0) {
    check_length_bounds(mil, mal);
    if (v.empty())
      throw EmptyDS("DashedString::DashedString");
    if (v.size() > static_cast<NSBlocks::size_type>(_MAX_STR_LENGTH))
      throw OutOfLimitsDS("DashedString::DashedString");
    int l = 0, u = 0;
    for (const NSBlock& block : v) {
      check_length_bounds(block.l, block.u);
      if ((block.S.empty() && block.l > 0) ||
          block.l > _MAX_STR_LENGTH - l)
        throw EmptyDS("DashedString::DashedString");
      l += block.l;
      if (!block.S.empty())
        u += min(block.u, _MAX_STR_LENGTH - u);
    }
    l = max(l, mil);
    u = min(u, mal);
    if (l > u)
      throw EmptyDS("DashedString::DashedString");
    v.normalize();
    if (u == 0)
      return;
    update(h, v);
    _min_length = l;
    _max_length = u;
    if (length() == 1) {
      at(0).l = l;
      at(0).u = u;
    }
    assert (is_normalized());
  }

  DashedString::DashedString(Space& h, const std::string& s)
    : _changed(false), _blocks(h), _min_length(0), _max_length(0) {
    if (s.size() > static_cast<std::string::size_type>(_MAX_STR_LENGTH))
      throw OutOfLimitsDS("DashedString::DashedString");
    _min_length = _max_length = static_cast<int>(s.size());
    this->_blocks.update(h, s);
  }

  DashedString::DashedString(Space& h, const StringVal& value)
    : _changed(false), _blocks(h), _min_length(0), _max_length(0) {
    if (value.size() > static_cast<StringVal::size_type>(_MAX_STR_LENGTH))
      throw OutOfLimitsDS("DashedString::DashedString");
    _min_length = _max_length = static_cast<int>(value.size());
    this->_blocks.update(h, value);
  }

  DashedString::DashedString(Space& h, const DashedString& d)
    : _changed(d._changed), _blocks(h, d._blocks),
      _min_length(d._min_length), _max_length(d._max_length) {}

  bool
  DashedString::equate(Space& h, DashedString& that) {
    _changed = that._changed = false;
    if (!refine_card_eq(h, that))
      return false;
    if (known()) {
      string s;
      if (try_val_bytes(s)) {
        if (that.known()) {
          string t;
          return that.try_val_bytes(t) && s == t;
        }
        if (check_sweep<char, string, DSBlock, DSBlocks>(s, that._blocks)) {
          that.update(h, s);
          that._changed = true;
          return true;
        }
        return false;
      }
      if (that.known())
        return equals(that);
      if (check_sweep<DSBlock, DSBlocks, DSBlock, DSBlocks>(
            _blocks, that._blocks)) {
        that.update(h, *this);
        that._changed = true;
        return true;
      }
      return false;
    }
    if (that.known()) {
      string s;
      if (that.try_val_bytes(s)) {
        if (check_sweep<DSBlock, DSBlocks, char, string>(_blocks, s)) {
          update(h, s);
          _changed = true;
          return true;
        }
        return false;
      }
      if (check_sweep<DSBlock, DSBlocks, DSBlock, DSBlocks>(
            _blocks, that._blocks)) {
        update(h, that);
        _changed = true;
        return true;
      }
      return false;
    }
    if (contains(that)) {
      if (*this == that)
        return true;
      update(h, that);
      _changed = true;
      return true;
    }
    if (that.contains(*this)) {
      that.update(h, *this);
      that._changed = true;
      return true;
    }
    if (!sweep_equate(h, *this, that))
      return false;
    if ((_changed || that._changed) && !refine_card_eq(h, that))
      return false;
    return true;
  }

  bool
  DashedString::equate(Space& h, const StringVal& that) {
    _changed = false;
    if (!check_equate(that))
      return false;
    if (known())
      return val_symbols() == that;
    update(h, that);
    _changed = true;
    return true;
  }

}}

// STATISTICS: string-var
