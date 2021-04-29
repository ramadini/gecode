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
    /// Test if this position is equal to \a p w.r.t. lexicographic ordering.
    /// NOTE: If position are not normalized, the result might be unexpected.
    forceinline bool
    operator==(const Position& p) const {
      return idx == p.idx && off == p.off;
    }
    /// Test if this position is not equal to \a p w.r.t. lexicographic ordering
    /// NOTE: If position are not normalized, the result might be unexpected.
    forceinline bool
    operator!=(const Position& p) const {
      return idx != p.idx || idx != p.idx;
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
  };
  forceinline std::ostream&
  operator<<(std::ostream& os, const Position& p) {
    os << "(" << p.idx << "," << p.off << ")";
    return os;
  }
  
  /// Immutable struct defining a matching region.
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

  forceinline int
  nabla(const Block& bx, const Block& by, int x) {
    return x > 0 && !bx.baseDisjoint(by) ? x : 0;
  }
  
  /// Possibly refines each x[i] according to its matching region m[i] in y. 
  /// It returns true iff at least a block has been refined.
  template <class ViewX, class ViewY>
  forceinline bool 
  refine_x(Space& home, ViewX& x, const ViewY& y, Matching m[], int& nBlocks) {
//    std::cerr << "Refining " << x << "\nMax. " << nBlocks << " new blocks needed.\n";
    int nx = x.size();    
    bool changed = false;
    Region r;
    Block* newBlocks = r.alloc<Block>(nBlocks);
    int* U = r.alloc<int>(2*nx);
    int newSize = 0, uSize = 0;
    for (int i = 0; i < nx; ++i) {
//      std::cerr << "Ref. x[" << i << "] = " << x[i] << "\n";
//      std::cerr << "ESP: " << m[i].ESP << "\nLSP: " << m[i].LSP << "\nEEP: " 
//                           << m[i].EEP << "\nLEP: " << m[i].LEP << "\n";
      Position& esp = m[i].ESP, eep = m[i].EEP, lsp = m[i].LSP, lep = m[i].LEP;
      Block& x_i = x[i];
      if (x_i.isFixed()) {
        changed |= nx > 1 && x_i.isNull();
        continue;
      }
      int l = x_i.lb(), u = x_i.ub(), l1 = y.min_len_mand(x_i, lsp, eep);
      if (u < l1)
        return false;
      int u1 = y.max_len_opt(x_i, esp, lep, l1);
//      std::cerr << "l'=" << l1 << ", u'=" << u1 << "\n";
      if (l1 == 0 || l1 < l || u1 > u) {
        if (u1 == 0) {
          x_i.nullify(home);
          changed = true;
          continue;
        }
        if (nx == 1 && l <= l1) {
          // x is a single block: we can expand x into |y| blocks if the length 
          // constraint |x|=|y| is propagated.
          Region r;
          Block* y1 = r.alloc<Block>(y.size());
          y.expandBlock(home, x_i, y1);
          DashedString d(home, y1, y.size());
          r.free();
          // If some prefix or suffix fixed, or d actually refines x_i
          if ((d[0].baseSize() == 1 && d[0].lb() > 0) 
          ||  (d[d.size()-1].baseSize() == 1 && d[d.size()-1].lb() > 0)
          ||  (d.logdim() < x_i.logdim()))
            x.update(home, d);
          else
            nBlocks = -1;
          return true;
        }
        // Crushing into a single block
        int m = x_i.baseSize();
        x_i.updateCard(home, std::max(l, l1), std::min(u, u1));
        y.crushBase(home, x_i, esp, lep);
//        std::cerr << "x[" << i << "] ref. into " << x_i << "\n";
        changed |= l < l1 || u > u1 || m < x_i.baseSize();
        continue;
      }
      assert (l1 > 0);
      Region r;
      int n = y.ub_new_blocks(m[i]);
      assert (n > 0);
      bool no_lopt = esp == lsp; // True iff the left-opt. region is empty.
      if (n == 1 && no_lopt) {
        // No need to unfold x_i.
        n = x_i.baseSize();
        ViewY::mand_region(home, x_i, y[lsp.idx], lsp, eep);
        changed |= l < x_i.lb() || u > x_i.ub() || n < x_i.baseSize();
//        std::cerr << "x[" << i << "] ref. into " << x_i << "\n";
        continue;
      }
      // Unfolding x_i into newBlocks
      Block* mreg = r.alloc<Block>(n);
      if (!no_lopt) {
        y.opt_region(home, x_i, mreg[0], esp, lsp);
        y.mand_region(home, x_i, &mreg[1], u1, lsp, eep);
      }
      else
        y.mand_region(home, x_i, &mreg[0], u1, lsp, eep);
      if (eep != lep)
        y.opt_region(home, x_i, mreg[n-1], eep, lep);
      DashedString d(home, mreg, n);
      std::cerr << "d: " << d << '\n';
      r.free();      
      n = d.size();                                                         
      for (int j = 0, k = newSize; j < n; ++j,++k)
        newBlocks[k].update(home, d[j]);        
      U[uSize++] = i;
      U[uSize++] = n;
      newSize += n;
    }
//    std::cerr << "newSize: " << newSize << ", changed: " << changed << "\n";
    if (newSize > 0)
      x.resize(home, newBlocks, newSize, U, uSize);
    else if (changed)
      x.normalize(home);
    else
      nBlocks = -1;
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
      if (!it.hasNextBlock())
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
  forceinline bool
  stretch(const Block& bx, IterY& it) {
//    std::cerr << "Streching " << bx << " from " << *it << '\n';
    int k = bx.ub();
    bool ndisj = false;
    while (it.hasNextBlock()) {
      bool disj_it = it.disj(bx);
      ndisj |= !disj_it;
      // Min. no. of chars that must be consumed.
      int m = it.must_consume();
//      std::cerr << "it=" << *it << "k=" << k << ", m=" << m << ", disj=" <<disj<< std::endl;      
      if (m == 0)
        it.nextBlock();
      else if (disj_it)
        return ndisj;
      else if (k < m) {
        it.consumeMand(k);
        return ndisj;
      }
      else {
        k -= m;
        it.nextBlock();
      }
    }
    return ndisj;
  };
  
  template <class ViewX, class ViewY, class IterY>
  forceinline bool
  pushESP(ViewX& x, ViewY& y, Matching m[], int i) {
//    std::cerr << "Pushing ESP of " << x[i] << " from " << m[i].ESP << '\n';
    int n = x.size();
    if (x[i].lb() == 0) {
      // x[i] nullable, not pushing ESP[i]
      if (i < n-1 && y.prec(m[i+1].ESP, m[i].ESP))
        // x[i+1] cannot start before x[i]
        m[i+1].ESP = m[i].ESP;
      return true;
    }
    IterY q(y, m[i].ESP);
    IterY p(y, push<IterY>(x[i], q));
    if (!p())
      return false;    
    if (i < n && y.prec(m[i+1].ESP, *q))
      // x[i+1] cannot start before *q
      m[i+1].ESP = *q;
    if (y.prec(m[i].ESP, *p))
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
      if (i > 0 && y.prec(m[i-1].LEP, m[i].LEP))
        // x[i-1] cannot end after x[i]
        m[i-1].LEP = m[i].LEP;
      return true;
    }
    IterY p(y, m[i].LEP);
    IterY q(y, push<IterY>(x[i], p));
//    std::cerr << "p = " << *p << ", q = " << *q << "\n";
    if (!q())
      return false;
    if (i > 0 && y.prec(*p, m[i-1].LEP))
      // x[i-1] cannot end after *p
      m[i-1].LEP = *p;
    if (y.prec(*q, m[i].LEP))
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
      if (!stretch<typename ViewY::SweepFwdIterator>(x[i], fwd_it)) {
        if (x[i].lb() > 0)
          return false;
        x[i].nullify(home);
      }
      m[i].LEP = *fwd_it;
//      std::cerr << i << ": " << x[i] << " LEP: " << m[i].LEP << '\n';
      if (!fwd_it.hasNextBlock()) {
        for (int j = i+1; j < nx; ++j)
          m[j].LEP = *fwd_it;
        break;
      }
    }
    if (fwd_it())
      return false;
    typename ViewY::SweepBwdIterator bwd_it = y.bwd_iterator();
    for (int i = nx-1; i >= 0; --i) {
      if (!stretch<typename ViewY::SweepBwdIterator>(x[i], bwd_it)) {
        if (x[i].lb() > 0)
          return false;
        x[i].nullify(home);
      }
      m[i].ESP = *bwd_it;
//      std::cerr << i << ": " << x[i] << " ESP: " << m[i].ESP << '\n';
      if (!bwd_it.hasNextBlock()) {
        for (int j = i-1; j >= 0; --j)
          m[j].ESP = *bwd_it;
        break;
      }
    }
    return !bwd_it();
  }
  
  /// TODO:
  template <class ViewX, class ViewY>
  forceinline bool
  sweep_x(Space& home, ViewX x, ViewY& y, Matching m[], int& n) {
    if (!init_x(home, x, y, m))
      return false;
    int nx = x.size(); 
    for (int i = 0; i < nx; ++i) {
      if (!pushESP<ViewX,ViewY,typename ViewY::SweepFwdIterator>(x, y, m, i))
        return false;
    }
    for (int i = nx-1; i >= 0; --i) {
      if (!pushLEP<ViewX,ViewY,typename ViewY::SweepBwdIterator>(x, y, m, i))
        return false;
    }
    m[0].LSP = m[0].ESP;
    assert (m[0].LSP == Position(0,0));
    for (int i = 1; i < nx; ++i) {
      m[i].LSP = m[i-1].LEP;
//      std::cerr << "ESP of " << x[i] << ": " << m[i].ESP << ", " 
//                << "LSP of " << x[i] << ": " << m[i].LSP << "\n";
      if (y.prec(m[i].LSP, m[i].ESP))
        return false;
      assert (m[i].ESP.isNorm(y) && m[i].LSP.isNorm(y));
    }
    m[nx-1].EEP = m[nx-1].LEP;
    n = y.ub_new_blocks(m[nx-1]);
    for (int i = nx-2; i >= 0; --i) {
      m[i].EEP = m[i+1].ESP;
//      std::cerr << "EEP of " << x[i] << ": " << m[i].EEP << ", " 
//                << "LEP of " << x[i] << ": " << m[i].LEP << "\n";
      if (y.prec(m[i].LEP, m[i].EEP))
        return false;
      n += y.ub_new_blocks(m[i]);
      assert (m[i].EEP.isNorm(y) && m[i].LEP.isNorm(y));
    }
    assert (m[nx-1].EEP == Position(y.size(),0));
    return true;
  }
  
  /// TODO: 
  template <class ViewX, class ViewY>
  forceinline ModEvent
  equate_x(Space& home, ViewX x, ViewY& y) {
    int lb = x.min_length(), ub = x.max_length();
    Matching m[x.size()];
    int n;
    if (!sweep_x(home, x, y, m, n) || !refine_x(home, x, y, m, n))
      return ME_STRING_FAILED;
    if (n == -1)
      return ME_STRING_NONE;
    if (x.assigned())
      return ME_STRING_VAL;
    if (x.min_length() > lb || x.max_length() < ub)
      return ME_STRING_CARD;
    return ME_STRING_BASE;
  }

}}
