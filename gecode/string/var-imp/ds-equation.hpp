namespace Gecode { namespace String {

  /// Struct abstracting a position in a dashed string. For a dashed 
  /// string X, a valid position (i,j)
  struct Position {
    /// Index of the position
    int idx;
    /// Offset of the position
    int off;
    /// Constructors
    forceinline Position(void): idx(0), off(0) {};
    forceinline Position(int i, int j): idx(i), off(j) {};
    /// Position equality.
    forceinline bool 
    operator ==(const Position& p) const {
      return idx == p.idx && off == p.off;
    }
    /// Position inequality.
    forceinline bool
    operator !=(const Position& p) const {
      return idx != p.idx || off != p.off;
    }
    /// Position lexicographic ordering.
    forceinline bool
    operator <(const Position& p) const {
      return idx < p.idx || (idx == p.idx && off < p.off); 
    }    
    forceinline bool
    operator >(const Position& p) const {
      return idx > p.idx || (idx == p.idx && off > p.off); 
    }
    forceinline bool
    operator <=(const Position& p) const {
      return idx < p.idx || (idx == p.idx && off <= p.off);
    }
    forceinline bool
    operator >=(const Position& p) const {
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
    
    /// TODO:
    template <class ViewY>
    forceinline void
    stretch_fwd(const Block& bx, const ViewY& y, typename ViewY::SweepFwdIterator it) {};
    template <class ViewY>
    forceinline void
    stretch_bwd(const Block& bx, const ViewY& y, typename ViewY::StretchBwdIterator it) {};
    
    
    /// TODO:
    template <class ViewX, class ViewY>
    forceinline bool
    init_x(Space& home, ViewX& x, const ViewY& y, Matching m[]) {
      typename ViewY::SweepFwdIterator fwd_it = x.sweep_fwd_iterator();
      int n = x.size();
      for (int i = 0; i < n; ++i) {
        fwd_it = stretch_fwd(x[i], y, fwd_it);
        m[i].LEP = *fwd_it;
      }
      if (m[n-1].LEP < Position(n,0))
        return false;
      typename ViewY::StretchBwdIterator bwd_it = x.stretch_bwd_iterator();
      for (int i = n-1; i >= 0; --i) {
        bwd_it = stretch_bwd(x[i], y, bwd_it);
        m[i].ESP = *bwd_it;
      }
      return m[0].ESP >= Position(0,0);
    }
    
    /// TODO:
    template <class ViewX, class ViewY>
    forceinline bool
    sweep_x(Space& home, ViewX& x, const ViewY& y, Matching m[]) {
      if (!init_x(home, x, y))
        return false;
      //TODO:
      refine_x(home, x, y, m);
      return true;
    }
    
    /// TODO: 
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
