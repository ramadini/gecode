#include <gecode/string/var-imp/ds-equation.hpp>

namespace Gecode { namespace String {

  /*
   * Creation of new variable implementations
   *
   */

  forceinline
  StringVarImp::StringVarImp(Space& home)
  : StringVarImpBase(home), ds(home) {}

  forceinline
  StringVarImp::StringVarImp(Space& home, StringVarImp& x)
  : StringVarImpBase(home, x), ds(home, x.ds) {}
  
  forceinline
  StringVarImp::StringVarImp(Space& home, const Block& b)
  : StringVarImpBase(home), ds(home, b) {}
  
  forceinline
  StringVarImp::StringVarImp(Space& home, const DashedString& d)
  : StringVarImpBase(home), ds(home, d) {}
  
  forceinline
  StringVarImp::StringVarImp(Space& home, int l, int u)
  : StringVarImpBase(home), ds(home) {
    Limits::check_length(l, u, "StringVarImp::StringVarImp");
    ds.min_length(home, l);
    ds.max_length(home, u);
    ds.updateCardAt(home, 0, l, u);
  }
  
  forceinline
  StringVarImp::StringVarImp(Space& home, const IntSet& s, int l, int u)
  : StringVarImpBase(home), ds(home) {
    Limits::check_length(l, u, "StringVarImp::StringVarImp");
    Limits::check_length(s.min(), s.max(), "StringVarImp::StringVarImp");
    ds.min_length(home, l);
    ds.max_length(home, u);
    ds.updateAt(home, 0, Block(home, CharSet(home, s), l, u));
  }  
  
  forceinline void
  StringVarImp::update(Space& home, const DashedString& dy) {
    assert (!assigned());    
    int l = min_length(), u = max_length();    
    ds.update(home, dy);
    StringDelta d;
    if (assigned())
      notify(home, ME_STRING_VAL, d);
    else if (min_length() > l || max_length() < u)
      notify(home, ME_STRING_CARD, d);
    else
      notify(home, ME_STRING_BASE, d);
  }
  
  forceinline void
  StringVarImp::update(Space& home, const StringVarImp& y) {
    update(home, y.ds);
  }
  
  forceinline void
  StringVarImp::update_rev(Space& home, const StringVarImp& y) {
    ds.update_rev(home, y.ds);
  }
  
  forceinline void
  StringVarImp::update(Space& home, const std::vector<int>& w) {
    assert (!assigned());
    ds.update(home, w);
    StringDelta d;
    notify(home, ME_STRING_VAL, d);
  }
  
  forceinline void
  StringVarImp::update(Space& home, const StringVarImp& x, const StringVarImp& y) {
    ds.update(home, x.ds, y.ds);
  }
  
  forceinline ModEvent
  StringVarImp::nullify(Space& home) {
    if (min_length() > 0)
      return ME_STRING_FAILED;
    if (assigned())
      return ME_STRING_NONE;
    StringDelta d;
    ds.nullify(home);
    return notify(home, ME_STRING_VAL, d);    
  }
  
  forceinline bool
  StringVarImp::isNull() const {
    return ds.isNull();
  }  
  
  forceinline int 
  StringVarImp::size() const {
    return ds.size();
  }
  
  forceinline void
  StringVarImp::splitBlock(Space& home, int idx, int c, unsigned a) {
    ds.splitBlock(home, idx, c, a);
    StringDelta d;
    notify(home, assigned() ? ME_STRING_VAL : ME_STRING_CARD, d);
  }

  forceinline void
  StringVarImp::normalize(Space& home) {
    ds.normalize(home);
  }  

  forceinline const Block&
  StringVarImp::operator[](int i) const {
    return ds[i];
  }
  
  forceinline int
  StringVarImp::min_length() const {
    return min_len;
  }
  
  forceinline int
  StringVarImp::max_length() const {
    return max_len;
  }
  
  forceinline double
  StringVarImp::logdim() const {
    return ds.logdim();
  }
  
  forceinline bool
  StringVarImp::contains(const StringVarImp& y) const {
    return ds.contains(y.ds);
  }
  
  forceinline bool
  StringVarImp::equals(const StringVarImp& y) const {
    return ds.equals(y.ds);
  }
  
  forceinline bool
  StringVarImp::equals_rev(const StringVarImp& y) const {
    return ds.equals_rev(y.ds);
  }
  
  forceinline bool
  StringVarImp::contains_rev(const StringVarImp& y) const {
    return ds.contains_rev(y.ds);
  }
  
  forceinline ModEvent
  StringVarImp::bnd_length(Space& home, int l, int u) {
    if (l <= min_len && u >= max_len)
      return ME_STRING_NONE;
    if (l > max_len || u < min_len)
      return ES_FAILED;
    ModEvent me = ME_STRING_NONE;
    if (l > min_len)
      me = min_length(home, l);
    if (me == ME_STRING_FAILED)
      return me;
    if (u < max_len)
      me = me_combine(me, max_length(home, u));
    return me;
  }
  
  forceinline ModEvent
  StringVarImp::min_length(Space& home, int l) {
    if (l > max_len)
      return ME_STRING_FAILED;
    int lx = min_len;
    if (l <= lx)
      return ME_STRING_NONE;
    min_len = l;
    ds.min_length(home, min_len);
    StringDelta d;
    return notify(home, assigned() ? ME_STRING_VAL : ME_STRING_CARD, d);
  }
  
  forceinline ModEvent
  StringVarImp::max_length(Space& home, int u) {
    if (u < min_len)
      return ME_STRING_FAILED;
    if (u >= max_len)
      return ME_STRING_NONE;
    max_len = u;
    ds.max_length(home, max_len);
    StringDelta d;
    return notify(home, assigned() ? ME_STRING_VAL : ME_STRING_CARD, d);
  }

  forceinline std::vector<int>
  StringVarImp::val() const {
    return ds.val();
  }
  
  forceinline bool
  StringVarImp::assigned() const {
    return ds.isFixed();
  }
   
  forceinline const DashedString&
  StringVarImp::dom() const {
    return ds;  
  }
  
  forceinline bool
  StringVarImp::isOK() const {
    return min_len >= ds.lb_sum() && max_len <= ds.ub_sum() 
        && ds.isOK() && ds.isNorm();
  }
  
  forceinline void
  StringVarImp::subscribe(Space& home, Propagator& p, PropCond pc, 
                                                      bool schedule) {
    StringVarImpBase::subscribe(home, p, pc, assigned(), schedule);
  }

  forceinline void
  StringVarImp::reschedule(Space& home, Propagator& p, PropCond pc) {
    StringVarImpBase::reschedule(home,p,pc,assigned());
  }
  
  forceinline StringVarImp*
  StringVarImp::copy(Space& home) {
    return copied() ? static_cast<StringVarImp*>(forward())
                    : new (home) StringVarImp(home, *this);
  }
  
  forceinline void
  StringVarImp::nullifyAt(Space& home, int i) {
    ds.nullifyAt(home, i);
  }
  forceinline void
  StringVarImp::lbAt(Space& home, int i, int l) {
    ds.lbAt(home, i, l);
  }
  forceinline void
  StringVarImp::ubAt(Space& home, int i, int u) {
    ds.ubAt(home, i, u);
  }
  forceinline void
  StringVarImp::updateCardAt(Space& home, int i, int l, int u) {
    ds.updateCardAt(home, i, l, u);
  }
  forceinline void
  StringVarImp::updateAt(Space& home, int i, const Block& b) {
    ds.updateAt(home, i, b);
  }
  forceinline void
  StringVarImp::baseIntersectAt(Space& home, int idx, const Set::BndSet& S) {
    ds.baseIntersectAt(home, idx, S);
  }
  forceinline void
  StringVarImp::baseIntersectAt(Space& home, int idx, const Block& b) {
    ds.baseIntersectAt(home, idx, b);
  }
  
  template<class Char, class Traits>
  forceinline  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringVarImp& x) {
    os << x.dom();
    return os;
  };
   
}}
