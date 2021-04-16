namespace Gecode { namespace String {

  /// Immutable struct abstracting a position in a dashed string. For a dashed 
  /// string X, a valid position (i,j)
  struct Position {
    /// Index of the position
    const int idx;
    /// Offset of the position
    const int off;
    /// Constructors
    Position(): idx(0), off(0) {};
    Position(int i): idx(i), off(0) {};
    Position(int i, int o): idx(i), off(o) {};
    /// Check if the position is valid for dashed string \a x. If |x|=n, the valid 
    /// positions for x are {(i,j) | 0 <= i < n, 0 <= j < x[i]} U {(n,0)}.
    forceinline bool validFor(const DashedString& x) {
      int n = x.size();
      return idx >= 0 && off >= 0 && 
            ((idx < n && off < x[idx].ub()) || (idx == n && off == 0));
    }
  };
  
}}

namespace Gecode { namespace String {
 
    // FIXME: Maybe define a namespace for equate-based functions?
    
    forceinline ModEvent
    equate_x(Space& home, DashedString& x, const DashedString& y) {
     
      return ME_STRING_NONE;
    }

}}
