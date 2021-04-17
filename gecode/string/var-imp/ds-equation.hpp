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
    /// Check if the position is within dashed string \a x. If |x|=n, the valid
    /// positions within x are {(i,j) | 0 <= i < n, 0 <= j < x[i]} U {(n,0)}
    forceinline bool 
    in(const DashedString& x) {
      int n = x.size();
      return idx >= 0 && off >= 0 && 
            ((idx < n && off < x[idx].ub()) || (idx == n && off == 0));
    }
    /// Position equality.
    forceinline bool
    operator==(const Position& p) { return idx == p.idx && off == p.off; }
    /// Position inequality.
    forceinline bool
    operator!=(const Position& p) { return !(*this == p); }
  };
  
  /// Struct defining a matching region.
  struct Matching {
    /// Earliest start position.
    Position ESP;
    /// Latest start position.
    Position LSP;
    /// Earliest end position.
    Position EEP;
    /// Latest end position.
    Position LEP;  
  };
  
}}

namespace Gecode { namespace String {
 
    // FIXME: Maybe define a namespace for equate-based functions?
    
    
    /// Possibly refines x[i] according to its matching region m[i] in y. 
    /// It returns true iff at least a block has been refined.
    template <class ViewX, class ViewY>
    bool refine_x(Space& home, ViewX& x, const ViewY& y, Matching m[]) {
      //TODO:
      return true;
    } 
    
    template <class ViewX, class ViewY>
    forceinline bool
    sweep_x(Space& home, ViewX& x, const ViewY& y, Matching m[]) {
      //TODO:
      refine_x(home, x, y, m);
      return true;
    }
     
    template <class ViewX, class ViewY>
    forceinline ModEvent
    equate_x(Space& home, ViewX& x, const ViewY& y) {
      int lb = x.min_length(), ub = x.max_length();
      Region r;
      { 
        Matching* m = r.alloc<Matching>(x.size());
        if (!sweep_x(home, x, y, m))
          return ME_STRING_FAILED;
        if (!refine_x(home, x, y, m))
          return ME_STRING_NONE;
        r.free();
      }
      if (x.assigned())
        return ME_STRING_VAL;
      else if (x.min_length() > lb || x.max_length() < ub)
        return ME_STRING_CARD;
      else
        return ME_STRING_BASE;
    }

}}
