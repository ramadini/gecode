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
  
  //TODO:
  
  forceinline int 
  StringVarImp::size() const {
    return ds.size();
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
  
  forceinline bool
  StringVarImp::assigned() const {
    return ds.isFixed();
  }
   
  forceinline const DashedString&
  StringVarImp::dom() const {
    return ds;  
  }
   
}}
