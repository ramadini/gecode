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
  forceinline int baseMin(int i) { return i; }
  forceinline int baseMin(const Block& b) { return b.baseMin(); }
  forceinline bool isFixed(const Block& b) { return b.isFixed(); }
  forceinline bool isFixed(int) { return 1; }
  template <class View> forceinline int 
  min_len_mand(const View& y, const Block& b, const Position& p, 
                                              const Position& q) {
    return y.min_len_mand(b, p, q);
  }
  template <class View> forceinline int 
  min_len_mand(const View&, int, const Position&, const Position&) {
    return 1;
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
    /// {(i,j) | 0 <= i < |y|, 0 <= j < y[i].ub()} U {(|y|,0)}
    template <class View>
    forceinline bool
    isNorm(const View& y) const {
      std::cerr << "isNorm " <<  "(" << idx << "," << off << ")" << " in " << y << '\n'; 
      int n = y.size();
      return (0 <= idx && idx < n && 0 <= off && (ubound(y[idx]) == 0 
                   || off < ubound(y[idx]))) || (idx == n && off == 0);
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
  refine_x(Space& home, ViewX& x, const ViewY& y, Matching m[], int xFixed,
                                                                int& nBlocks) {
//    std::cerr << "Refining " << x << "  vs  " << y << "\nMax. " << nBlocks << " new blocks needed.\n";
    int nx = x.size(), ux = 2*(nx-xFixed);    
    bool changed = false;
    Region r;
    Block* newBlocks = nullptr;
    int* U = nullptr;
    int newSize = 0, uSize = 0;
    for (int i = 0; i < nx; ++i) {
      if (x[i].isFixed())
        continue;
      Position& esp = m[i].ESP, eep = m[i].EEP, lsp = m[i].LSP, lep = m[i].LEP;
//      std::cerr<<"Ref. x[" << i << "] = " << x[i] <<"\tESP: "<<esp<<"\tLSP: "<<lsp <<"\tEEP: "<<eep<<"\tLEP: "<<lep<< "\n";
      const Block& x_i = x[i];
      int l = x_i.lb(), u = x_i.ub(), l1 = min_len_mand(y, x_i, lsp, eep);
//      std::cerr << "l'=" << l1 << "\n";
      if (u < l1)
        return false;
      long u1 = y.max_len_opt(x_i, esp, lep, l1);
//      std::cerr << "u'=" << u1 << "\n";
      if (l > u1)
        return false;
      assert (l1 <= u1);
      if (l1 == 0 || l1 < l || u1 > u) {
        if (u1 == 0) {
          x.nullifyAt(home, i);
          changed = true;
          ux -= 2;
//          std::cerr << "x[" << i << "] nullified.\n";
          continue;
        }
        int m = x_i.baseSize();
        y.crushBase(home, x, i, esp, lep);
        changed |= l < l1 || u > u1 || m > x_i.baseSize();
        if (l1 == 0 || x[i].baseSize() == 1) {
          if (l1 > l)
            x.lbAt(home, i, l1);
          if (u1 < u)
            x.ubAt(home, i, u1);
//          std::cerr << "0) x[" << i << "] ref. into " << x[i] << "\n";
          ux -= 2;
          continue;
        }
        int np = 0, ns = 0;
        std::vector<int> vp, vs;
        if (esp == lsp)
          vp = y.fixed_pref(lsp, eep, np);
        if (eep == lep)
          vs = y.fixed_suff(lsp, eep, ns);
        np = np < 0 ? -np : np;
        ns = ns < 0 ? -ns : ns;
        if (np + ns > l1)
          return false;
        assert (vp.size() % 2 == 0 && vs.size() % 2 == 0 && np + ns <= u1);
        if (np == 0 && ns == 0) {
          nBlocks--;
          if (l1 > l)
            x.lbAt(home, i, l1);
          if (u1 < u)
            x.ubAt(home, i, u1);
          ux -= 2;
//          std::cerr << "1) x[" << i << "] ref. into " << x[i] << "\n";
          continue;
        }
//        std::cerr<<"x: " << x << "\ny: "<< y << "\ni: "<<i<<", x[i]: "<<x[i]
//        <<"\nESP: "<<esp<<"\tLSP: "<<lsp <<"\tEEP: "<<eep<<"\tLEP: "<<lep
//        << "\nPref: " << vec2str(vp) << " ; Suff: " << vec2str(vs)
//        << "\nnp: "<<np<<" ; ns: "<<ns<<"\n";
        assert (y.prec(lsp, eep) && (esp == lsp || eep == lep));
        if (U == nullptr)
          U = r.alloc<int>(ux);
        if (newBlocks == nullptr)
          newBlocks = r.alloc<Block>(nBlocks);        
        int zp = vp.size();
        for (int i = 0; i < zp-1; i += 2)
          newBlocks[newSize++].update(home, Block(vp[i], vp[i+1]));
        newBlocks[newSize].update(home, x[i]);
        newBlocks[newSize++].updateCard(home, std::max(x_i.lb(),l1)-ns-np,
                                              std::min(long(x_i.ub()),u1)-ns-np);
        int zs = vs.size();
        for (int i = zs-1; i > 0; i -= 2)
          newBlocks[newSize++].update(home, Block(vs[i-1], vs[i]));
        U[uSize++] = i;
        U[uSize++] = zp/2 + zs/2 + 1;
        continue;
      }
      assert (l1 > 0);
      int n = y.max_new_blocks(m[i]);
      assert (n > 0);
      if (n == 1) {
        nBlocks--;
        ux -= 2;
        // No need to unfold x_i.
        n = x_i.baseSize();
        y.mand_region(home, x, i, lsp, eep);
        changed |= l < x_i.lb() || u > x_i.ub() || n > x_i.baseSize();
//        std::cerr << "2) x[" << i << "] ref. into " << x_i << "\n";
        continue;
      }
      // Unfolding x_i into newBlocks
      Region r1;
      Block* mreg = r1.alloc<Block>(n);
//      std::cerr << "Before unfolding: "  << x_i << ' ' << l1 << '\n';
      if (esp == lsp)
        y.mand_region(home, x_i, &mreg[0], u1, lsp, eep);
      else {
        y.opt_region(home, x_i, mreg[0], esp, lsp, l1);
        y.mand_region(home, x_i, &mreg[1], u1, lsp, eep);
      }
      if (eep != lep)
        y.opt_region(home, x_i, mreg[n-1], eep, lep, l1);
      DashedString d(home, mreg, n);
      if (d.ub_sum() > u1)
        d.max_length(home, u1);
      r1.free();
//      std::cerr << "d = " << d << ' ' << n << "\n";
      n = d.size();
      if (n == 1) {
        nBlocks--;
        ux -= 2;
        if (d[0].ub() > u)
          d.ubAt(home, 0, u);
        if (d[0].equals(x_i))
          continue;
        x.updateAt(home, i, d[0]);
        changed = true;
//        std::cerr << "3) x[" << i << "] ref. into " << d[0] << "\n";
        continue;
      }
      if (U == nullptr)
        U = r.alloc<int>(ux);
      if (newBlocks == nullptr)
        newBlocks = r.alloc<Block>(nBlocks);
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
      // No modifications.
      nBlocks = -1;
//    std::cerr << "After refine: " << x << "\n";
    assert (x.isOK());
    return true;
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
    SweepFwdIterator<ViewY> p(y, x.push(i,q));
//    std::cerr << *p << ", " << *q << ' ' << p() << "\n";
    if (!p())
      return false;
    if (i < n-1 && y.prec(m[i+1].ESP, *q))
      // x[i+1] cannot start before *q
      m[i+1].ESP = *q;
    if (y.prec(m[i].ESP, *p))
      // Pushing ESP forward.
      m[i].ESP = *p;
//    std::cerr << "ESP of " << x[i] << ": " << m[i].ESP << '\n';
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
    SweepBwdIterator<ViewY> q(y, x.push(i,p));
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
      x.stretch(i, fwd_it);
      m[i].LEP = *fwd_it;
//      std::cerr << i << ") LEP of " << x[i] << ": " << m[i].LEP << "\n";
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
      x.stretch(i, bwd_it);
      m[i].ESP = *bwd_it;
//      std::cerr << i << ") ESP of " << x[i] << ": " << m[i].ESP << "\n";
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
  sweep_x(ViewX& x, const ViewY& y, Matching m[], int& xFixed, int& n) {
//    std::cerr << "sweep_x: " << x << "  vs  " << y << "\n";
    if (!init_x(x, y, m))
      return false;
    int nx = x.size(); 
    for (int i = 0; i < nx; ++i) {
      if (!pushESP<ViewX,ViewY>(x, y, m, i))
        return false;
//      std::cerr << i << ") ESP of " << x[i] << ": " << m[i].ESP << "\n";
    }
    for (int i = nx-1; i >= 0; --i) {
      if (!pushLEP<ViewX,ViewY>(x, y, m, i))
        return false;
//      std::cerr << i << ") LEP of " << x[i] << ": " << m[i].LEP << "\n";
    }
    m[0].LSP = m[0].ESP;
    for (int i = 1; i < nx; ++i) {
      m[i].LSP = m[i-1].LEP;
//      std::cerr << i << ") ESP of " << x[i] << ": " << m[i].LSP << "\n";
//      std::cerr << i << ") LSP of " << x[i] << ": " << m[i].LSP << "\n";      
      if (y.prec(m[i].LSP, m[i].ESP))
        return false;
//      assert (m[i].ESP.isNorm(y) && m[i].LSP.isNorm(y));
    }
    m[nx-1].EEP = m[nx-1].LEP;    
    xFixed = x[nx-1].isFixed();
    n = xFixed ? 0 : y.max_new_blocks(m[nx-1]);
    for (int i = nx-2; i >= 0; --i) {
      m[i].EEP = m[i+1].ESP;
//      std::cerr << i << ") EEP of " << x[i] << ": " << m[i].EEP << "\n";
//      std::cerr << i << ") LEP of " << x[i] << ": " << m[i].EEP << "\n";
      if (y.prec(m[i].LEP, m[i].EEP))
        return false;
      if (x[i].isFixed())
        xFixed++;
      else
        n += y.max_new_blocks(m[i]);
//      assert (m[i].EEP.isNorm(y) && m[i].LEP.isNorm(y));
    }
    return true;
  }
  
  template <class ViewX, class ViewY>
  forceinline bool
  check_equate_x(const ViewX& x, const ViewY& y) {
//    std::cerr << "Checking eq: " << x << "  vs  " << y << "\n";
    Matching m[x.size()];
    if (!init_x(x, y, m))
      return false;
    int nx = x.size();
    for (int i = 0; i < nx; ++i)
      if (!pushESP<ViewX,ViewY>(x, y, m, i))
        return false;
    for (int i = nx-1; i >= 0; --i)
      if (!pushLEP<ViewX,ViewY>(x, y, m, i))
        return false;
    m[0].LSP = m[0].ESP;
//    assert (m[0].LSP == Position(0,0));
    for (int i = 1; i < nx; ++i) {
      m[i].LSP = m[i-1].LEP;
      if (y.prec(m[i].LSP, m[i].ESP))
        return false;
//      assert (m[i].ESP.isNorm(y) && m[i].LSP.isNorm(y));
    }
    m[nx-1].EEP = m[nx-1].LEP;
    for (int i = nx-2; i >= 0; --i) {
      m[i].EEP = m[i+1].ESP;
      if (y.prec(m[i].LEP, m[i].EEP))
        return false;
//      assert (m[i].EEP.isNorm(y) && m[i].LEP.isNorm(y));
    }
//    assert (m[nx-1].EEP == Position(y.size(),0));
    for (int i = 0; i < nx; ++i) {
//      std::cerr << "Checking x[" << i << "] = " << x[i] << "\n";
//      std::cerr << "ESP: " << m[i].ESP << "\nLSP: " << m[i].LSP << "\nEEP: " 
//                           << m[i].EEP << "\nLEP: " << m[i].LEP << "\n";
      if (isFixed(x[i]))
        continue;
      if (ubound(x[i]) < min_len_mand(y, x[i], m[i].LSP, m[i].EEP))
        return false;
    }
    return true;
  }

}}
