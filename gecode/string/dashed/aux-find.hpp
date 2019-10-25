namespace Gecode { namespace String {
  
  // Pushing the earliest start positions of x in y for find propagator.
  forceinline bool
  push_esp_find(
    const DSBlocks& x, const DSBlocks& y, const Position& start,
    int& lb, int& ub, matching& m, int xmin, int ymax, bool mod
  ) {
    // std::cerr << "push_esp_find " << x << ' ' << y << '\n';
    bool gap = false;
    do {
      int xlen = x.length();
      Position end(start);
      Position last = last_fwd(y);
      for (int i = 0; i < xlen; ++i) {
        if (gap)
          end = m.esp[i];
        m.esp[i] = push<Fwd, DSBlock, DSBlock, DSBlocks>(y, x.at(i), end, last);
        // std::cerr << "ESP[" << i << "] = " << m.esp[i] << end << "\n";
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
      last = last_bwd(y);
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
  
  // Pushing forward the earliest start of x-blocks in y for replace propagator.
  forceinline bool
  push_esp_repl(const DSBlocks& x, const DSBlocks& y, matching& m) {
    // std::cerr << "push_esp_repl " << x << ' ' << y << '\n';    
    bool gap = false;
    do {      
      int xlen = x.length();
      Position end({0, 0});
      Position last = last_fwd(y);
      for (int i = 0; i < xlen; ++i) {
        if (gap)
          end = m.esp[i];
        m.esp[i] = push<Fwd, DSBlock, DSBlock, DSBlocks>(y, x.at(i), end, last);
        //std::cerr << "ESP[" << i << "] = " << m.esp[i] << "\n";
        if (last == m.esp[i])
          // Prefix cannot fit.
          return false;
      }
      gap = false;
      end = dual(y, end);
      last = last_bwd(y);
      for (int i = xlen - 1; i >= 0; --i) {
        end = dual(y, 
          stretch<Bwd, DSBlock, DSBlock, DSBlocks>(y, x.at(i), end, last)
        );
        if (Fwd::lt(m.esp[i], end)) {
          // There is a gap between the earliest start position of block x[i] 
          // and the position of the maximum backward stretch for the earliest 
          // end of x[i].
          m.esp[i] = end;
          gap = true;
          // std::cerr << "ESP[" << i << "] = " << m.esp[i] << " (***GAP***)\n";
        }
        end = dual(y, end);        
      }
    } while (gap);
    return true;
  }
  
  // Pushing backward latest ends of x-blocks in y for find/replace propagators.
  forceinline bool
  push_lep(const DSBlocks& x, const DSBlocks& y, matching& m) {
    // std::cerr << "push_lep " << x << ' ' << y << '\n';
    bool gap = false;
    do {      
      int xlen = x.length();
      Position end = first_bwd(y);
      Position last = last_bwd(y);
      for (int i = xlen - 1; i >= 0; --i) {
        if (gap)
          end = m.lep[i];
        m.lep[i] = push<Bwd, DSBlock, DSBlock, DSBlocks>(y, x.at(i), end, last);
        // std::cerr << "LEP[" << i << "] = " << m.lep[i] << "\n";
        if (last == m.lep[i])
          // Suffix cannot fit.
          return false;
      }
      gap = false;
      end = dual(y, end);
      last = last_fwd(y);
      for (int i = 0; i < xlen; ++i) {
        end = dual(y, 
          stretch<Fwd, DSBlock, DSBlock, DSBlocks>(y, x.at(i), end, last)
        );
        if (Bwd::lt(m.lep[i], end)) {
          // There is a gap between the latest end position of block x[i] 
          // and the position of the maximum forward stretch for the latest 
          // start of x[i].
          m.lep[i] = end;
          gap = true;
          // std::cerr << "LEP[" << i << "] = " << m.lep[i] << " (***GAP***)\n";
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
  check_find(const DashedString& x, const DashedString& y, Position* pos) {
    const DSBlocks& xblocks = x.blocks();
    const DSBlocks& yblocks = y.blocks();
    Position firstf({0, 0}), firstb(first_bwd(y));
    matching m;
    int xlen = x.length();
    for (int i = 0; i < xlen; ++i) {
      m.esp.push(firstf);
      m.lep.push(firstb);
    }
    if (!push_esp_repl(xblocks, yblocks, m))
      return false;
    if (!push_lep(xblocks, yblocks, m))
      return false;
    for (int i = 0; i < xlen; ++i) {
      Position es = m.esp[i];
      Position le = m.lep[i];
      Position ls = i ? dual(y, m.lep[i - 1]) : m.esp[0];
      Position ee = i < xlen - 1 ? dual(y, m.esp[i + 1]) : m.lep[i];
      // std::cerr<<"Block "<<i<<": "<<x.at(i)<<"  es: "<<es<<" ee: "<<ee
      //  <<" ls: "<<ls<<" le: "<<le<<'\n';
      if (!check_positions(y, es, ls, ee, le))
        return false;
      if (i == 0)
        pos[0] = es;
      if (i == xlen - 1)
        pos[1] = dual(y, le);
    }
    return Fwd::le(pos[0], pos[1], y.at(pos[1].idx).u);
  }
  
  // Possibly refines x and y for find propagator, knowing that x occurs in y.
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
    // ymatch[i] = (j, k) if blocks x[j]x[j+1]...x[k] all fit in y[i].
    std::map<int, tpl2> ymatch;
    for (int i = 0; i < xlen; ++i) {
      DSBlock& xi = x.at(i);
      Position es = m.esp[i];
      Position le = m.lep[i];
      if (!Fwd::le(es, dual(y, le), upper(y.at(le.idx))))
        return false;
      // If the x-block fits all in a single y-block b, we can update b.
      if (es.idx == le.idx && !y.at(es.idx).known()) {
        std::map<int, tpl2>::iterator it = ymatch.find(es.idx);
        if (it == ymatch.end())
          ymatch[es.idx] = tpl2(i, i);
        else
          it->second.second = i;          
      }
      Position ls = i ? dual(y, m.lep[i - 1]) : m.esp[0];
      Position ee = i < xlen - 1 ? dual(y, m.esp[i + 1]) : m.lep[i];
      if (!check_positions(y, es, ls, ee, le))
        return false;
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
    // Possibly refining y-blocks.        
    for (std::map<int, tpl2>::iterator it  = ymatch.begin(); 
                                       it != ymatch.end(); ++it) {
      int j = it->first;
      const DSBlock& y_j = y.at(j);
      const tpl2& xreg = it->second;      
      int sl = 0;
      for (int i = xreg.first; sl <= y_j.u && i <= xreg.second; ++i)
        sl += x.at(i).l;
      if (sl == y_j.u) {
        NSBlocks v;
        for (int i = xreg.first; i <= xreg.second; ++i) {
          DSBlock& x_i = x.at(i);
          assert (x_i.known() || modx);
          if (x_i.l > 0) {
            NSBlock b = NSBlock(x_i);            
            b.u = b.l;
            if (x_i.known()) {
              assert (y_j.S.in(b.S.min()));
              b.S.intersect(y_j.S);
            }
            else {
              x_i.u = b.l;
              int n = b.S.size();
              b.S.intersect(h, y_j.S);
              if (n > b.S.size())
                x_i.S.update(h, b.S);
            }
            v.push_back(b);
          }
          else
            x_i.set_null(h);
        }
        if (v.logdim() < y_j.logdim()) {
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
    // Looking for the first position in y where x can occur.
    int b = 0, p = lb - 1;
    while (b < y.length() && p >= y.at(b).u) {
      p -= y.at(b).u;
      b++;
    }
    Position start({b, p});
    DSBlocks& xblocks = x.blocks();
    DSBlocks& yblocks = y.blocks();
    matching m;
    Position firstf({0, 0}), firstb(first_bwd(y));
    for (int i = 0; i < x.length(); ++i) {
      m.esp.push(start);
      m.lep.push(firstb);
    }
    if (!push_esp_find(
      xblocks, yblocks, start, lb, ub, m, x.min_length(), y.max_length(), mod
    ))
      return false;
    if (mod) {
      if (!push_lep(xblocks, yblocks, m))
        return false;
      uvec upx, upy;
      bool modx = false, mody = false;
      if (!refine_find(h, xblocks, yblocks, m, upx, upy, modx, mody))
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
  
}}
