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
      return idx == p.idx && idx == p.idx;
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

  forceinline int
  nabla(const Block& bx, const Block& by, int x) {
    return x > 0 && bx.baseDisjoint(by) ? x : 0;
  }
  
  /// Possibly refines each x[i] according to its matching region m[i] in y. 
  /// It returns true iff at least a block has been refined.
  template <class ViewX, class ViewY>
  forceinline bool 
  refine_x(Space& home, ViewX x, const ViewY& y, Matching m[], int n) {
    int nx = x.size();
    int newSize = nx;
    bool changed = false;
    Region r;
    r.alloc<Block>(n);
    for (int i = 0; i < nx; ++i) {
      Block& x_i = x[i];
      if (x_i.isFixed())
        continue;
      int l = x_i.lb(), u = x_i.ub(), 
         l1 = y.min_len_mand(x_i, m[i].LSP, m[i].ESP);
      if (u < l1)
        return false;
      int u1 = y.max_len_opt(x_i, m[i].ESP, m[i].LSP, l1);
      if (l1 == 0 || l1 < l || u1 > u) {
        if (u1 == 0) {
          x_i.nullify(home);
          changed = true;
          newSize--;
          continue;
        }        
        int ny = y.size();
        // kc true iff there is at least a known char in the prefix of suffix of y
        bool kc = (y[0].lb() > 0 && y[0].baseSize() == 1) ||
               (y[ny-1].lb() > 0 && y[ny-1].baseSize() == 1);
        if (nx == 1 && l <= l1) {
          // Special case where x is a single block: the soundness of the
          // propagation is preserved by constraining the min/max length.
          Region r;          
          Block* ry = r.alloc<Block>(ny);
          double ld = 0.0;
          for (int i = 0; i < ny; ++i) {
            ry[i].update(home, y[i]);
            ry[i].baseIntersect(home, x_i);
            if (ry[i].ub() > x_i.ub())
              ry[i].ub(home, x_i.ub());
            ld += ry[i].logdim();
          }
          
          if (kc || ld < x_i.logdim()) {
            x.update(home, DashedString(home, r));
            changed = true;
          }
          r.free();          
        }        
        Gecode::Set::GLBndSet s;          
        int n = x_i.baseSize();
        for (int j = m[i].ESP.idx; j <= m[i].LEP.idx; ++j) {
          if (y[j].baseSize() == 1) {
            int m = y[j].baseMin();
            Gecode::Set::SetDelta d;
            s.include(home, m, m, d);              
          }
          else {
            Gecode::Set::BndSetRanges i(x_i.ranges());
            s.includeI(home, i);
          }
        }
        x_i.baseIntersect(home, s);
        if (x_i.isNull()) {
          changed = true;
          continue;
        }
        if (kc) {
          
          changed = true;
          continue;
        }
        x_i.updateCard(home, l > l1 ? l : l1, u1);  
        changed |= l < l1 || u > u1 || n < x_i.baseSize();        
        continue;
      }
      // Compute unfolding
    }
    // Possibly unfold with d.s. in r if (nx1 > nx) ...
    if (changed)
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
      if (i < n-1 && y.prec(m[i+1].ESP, m[i].ESP))
        // x[i+1] cannot start before x[i]
        m[i+1].ESP = m[i].ESP;
      return true;
    }
    IterY q(y, m[i].ESP);
    IterY p(y, push<IterY>(x[i], q));
    if (!p.hasNext())
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
    if (!q.hasNext())
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
    for (int i = 1; i < nx; ++i) {
      m[i].LSP = m[i-1].LEP;
//      std::cerr << "ESP of " << x[i] << ": " << m[i].ESP << ", " 
//                << "LSP of " << x[i] << ": " << m[i].LSP << "\n";
      if (y.prec(m[i].LSP, m[i].ESP))
        return false;
    }
    m[nx-1].EEP = m[nx-1].LEP;
    n = 0;
    for (int i = nx-2; i >= 0; --i) {
      m[i].EEP = m[i+1].ESP;
//      std::cerr << "EEP of " << x[i] << ": " << m[i].EEP << ", " 
//                << "LEP of " << x[i] << ": " << m[i].LEP << "\n";
      if (y.prec(m[i].LEP, m[i].EEP))
        return false;
      n += y.ub_new_blocks(m[i]);
    }
    return true;
  }
  
  /// TODO: 
  template <class ViewX, class ViewY>
  forceinline ModEvent
  equate_x(Space& home, ViewX x, ViewY& y) {
    int lb = x.min_length(), ub = x.max_length();
    Matching m[x.size()];
    int n;
    if (!sweep_x(home, x, y, m, n))
      return ME_STRING_FAILED;
    if (!refine_x(home, x, y, m, n))
      return ME_STRING_NONE;
    if (x.assigned())
      return ME_STRING_VAL;
    else if (x.min_length() > lb || x.max_length() < ub)
      return ME_STRING_CARD;
    else
      return ME_STRING_BASE;
  }

}}
