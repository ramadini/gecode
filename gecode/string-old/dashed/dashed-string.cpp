#include <gecode/string/dashed/dashed-string.hh>

namespace Gecode { namespace String {

  OutOfLimitsDS::OutOfLimitsDS(const char* l) :
    Exception(l, "Dashed string out of limits") {};

  EmptyDS::EmptyDS(const char* l) :
    Exception(l, "Empty dashed string.") {};

  UnknownValDS::UnknownValDS(const char* l) :
    Exception(l, "Access to unknown value.") {};

}}


namespace Gecode { namespace String {

  DSIntSet::DSIntSet(): USET() {};

  DSIntSet::DSIntSet(Space& h, int a, int b): USET(h, a, b) {
    if (a < 0 || b > DashedString::_MAX_STR_ALPHA)
      throw OutOfLimitsDS("DSIntSet::DSIntSet");
  };

  DSIntSet::DSIntSet(Space& h, const NSIntSet& s): USET() {
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

  DSIntSet::DSIntSet(Space& h, const DSIntSet& s): USET() {
    this->init(h, s);
  }

}}

namespace Gecode { namespace String {

  DSBlock::DSBlock(): l(0), u(0), S() {};

  DSBlock::DSBlock(Space& h, const DSBlock& b): l(0), u(0), S() {
    init(h, b);
  }

  DSBlock::DSBlock(Space& h, const NSBlock& b): l(b.l), u(b.u), S(h, b.S) {};

  DSBlock::DSBlock(Space& h, const DSIntSet& s, int a, int b): l(0), u(0), S() {
    if (b == 0 || a > b || s.size() == 0)
      return;
    if (a < 0)
      a = 0;
    if (b > DashedString::_MAX_STR_LENGTH)
      b = DashedString::_MAX_STR_LENGTH;
    l = a;
    u = b;
    S.init(h, s);
  };

}}

using Gecode::Support::DynamicArray;

namespace Gecode { namespace String {

  DSBlocks::DSBlocks(Space& h): DynamicArray(h, 1), _size(1) {
    x[0].init(h, DSBlock());
  }

  DSBlocks::DSBlocks(Space& h, int k): DynamicArray(h, k), _size(k) {
    if (k < 0 || k > DashedString::_MAX_STR_LENGTH)
      throw OutOfLimitsDS("DSBlocks::DSBlocks");
    for (int i = 0; i < k; ++i)
      x[i].init(h, DSBlock());
  }

  DSBlocks::DSBlocks(Space& h, int k, const DSBlock& b): DynamicArray(h, k),
  _size(k) {
    if (k < 0 || k > DashedString::_MAX_STR_LENGTH)
      throw OutOfLimitsDS("DSBlocks::DSBlocks");
    for (int i = 0; i < k; ++i)
      x[i].init(h, b);
  }

  DSBlocks::DSBlocks(Space& h, const DSBlocks& d): DynamicArray(h, d._size),
  _size(d._size) {
    for (int i = 0; i < _size; ++i)
      x[i].init(h, d.at(i));
  }

  DSBlocks::DSBlocks(Space& h, const NSBlocks& v):
  DynamicArray(h,(int) v.size()), _size(v.size()) {
    for (int i = 0; i < (int) v.size(); ++i)
      x[i].init(h, DSBlock(h, v[i]));
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
  // Characters that must appear in each solution.
  NSIntSet DashedString::_MUST_CHARS = NSIntSet();

  DashedString::DashedString(Space& h)
  : _changed(false), _blocks(h), _min_length(0), _max_length(0) {}

  DashedString::DashedString(Space& h, const DSBlock& b)
  : _changed(false), _blocks(h, 1, b), _min_length(b.l), _max_length(b.u) {}

  DashedString::DashedString(Space& h, NSBlocks& v, int mil, int mal)
  : _changed(false), _blocks(h) {
    if (v.empty())
      throw EmptyDS("DashedString::DashedString");
    if ((int) v.size() > _MAX_STR_LENGTH)
      throw OutOfLimitsDS("DashedString::DashedString");
    v.normalize();
    update(h, v);
    _min_length = max(_min_length, mil);
    _max_length = min(_max_length, mal);
  };

  DashedString::DashedString(Space& h, const std::string& s)
  : _changed(false), _blocks(h), _min_length(s.size()), _max_length(s.size()) {
    this->_blocks.update(h, s);
  }

  DashedString::DashedString(Space& h, const DashedString& d)
  : _changed(d._changed), _blocks(h, d._blocks), _min_length(d._min_length),
    _max_length(d._max_length) {}

}}
