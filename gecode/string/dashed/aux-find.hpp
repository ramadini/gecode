namespace Gecode { namespace String {
  
  /******************
  * Find propagator *
  *******************/
  
  // Pushing the earliest start positions of x in y for find propagator.
  forceinline bool
  push_esp_find(
    const DSBlocks& x, const DSBlocks& y, const Position& start,
    int& lb, int& ub, matching& m, int xmin, int ymax, bool mod
  ) {
    // std::cerr << "push_esp_find " << x << ' ' << y << '\n';
    bool gap = false;
    int xlen = x.length();
    Position end(start);
    // Refining esp for each x-block.
    do {
      Position last({(int) y.length() - 1, y.back().u});
      for (int i = 0; i < xlen; ++i) {
        if (gap)
          end = m.esp[i];
        m.esp[i] = push<Fwd, DSBlock, DSBlock, DSBlocks>(y, x.at(i), end, last);
        // std::cerr << "ESP[" << i << "] = " << m.esp[i] << "; " << end << "\n";
        if (last == m.esp[i] && x.at(i).l > 0) {
          // Prefix cannot fit.
          if (mod)
            return false;
          if (ub > 0)
            ub = 0;
          return true;
        }
      }
      gap = false;
      end = dual(y, end);
      last = Position({0, y.at(0).u});
      for (int i = xlen - 1; i >= 0; --i) {
        end = dual(y, 
          stretch<Bwd, DSBlock, DSBlock, DSBlocks>(y, x.at(i), end, last)
        );
        if (Fwd::lt(m.esp[i], end)) {
          // If there is a gap between the earliest start position of block x[i] 
          // and the position of the maximum backward stretch for the earliest 
          // end of x[i].
          m.esp[i] = end;
          gap = true;
          // std::cerr << "ESP[" << i << "] = " << m.esp[i] << " (gap!)\n";
        }
        end = dual(y, end);        
      }
    } while (gap);
    // Possibly refining lb and ub (converting from position to index).
    long l = m.esp[0].off + 1;
    for (int i = 0; i < m.esp[0].idx; ++i)
      l += y.at(i).l;
    lb = max(lb, l);
    ub = min(ub, ymax - xmin + 1);
    if (lb > ub) {
      if (mod)
        return false;
      if (ub > 0)
        ub = 0;
      return true;
    }
    return true;
  }
  
  // Possibly refines x and y, knowing that find(x, y) > 0.
  forceinline bool
  refine_find(Space& h, DSBlocks& x, DSBlocks& y,
    matching& m, uvec& upx, uvec& upy, bool& modx, bool& mody
  ) {
    // std::cerr << "refine_find " << x << ' ' << y << '\n';
    NSIntSet p_set;
    NSBlocks p_reg;
    Position p_es{-1, -1}, p_ls{-1, -1}, p_ee{-1, -1}, p_le{-1, -1};
    int p_l = 0;
    int xlen = x.length();
    std::map<int, sweep_stack> ymatch;
    for (int i = 0; i < xlen; ++i) {
      DSBlock& xi = x.at(i);
      Position es = m.esp[i];
      Position le = m.lep[i];
      if (!Fwd::le(es, dual(y, le), upper(y.at(le.idx))))
        return false;
      // If the x-block fits all in a single y-block b, we can update b.
      if (es.idx == le.idx && !y.at(es.idx).known())
        ymatch[es.idx].push(i);
      Position ls = i ? dual(y, m.lep[i - 1]) : m.esp[0];
      Position ee = i < xlen - 1 ? dual(y, m.esp[i + 1]) : m.lep[i];
      if (!Fwd::le(es, ls, upper(y.at(ls.idx)))) {
        if (es.idx == ls.idx) {
          if (es.off - ls.off > upper(y.at(es.idx)) - lower(y.at(es.idx)))
            return false;
          es = ls;
        }
        return false;
      } 
      if (!Bwd::le(le, ee, upper(y.at(ee.idx)))) {
        if (ee.idx == le.idx) {
          if (ee.off - le.off > upper(y.at(ee.idx)) - lower(y.at(ee.idx)))
            return false;
          le = ee;
        }
        else
          return false;
      }
      // std::cerr<<"Block "<<i<<": "<<x.at(i)<<"  es: "<<es<<" ee: "<<ee
      //  <<" ls: "<<ls<<" le: "<<le<<'\n';
      if (xi.known())
        continue;
      if (es != p_es || ls != p_ls || ee != p_ee || le != p_le) {
        // Current matching region is different from the previous one.
        p_reg.clear(); p_set.clear();
        p_es = es, p_ls = ls, p_ee = ee, p_le = le;
        assert (p_l >= 0);
        if (es != ls)
          opt_region<DSBlock, DSBlocks>(y, es, dual(y, ls), p_reg);
        p_l = man_region<DSBlock, DSBlocks>(y, ls, ee, p_reg);
        if (ee != le)
          opt_region<DSBlock, DSBlocks>(y, dual(y, ee), le, p_reg);
        for (unsigned i = 0; i < p_reg.size(); ++i)
          p_set.include(p_reg[i].S);
      }
      long u = 0;  
      for (int i = 0; i < p_reg.length(); ++i) {
        if (xi.S.disjoint(p_set))
          continue;
        // p_reg[i].u - p.reg[i].l + p_l <= xi.u.
        u += p_reg[i].u;
      }
      // std::cerr << p_reg << ' ' << p_l << '\n';
      if (xi.l > u)
        return false;
      modx = true;
      if (u == 0) {
        upx.push(std::make_pair(i, NSBlocks(1)));
        continue;
      }
      if (xi.l == p_l && u <= xi.u)
        upx.push(std::make_pair(i, p_reg));
      else {
        p_set.intersect(xi.S);
        NSBlocks v(1, NSBlock(p_set, xi.l, min(xi.u, u)));
        upx.push(std::make_pair(i, v));
      }
    }
    // Possibly refining y.
    for (std::map<int, sweep_stack>::iterator it  = ymatch.begin();
                                              it != ymatch.end(); ++it) {
      int j = it->first;
      const DSBlock& yj = y.at(j);
      const sweep_stack& s = it->second;
      int sl = 0;
      for (int i = 0; i < s.size() && sl <= yj.u; ++i)
        sl += x.at(s[i]).l;
      if (yj.u == sl) {
        NSBlocks v;
        for (int k = 0; k < s.size(); ++k) {
          int i = s[k];
          NSBlock b = NSBlock(x.at(i));
          if (b.l > 0) {
            b.S.intersect(h, yj.S);
            b.u = b.l;
            x.at(s[i]).update(h, b);
          }
          else {
            x.at(s[i]).u = b.u = 0;
            b.S.clear();
          }
          v.push_back(b);
        }
        if (v.logdim() < yj.logdim()) {
          mody = true;
          upy.push(std::make_pair(j, v));
        }
      }
    }
    return true;
  }
  
  // Sweep-based algorithm for find propagator. It returns true iff !mod or 
  // x can be a substring of y. If !mod and x can't be substring of y, then ub 
  // parameter is set to 0, but true is still returned.
  forceinline bool
  sweep_find(
    Space& h, DashedString& x, DashedString& y, int& lb, int& ub, bool mod
  ) {
    // std::cerr<<"sweep_find "<<x<<' '<<y<<' '<<"["<<lb<<", "<<ub<<"] "<<mod<<'\n';
    assert (1 <= lb && lb <= ub);    
    DSBlocks& xblocks = x.blocks();
    DSBlocks& yblocks = y.blocks();
    matching m;
    for (int i = 0; i < x.length(); ++i) {
      m.esp.push(Position({0, 0}));
      m.lep.push(Position({0, 0}));
    }
    // Looking for the first position in y where x can occur.
    int b = 0, p = lb - 1;
    while (b < y.length() && p >= y.at(b).u) {
      p -= y.at(b).u;
      b++;
    }
    Position start({b, p});
    if (!push_esp_find(
      xblocks, yblocks, start, lb, ub, m, x.min_length(), y.max_length(), mod
    ))
      return false;
    if (mod) {
      Position end({(int) y.length() - 1, 0}), last({0, y.at(0).u});
      for (int i = x.length() - 1; i >= 0; --i)
        m.lep[i] = 
          push<Bwd, DSBlock, DSBlock, DSBlocks>(yblocks, x.at(i), end, last);
      uvec upx, upy;
      bool modx = false, mody = false;
      if (!refine_find(
        h, x.blocks(), y.blocks(), m, upx, upy, modx, mody
      ))
        return false;
      int k = 0;
      for (auto u : upx) {
        const NSBlocks& us = u.second;
        while (u.first == k && 
        (us.size() == 0 || (us.size() == 1 && us[0].null()))) {
          ++k;
        }
      }
      int n = m.esp[k].off + 1;
      for (int i = 0; i < m.esp[k].idx; ++i)
        n += y.at(i).l;
      if (n > lb)
        lb = n;
      n = y.at(m.lep[k].idx).u - m.lep[k].off - x.at(k).l + 1;
      for (int i = 0; i < m.lep[k].idx; ++i)
        n += y.at(i).u;
      if (n < ub)
        ub = n;
      NSIntSet ychars = y.may_chars();
      // Nullify incompatible x-blocks.
      for (int i = 0; i < x.length(); ++i)
        if (ychars.disjoint(x.at(i).S)) {
          assert (x.at(i).l == 0);
          x.at(i).u = 0;
          modx = true;
        }
      if (modx)
        refine_eq(h, x, upx);      
      if (mody)
        refine_eq(h, y, upy);
      
    }
    // std::cerr<<"swept: "<<x<<' '<<y<<' '<<"["<<lb<<", "<<ub<<"] "<<mod<<'\n';
    return true;
  }


  /*********************
  * Replace propagator *
  *********************/

  // Pushing the earliest start positions of x in y for replace propagator.
  forceinline bool
  push_esp_repl(const DSBlocks& x, const DSBlocks& y, matching& m) {
    // std::cerr << "push_esp_find " << x << ' ' << y << '\n';
    bool gap = false;
    int xlen = x.length();
    Position end({0, 0});
    // Refining esp for each x-block.
    do {
      Position last({(int) y.length() - 1, y.back().u});
      for (int i = 0; i < xlen; ++i) {
        if (gap)
          end = m.esp[i];
        m.esp[i] = push<Fwd, DSBlock, DSBlock, DSBlocks>(y, x.at(i), end, last);
        // std::cerr << "ESP[" << i << "] = " << m.esp[i] << "; " << end << "\n";
        if (last == m.esp[i] && x.at(i).l > 0)
          // Prefix cannot fit.
          return false;
      }
      gap = false;
      end = dual(y, end);
      last = Position({0, y.at(0).u});
      for (int i = xlen - 1; i >= 0; --i) {
        end = dual(y, 
          stretch<Bwd, DSBlock, DSBlock, DSBlocks>(y, x.at(i), end, last)
        );
        if (Fwd::lt(m.esp[i], end)) {
          // If there is a gap between the earliest start position of block x[i] 
          // and the position of the maximum backward stretch for the earliest 
          // end of x[i].
          m.esp[i] = end;
          gap = true;
          // std::cerr << "ESP[" << i << "] = " << m.esp[i] << " (gap!)\n";
        }
        end = dual(y, end);        
      }
    } while (gap);
    return true;
  }
 
  // Returns true iff x may be a substring of y, and initializes pos such that:
  //   pos[0] = earliest start position of x in y
  //   pos[1] = latest end position of x in y
  // The offset of both pos[0] and pos[1] is positive.
  forceinline bool
  sweep_replace(DashedString& x, DashedString& y, Position* pos) {
    // std::cerr << "sweep_replace " << x << ' ' << y << '\n';
    DSBlocks& xblocks = x.blocks();
    DSBlocks& yblocks = y.blocks();
    matching m;
    for (int i = 0; i < x.length(); ++i) {
      m.esp.push(Position({0, 0}));
      m.lep.push(Position({0, 0}));
    }
    if (!push_esp_repl(xblocks, yblocks, m))
      return false;
    Position end({(int) y.length() - 1, 0}), last({0, y.at(0).u});
    for (int i = x.length() - 1; i >= 0; --i)
      m.lep[i] = 
        push<Bwd, DSBlock, DSBlock, DSBlocks>(yblocks, x.at(i), end, last);
    int xlen = x.length();
    for (int i = 0; i < xlen; ++i) {
      Position es = m.esp[i];
      Position le = m.lep[i];
      Position ls = i ? dual(y, m.lep[i - 1]) : m.esp[0];
      Position ee = i < xlen - 1 ? dual(y, m.esp[i + 1]) : m.lep[i];
      // std::cerr<<"Block "<<i<<": "<<x.at(i)<<"  es: "<<es<<" ee: "<<ee
      //  <<" ls: "<<ls<<" le: "<<le<<'\n';
      if (!Fwd::le(es, dual(y, le), upper(y.at(le.idx))))
        return false;
      if (!Fwd::le(es, ls, upper(y.at(ls.idx)))) {
        if (es.idx == ls.idx) {
          if (es.off - ls.off > upper(y.at(es.idx)) - lower(y.at(es.idx)))
            return false;
          es = ls;
        }
        return false;
      } 
      if (!Bwd::le(le, ee, upper(y.at(ee.idx)))) {
        if (ee.idx == le.idx) {
          if (ee.off - le.off > upper(y.at(ee.idx)) - lower(y.at(ee.idx)))
            return false;
          le = ee;
        }
        else
          return false;
      }
      if (i == 0)
        pos[0] = es;
      if (i == xlen - 1)
        pos[1] = dual(y, ee);
    }
    assert (!Fwd::lt(pos[1], pos[0]));
    return true;
  }
  
}}
