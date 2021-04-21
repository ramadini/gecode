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
  forceinline std::ostream&
  operator<<(std::ostream& os, const Position& p) {
    os << "(" << p.idx << "," << p.off << ")";
    return os;
  }
  
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
  bool refine_x(Space& home, ViewX x, const ViewY& y, Matching m[]) {

    return true;
  }
  
  /// TODO:
  template <bool fwd, class IterY>
  forceinline Position
  push(const Block& bx, IterY& it) {
//    std::cerr << "Pushing " << (fwd ? "fwd " : "bwd ") << bx << " from " << *it << '\n';
    Position p = *it;
    // No. of chars. that must be consumed
    int k = bx.lb(); 
    while (k > 0) {
//      std::cerr << "k=" << k << ", it=" << *it << std::endl;
      if (!it())
        return *it;
      int l = it.lb();
      if (it.disj(bx)) {
        it.next();
        if (l > 0) {
          p = *it;
          k = bx.lb();
        }
      }
      else {
        // Max. no. of consumable chars
        int m = fwd ? it.ub() - (*it).off : (*it).off; 
        if (k <= m) {
          it.consume(k);
          return p;
        }
        else {
          k -= m;
          it.next();
        }
      }
    }
    return p;
  };
  
  /// TODO:
  template <bool fwd, class IterY>
  forceinline void
  stretch(const Block& bx, IterY& it) {
//    std::cerr << "Streching " << (fwd ? "fwd " : "bwd ") << bx << " from " << *it << '\n';
    int k = bx.ub();
    while (it()) {
      int l = it.lb();
      if (l == 0)
        it.next();
      else if (it.disj(bx))
        return;
      else if (k < l) {
        fwd ? (*it).off += k : (*it).off -= k;
        return;
      }
      else {
        k -= l;
        it.next();
      } 
    }
  };
  
//    /// TODO:
//    template <class ViewX, class ViewY>
//    pushESP(const ViewX& x, const ViewY& y, int i, Matching m[]) {
//      if (x[i].lb() == 0) {
//        if (i < n-1 && m.ESP[i+1] < m.ESP[i])
//          m.ESP[i+1] = m.ESP[i];
//        return;
//      }
//      .... es = push_fwd(x[i], y, m.ESP[i]);
//      ls = m.ESP[i]
//      if (!es())
//        _|_
//      if (i < n && m.ESP[i+1] < ls)
//        m.ESP[i+1] = le;
//      if (...
//        
//    }
    
    /// TODO:
    template <class ViewX, class ViewY>
    forceinline bool
    init_x(Space& home, ViewX x, const ViewY& y, Matching m[]) {
      typename ViewY::SweepFwdIterator fwd_it = x.sweep_fwd_iterator();
      int n = x.size();
      for (int i = 0; i < n; ++i) {
        stretch<true, typename ViewY::SweepFwdIterator>(x[i], fwd_it);
        m[i].LEP = *fwd_it;
        std::cerr << i << ": " << x[i] << " LEP: " << m[i].LEP << '\n';
      }
      if (m[n-1].LEP < Position(n,0))
        return false;
      typename ViewY::StretchBwdIterator bwd_it = x.stretch_bwd_iterator();
      for (int i = n-1; i >= 0; --i) {
        stretch<false, typename ViewY::StretchBwdIterator>(x[i], bwd_it);
        m[i].ESP = *bwd_it;
        std::cerr << i << ": " << x[i] << " ESP: " << m[i].ESP << '\n';
      }
      return m[0].ESP >= Position(0,0);
    }
    
    /// TODO:
    template <class ViewX, class ViewY>
    forceinline bool
    sweep_x(Space& home, ViewX x, const ViewY& y, Matching m[]) {
      if (!init_x(home, x, y, m))
        return false;
      //TODO: pushESP/pushLEP
      refine_x(home, x, y, m);
      return true;
    }
    
    /// TODO: 
    template <class ViewX, class ViewY>
    forceinline ModEvent
    equate_x(Space& home, ViewX x, const ViewY& y) {
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
