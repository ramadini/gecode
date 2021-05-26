namespace Gecode { namespace String {

  struct Position;
  template <class T> class SweepFwdIterator;
  template <class T> class SweepBwdIterator;

  /// Auxiliary functions
  
  forceinline int
  nabla(const Block& bx, const Block& by, int x) {
    return x <= 0 || bx.baseDisjoint(by) ? 0 : x;
  }
  
  forceinline int ubound(int) { return 1; }
  forceinline int lbound(int) { return 1; }
  forceinline int ubound(const Block& b) { return b.ub(); }
  forceinline int lbound(const Block& b) { return b.lb(); }
  forceinline int lbound(const GBlock& g) { return g.lb(); }
  forceinline int ubound(const GBlock& g) { return g.ub(); }
  
  template <class View> forceinline int 
  min_len_mand(const View& y, const Block& b, const Position& p, 
                                              const Position& q) {
    return y.min_len_mand(b, p, q);
  }
  template <class View> forceinline int 
  min_len_mand(const View&, int, const Position&, const Position&) {
    return 1;
  }
  template <class View> forceinline int 
  min_len_mand(const View& y, const GBlock& g, const Position& p, 
                                               const Position& q) {
    return g.isChar() ? 1 : y.min_len_mand(g.block(), p, q);
  }
  
  /// Struct abstracting a position in a dashed string.
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
      return idx != p.idx || off != p.off;
    }
    forceinline Position
    operator-(int k) const {
      assert (k <= idx);
      return Position(idx-k,off);
    }
    
    /// Test if this is normalized w.r.t. to y, i.e., it belongs to the set 
    /// {(i,j) | 0 <= i < |y|, 0 <= j < ub(y)} U {(|y|,0)}
    template <class View>
    forceinline bool
    isNorm(View& y) const {
      int n = y.size();
      return (0 <= idx && idx < n && 0 <= off && off < ubound(y[idx]))
          || (idx == n && off == 0);
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
  
}}

namespace Gecode { namespace String {
 
  // FIXME: Maybe define a namespace for equate-based functions?

 
  /// Possibly refines each x[i] according to its matching region m[i] in y. 
  /// It returns true iff at least a block has been refined.
  template <class ViewX, class ViewY>
  forceinline bool 
  refine_x(Space& home, ViewX& x, const ViewY& y, Matching m[], int& nBlocks) {
//    std::cerr << "Refining " << x << "  vs  " << y << "\nMax. " << nBlocks << " new blocks needed.\n";
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
      GBlock g_i(x[i]);
      int l = g_i.lb(), u = g_i.ub(), l1 = min_len_mand(y, g_i, lsp, eep);
      if (u < l1)
        return false;
      if (g_i.isFixed())
        continue;
      Block& x_i = g_i.block();
      int u1 = y.max_len_opt(x_i, esp, lep, l1);
//      std::cerr << "l'=" << l1 << ", u'=" << u1 << "\n";
      assert (l1 <= u1);
      if (l1 == 0 || l1 < l || u1 > u) {
        if (u1 == 0) {
          x_i.nullify(home);
          changed = true;
          continue;
        }
        if (nx == 1 && l <= l1) {
          // FIXME: x is a single block, so we can expand it into |y| blocks but
          // only if we propagate |x| <= y.max_length(), otherwise we lose the
          // length information and the propagation can be unsound!!!
          Region r;
          Block* y1 = r.alloc<Block>(y.size());
          y.expandBlock(home, x_i, y1);
          DashedString d(home, y1, y.size());
          r.free();
          // If some prefix or suffix fixed, or d actually refines x_i
          if ((d[0].baseSize() == 1 && d[0].lb() > 0) 
          ||  (d[d.size()-1].baseSize() == 1 && d[d.size()-1].lb() > 0)
          ||  (d.logdim() < x_i.logdim())) {
            x.gets(home, d);
            changed = true;
            return true;
          }
        }
        // Crushing into a single block
        int m = x_i.baseSize();
        x_i.updateCard(home, std::max(l, l1), std::min(u, u1));
        y.crushBase(home, x_i, esp, lep);
//        std::cerr << "x[" << i << "] ref. into " << x_i << "\n";
        changed |= l < l1 || u > u1 || m > x_i.baseSize();
        continue;
      }
      assert (l1 > 0);
      Region r;
      int n = y.ub_new_blocks(m[i]);
      assert (n > 0);
      if (n == 1 && esp == lsp) {
        // No need to unfold x_i.
        n = x_i.baseSize();
        Block b;
        b.update(home, y[lsp.idx]);
        y.mand_region(home, x_i, b, lsp, eep);
        changed |= l < x_i.lb() || u > x_i.ub() || n > x_i.baseSize();
//        std::cerr << "x[" << i << "] ref. into " << x_i << "\n";
        continue;
      }
      // Unfolding x_i into newBlocks
      Block* mreg = r.alloc<Block>(n);
      if (esp == lsp)
        y.mand_region(home, x_i, &mreg[0], u1, lsp, eep);
      else {
        y.opt_region(home, x_i, mreg[0], esp, lsp, l1);
        y.mand_region(home, x_i, &mreg[1], u1, lsp, eep);
      }
      if (eep != lep)
        y.opt_region(home, x_i, mreg[n-1], eep, lep, l1);
      DashedString d(home, mreg, n);
//      std::cerr << "x[" << i << "] ref. into " << d << "\n";
      r.free();      
      n = d.size();
      if (n == 1) {
        // No need to unfold x_i.
        x_i.update(home, d[0]);
        changed |= l < x_i.lb() || u > x_i.ub() || n > x_i.baseSize();
        continue;
      }
      for (int j = 0, k = newSize; j < n; ++j,++k)
        newBlocks[k].update(home, d[j]);              
      U[uSize++] = i;
      U[uSize++] = n;
      newSize += n;
    }
//    std::cerr << "newSize: " << newSize << ", uSize: " << uSize << ", changed: " << changed << "\n";
    if (newSize > 0)
      x.resize(home, newBlocks, newSize, U, uSize);
    else if (changed)
      x.normalize(home);
    else
      nBlocks = -1;
    // FIXME: An alternative approach is to keep track of max_feas_len for string views, and
    // use a x.check_length() that e.g. returning x.max_length() <= x.max_feas_len() for a StringView
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
  
  template <class IterY>
  forceinline Position
  push(int cx, IterY& it) {
//    std::cerr << "Pushing " << bx << " from " << *it << '\n';
    Position p = *it;
    // No. of chars. that must be consumed
    int k = 1;
    while (k > 0) {
//      std::cerr << "p=" << p << ", it=" << *it << ", k=" << k << std::endl;
      if (!it.hasNextBlock())
        return *it;
      if (it.disj(cx)) {
        // Skipping block, possibly resetting k
        if (it.lb() > 0) {
          it.nextBlock();
          p = *it;
          k = 1;
        }
        else
          it.nextBlock();
      }
      else {
        it.consume(1);
        return p;
      }
    }
    return p;
  };
  
  template <class IterY>
  forceinline Position
  push(const GBlock& g, IterY& it) {
    return g.isFixed() ? push(g.val(), it) : push(g.block(), it);
  }
  
  /// TODO:
  template <class IterY>
  forceinline void
  stretch(const Block& bx, IterY& it) {
//    std::cerr << "Streching " << bx << " from " << *it << '\n';
    int k = bx.ub();
    while (it.hasNextBlock()) {
      // Min. no. of chars that must be consumed.
      int m = it.must_consume();
//      std::cerr << "it=" << *it << ", k=" << k << ", m=" << m << std::endl;      
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
  
  template <class IterY>
  forceinline void
  stretch(int cx, IterY& it) {
//    std::cerr << "Streching " << cx << " from " << *it << '\n';
    int k = 1;
    while (it.hasNextBlock()) {
      // Min. no. of chars that must be consumed.
      int m = it.must_consume();
//      std::cerr << "it=" << *it << ", k=" << k << ", m=" << m << std::endl;      
      if (m == 0)
        it.nextBlock();
      else if (it.disj(cx))
        return;
      else if (k < m) {
        it.consumeMand(k);
        return;
      }
      else {
        k = 0;
        it.nextBlock();
      }
    }
  };
  
  template <class IterY>
  forceinline void
  stretch(const GBlock& g, IterY& it) {
    g.isFixed() ? stretch(g.val(), it) : stretch(g.block(), it);
  }
  
  template <class ViewX, class ViewY>
  forceinline bool
  pushESP(const ViewX& x, const ViewY& y, Matching m[], int i) {
//    std::cerr << "Pushing ESP of " << x[i] << " from " << m[i].ESP << '\n';
    int n = x.size();
    if (lbound(x[i]) == 0) {
      // x[i] nullable, not pushing ESP[i]
      if (i < n-1 && y.prec(m[i+1].ESP, m[i].ESP))
        // x[i+1] cannot start before x[i]
        m[i+1].ESP = m[i].ESP;
      return true;
    }
    SweepFwdIterator<ViewY> q(y, m[i].ESP);
    SweepFwdIterator<ViewY> p(y, push<SweepFwdIterator<ViewY>>(x[i], q));
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
  
  template <class ViewX, class ViewY>
  forceinline bool
  pushLEP(const ViewX& x, const ViewY& y, Matching m[], int i) {
//    std::cerr << "Pushing LEP of " << x[i] << " from " << m[i].LEP << '\n';
    if (lbound(x[i]) == 0) {
      // x[i] nullable, not pushing LEP[i]
      if (i > 0 && y.prec(m[i].LEP, m[i-1].LEP))
        // x[i-1] cannot end after x[i]
        m[i-1].LEP = m[i].LEP;
      return true;
    }
    SweepBwdIterator<ViewY> p(y, m[i].LEP);
    SweepBwdIterator<ViewY> q(y, push<SweepBwdIterator<ViewY>>(x[i], p));
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
  init_x(ViewX x, const ViewY& y, Matching m[]) {
//    std::cerr << "Init: " << x << "  vs  " << y << "\n";
    SweepFwdIterator<ViewY> fwd_it = y.fwd_iterator();
    int nx = x.size();
    for (int i = 0; i < nx; ++i) {
      stretch(x[i], fwd_it);
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
    SweepBwdIterator<ViewY> bwd_it = y.bwd_iterator();
    for (int i = nx-1; i >= 0; --i) {
      stretch(x[i], bwd_it);
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
  sweep_x(ViewX& x, const ViewY& y, Matching m[], int& n) {
//    std::cerr << "sweep_x: " << x << "  vs  " << y << "\n";
    if (!init_x(x, y, m))
      return false;
    if (x.assigned() && y.assigned())
      return true;
    int nx = x.size(); 
    for (int i = 0; i < nx; ++i)
      if (!pushESP<ViewX,ViewY>(x, y, m, i))
        return false;
    for (int i = nx-1; i >= 0; --i)
      if (!pushLEP<ViewX,ViewY>(x, y, m, i))
        return false;
    m[0].LSP = m[0].ESP;
    assert (m[0].LSP == Position(0,0));
    for (int i = 1; i < nx; ++i) {
      m[i].LSP = m[i-1].LEP;
//      std::cerr << "ESP of " << GBlock(x[i]) << ": " << m[i].ESP << ", " 
//                << "LSP of " << GBlock(x[i]) << ": " << m[i].LSP << "\n";
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
  
  /// TODO: Remove this:
  template <class ViewX, class ViewY>
  forceinline ModEvent
  equate_x(Space& home, ViewX& x, const ViewY& y) {
    int lb = x.min_length();
    long ub = x.max_length();
    if (ub == Limits::MAX_STRING_LENGTH && lb < ub && x.size() > 1) {
      ub = 0;
      for (int i = 0; i < x.size(); ++i)
        ub += ubound(x[i]);
    }
    else
      ub = x.max_length();
    Matching m[x.size()];
    int n;
    if (sweep_x(x, y, m, n) && refine_x(home, x, y, m, n)) {
      if (n == -1)
        return ME_STRING_NONE;      
      if (x.assigned())
        return ME_STRING_VAL;
      StringDelta d;
      int ux = x.max_length();
      if (x.min_length() > lb || (ux < ub && ux < MAX_STRING_LENGTH))
        return x.varimp()->notify(home, ME_STRING_CARD, d);
      if (ux == MAX_STRING_LENGTH && ub > MAX_STRING_LENGTH) {
        long u = 0L;
        for (int i = 0; i < x.size(); ++i) {
          u += ubound(x[i]);
          if (u >= ub)
            return x.varimp()->notify(home, ME_STRING_BASE, d);
        }
        return x.varimp()->notify(home, ME_STRING_CARD, d);
      }
      else
        return x.varimp()->notify(home, ME_STRING_BASE, d);
    }
    else
      return ME_STRING_FAILED;
  }
  
  template <class ViewX, class ViewY>
  forceinline bool
  check_equate_x(const ViewX& x, const ViewY& y) {
//    std::cerr << "Checking eq: " << x << "  vs  " << y << "\n";
    Matching m[x.size()];
    if (!init_x(x, y, m))
      return false;
    if (x.min_length() == x.max_length() && y.min_length() == y.max_length())
      return true;
    int nx = x.size();
    for (int i = 0; i < nx; ++i)
      if (!pushESP<ViewX,ViewY>(x, y, m, i))
        return false;
    for (int i = nx-1; i >= 0; --i)
      if (!pushLEP<ViewX,ViewY>(x, y, m, i))
        return false;
    m[0].LSP = m[0].ESP;
    assert (m[0].LSP == Position(0,0));
    for (int i = 1; i < nx; ++i) {
      m[i].LSP = m[i-1].LEP;
      if (y.prec(m[i].LSP, m[i].ESP))
        return false;
      assert (m[i].ESP.isNorm(y) && m[i].LSP.isNorm(y));
    }
    m[nx-1].EEP = m[nx-1].LEP;
    for (int i = nx-2; i >= 0; --i) {
      m[i].EEP = m[i+1].ESP;
      if (y.prec(m[i].LEP, m[i].EEP))
        return false;
      assert (m[i].EEP.isNorm(y) && m[i].LEP.isNorm(y));
    }
    assert (m[nx-1].EEP == Position(y.size(),0));
    for (int i = 0; i < nx; ++i)
      if (ubound(x[i]) < min_len_mand(y, x[i], m[i].LSP, m[i].EEP))
        return false;    
    return true;
  }

}}
