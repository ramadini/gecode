#include <gecode/string/var-imp/ds-equation.hpp>

namespace Gecode { namespace String {

  /*
   * Creation of new variable implementations
   *
   */
  
  //TODO:
  
  
//  forceinline ModEvent
//  StringVarImp::equate(Space& home, const DashedString& x) {
//    return equate_x(home, ds, x);
//  }

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
   
   
}}
