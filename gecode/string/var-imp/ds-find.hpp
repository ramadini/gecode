namespace Gecode { namespace String {

  template <class ViewX, class ViewY>
  forceinline int
  find_fixed(ViewX x, ViewY y) {
    int j = 0, k = 0, nx = x.size(), ny = y.size();
    for (int i = 0; i < nx && ny > 0; ) {
      if (baseMin(x[i]) == baseMin(y[j])) {
        int lx = lbound(x[i]), ly = ubound(y[j]);
        if (lx == ly || (lx > ly && (j == 0 || j == ny-1))) {
          ++i;
          ++j;
          ny -= ly;
          continue;
        }
      }
      ++i;
      j = 0;
      k = i;
      ny = y.size();
    }
    return ny == 0 ? k : 0;
  }
    
    
  template <class ViewX, class ViewY>
  forceinline ModEvent
  fixed_comp(Space& home, ViewX x, ViewY y, Gecode::Int::IntView iv) {
    Position start(0,0);
    Region r;    
    int n = x.max_length(), nx = x.size(), k = 0;
    Block* curr = r.alloc<Block>(nx);
    for (int i = 0; n > 0 && i < nx; ++i) {
      const Block& b = x[i];
      if (b.baseSize() == 1) {
        Block t(b.baseMin(), std::min(b.lb(), n));
        curr[k++].update(home, t);
        int k = find_fixed(ConstDashedView(*curr,k), y);
        if (k > 0) {
          GECODE_ME_CHECK(iv.gq(home,1));
          int ub = start.off + k + 1;
          for (int j = 0; j < start.idx && ub < iv.max(); ++j)
            ub += x[j].ub();
          GECODE_ME_CHECK(iv.lq(home, ub));
          return ME_STRING_NONE;
        }
        int h = b.ub()-b.lb();
        if (h > 0) {
          curr[0].update(home, t);
          k = 1;
          start = Position(i, h);
        }
      }
      else {
        k = 0;
        start = Position(i, b.ub());
      }
      n -= b.ub();
    }
    return ME_STRING_NONE;
  }

  // Auxiliary functions for find(x,y).
  template <class View>
  forceinline Position
  idx2pos(const View& x, int idx) {
    int i = 0, j = idx-1, n = x.size();
    while (i < n && j >= x[i].ub())
      j -= x[i++].ub();
    assert (Position(i,j).isNorm(x));
    return Position(i,j);
  }
  template <class View>
  forceinline int
  pos2idx(const View& x, const Position& p) {
    assert (p.isNorm(x));
    int idx = 1;
    for (int i = 0; i < p.idx; ++i)
      idx += x[i].lb();
    return ubounded_sum(idx, p.off);
  }
  
  // Pushing the earliest start position of each y-block in x for find(x,y).
  template <class ViewX, class ViewY>
  forceinline bool
  pushESP_find(const ViewX x, ViewY y, Matching m[], int& l, int& u, bool occ) {
    bool no_change = false;
    int ny = y.size();
    Position start = m[0].ESP;
    do {      
      for (int j = 0; j < ny; ++j) {
        if (!no_change)
          start = m[j].ESP;
        SweepFwdIterator<ViewX> fwd_it(x, start);
        m[j].ESP = y.push(j, fwd_it);
        start = *fwd_it;
      }
      // y can't fit in x.
      if (x.equiv(start, Position(x.size(),0))) {
        if (occ)
          return false;
        if (u > 0)
          u = 0;
        return true;
      }
      no_change = false;
      for (int j=ny-1; j >= 0; --j) {
        SweepBwdIterator<ViewX> bwd_it = x.bwd_iterator();
        y.stretch(j, bwd_it);
        start = *bwd_it;
        if (x.prec(m[j].ESP, start)) {
          m[j].ESP = start;
          no_change = true;
        }
      }
    } while (no_change);
    // Possibly refining lb and ub (converting from position to index).
    l = std::max(l, pos2idx(x, start));
    if (l > u) {
      if (occ)
        return false;
      if (u > 0)
        u = 0;
      return true;
    }
    return true;
  }

  // Sweep-based algorithm for find propagator. It returns true iff !occ OR
  // x can be a substring of y. If !occ and x can't be substring of y, then ub
  // parameter is set to 0, but true is still returned.
  // If x is not modified, then lb is set to -lb. 
  // If y is not modified, then ub is set to -ub.
  template <class ViewX, class ViewY>
  forceinline bool
  sweep_find(ViewX x, ViewY y, int& lb, int& ub, bool occ) {
    assert (lb >= 0 && ub >= 0);    
    int ny = y.size();
    Matching m[ny];
    Position start = idx2pos(x,lb), end = Position(x.size(),0);
    for (int i = 0; i < ny; ++i) {
      m[i].ESP = start;
      m[i].LEP = end;
    }
    if (!pushESP_find(x ,y, m, lb, ub, occ))
      return false;
    if (occ) {
      //TODO
    }
    return true;
  }

}}






