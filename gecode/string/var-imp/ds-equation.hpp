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
    /// Test if this strictly precedes p according to the direction defined by y
    template <class View>
    forceinline bool
    isNorm(View& y) const {
      int n = y.size();
      return (0 <= idx && idx < n && 0 <= off && off < y[idx].ub())
          || (idx == n && off == 0);
    }
    /// Test if this position is equal to \a p w.r.t. lexicographic ordering.
    /// NOTE: If position are not normalized, the result might be unexpected.
    forceinline bool
    operator==(const Position& p) const {
      return idx == p.idx && idx == p.idx;
    }
    /// Test if this position is not equal to \a p w.r.t. lexicographic ordering.
    /// NOTE: If position are not normalized, the result might be unexpected.
    forceinline bool
    operator!=(const Position& p) const {
      return idx != p.idx || idx != p.idx;
    }
    template <class IterY>
    forceinline bool
    prec(const Position& p, IterY& y) const {
      return (idx < p.idx-1)
          || (idx == p.idx && off < p.off)
          || (idx == p.idx-1 && (p.off == 0 || off != y[idx].ub()));
    }
    /// Test if this is normalized w.r.t. to y, i.e., it belongs to the set 
    /// {(i,j) | 0 <= i < |y|, 0 <= j < ub(y)} U {(|y|,0)}
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
  
  /// Possible states after refining a block
  enum BlockEvent {
    BE_FAIL = -1,   // Inconsistency detected 
    BE_NONE,        // No modification
    BE_UPDATE,      // Block updated, no normalization needed
    BE_UNFOLD,      // Block to be unfolded in n > 1 blocks
    BE_UPDATE_NORM, // Block updated, normalization of its dashed string needed
    BE_UNFOLD_NORM  // Block to be unfolded, and normalization needed
  };
  
}}

namespace Gecode { namespace String {
 
  // FIXME: Maybe define a namespace for equate-based functions?

  forceinline bool
  nabla(const Block& bx, const Block& by, int x) {
    return x > 0 && bx.baseDisjoint(by) ? x : 0;
  }

  template <class ViewY>
  forceinline int 
  min_len_mand(const Block& bx, const ViewY& y,
               const Position& lsp, const Position& eep) {
    if (eep.prec(lsp, y))
      return 0;
    int h = lsp.idx, h1 = eep.idx, k = lsp.off, k1 = eep.off;
    if (h == h1)
      return std::max(bx.lb(), nabla(bx, y[h], k1 - k));
    int s = nabla(bx.lb(), y[h], y[h].lb() - k);
    for (int i = h+1; i < h1; i++) 
      s += nabla(bx, y[i], y[i].lb());
    return std::max(bx.lb(), s + nabla(bx, y[k], k1));
  }
  
  template <class ViewY>
  forceinline int 
  max_len_opt(const Block& bx, const ViewY& y, int lb_mand,
              const Position& esp, const Position& lep) {
    assert(!lep.prec(esp, y));
    int p = esp.idx, p1 = lep.idx, q = esp.off, q1 = lep.off;
    if (p == p1)
      return std::min(bx.ub(), nabla(bx, y[p], q1 - q));
    int s = nabla(bx, y[p], y[p].ub() - q);
    for (int i = p+1; i < p1; i++) 
      s += nabla(bx, y[i], y[i].ub());
    return std::min(bx.ub(), s + nabla(bx, y[q], q1));
  }
  
  /// Possibly refines each x[i] according to its matching region m[i] in y. 
  /// It returns true iff at least a block has been refined.
  template <class ViewX, class ViewY>
  forceinline bool 
  refine_x(Space& home, ViewX x, const ViewY& y, Matching m[]) {
    int nx = x.size();
    bool norm = false;
    for (int i = 0; i < nx; ++i) {
      Block& bx = x[i];
      if (bx.isFixed())
        continue;
//        if (bx.ub() < lb_mand)
//          return false;
//        ub_opt = max_len_opt(bx, y, esp, lep);
//        if (lb_mand > 0 && lb_mand >= bx.lb() && ub_opt <= bx.ub()) {
//          lman = -1;//TODO...
//          rman = -1;//TODO....
//        }
//        else {
//          int l = std::max(bx.lb(), lb_mand), u = std::min(bx.ub(), ub_opt);
//          CharSet S; //TODO...
//          bx.baseIntersect(S);
//          bx.updateCard(home, l, u);                              
//        }

//      }
//      else {
//        if (bx.ub() < lb_mand)
//          return false;
//        ub_opt = max_len_opt(...);        
//      }
//      if (lb_mand > 0 && lb_mand >= bx.lb() && ub_opt <= bx.ub()) {
//        mand_reg = ...
//        m = mand_reg.size()
//        assert (m > 0);
//        if (m > 1)
//          new_blocks += m - 1;
//        else
//          update block ...
//      }
//      else {
//        //crush or special cases
//        base_union_opt...
//      }      
    }
//    if (new_blocks > 0)
//     update_x(..., new_blocks);
    if (norm)
      x.normalize(home);
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
//      std::cerr << "p=" << p << ", it=" << *it << ", k=" << k << std::endl;
      if (!it.hasNext())
        return *it;
      if (it.disj(bx)) {
        // Skipping block, possibly resetting k
        if (it.lb() > 0) {
          it.nextBlock();
          p = *it;
          k = bx.lb();
        }
        else
          it.nextBlock();
      }
      else {
        // Max. no. of chars that may be consumed.
        int m = it.may_consume();
//        std::cerr << "m=" << m << std::endl;
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
//      std::cerr << "it=" << *it << "k=" << k << ", m=" << m << std::endl;
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
  
  template <class ViewX, class ViewY, class IterY>
  forceinline bool
  pushESP(ViewX& x, ViewY& y, Matching m[], int i) {
//    std::cerr << "Pushing ESP of " << x[i] << " from " << m[i].ESP << '\n';
    int n = x.size();
    if (x[i].lb() == 0) {
      // x[i] nullable, not pushing ESP[i]
      if (i < n-1 && m[i+1].ESP.prec(m[i].ESP, y))
        // x[i+1] cannot start before x[i]
        m[i+1].ESP = m[i].ESP;
      return true;
    }
    IterY q(y, m[i].ESP);
    IterY p(y, push<IterY>(x[i], q));
    if (!p.hasNext())
      return false;
    if (i < n && m[i+1].ESP.prec(*q, y))
      // x[i+1] cannot start before *q
      m[i+1].ESP = *q;
    if (m[i].ESP.prec(*p, y))
      // Pushing ESP forward.
      m[i].ESP = *p;
    return true;
  }
  
  template <class ViewX, class ViewY, class IterY>
  forceinline bool
  pushLEP(ViewX& x, ViewY& y, Matching m[], int i) {
//    std::cerr << "Pushing LEP of " << x[i] << " from " << m[i].LEP << '\n';
    if (x[i].lb() == 0) {
      // x[i] nullable, not pushing LEP[i]
      if (i > 0 && m[i-1].LEP.prec(m[i].LEP, y))
        // x[i-1] cannot end after x[i]
        m[i-1].LEP = m[i].LEP;
      return true;
    }
    IterY p(y, m[i].LEP);
    IterY q(y, push<IterY>(x[i], p));
//    std::cerr << "p = " << *p << ", q = " << *q << "\n";
    if (!q.hasNext())
      return false;
    if (i > 0 && (*p).prec(m[i-1].LEP, y))
      // x[i-1] cannot end after *p
      m[i-1].LEP = *p;
    if ((*q).prec(m[i].LEP, y))
      // Pushing LEP backward.
      m[i].LEP = *q;
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
      if (!pushESP<ViewX,ViewY,typename ViewY::SweepFwdIterator>(x, y, m, i))
        return false;
    }
    for (int i = n-1; i >= 0; --i) {
      if (!pushLEP<ViewX,ViewY,typename ViewY::SweepBwdIterator>(x, y, m, i))
        return false;
    }
    m[0].LSP = m[0].ESP;
    for (int i = 1; i < n; ++i) {
      m[i].LSP = m[i-1].LEP;
//      std::cerr << "ESP of " << x[i] << ": " << m[i].ESP << ", " 
//                << "LSP of " << x[i] << ": " << m[i].LSP << "\n";
      if (m[i].LSP.prec(m[i].ESP, y))
        return false;
    }
    m[n-1].EEP = m[n-1].LEP;
    for (int i = n-2; i >= 0; --i) {
      m[i].EEP = m[i+1].ESP;
//      std::cerr << "EEP of " << x[i] << ": " << m[i].EEP << ", " 
//                << "LEP of " << x[i] << ": " << m[i].LEP << "\n";
      if (m[i].LEP.prec(m[i].EEP, y))
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
