#include <gecode/string/var-imp/ds-equation.hpp>

namespace Gecode { namespace String {

  /*
   * Creation of new variable implementations
   *
   */

  forceinline
  StringVarImp::StringVarImp(Space& home)
  : StringVarImpBase(home), ds(home), min_len(0), max_len(MAX_STRING_LENGTH) {
    assert (isOK());
  }

  forceinline
  StringVarImp::StringVarImp(Space& home, StringVarImp& x)
  : StringVarImpBase(home, x), ds(home, x.ds), min_len(x.min_len), 
                                               max_len(x.max_len) {
    assert (isOK());                                             
  }
  
  forceinline
  StringVarImp::StringVarImp(Space& home, const Block& b)
  : StringVarImpBase(home), ds(home, b), min_len(b.lb()), max_len(b.ub()) {
    assert (isOK());
  }
  
  forceinline
  StringVarImp::StringVarImp(Space& home, const DashedString& d)
  : StringVarImpBase(home), ds(home, d), min_len(d.lb_sum()), 
                                         max_len(d.ub_sum()) { 
    assert (isOK()); 
  }
  
  forceinline
  StringVarImp::StringVarImp(Space& home, int l, int u)
  : StringVarImpBase(home), ds(home), min_len(l), max_len(u) {
    Limits::check_length(l, u, "StringVarImp::StringVarImp");
    ds.min_length(home, l);
    ds.max_length(home, u);
    ds.updateCardAt(home, 0, l, u);
    assert (isOK());
  }
  
  forceinline
  StringVarImp::StringVarImp(Space& home, const IntSet& s, int l, int u)
  : StringVarImpBase(home), ds(home), min_len(l), max_len(u) {
    Limits::check_length(l, u, "StringVarImp::StringVarImp");
    Limits::check_length(s.min(), s.max(), "StringVarImp::StringVarImp");
    ds.min_length(home, l);
    ds.max_length(home, u);
    ds.updateAt(home, 0, Block(home, CharSet(home, s), l, u));
    assert (isOK());
  }  
  
  forceinline void
  StringVarImp::update(Space& home, const DashedString& dy) {
    assert (!assigned());
    int l = min_length(), u = max_length();    
    ds.update(home, dy);
    min_len = dy.lb_sum();
    max_len = dy.ub_sum();
    StringDelta d;
    if (assigned())
      notify(home, ME_STRING_VAL, d);
    else if (min_length() > l || max_length() < u)
      notify(home, ME_STRING_CARD, d);
    else
      notify(home, ME_STRING_BASE, d);
    assert (isOK());  
  }
  
  forceinline void
  StringVarImp::update(Space& home, const StringVarImp& y) {
    assert (!assigned());
    int l = min_length(), u = max_length();    
    ds.update(home, y.ds);
    min_len = y.min_length();
    max_len = y.max_length();
    StringDelta d;
    if (assigned())
      notify(home, ME_STRING_VAL, d);
    else if (min_length() > l || max_length() < u)
      notify(home, ME_STRING_CARD, d);
    else
      notify(home, ME_STRING_BASE, d);
    assert (isOK());
  }
  
  forceinline void
  StringVarImp::update_rev(Space& home, const StringVarImp& y) {
    assert (!assigned());
    int l = min_length(), u = max_length();    
    ds.update_rev(home, y.ds);
    min_len = y.min_length();
    max_len = y.max_length();
    StringDelta d;
    if (assigned())
      notify(home, ME_STRING_VAL, d);
    else if (min_length() > l || max_length() < u)
      notify(home, ME_STRING_CARD, d);
    else
      notify(home, ME_STRING_BASE, d);
    assert (isOK());
  }
  
  forceinline void
  StringVarImp::update(Space& home, const std::vector<int>& w) {
    assert (!assigned());
    ds.update(home, w);
    min_len = max_len = w.size();
    StringDelta d;
    notify(home, ME_STRING_VAL, d);
    assert (isOK());
  }
  
  forceinline void
  StringVarImp::update(Space& home, const StringVarImp& x, const StringVarImp& y) {
    ds.update(home, x.ds, y.ds);
    min_len = x.min_length() + y.min_length();
    max_len = x.max_length() + y.max_length();
    assert (isOK());
  }
  
  forceinline ModEvent
  StringVarImp::nullify(Space& home) {
    if (min_len > 0)
      return ME_STRING_FAILED;
    if (assigned()) {
      assert (isNull());
      return ME_STRING_NONE;
    }
    StringDelta d;
    ds.nullify(home);
    max_len = 0;
    assert (isOK());
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
    assert (min_len == max_len);
    ds.splitBlock(home, idx, c, a);
    StringDelta d;
    notify(home, assigned() ? ME_STRING_VAL : ME_STRING_CARD, d);
    assert (isOK());
  }

  forceinline void
  StringVarImp::normalize(Space& home) {
    ds.normalize(home);
    assert (isOK());
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
    if (l > min_len) {
      min_len = l;
      me = min_length(home, l);
    }
    if (me == ME_STRING_FAILED)
      return me;
    if (u < max_len) {
      max_len = u;
      me = me_combine(me, max_length(home, u));
    }
    assert (isOK());
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
    assert (isOK());
    StringDelta d;
    return notify(home, assigned() ? ME_STRING_VAL : ME_STRING_CARD, d);
  }
  
  forceinline ModEvent
  StringVarImp::max_length(Space& home, int u, bool dom) {
    if (u < min_len)
      return ME_STRING_FAILED;
    if (u >= max_len)
      return ME_STRING_NONE;
    max_len = u;
    if (dom)
      ds.max_length(home, max_len);
    assert (isOK());
    StringDelta d;
    return notify(home, assigned() ? ME_STRING_VAL : ME_STRING_CARD, d);
  }

  forceinline void 
  StringVarImp::sync_length() {
    if (ds.lb_sum() > min_len)
      min_len = ds.lb_sum();
    if (ds.ub_sum() < max_len)
      max_len = ds.ub_sum();
    assert (isOK());
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
    return min_len >= ds.lb_sum() && max_len <= ds.ub_sum() && ds.isOK();
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
    if (ds.ub_sum() < max_len)
      max_len = ds.ub_sum();
    assert (isOK());
  }
  forceinline void
  StringVarImp::lbAt(Space& home, int i, int l) {
    ds.lbAt(home, i, l);
    if (ds.lb_sum() > min_len)
      min_len = ds.lb_sum();
    assert (isOK());
  }
  forceinline void
  StringVarImp::ubAt(Space& home, int i, int u) {
    ds.ubAt(home, i, u);
    if (ds.ub_sum() < max_len)
      max_len = ds.ub_sum();
    assert (isOK());
  }
  forceinline void
  StringVarImp::updateCardAt(Space& home, int i, int l, int u) {
    ds.updateCardAt(home, i, l, u);
    sync_length();
    assert (isOK());
  }
  forceinline void
  StringVarImp::updateAt(Space& home, int i, const Block& b) {
    ds.updateAt(home, i, b);
    sync_length();
    assert (isOK());
  }
  forceinline void
  StringVarImp::baseIntersectAt(Space& home, int idx, const Set::BndSet& S) {
    ds.baseIntersectAt(home, idx, S);
    sync_length();
    assert (isOK());
  }
  forceinline void
  StringVarImp::baseIntersectAt(Space& home, int idx, const Block& b) {
    ds.baseIntersectAt(home, idx, b);
    sync_length();
    assert (isOK());
  }
  
  template<class Char, class Traits>
  forceinline  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringVarImp& x) {
    os << x.dom();
    return os;
  };
   
}}
