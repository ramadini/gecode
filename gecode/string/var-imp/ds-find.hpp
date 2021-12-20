#include <map>

namespace Gecode { namespace String {

  // Returns the least position p s.t. x[:p] has at least k characters.
  template <class View>
  forceinline Position
  idx2min_pos(const View& x, int k) {
    int idx = 0, off = k-1, n = x.size();
    while (idx < n && off >= x[idx].ub())
      off -= x[idx++].ub();
    assert (off < 0 || Position(idx,off).isNorm(x));
    return off < 0 ? Position(0,0) : Position(idx,off);
  }
  
  // Returns the least index i s.t. x[:p] has at least i-1 characters.   
  template <class View>
  forceinline int
  pos2min_idx(const View& x, const Position& p) {
    assert (p.isNorm(x));
    int idx = 1;
    for (int i = 0; i < p.idx; ++i)
      idx += lbound(x[i]);
    return ubounded_sum(idx, p.off);
  }

  // Returns the 1-based index of the first occurrence of y in x when both fixed.
  template <class ViewX, class ViewY>
  forceinline int
  find_fixed(ViewX x, ViewY y) {
    int ny = y.size();
    Matching m[ny];
    Position start(0,0);
    for (int j = 0; j < ny; ++j)
      m[j].ESP = start;
    return pushESP_find(x, y, m) ? pos2min_idx(x,m[0].ESP) : -1;
  }
 
  // Returns the index of the last occurrence of y in x when both fixed.
  template <class ViewX, class ViewY>
  forceinline int
  rfind_fixed(ViewX x, ViewY y) {
    int nx = x.size(), ny = y.size();
    Matching m[ny];
    Position end(x.size(),0);
    for (int j = 0; j < ny; ++j)
      m[j].LEP = end;
    return pushLEP_find(x, y, m, ny, ny) 
      ? pos2min_idx(x,m[nx-1].LEP) - y.max_length() + 1 : -1;
  }
    
  // Computes the fixed components of x and checks if y can occur in it. If so,
  // iv is refined accordingly.  
  template <class ViewX, class ViewY>
  forceinline ModEvent
  fixed_comp(Space& home, ViewX x, ViewY y, Gecode::Int::IntView iv) {
//    std::cerr << "fixed_comp " << x << ".find( " << y << " ) = "<<iv<<"\n";
    Region r;    
    int n = x.max_length(), nx = x.size(), k = 0;
    Block* curr = r.alloc<Block>(nx);
    Position start = idx2min_pos(x,iv.min());
    for (int i = start.idx; n > 0 && i < nx; ++i) {
      const Block& b = x[i];
//      std::cerr << i << ": " << b << ", start: " << start << "\n";
      if (b.baseSize() == 1 && b.lb() > 0) {
        Block t(b.baseMin(), std::min(b.lb(), n));
        curr[k++].update(home, t);
        ConstDashedView cv(*curr,k);        
        int k = find_fixed(cv, y);
//        std::cerr << "Curr: " << cv << ", k = " << k << '\n';
        if (k > 0) {
          ModEvent ml = iv.gq(home,1);
          GECODE_ME_CHECK(ml);
          int ub = start.off + k;
          for (int i = 0; i < start.idx && ub < iv.max(); ++i)
            ub += x[i].ub();
          ModEvent mu = iv.lq(home, ub);
          GECODE_ME_CHECK(mu);
          return Gecode::Int::IntVarImp::me_combine(ml, mu);
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
        start = Position(i+1, 0);
      }
      n -= b.ub();
    }
    return Gecode::Int::ME_INT_NONE;
  }

  // Pushes fwd the earliest start position of each y-block in x for find(x,y),
  // knowing that, if occ, y must start between index l and u (possibly refined
  // after the pushing).
  template <class ViewX, class ViewY>
  forceinline bool
  pushESP_find(const ViewX x, ViewY y, Matching m[], int& l, int& u, bool occ) {
    bool again = false;
    int ny = y.size();
    Position start = m[0].ESP;
    do {
      for (int j = 0; j < ny; ++j) {
        if (again)
          start = x.prec(m[j].ESP, start) ? start : m[j].ESP;
        SweepFwdIterator<ViewX> fwd_it(x, start);
        m[j].ESP = y.push(j, fwd_it);
        start = *fwd_it;
        assert (!x.prec(start,m[j].ESP));
//        std::cerr << "ESP of y[" << j << "] = " << y[j] << ": " << m[j].ESP << '\n';        
        if (y[j].lb() > 0 && x.equiv(m[j].ESP, Position(x.size(),0))) {
          // y can't fit in x.
          if (occ)
            return false;
          if (u > 0)
            u = 0;
          return true;
        }
      }
      again = false;
      // Stretching backward to see if we can further move forward the ESPs.
      for (int j = ny-1; j >= 0; --j) {
        SweepBwdIterator<ViewX> bwd_it(x,start);
        y.stretch(j, bwd_it);
        start = *bwd_it;
//        std::cerr << "Stretching bwd: y[" << j << "] = " << y[j] << ": " << start << " vs " << m[j].ESP << '\n';
        if (x.prec(m[j].ESP, start)) {
          m[j].ESP = start;
          again = true;
//          std::cerr << "Adjusted ESP of y[" << j << "] = " << y[j] << ": " << m[j].ESP << '\n';
        }
      }
//      std::cerr << (again ? "Again!\n" : "");
    } while (again);
    // Possibly refining l and u, by converting from position to index.
    l = std::max(l, pos2min_idx(x, m[0].ESP));
    if (l > u) {
      // y can't fit in x.
      if (occ)
        return false;
      if (u > 0)
        u = 0;
      return true;
    }
    return true;
  }
  
  // Pushes fwd the earliest start position of each y-block in x for replace prop.
  template <class ViewX, class ViewY>
  forceinline bool
  pushESP_find(const ViewX x, ViewY y, Matching m[]) {
//    std::cerr << "pushESP_find(" << x << ", " << y << ")\n";
    bool again = false;
    int ny = y.size();
    Position start = m[0].ESP;
    do {
      for (int j = 0; j < ny; ++j) {
        if (again)
          start = x.prec(m[j].ESP, start) ? start : m[j].ESP;
        SweepFwdIterator<ViewX> fwd_it(x, start);
        m[j].ESP = y.push(j, fwd_it);
        start = *fwd_it;
        assert (!x.prec(start,m[j].ESP));
//        std::cerr << "ESP of y[" << j << "] = " << y[j] << ": " << m[j].ESP << '\n';        
        if (lbound(y[j]) > 0 && x.equiv(m[j].ESP, Position(x.size(),0)))
          return false;
      }
      again = false;
      // Stretching backward to see if we can further move forward the ESPs.
      for (int j = ny-1; j >= 0; --j) {
        SweepBwdIterator<ViewX> bwd_it(x,start);
        y.stretch(j, bwd_it);
        start = *bwd_it;
//        std::cerr << "Stretching bwd: y[" << j << "] = " << y[j] << ": " << start << " vs " << m[j].ESP << '\n';
        if (x.prec(m[j].ESP, start)) {
          m[j].ESP = start;
          again = true;
//          std::cerr << "Adjusted ESP of y[" << j << "] = " << y[j] << ": " << m[j].ESP << '\n';
        }
      }
//      std::cerr << (again ? "Again!\n" : "");
    } while (again);
    return true;
  }

  // Pushes bwd the latest end position of each y-block in x for find(x,y), 
  // assuming that y must occur in x. It also sets latest starts, earliest ends
  // and yFixed, nBlocks parameters for the following refining of y.
  template <class ViewX, class ViewY>
  forceinline bool
  pushLEP_find(const ViewX& x, const ViewY& y, Matching m[], int& yFixed, 
                                                             int& nBlocks) {
//    std::cerr << "pushLEP_find(" << x << ", " << y << ")\n";
    bool again = false;
    int ny = y.size();
    Position start = m[ny-1].LEP;
    do {      
      for (int j = ny-1; j >= 0; --j) {
        if (again)
          start = x.prec(start, m[j].LEP) ? start : m[j].LEP;
        SweepBwdIterator<ViewX> bwd_it(x, start);
        m[j].LEP = y.push(j, bwd_it);
        start = *bwd_it;
        assert (!x.prec(m[j].LEP, start));
//        std::cerr << "pushed bwd: y[" << j << "] = " << y[j] << ": " << start << '\n';
        if (x.equiv(Position(0,0), m[j].LEP))
          // y cannot fit in x.
          return false;
//        std::cerr << "LEP of y[" << j << "] = " << y[j] << ": " << m[j].LEP << '\n';
      }
      again = false;
      yFixed = 0, nBlocks = 0;
      // Stretching forward to see if we can further move backward the LEPs.
      for (int j = 0; j < ny; ++j) {
        SweepFwdIterator<ViewX> fwd_it(x,start);
        y.stretch(j, fwd_it);
        start = *fwd_it;
//        std::cerr << "pushing fwd: y[" << j << "] = " << y[j] << ": " << start << '\n';
        if (x.prec(start, m[j].LEP)) {
          // There is a gap between the latest end position of y[j] and the 
          // position of the maximum forward stretch for its latest start.
          m[j].LEP = start;
          again = true;
//          std::cerr << "Adjusted LEP of y[" << j << "] = " << y[j] << ": " << m[j].LEP << '\n';
          continue;
        }
        if (x.prec(m[j].LEP, m[j].ESP) 
        || (x.equiv(m[j].ESP, m[j].LEP) && y[j].lb() > 0))
          return false;
        m[j].LSP = j > 0 ? m[j-1].LEP : m[0].ESP;
        if (x.prec(m[j].LSP,m[j].ESP))
          return false;
        m[j].EEP = j < ny-1 ? m[j+1].ESP : m[j].LEP;
        if (x.prec(m[j].LEP,m[j].EEP))
          return false;        
        if (y[j].isFixed())
          yFixed++;
        else
          nBlocks += x.max_new_blocks(m[j]);        
      }
    } while (again);
    return true;
  }
  
  // Possibly refines x and y for find propagator, assuming that y must occur 
  // in x.
  template <class ViewX, class ViewY>
  forceinline bool
  refine_find(Space& home, ViewX& x, ViewY& y, int& lb, int& ub, Matching m[], 
                                               int& yFixed, int& nBlocks) {
//    std::cerr << "refine_find " << x << ' ' << y << '\n';
    bool yChanged = false; 
    int ny = y.size(), uy = 2*(ny-yFixed);
    Region r;
    Block* newBlocks = nullptr;
    int* U = nullptr;
    int newSize = 0, uSize = 0, nx = 0, idxNotNull = -1;
    // xfit[i] = (j, k) if blocks y[j]y[j+1]...y[k] all fit in x[i].
    std::map<int, std::pair<int,int>> xfit;
    for (int j = 0; j < ny; ++j) {
      const Block& y_j = y[j];
      const Position& esp = m[j].ESP, eep = m[j].EEP,  lsp = m[j].LSP, 
                      lep =m[j].LEP;
      assert (j == ny-1 || !x.prec(m[j+1].LSP, m[j].LEP));
//      std::cerr<<"Ref. y[" << j << "] = " << y[j] <<"\tESP: "<<esp<<"\tLSP: "<<lsp <<"\tEEP: "<<eep<<"\tLEP: "<<lep<< "\n";
      assert (esp.isNorm(x) && lsp.isNorm(x) && eep.isNorm(x) && lep.isNorm(x));
      if (esp == lep) {
        assert (y_j.lb() == 0);
        y.nullifyAt(home, j);
        continue;
      }
      // If some y-blocks fits all in a single x-block x[i], we will update x[i]
      if ((esp.idx == lep.idx || (esp.idx == lep.idx-1 && lep.off == 0)) &&
        !x[esp.idx].isFixed()) {
        int i = esp.idx;
        std::map<int, std::pair<int,int>>::iterator it = xfit.find(i);
        if (it == xfit.end())
          xfit[i] = std::pair<int,int>(j,j);
        else {
//          std::cerr << it->second.second << ' ' << j << '\n';
          assert (it->second.second == j-1);
          it->second.second = j;
        }
        nx++;
      }
      if (y_j.isFixed())
        continue;
      int l = y_j.lb(), u = y_j.ub(), l1 = min_len_mand(x, y_j, lsp, eep);
//      std::cerr << "l'=" << l1 << "\n";
      int u1 = 0;
      for (int i = esp.idx; i < lep.idx + (lep.off > 0); ++i) {
        if (!x[i].baseDisjoint(y[j]))
          u1 += x[i].ub();
        if (u1 > u)
          break;
      }
//      std::cerr << "u'=" << u1 << "\n";
      if (l > u1)
        return false;
      if (u1 == 0) {
        y.nullifyAt(home, j);
        if (idxNotNull == j-1)
          idxNotNull = j;
        yChanged = true;
        uy -= 2;
        continue;
      }
      if (l1 == 0 || l != l1 || u1 > u) {
        int m = y_j.baseSize();
        x.crushBase(home, y, j, esp, lep);
        if (u1 < u)
          y.ubAt(home, j, u1);
        yChanged |= u > u1 || m > y_j.baseSize();
        uy -= 2;
        continue;
      }
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
    
    // Possibly refining y.
    if (newSize > 0) {
      y.resize(home, newBlocks, newSize, U, uSize);
      yChanged = true;
    }
    r.free(U, uSize);
    r.free(newBlocks, newSize);
    
    // Possibly refining x (and y).
    bool xChanged = false;
    newSize = 0;
    if (!x.assigned()) {
      int ux = 2*nx;
      uSize = 0;
      for (std::map<int, std::pair<int,int>>::iterator it = xfit.begin(); 
                                                       it != xfit.end(); ++it) {
        int i = it->first;
        const Block& x_i = x[i];
        assert (!x_i.isFixed());
        const std::pair<int,int>& yreg = it->second;      
        int sl = 0, j0 = yreg.first, j1 = yreg.second;
        for (int j = j0; sl <= x_i.ub() && j <= j1; ++j)
          sl += y[j].lb();
//        std::cerr << sl << '\n';
        if (sl == x_i.ub()) {
          // y[j0]...y[j1] matching region falls within block x[i] and the no. 
          // of mandatory chars is equal to x[i].ub(). Hence we can refine x[i].
          int k = 0, nx = j1-j0+1;
          Block* vx = r.alloc<Block>(nx);
          for (int j = j0; j <= j1; ++j) {
            const Block& y_j = y[j];
            int lj = y_j.lb();
            if (lj > 0) {
              vx[k].update(home, y_j);
              vx[k].updateCard(home, lj, lj);
              vx[k++].baseIntersect(home, x_i);
            }
            else {
              y.nullifyAt(home, j);
              if (idxNotNull == j-1)
                idxNotNull = j;
              yChanged = true;
            }
          }        
          DashedString dx(home, vx, k);
          r.free(vx, nx);
          if (dx.logdim() < x_i.logdim()) {
            xChanged = true;
            int n = dx.size();
            if (n == 1) {
              nx--;
              ux -= 2;
              if (dx[0].ub() > x_i.ub())
                dx.ubAt(home, 0, x_i.ub());
              if (dx[0].equals(x_i))
                continue;
              x.updateAt(home, i, dx[0]);
              continue;
            }
            if (U == nullptr)
              U = r.alloc<int>(ux);
            if (newBlocks == nullptr)
              newBlocks = r.alloc<Block>(nx);
            for (int j = 0, k = newSize; j < n; ++j,++k)
              newBlocks[k].update(home, dx[j]);
            U[uSize++] = i;
            U[uSize++] = n;
            newSize += n;
          }
        }
      }
      // Nullify incompatible y-blocks.
      Set::GLBndSet s;
      for (int i = 0; i < x.size(); ++i) {
        if (x[i].isUniverse())
          goto refine; 
        x[i].includeBaseIn(home, s);
      }
      const Block& xchars = Block(home, CharSet(home, s));
      for (int j = 0; j < y.size(); ++j) {
        if (y[j].baseDisjoint(xchars)) {
          assert (y[j].lb() == 0);
          y.nullifyAt(home, j);
          yChanged = true;
        }
      }
    }
  refine:
    // Refining x and y.
    if (newSize > 0)
      x.resize(home, newBlocks, newSize, U, uSize);
    else if (xChanged)
      x.normalize(home);
    else
      // x not modified.
      yFixed = -1;
    if (yChanged)
      y.normalize(home);
    else
      // y not modified.
      nBlocks = -1;
      
    // Possibly refining lower and upper bounds of the index variable.
    if (idxNotNull != -1) {
      int n = m[idxNotNull].ESP.off + 1;
      for (int i = 0; i < m[idxNotNull].ESP.idx; ++i)
        n += x[i].lb();
      if (n > lb)
        lb = n;
      n = m[idxNotNull].LSP.off + 1;
      for (int i = 0; i < m[idxNotNull].LEP.idx; ++i)
        n += x[i].ub();
      if (n < ub)
        ub = n;
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
  sweep_find(Space& home, ViewX x, ViewY y, int& lb, int& ub, bool occ) {
//    std::cerr << "sweep_find "<<x<<".find( "<<y<<" ) = "<<lb<<".."<<ub<<" \n";
    assert (lb >= 0 && ub >= 0);    
    int ny = y.size();
    Matching m[ny];
    Position start = idx2min_pos(x,lb), end = Position(x.size(),0);
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
      if (!refine_find(home, x, y, lb, ub, m, yFixed, nBlocks))
        return false;
      if (yFixed == -1)
        lb = -lb;
      if (nBlocks == -1)
        ub = -ub;
    }
    return true;
  }
  
  // Initializes pos such that:
  //   pos[0] = earliest start position of y in x
  //   pos[1] = latest end position of y in x
  // If pos = nullptr, then y cannot be substring of x.
  template <class ViewX, class ViewY>
  forceinline void
  check_find(const ViewX& x, const ViewY& y, Position* pos) {
//    std::cerr << "check_find("<<x<<", "<<y<<")\n";
    int ny = y.size();
    Matching m[ny];
    Position start(0,0), end(x.size(),0);
    for (int j = 0; j < ny; ++j) {
      m[j].ESP = start;
      m[j].LEP = end;
    }
    if (!pushESP_find(x, y, m)) {
      pos[0].idx = -1;
      return;
    }
    int tmp1, tmp2;
    if (!pushLEP_find(x, y, m, tmp1, tmp2)) {
      pos[0].idx = -1;
      return;
    }
    pos[0] = m[0].ESP;
    pos[1] = m[ny-1].LEP;
    assert(x.prec(pos[0],pos[1]));
    if (!pos[1].isNorm(x)) {
      pos[1].idx++;
      pos[1].off = 0;
    }
  }

}}






