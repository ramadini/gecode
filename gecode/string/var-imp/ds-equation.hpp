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
      return (idx == p.idx && off == p.off);
    }
    forceinline bool
    equiv(const Position& p, int u) const {
      return *this == p ||
        (off == 0 && idx == p.idx+1 && u == p.off) ||
        (p.off == 0 && p.idx == idx+1 && u == off);        
    }
    /// Position inequality.
    forceinline bool
    operator !=(const Position& p) const {
      return idx != p.idx || off != p.off;
    }
    forceinline bool
    nequiv(const Position& p, int u) const {
      return !equiv(p, u);
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
  template <class IterY>
  forceinline Position
  push(const Block& bx, IterY& it) {
//    std::cerr << "Pushing " << bx << " from " << *it << '\n';
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
        // Max. no. of chars that may be consumed.
        int m = it.may_consume(); 
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
  template <class IterY>
  forceinline void
  stretch(const Block& bx, IterY& it) {
//    std::cerr << "Streching " << bx << " from " << *it << '\n';
    int k = bx.ub();
    while (it()) {
      // Min. no. of chars that must be consumed.
      int m = it.must_consume();
//      std::cerr << "k=" << k << ", m=" << m << ", it=" << *it << std::endl;
      if (m == 0)
        it.next();
      else if (it.disj(bx))
        return;
      else if (k < m) {
        it.consume(k);
        return;
      }
      else {
        k -= m;
        it.next();
      } 
    }
  };
  
  template <class ViewX, class ViewY>
  bool
  pushESP(ViewX& x, ViewY& y, int i, Matching m[]) {
    int n = x.size();
    if (x[i].lb() == 0) {
      // x[i] nullable, not pushing ESP[i]
      if (i < n-1 && m[i+1].ESP < m[i].ESP)
        // x[i+1] cannot start before x[i]
        m[i+1].ESP = m[i].ESP;
      return true;
    }
    typename ViewY::SweepFwdIterator fwd_it(y, m[i].ESP);
    Position start = push_fwd(x[i], y, m[i].ESP);
    if (start == Position(n,0))
      return false;
    Position end = m[i].ESP;
    if (i < n && m[i+1].ESP < end)
      // x[i+1] cannot start before end
      m[i+1].ESP = end;
    if (m[i].ESP < start)
      // Pushing ESP forward.
      m[i].ESP = start;
    return true;   
  }
    
  /// TODO:
  template <class ViewX, class ViewY>
  forceinline bool
  init_x(Space& home, ViewX x, ViewY& y, Matching m[]) {
    typename ViewY::SweepFwdIterator fwd_it = y.sweep_fwd_iterator();
    int nx = x.size(), ny = y.size();
    Position p = Position(ny, 0); 
    for (int i = 0; i < nx; ++i) {
      stretch<typename ViewY::SweepFwdIterator>(x[i], fwd_it);
      m[i].LEP = *fwd_it;
//        std::cerr << i << ": " << x[i] << " LEP: " << m[i].LEP << '\n';
      if (*fwd_it == p) {
        for (int j = i+1; j < nx; ++j)
          m[j].LEP = p;
        break;
      }
    }
    if (m[nx-1].LEP < p)
      return false;
    typename ViewY::StretchBwdIterator bwd_it = y.stretch_bwd_iterator();
    p.idx = 0;
    for (int i = nx-1; i >= 0; --i) {
      stretch<typename ViewY::StretchBwdIterator>(x[i], bwd_it);
      m[i].ESP = *bwd_it;
//        std::cerr << i << ": " << x[i] << " ESP: " << m[i].ESP << '\n';
      if (*bwd_it == p) {
        for (int j = i-1; j >= 0; --j)
          m[j].ESP = p;
        break;
      }
    }
    return m[0].ESP <= p;
  }
  
  /// TODO:
  template <class ViewX, class ViewY>
  forceinline bool
  sweep_x(Space& home, ViewX x, ViewY& y, Matching m[]) {
    if (!init_x(home, x, y, m))
      return false;
    //TODO: pushESP/pushLEP
    refine_x(home, x, y, m);
    return true;
  }
  
  /// TODO: 
  template <class ViewX, class ViewY>
  forceinline ModEvent
  equate_x(Space& home, ViewX x, ViewY& y) {
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
