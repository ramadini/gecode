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
    /// Test if this and p refer to the same position in y
    template <class View>
    forceinline bool
    same(const Position& p, View& y) const {
      return (idx == p.idx   && off == p.off)
          || (idx == p.idx+1 && off == 0 && p.off == y[p.idx].ub())
          || (idx == p.idx-1 && p.off == 0 && off == y[idx].ub());
    }
    /// Test if this strictly precedes p in y
    template <class View>
    forceinline bool
    prec(const Position& p, View& y) const {
      return (idx < p.idx-1)
          || (idx == p.idx && off < p.off)
          || (idx == p.idx-1 && (p.off == 0 || off != y[idx].ub()));
    }
    /// Test if this precedes p in y, or they are the same position
    template <class View>
    forceinline bool
    preceq(const Position& p, View& y) const {
      return (idx < p.idx)
          || (idx == p.idx && off <= p.off)
          || (idx == p.idx+1 && off == 0 && p.off == y[p.idx].ub());
    }
    /// Test if this is normalized w.r.t. to y, i.e., it belongs to the set 
    /// {(i,j) | 0 <= i < |y|, 0 <= j < ub(y)} U {(|y|,0)}
    template <class View>
    forceinline bool
    isNorm(View& y) const {
      int n = y.size();
      return (0 <= idx && idx < n && 0 <= off && off < y[idx].ub())
          || (idx == n && off == 0);
    }
    /// Test if this position is less than \a p w.r.t. lexicographic ordering.
    /// NOTE: If position are not normalized, the result might be unexpected.
    forceinline bool
    operator<(const Position& p) const {
      return idx < p.idx || (idx == p.idx && off <= p.off);
    }
    /// Test if this position is less than \a p w.r.t. lexicographic ordering.
    /// NOTE: If position are not normalized, the result might be unexpected.
    forceinline bool
    operator==(const Position& p) const {
      return idx == p.idx && idx == p.idx;
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
      if (!it.hasNext())
        return *it;
      if (it.disj(bx)) {
        // Skipping block, possibly resetting k
        it.nextBlock();
        if (it.lb() > 0) {
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
          it.nextBlock();
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
    while (it.hasNext()) {
      // Min. no. of chars that must be consumed.
      int m = it.must_consume();
//      std::cerr << "k=" << k << ", m=" << m << ", it=" << *it << std::endl;
      if (m == 0)
        it.nextBlock();
      else if (it.disj(bx))
        return;
      else if (k < m) {
        it.consumeMand(k);
        return;
      }
      else {
        k -= m;
        it.nextBlock();
      } 
    }
  };
  
  template <class ViewX, class ViewY>
  bool
  pushESP(ViewX& x, ViewY& y, Matching m[], int i) {
//    int n = x.size();
//    if (x[i].lb() == 0) {
//      // x[i] nullable, not pushing ESP[i]
//      if (i < n-1 && m[i+1].ESP < m[i].ESP)
//        // x[i+1] cannot start before x[i]
//        m[i+1].ESP = m[i].ESP;
//      return true;
//    }
//    typename ViewY::SweepFwdIterator fwd_it(y, m[i].ESP);
//    Position start = push_fwd(x[i], y, m[i].ESP);
//    if (start == Position(n,0))
//      return false;
//    Position end = m[i].ESP;
//    if (i < n && m[i+1].ESP < end)
//      // x[i+1] cannot start before end
//      m[i+1].ESP = end;
//    if (m[i].ESP < start)
//      // Pushing ESP forward.
//      m[i].ESP = start;
    return true;   
  }
    
  /// TODO:
  template <class ViewX, class ViewY>
  forceinline bool
  init_x(Space& home, ViewX x, ViewY& y, Matching m[]) {
    typename ViewY::SweepFwdIterator fwd_it = y.fwd_iterator();
    int nx = x.size(), ny = y.size();
    for (int i = 0; i < nx; ++i) {
      stretch<typename ViewY::SweepFwdIterator>(x[i], fwd_it);
      m[i].LEP = *fwd_it;
//        std::cerr << i << ": " << x[i] << " LEP: " << m[i].LEP << '\n';
      if (!fwd_it.hasNext()) {
        for (int j = i+1; j < nx; ++j)
          m[j].LEP = *fwd_it;
        break;
      }
    }
    if (fwd_it.hasNext())
      return false;
    typename ViewY::SweepBwdIterator bwd_it = y.bwd_iterator();
    for (int i = nx-1; i >= 0; --i) {
      stretch<typename ViewY::SweepBwdIterator>(x[i], bwd_it);
      m[i].ESP = *bwd_it;
//        std::cerr << i << ": " << x[i] << " ESP: " << m[i].ESP << '\n';
      if (!bwd_it.hasNext()) {
        for (int j = i-1; j >= 0; --j)
          m[j].ESP = *bwd_it;
        break;
      }
    }
    return !bwd_it.hasNext();
  }
  
  /// TODO:
  template <class ViewX, class ViewY>
  forceinline bool
  sweep_x(Space& home, ViewX x, ViewY& y, Matching m[]) {
    if (!init_x(home, x, y, m))
      return false;
    int n = x.size(); 
    for (int i = 0; i < n; ++i) {
      if (!pushESP(x, y, m, i))
        return false;
    }
    for (int i = n-1; i >= 0; --i) {
//      if (!pushLEP(x, y, m, i))
//        return false;
    }
    m[0].LSP = m[0].ESP;
    for (int i = 1; i < n; ++i) {
      m[i].LSP = m[i-1].LEP;
      if (m[i].LSP < m[i].ESP)
        return false;
    }
    m[n-1].EEP = m[n-1].LEP;
    for (int i = n-2; i >= 0; --i) {
      m[i].EEP = m[i-1].ESP;
      if (m[i].LSP < m[i].ESP)
        return false;
    }
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
