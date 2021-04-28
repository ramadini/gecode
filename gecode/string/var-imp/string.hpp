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
  StringVarImp::update(Space& home, const DashedString& d) {
    ds.update(home, d);
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
  StringVarImp::max_length() const {
    return ds.max_length();
  }
  
  forceinline int
  StringVarImp::min_length() const {
    return ds.min_length();
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
  
  template<class Char, class Traits>
  forceinline  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringVarImp& x) {
    os << x.dom();
    return os;
  };
   
}}
