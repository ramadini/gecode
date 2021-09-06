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
  StringVarImp::StringVarImp(Space& home, int n)
  : StringVarImpBase(home), ds(home, n), min_len(0), max_len(0) {
    assert (isOK());
  } 

  forceinline
  StringVarImp::StringVarImp(Space& home, StringVarImp& x)
  : StringVarImpBase(home, x), ds(home), min_len(x.min_len), 
                                         max_len(x.max_len) {
    ds.update(home, x.ds);
    assert (isOK());                                             
  }
  
  forceinline
  StringVarImp::StringVarImp(Space& home, const Block& b)
  : StringVarImpBase(home), ds(home, b), min_len(b.lb()), max_len(b.ub()) {
    assert (isOK());
  }
  
  forceinline
  StringVarImp::StringVarImp(Space& home, const DashedString& d)
  : StringVarImpBase(home), ds(home), min_len(d.lb_sum()) {
    ds.update(home, d);
    max_len = std::min((long) MAX_STRING_LENGTH, d.ub_sum()); 
    assert (isOK()); 
  }
  
  forceinline
  StringVarImp::StringVarImp(Space& home, int l, int u)
  : StringVarImpBase(home), ds(home), min_len(l), max_len(u) {
    Limits::check_length(l, u, "StringVarImp::StringVarImp");
    ds.updateCardAt(home, 0, l, u);
    assert (isOK());
  }
  
  forceinline
  StringVarImp::StringVarImp(Space& home, const IntSet& s, int l, int u)
  : StringVarImpBase(home), ds(home), min_len(l), max_len(u) {
    Limits::check_length(l, u, "StringVarImp::StringVarImp");
    Limits::check_alphabet(s.min(), s.max(), "StringVarImp::StringVarImp");
    ds.updateAt(home, 0, Block(home, CharSet(home, s), l, u));
    assert (isOK());
  }  
  
  forceinline void
  StringVarImp::gets(Space& home, const DashedString& dy) {
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
  StringVarImp::gets(Space& home, const StringVarImp& y) {
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
  StringVarImp::gets_rev(Space& home, const StringVarImp& y) {
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
  StringVarImp::gets(Space& home, const std::vector<int>& w) {
    assert (!assigned());
    ds.update(home, w);
    min_len = max_len = w.size();
    StringDelta d;
    notify(home, ME_STRING_VAL, d);
    assert (isOK());
  }
  
  forceinline void
  StringVarImp::gets(Space& home, const StringVarImp& x, 
                                    const StringVarImp& y) {
    assert (!assigned());
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
  
  forceinline ModEvent
  StringVarImp::splitBlock(Space& home, int idx, int c, unsigned a) {
    assert (min_len == max_len);
    ds.splitBlock(home, idx, c, a);
    assert (isOK());
    return assigned() ? ME_STRING_VAL : ME_STRING_CARD;
  }

  forceinline void
  StringVarImp::normalize(Space& home) {std::cerr << *this << '\n';
    ds.normalize(home);
    std::cerr << *this << '\n';
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
    if (min_len > y.min_len || max_len < y.max_len)
      return false;
    return ds.contains(y.ds);
  }
  
  forceinline bool
  StringVarImp::equals(const StringVarImp& y) const {
    if (min_len != y.min_len || max_len != y.max_len)
      return false;
    return ds.equals(y.ds);
  }
  
  forceinline bool
  StringVarImp::equals_rev(const StringVarImp& y) const {
    if (min_len != y.min_len || max_len != y.max_len)
      return false;
    return ds.equals_rev(y.ds);
  }
  
  forceinline bool
  StringVarImp::contains_rev(const StringVarImp& y) const {
     if (min_len > y.min_len || max_len < y.max_len)
      return false;
    return ds.contains_rev(y.ds);
  }
  
  forceinline ModEvent
  StringVarImp::bnd_length(Space& home, int l, int u) {
    return me_combine(min_length(home, l), max_length(home, u));
  }
  
  forceinline ModEvent
  StringVarImp::min_length(Space& home, int l) {
    if (l > max_len)
      return ME_STRING_FAILED;
    if (l > min_len) {
      min_len = l;
      ds.min_length(home, min_len);      
    }
    else if (l > ds.lb_sum())
      ds.min_length(home, l);
    else
      return ME_STRING_NONE;
    assert (isOK());
    StringDelta d;
    return notify(home, assigned() ? ME_STRING_VAL : ME_STRING_CARD, d);
  }
  
  forceinline ModEvent
  StringVarImp::max_length(Space& home, int u) {std::cerr << u << '\n';
    if (u < min_len)
      return ME_STRING_FAILED;
    if (u < max_len) {
      max_len = u;
      ds.max_length(home, max_len);
    }
    else if (u < ds.ub_sum())
      ds.max_length(home, u);
    else
      return ME_STRING_NONE;
    assert (isOK());
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
  StringVarImp::copy(Space& home) {std::cerr<<*this <<'\n';
    return copied() ? static_cast<StringVarImp*>(forward())
                    : new (home) StringVarImp(home, *this);
  }
  
  forceinline ModEvent
  StringVarImp::nullifyAt(Space& home, int i) {
    ds.nullifyAt(home, i);
    return bnd_length(home, min_len, std::min((long) max_len, ds.ub_sum()));
  }
  forceinline ModEvent
  StringVarImp::lbAt(Space& home, int i, int l) {std::cerr<<"lbAt "<<*this<< ' ' << i << ' ' << l << '\n';
    ds.lbAt(home, i, l);
    return bnd_length(home, std::max(min_len, ds.lb_sum()), max_len);
  }
  forceinline ModEvent
  StringVarImp::ubAt(Space& home, int i, int u) {std::cerr<<"ubAt "<<*this<< ' ' << i << ' ' << u << '\n';
    ds.ubAt(home, i, u);
    return bnd_length(home, min_len, std::min((long) max_len, ds.ub_sum()));
  }  
  forceinline ModEvent
  StringVarImp::baseIntersectAt(Space& home, int i, const Set::BndSet& S) {
    ds.baseIntersectAt(home, i, S);
    return bnd_length(home, std::max(min_len, ds.lb_sum()), 
                            std::min((long) max_len, ds.ub_sum()));
  }
  forceinline ModEvent
  StringVarImp::baseIntersectAt(Space& home, int i, const Block& b) {
    ds.baseIntersectAt(home, i, b);
    return bnd_length(home, std::max(min_len, ds.lb_sum()), 
                            std::min((long) max_len, ds.ub_sum()));
  }
  forceinline void
  StringVarImp::updateCardAt(Space& home, int i, int l, int u) {
    min_len += l - ds[i].lb();
    max_len  = ds.ub_sum() < MAX_STRING_LENGTH ? max_len + u - ds[i].ub()
                                               : MAX_STRING_LENGTH;
    ds.updateCardAt(home, i, l, u);
  }
  forceinline void
  StringVarImp::updateAt(Space& home, int i, const Block& b) {
    min_len += b.lb() - ds[i].lb();
    max_len  = ds.ub_sum() < MAX_STRING_LENGTH ? max_len + b.ub() - ds[i].ub()
                                               : MAX_STRING_LENGTH;
    ds.updateAt(home, i, b);
  }
  
  template<class Char, class Traits>
  forceinline  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringVarImp& x) {
    os << x.dom() << " |" << x.min_length() << ".." << x.max_length() << "|";
    return os;
  };
   
}}
