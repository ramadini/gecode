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
  
  forceinline void
  StringVarImp::gets(Space& home, const DashedString& dy) {
    assert (!assigned() && !ds.equals(dy));
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
  StringVarImp::gets(Space& home, const StringVarImp& y) {
    gets(home, y.ds);
  }
  
  forceinline void
  StringVarImp::gets(Space& home, const std::vector<int>& w) {
    assert (!assigned());
    ds.update(home, w);
    StringDelta d;
    notify(home, ME_STRING_VAL, d);
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
  StringVarImp::normalize(Space& home) {
    ds.normalize(home);
  }  

  forceinline Block&
  StringVarImp::operator[](int i) {
    return ds[i];
  }  
  forceinline const Block&
  StringVarImp::operator[](int i) const {
    return ds[i];
  }
  
  forceinline int
  StringVarImp::min_length() const {
    return ds.min_length();
  }
  
  forceinline int
  StringVarImp::max_length() const {
    return ds.max_length();
  }
  
  forceinline bool
  StringVarImp::contains(const StringVarImp& y) const {
    return ds.contains(y.ds);
  }
  
  forceinline bool
  StringVarImp::equals(const StringVarImp& y) const {
    return ds.equals(y.ds);
  }
  
  forceinline ModEvent
  StringVarImp::min_length(Space& home, int l) {
    if (l > max_length())
      return ME_STRING_FAILED;
    int lx = min_length();
    if (l <= lx)
      return ME_STRING_NONE;
    ds.min_length(home, l);
    StringDelta d;
    return notify(home, assigned() ? ME_STRING_VAL : ME_STRING_CARD, d);
  }
  
  forceinline ModEvent
  StringVarImp::bnd_length(Space& home, int l, int u) {
    int lx = min_length(), ux = max_length();    
    if (l < lx || u > ux)
      return ME_STRING_NONE;
    if (l > ux || u < lx)
      return ES_FAILED;
    ModEvent me = ME_STRING_NONE;
    if (l > lx)
      me = min_length(home, l);
    if (u < ux)
      me = me_combine(me, max_length(home, u));
    return me;
  }
  
  forceinline ModEvent
  StringVarImp::max_length(Space& home, int u) {
    if (u < min_length())
      return ME_STRING_FAILED;
    int ux = max_length();
    if (u >= ux)
      return ME_STRING_NONE;
    ds.max_length(home, u);
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
    return ds.isOK() && ds.isNorm();
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
  
  template<class Char, class Traits>
  forceinline  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringVarImp& x) {
    os << x.dom();
    return os;
  };
   
}}
