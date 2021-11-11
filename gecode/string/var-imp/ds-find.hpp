#include <map>

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
      for (int j = ny-1; j >= 0; --j) {
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

  template <class ViewX, class ViewY>
  forceinline bool
  pushLEP_find(const ViewX& x, const ViewY& y, Matching m[], int& yFixed, 
                                                             int& nBlocks) {
    bool no_change = false;
    int ny = y.size();
    Position start = m[ny-1].LEP;
    do {      
      for (int j = ny-1; j >= 0; --j) {
        if (!no_change)
          start = m[j].ESP;
        SweepBwdIterator<ViewX> bwd_it(x, start);
        m[j].LEP = y.push(j, bwd_it);
        start = *bwd_it;
        if (x.equiv(start, m[j].ESP))
          // Prefix cannot fit.
          return false;
      }
      no_change = false;
      yFixed = 0, nBlocks = 0;
      for (int j = 0; j < ny; ++j) {
        SweepFwdIterator<ViewX> fwd_it = x.fwd_iterator();
        y.stretch(j, fwd_it);
        start = *fwd_it;
        if (x.prec(start, m[j].LEP)) {
          // There is a gap between the latest end position of y[j] and the 
          // position of the maximum forward stretch for its latest start.
          m[j].LEP = start;
          no_change = false;
        }
        if (x.prec(m[j].LEP, m[j].ESP))
          return false;
        if (y[j].isFixed())
          yFixed++;
        else
          nBlocks += x.max_new_blocks(m[j]);
      }
    } while (no_change);
    return true;
  }
  
  // Possibly refines x and y for find propagator, knowing that x occurs in y.
  template <class ViewX, class ViewY>
  forceinline bool
  refine_find_y(Space& home, ViewX& x, ViewY& y, Matching m[], int& yFixed, 
                                                               int& nBlocks) {
    // std::cerr << "refine_find " << x << ' ' << y << '\n';
    bool yChanged = false; 
    int ny = y.size(), uy = 2*(ny-yFixed);
    Region r;
    Block* newBlocks = nullptr;
    int* U = nullptr;
    int newSize = 0, uSize = 0;
    // xfit[i] = (j, k) if blocks y[j]y[j+1]...y[k] all fit in x[i].
    std::map<int, std::pair<int,int>> xfit;
    for (int j = 0; j < ny; ++j) {
      const Block& y_j = y[j];
      Position& esp = m[j].ESP, eep = m[j].EEP, lsp = m[j].LSP, lep = m[j].LEP;
      assert (esp.isNorm(x) && lep.isNorm(x));
      // If the x-block fits all in a single y-block b, we can update b.
      if (esp.idx == lep.idx && !x[esp.idx].isFixed()) {
        int i = esp.idx;
        std::map<int, std::pair<int,int>>::iterator it = xfit.find(i);
        if (it == xfit.end())
          xfit[i] = std::pair<int,int>(j,j);
        else {
          assert (it->second.second == j-1);
          it->second.second = j;
        }
      }
      if (y_j.isFixed())
        continue;
      int l = y_j.lb(), u = y_j.ub(), l1 = min_len_mand(x, y_j, lsp, eep);
//      std::cerr << "l'=" << l1 << "\n";
      if (u < l1)
        return false;
      long u1 = x.max_len_opt(y_j, esp, lep, l1);
//      std::cerr << "u'=" << u1 << "\n";
      if (l > u1)
        return false;
      assert (l1 <= u1);
      if (l == 0 || l != l1 || u1 > u) {
        if (u1 == 0) {
          y.nullifyAt(home, j);
          yChanged = true;
          uy -= 2;
          continue;
        }
        int m = y_j.baseSize();
        x.crushBase(home, y, j, esp, lep);
        yChanged |= l < l1 || u > u1 || m > y_j.baseSize();
        if (l1 == 0 || x[j].baseSize() == 1) {
          if (l1 > l)
            y.lbAt(home, j, l1);
          if (u1 < u)
            y.ubAt(home, j, u1);
          uy -= 2;
          continue;
        }
        int np = 0, ns = 0;
        std::vector<int> vp, vs;
        if (esp == lsp)
          vp = x.fixed_pref(lsp, eep, np);
        if (eep == lep)
          vs = x.fixed_suff(lsp, eep, ns);
        np = np < 0 ? -np : np;
        ns = ns < 0 ? -ns : ns;
         if (np + ns > l1)
          return false;
        assert (vp.size() % 2 == 0 && vs.size() % 2 == 0 && np + ns <= u1);
        if (np == 0 && ns == 0) {
          nBlocks--;
          if (l1 > l)
            y.lbAt(home, j, l1);
          if (u1 < u)
            y.ubAt(home, j, u1);
          uy -= 2;
          continue;
        }
        assert (x.prec(lsp, eep) && (esp == lsp || eep == lep));
        if (U == nullptr)
          U = r.alloc<int>(uy);
        if (newBlocks == nullptr)
          newBlocks = r.alloc<Block>(nBlocks);        
        int zp = vp.size();
        for (int i = 0; i < zp-1; i += 2)
          newBlocks[newSize++].update(home, Block(vp[i], vp[i+1]));
        newBlocks[newSize].update(home, y[j]);
        newBlocks[newSize++].updateCard(home, std::max(y_j.lb(),l1)-ns-np,
                                              std::min(long(y_j.ub()),u1)-ns-np);
        int zs = vs.size();
        for (int i = zs-1; i > 0; i -= 2)
          newBlocks[newSize++].update(home, Block(vs[i-1], vs[i]));
        U[uSize++] = j;
        U[uSize++] = zp/2 + zs/2 + 1;
        continue;
      }
      assert (l1 > 0);
      int n = x.max_new_blocks(m[j]);
      assert (n > 0);
      if (n == 1) {
        nBlocks--;
        uy -= 2;
        // No need to unfold y_j.
        n = y_j.baseSize();
        x.mand_region(home, y, j, lsp, eep);
        yChanged |= l < y_j.lb() || u > y_j.ub() || n > y_j.baseSize();
        continue;
      }
      // Unfolding y_j into newBlocks
      Region r1;
      Block* mreg = r1.alloc<Block>(n);
//      std::cerr << "Before unfolding: "  << y_j << ' ' << l1 << '\n';
      if (esp == lsp)
        x.mand_region(home, y_j, &mreg[0], u1, lsp, eep);
      else {
        x.opt_region(home, y_j, mreg[0], esp, lsp, l1);
        x.mand_region(home, y_j, &mreg[1], u1, lsp, eep);
      }
      if (eep != lep)
        x.opt_region(home, y_j, mreg[n-1], eep, lep, l1);
      DashedString d(home, mreg, n);
      if (d.ub_sum() > u1)
        d.max_length(home, u1);
      r1.free();
//      std::cerr << "d = " << d << ' ' << n << "\n";
      n = d.size();
      if (n == 1) {
        nBlocks--;
        uy -= 2;
        if (d[0].ub() > u)
          d.ubAt(home, 0, u);
        if (d[0].equals(y_j))
          continue;
        y.updateAt(home, j, d[0]);
        yChanged = true;
//        std::cerr << "3) x[" << i << "] ref. into " << d[0] << "\n";
        continue;
      }
      if (U == nullptr)
        U = r.alloc<int>(uy);
      if (newBlocks == nullptr)
        newBlocks = r.alloc<Block>(nBlocks);
      for (int i = 0, k = newSize; i < n; ++i,++k)
        newBlocks[k].update(home, d[i]);
      U[uSize++] = j;
      U[uSize++] = n;
      newSize += n;
    }
    if (newSize > 0) {
      y.resize(home, newBlocks, newSize, U, uSize);
      yChanged = true;
    }
    else if (yChanged)
      x.normalize(home);
    else
      nBlocks = -1;
      
    // Possibly refining x.
//    for (std::map<int, tpl2>::iterator it  = ymatch.begin(); 
//                                       it != ymatch.end(); ++it) {
//      int j = it->first;
//      const DSBlock& y_j = y.at(j);
//      const tpl2& xreg = it->second;      
//      int sl = 0;
//      for (int i = xreg.first; sl <= y_j.u && i <= xreg.second; ++i)
//        sl += x.at(i).l;
//      if (sl == y_j.u) {
//        NSBlocks v;
//        for (int i = xreg.first; i <= xreg.second; ++i) {
//          DSBlock& y_j = x.at(i);
//          assert (y_j.known() || modx);
//          if (y_j.l > 0) {
//            NSBlock b = NSBlock(y_j);            
//            b.u = b.l;
//            if (y_j.known()) {
//              assert (y_j.S.in(b.S.min()));
//              b.S.intersect(y_j.S);
//            }
//            else {
//              y_j.u = b.l;
//              int n = b.S.size();
//              b.S.intersect(h, y_j.S);
//              if (n > b.S.size())
//                y_j.S.update(h, b.S);
//            }
//            v.push_back(b);
//          }
//          else
//            y_j.set_null(h);
//        }
//        if (v.logdim() < y_j.logdim()) {
//          mody = true;
//          upy.push(std::make_pair(j, v));
//        }
//      }
//    }

      // Possibly refining lb and ub.
//      int k = 0;
//      for (auto u : upx) {
//        const NSBlocks& us = u.second;
//        while (u.first == k && 
//        (us.size() == 0 || (us.size() == 1 && us[0].null()))) {
//          ++k;
//        }
//      }
//      int n = m.esp[k].off + 1;
//      for (int i = 0; i < m.esp[k].idx; ++i)
//        n += y.at(i).l;
//      if (n > lb)
//        lb = n;
//      n = y.at(m.lep[k].idx).u - m.lep[k].off - x.at(k).l + 1;
//      for (int i = 0; i < m.lep[k].idx; ++i)
//        n += y.at(i).u;
//      if (n < ub)
//        ub = n;
//      NSIntSet ychars = y.may_chars();
//      // Nullify incompatible x-blocks.
//      for (int i = 0; i < x.length(); ++i)
//        if (ychars.disjoint(x.at(i).S)) {
//          assert (x.at(i).l == 0);
//          x.at(i).u = 0;
//          modx = true;
//        }
    return true;
  }

  // Sweep-based algorithm for find propagator. It returns true iff !occ OR
  // x can be a substring of y. If !occ and x can't be substring of y, then ub
  // parameter is set to 0, but true is still returned.
  // If x is not modified, then lb is set to -lb. 
  // If y is not modified, then ub is set to -ub.
  template <class ViewX, class ViewY>
  forceinline bool
  sweep_find(Space& home, ViewX x, ViewY y, int& lb, int& ub, bool occ) {
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
      int yFixed = 0, nBlocks = 0;
      if (!pushLEP_find(x, y, m, yFixed, nBlocks))
        return false;
      if (!refine_find_y(home, x, y, m, yFixed, nBlocks))
        return false;
      if (yFixed == -1)
        lb = -lb;
      if (nBlocks == -1)
        ub = -ub;
    }
    return true;
  }

}}






