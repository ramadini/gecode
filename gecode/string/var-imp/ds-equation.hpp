namespace Gecode { namespace String {

  /// Struct abstracting a position in a dashed string. For a dashed 
  /// string X, a valid position (i,j)
  struct Position {
    /// Index of the position
    int idx;
    /// Offset of the position
    int off;
    /// Position equality.
    forceinline bool
    operator ==(const Position& p) {
      return idx == p.idx && off == p.off;
    }
    /// Position inequality.
    forceinline bool
    operator !=(const Position& p) {
      return idx != p.idx || off != p.off;
    }
    /// Position lexicographic ordering.
    forceinline bool
    operator <(const Position& p) {
      return idx < p.idx || (idx == p.idx && off < p.off); 
    }    
    forceinline bool
    operator >(const Position& p) {
      return idx > p.idx || (idx == p.idx && off > p.off); 
    }
    forceinline bool
    operator <=(const Position& p) {
      return idx < p.idx || (idx == p.idx && off <= p.off);
    }
    forceinline bool
    operator >=(const Position& p) {
      return idx > p.idx || (idx == p.idx && off >= p.off);
    }
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
  
  /// Struct to unfold the i-th block of a dashed string with the n>1 blocks 
  /// defined in array b.
  struct BlockUnfold {
    int i;
    int n;
    Block* b;
  };
  typedef Gecode::Support::DynamicArray<BlockUnfold,Space> BlockUnfoldings;
  
}}

namespace Gecode { namespace String {
 
    // FIXME: Maybe define a namespace for equate-based functions?
    
    // Add m blocks to x, according to the unfoldings array.
    template <class ViewX>
    void unfold(ViewX x, BlockUnfoldings unfoldings, int m) {
      //
    }
    
    
    /// Possibly refines x[i] according to its matching region m[i] in y. 
    /// It returns true iff at least a block has been refined.
    template <class ViewX, class ViewY>
    bool refine_x(Space& home, ViewX& x, const ViewY& y, Matching m[]) {
      //TODO:
      { 
        // FIXME: Just playing around.
        BlockUnfoldings unfoldings;
        int m = 0;
        for (int i = 0; i < x.size(); ++i) {
          if (1) {
            BlockUnfold bu;
            bu.b = {Block(), Block(home), Block(home, 1, 2)};
            bu.b[0].update(home, x[2]);
            bu.n = 5;
            bu.i = i;
            m++;
          }
        }
        if (m > 0)
          unfold(x, unfoldings, m);
      }
      x.normalize(home);
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
      Matching m[x.size()];
      if (!sweep_x(home, x, y, m))
        return ME_STRING_FAILED;
      if (!refine_x(home, x, y, m))
        return ME_STRING_NONE;
      if (x.assigned())
        return ME_STRING_VAL;
      else if (x.min_length() > lb || x.max_length() < ub)
        return ME_STRING_CARD;
      else
        return ME_STRING_BASE;
    }

}}
