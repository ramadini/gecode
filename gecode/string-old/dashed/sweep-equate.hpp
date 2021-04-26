#include <gecode/string/dashed/aux-equate.hpp>

namespace Gecode { namespace String {

/*************************
* Push/stretch functions *
*************************/

  // Given block b and initial position p, find the earliest start/end for b in 
  // x starting from p (note that push can skip incompatible x-blocks).
  // The position returned is the earliest start of b, while p will be updated 
  // with its earliest end. If it returns end then there is no match for b in 
  // x[p, end].
  template <class Succ, class Block1, class Block2, class Blocks2>
  forceinline Position
  push(const Blocks2& x, const Block1& b, Position& p, const Position& end) {
    // std::cerr << "push " <<b<< " in " <<x<< " from " <<p<< " to " <<end<< "\n";  
    Position start(p); 
    int k = lower(b);
    while (k > 0) {
      // End reached: b has no match in x.
      if (!Succ::lt(p, end))
        return end;
      assert (p.idx >= 0 && p.idx < (int) x.length());
      const Block2& x_i = x.at(p.idx);
      if (disjoint(b, x_i)) {
        NEXT(p, end);
        if (p.off < lower(x_i)) {
          // The current block _must_ start after here: we update start.
          k = lower(b);
          start = p;
        }
      }
      else {
        // Fit as much of x into pos.b as we can.
        if (k > upper(x_i) - p.off) {
          k -= upper(x_i) - p.off;
          NEXT(p, end);
        }
        else {
          p.off += k;
          break;
        }
      }
    }
    // Shift to the start of the following block.
    if (start.off == upper(x.at(start.idx)))
      NEXT(start, end);
    return start;
  }

  // Stretch block b as far from position p as possible (differently from push, 
  // stretch does not skip incompatible blocks). It returns the latest position 
  // from p where b can finish.
  template <class Succ, class Block1, class Block2, class Blocks2>
  forceinline Position
  stretch(const Blocks2& x, const Block1& b, Position& p, const Position& end) {
    // std::cerr << "stretch " <<b<< " in " <<x<< " from " <<p<< " to " <<end<< "\n";  
    Position start(p);
    int k = upper(b);
    while (Succ::lt(p, end)) {
      // End reached: we can stretch b until the end of x.
      if (!Succ::lt(p, end))
        return end;
      const Block2& x_i = x.at(p.idx);
      int lb;
      if (Succ::fwd())
        lb = lower(x_i) - p.off;
      else {
        int d = upper(x_i) - lower(x_i);
        if (p.off < d) {
          p.off = d;
          lb = lower(x_i);
        }
        else
          lb = upper(x_i) - p.off;
      }
      if (lb <= 0) {
        NEXT(p, end);
        continue;
      }
      // Block b is incompatible with x[p.idx].
      if (disjoint(b, x_i))
        return p;
      // No more characters.
       if (k < lb) {
         p.off += k;
         return p;
       }
      // Consume lb characters.
      else {
        k -= lb;
        NEXT(p, end);
      }
    }
    return p;
  }
  
  // Stretching backward the earliest end of x[i] in y. This may in turn stretch
  // backward the earliest end of x[i-1], x[i-2], ...
  template <class Block1, class Blocks1, class Block2, class Blocks2>
  bool
  stretch_eep(
    const Blocks1& x, const Blocks2& y, matching& m, int i, sweep_stack& stack
  ) {
    // See if we can stretch x[i] from the earliest start of x[i + 1] to its 
    // current earliest start.
    Position start(dual(y, m.esp[i + 1]));
    Position end(dual(y,
      stretch<Bwd,Block1,Block2,Blocks2>(y, x.at(i), start, dual(y, m.esp[i]))
    ));
    // If we cannot stretch x[i] to its current earliest start, we update it and 
    // we iterate the process for i-1, i-2, ...
    if (Fwd::lt(m.esp[i], end)) {
      m.esp[i] = end;
      // Each stretched block has to be pushed forward again afterwards, so we 
      // put its index on the stack.
      if (i > 0) {
        stack.sweep_push(i);
        return stretch_eep<Block1,Blocks1,Block2,Blocks2>(x, y, m, i-1, stack);
      }
    }
    return true;
  }

  // Pushing forward the earliest start of x[i] in y.
  template <class Block1, class Block2, class Blocks2>
  forceinline bool
  push_esp(const Block1& b, const Blocks2& y, matching& m, int i) {
    assert(!DashedString::_QUAD_SWEEP);
    int n = m.esp.size() - 1;
    if (lower(b) == 0) {
      // x[i] is nullable, we will push x[i+1] from the earliest start of x[i].
      if (i < n && Fwd::lt(m.esp[i + 1], m.esp[i]))
        m.esp[i + 1] = m.esp[i];
      return true;
    }
    Position last(last_fwd(y));
    Position end(m.esp[i]);
    Position start = push<Fwd, Block1, Block2, Blocks2>(y, b, end, last);
    if (!Fwd::lt(start, last))
      return false;
    // The earliest start of x[i + 1] is <= the earliest end of x[i].
    if (i < n && Fwd::lt(m.esp[i + 1], end))
      m.esp[i + 1] = end;
    // If we pushed x[i] forward, we update its earliest start position.
    if (Fwd::lt(m.esp[i], start))
      m.esp[i] = start;
    return true;
  }
  template <class Block1, class Blocks1, class Block2, class Blocks2>
  forceinline bool
  push_esp(
    const Blocks1& x, const Blocks2& y, matching& m, int i, sweep_stack& stack
  ) {
    assert(DashedString::_QUAD_SWEEP && (stack.size() == 0 || stack.top() > i));
    if (lower(x.at(i)) == 0) {
      // x[i] is nullable, we will push x[i+1] from the earliest start of x[i].
      if (i + 1 < (int) x.length() && Fwd::lt(m.esp[i + 1], m.esp[i]))
        m.esp[i + 1] = m.esp[i];
      return true;
    }
    Position last(last_fwd(y));
    Position end(m.esp[i]);
    Position start = push<Fwd, Block1, Block2, Blocks2>(y, x.at(i), end, last);
    if (!Fwd::lt(start, last))
      return false;
    // The earliest start of x[i + 1] is <= the earliest end of x[i].
    if (i + 1 < (int) x.length() && Fwd::lt(m.esp[i + 1], end))
      m.esp[i + 1] = end;
    // If we pushed x[i] forward, we update its earliest start position.
    if (Fwd::lt(m.esp[i], start)) {      
      m.esp[i] = start;
      if (i > 0) {
        // Before pushing x[i+1], we may stretch backward x[i-1] from the new 
        // earliest start of x[i] to check if we can find a better earliest end 
        // for x[i-1]. Note that in this way the sweeping might be quadratic.
        stack.sweep_push(i);
        return stretch_eep<Block1,Blocks1,Block2,Blocks2>(x, y, m, i-1, stack);
      }
    }
    return true;
  }

  // Stretching forward the latest start of x[i] in y. This may in turn stretch
  // forward the latest start of x[i+1], x[i+2], ...
  template <class Block1, class Blocks1, class Block2, class Blocks2>
  bool
  stretch_lsp(
    const Blocks1& x, const Blocks2& y, matching& m, int i, sweep_stack& stack
  ) {
    // See if we can stretch x[i] from the latest end of x[i - 1] to its 
    // current latest end.
    assert (i > 0);
    Position start(dual(y, m.lep[i - 1]));
    Position end(dual(y,
      stretch<Fwd,Block1,Block2,Blocks2>(y, x.at(i), start, dual(y, m.lep[i]))
    ));
    // If we cannot stretch x[i] to its current latest end, we update it and 
    // we iterate the process for i+1, i+2, ...
    if (Bwd::lt(m.lep[i], end)) {      
      m.lep[i] = end;
      // Each stretched block has to be pushed backward again afterwards, so we 
      // put its index on the stack.
      if (i + 1 < (int) x.length()) {
        stack.sweep_push(i);
        return stretch_lsp<Block1,Blocks1,Block2,Blocks2>(x, y, m, i+1, stack);
      }
    }
    return true;
  }

  // Pushing backward the latest end of x[i] in y. The eq parameter is for find 
  // propagator.
  template <class Block1, class Block2, class Blocks2>
  forceinline bool
  push_lep(
    const Block1& b, const Blocks2& y, matching& m, int i, bool eq = true
  ) {
    assert(!DashedString::_QUAD_SWEEP);
    if (lower(b) == 0) {
      // x[i] is nullable, we will push x[i-1] from the latest end of x[i].
      if (i > 0 && Bwd::lt(m.lep[i - 1], m.lep[i]))
        m.lep[i - 1] = m.lep[i];
      return true;
    }
    Position last(last_bwd(y));
    Position start(m.lep[i]);
    Position end = push<Bwd, Block1, Block2, Blocks2>(y, b, start, last);
    if (eq && !Bwd::lt(end, last))
      return false;
    // If the latest end of x[i-1] is before the earliest start of x[i].
    if (i > 0 && Bwd::lt(m.lep[i - 1], start))
      m.lep[i - 1] = start;
    // If we pushed x[i] backward, we update its latest end position.
    if (Bwd::lt(m.lep[i], end))
      m.lep[i] = end;
    return true;
  }
  template <class Block1, class Blocks1, class Block2, class Blocks2>
  forceinline bool
  push_lep(
    const Blocks1& x, const Blocks2& y, matching& m, int i, sweep_stack& stack,
    bool eq = true
  ) {
    assert(DashedString::_QUAD_SWEEP && (stack.size() == 0 || stack.top() < i));
    if (lower(x.at(i)) == 0) {
      // x[i] is nullable, we will push x[i-1] from the latest end of x[i].
      if (i > 0 && Bwd::lt(m.lep[i - 1], m.lep[i]))
        m.lep[i - 1] = m.lep[i];
      return true;
    }
    Position last(last_bwd(y));
    Position start(m.lep[i]);
    Position end = push<Bwd, Block1, Block2, Blocks2>(y, x.at(i), start, last);
    if (eq && !Bwd::lt(end, last))
      return false;
    // If the latest end of x[i-1] is before the earliest start of x[i].
    if (i > 0 && Bwd::lt(m.lep[i - 1], start))
      m.lep[i - 1] = start;
    // If we pushed x[i] backward, we update its latest end position.
    if (Bwd::lt(m.lep[i], end)) {
      m.lep[i] = end;      
      if (i + 1 < (int) x.length()) {
        // Before pushing x[i-1], we may stretch forward x[i+1] from the new 
        // latest end of x[i] to check if we can find a better latest start for 
        // x[i-1]. Note that in this way the sweeping might be quadratic.
        stack.sweep_push(i);
        return stretch_lsp<Block1,Blocks1,Block2,Blocks2>(x, y, m, i+1, stack);
      }
    }
    return true;
  }  


/*****************
* Core functions *
*****************/

  // Extends v with the block corresponding to the optional region x[p, q].
  // Position p has positive offset, while position q negative offset.
  template <class Block, class Blocks>
  forceinline void
  opt_region(
    const Blocks& x, const Position& p, const Position& q, NSBlocks& v
  ) {
    // std::cerr << "split opt " <<p<<' '<<q << std::endl;
    if (p.idx > q.idx)
      return;
    if (p.idx == q.idx) {
      const Block& xi = x.at(p.idx);
      int u = xi.u - q.off - p.off;
      if (u > 0)
        v.push_back(NSBlock(xi.S, 0, u));
      return;
    }
    NSBlock b;
    const Block& b1 = x.at(p.idx);    
    if (p.off < b1.u)
      b = NSBlock(b1.S, 0, b1.u - p.off);
    long u = b.u;
    for (int j = p.idx + 1; j < q.idx; ++j) {
      const Block& bj = x.at(j);
      b.S.include(bj.S);
      u += bj.u;
    }
    const Block& b2 = x.at(q.idx);
    if (q.off < b2.u) {
      b.S.include(b2.S);
      u += b2.u - q.off;
    }
    b.u = u < DashedString::_MAX_STR_LENGTH ? u : DashedString::_MAX_STR_LENGTH;
    v.push_back(b);
  }

  // Extends v with the mandatory region of x between p and q.
  // Position p has positive offset, while position q negative offset.
  // Returns the minimum length of the mandatory region.
  template <class Block, class Blocks>
  forceinline int
  man_region(
    const Blocks& x, const Position& p, const Position& q, NSBlocks& v
  ) {
    int l = 0;
    if (p.idx > q.idx)
      return l;
    if (p.idx == q.idx) {
      const Block& xi = x.at(p.idx);
      if (p.off >= xi.u - q.off)
        return l;
      l = max(0, xi.l - q.off - p.off);
      v.push_back(NSBlock(xi.S, l, xi.u - p.off - q.off));
      return l;
    }
    const Block& b1 = x.at(p.idx);
    if (p.off < b1.u) {
      l = max(0, b1.l - p.off);
      v.push_back(NSBlock(b1.S, l, b1.u - p.off));
    }
    for (int j = p.idx + 1; j < q.idx; ++j) {
      const Block& bj = x.at(j);
      v.push_back(NSBlock(bj));
      l += bj.l;
    }
    const Block& b2 = x.at(q.idx);
    if (q.off < b2.u) {
      int ll = max(0, b2.l - q.off);
      v.push_back(NSBlock(b2.S, ll, b2.u - q.off));
      l += ll;
    }
    return l;
  }
  
  // Set up latest/earliest start/end positions for x-blocks in y.
  template <class Block1, class Blocks1, class Block2, class Blocks2>
  forceinline bool
  init_x(const Blocks1& x, const Blocks2& y, matching& m) {
    Position firstf({0, 0});
    int xlen = x.length(), ylen = y.length();
    // Checks for concrete strings.
    if (xlen == 0) {
      if (ylen == 0)
        return true;
      Position firstb(first_bwd(y));
      for (int i = 0; i < ylen; ++i) {
        if (lower(y.at(i)) > 0)
          return false;
        m.lep.push(firstf);
        m.esp.push(firstb);
      }
      return true;
    }
    if (ylen == 0) {
      Position firstb(first_bwd(y));
      for (int i = 0; i < xlen; ++i) {
        if (lower(x.at(i)) > 0)
          return false;
        m.lep.push(firstf);
        m.esp.push(firstb);
      }
      return true;
    }
    Position firstb(first_bwd(y)), lastf(last_fwd(y)), lastb(last_bwd(y));
    for (int i = 0; i < xlen; ++i) {
      m.esp.push(firstf);
      m.lep.push(firstb);
    }
    // Stretching forward for latest end positions.
    Position curr = firstf;
    for (int i = 0; i < xlen; ++i) {
      curr = stretch<Fwd, Block1, Block2, Blocks2>(y, x.at(i), curr, lastf);
      m.lep[i] = dual(y, curr);
      // std::cerr << i << ": LEP(" << x.at(i) << ") = " << m.lep[i] << "\n";
    }
    if (Bwd::lt(firstb, m.lep.last()))
      return false;
    // Stretching backward for earliest start positions.
    curr = firstb;
    for (int i = xlen - 1; i >= 0; --i) {
      curr = stretch<Bwd, Block1, Block2, Blocks2>(y, x.at(i), curr, lastb);      
      m.esp[i] = dual(y, curr);
      // std::cerr << i << ": ESP(" << x.at(i) << ") = " << m.esp[i] << "\n";
    }
    return !Fwd::lt(firstf, m.esp[0]);
  }

  // Sanity checks on earliest/latest start/end positions.
  template <class Blocks>
  forceinline bool
  check_positions(
    const Blocks& x, Position& es, Position& ls, Position& ee, Position& le
  ) {
    // std::cerr<<"check "<<"  es: "<<es<<" ls: "<<ls<<" ee: "<<ee<<" le: "<<le<<'\n';
    // std::cerr<<"in "<<x<<'\n';
    if (!Fwd::le(es, ls, upper(x.at(ls.idx))) || 
        !Bwd::le(le, ee, upper(x.at(ee.idx))))
      return false;
    assert (Fwd::le(es, dual(x, le), upper(x.at(le.idx))));
    return true;
  }
  
  // Fwd declaration for deep check.
  template <class Block1, class Blocks1, class Block2, class Blocks2>
  forceinline bool check_sweep(const Blocks1&, const Blocks2&);

  // Matches each x-block against the blocks of y, and records into up parameter
  // all the blocks that must be updated.
  template <class Block1, class Blocks1, class Block2, class Blocks2>
  forceinline bool
  sweep_x(Space& h, Blocks1& x, const Blocks2& y, uvec& up) {
    // std::cerr << "sweep_x " << x << " vs " << y << std::endl;
    matching m;
    if (!init_x<Block1, Blocks1, Block2, Blocks2>(x, y, m))
      return false;
    int xlen = x.length();
    if (DashedString::_QUAD_SWEEP) {
      sweep_stack stack;
      // Update EST and EET for all x-blocks
      for (int i = xlen - 1; i >= 0; --i)
        stack.push(i);
      while (stack.size() > 0) {
        int i = stack.top();
        stack.pop();
        // Pushing EST forward for x[0], x[1], ...
        if (!push_esp<Block1, Blocks1, Block2, Blocks2>(x, y, m, i, stack))
          return false;
      }
      // Update LET and LST for all x-blocks
      for (int i = 0; i < xlen; ++i)
        stack.push(i);
      while (stack.size() > 0) {
        int i = stack.top();
        stack.pop();
        // Pushing LET backward for x[xlen-1], x[xlen-2], ... 
        if (!push_lep<Block1, Blocks1, Block2, Blocks2>(x, y, m, i, stack))
          return false;
      }
    }
    else {
      for (int i = 0; i < xlen; ++i)
        if (!push_esp<Block1, Block2, Blocks2>(x.at(i), y, m, i))
          return false;
      for (int i = xlen - 1; i >= 0; --i)
        if (!push_lep<Block1, Block2, Blocks2>(x.at(i), y, m, i))
          return false;
    }
    // Now look for possible refinements.
    NSIntSet p_set;
    NSBlocks p_reg;
    Position p_es{-1, -1}, p_ls{-1, -1}, p_ee{-1, -1}, p_le{-1, -1};
    int p_l = 0;
    for (int i = 0; i < xlen; ++i) {
      Position es = m.esp[i]; // Fwd direction (positive offset).
      Position ls = i ? dual(y, m.lep[i - 1]) : m.esp[0]; // Fwd.
      Position le = m.lep[i]; // Bwd direction (negative offset).
      Position ee = i < xlen - 1 ? dual(y, m.esp[i + 1]) : m.lep[i]; // Bwd.
      // std::cerr<<"Block "<<i<<": "<<x.at(i)<<"  es: "<<es<<" ls: "<<ls
      //   <<" ee: "<<ee<<" le: "<<le<<'\n';
      if (!check_positions(y, es, ls, ee, le))
        return false;     
      Block1& xi = x.at(i);
      if (xi.known())
        continue;
      if (es != p_es || ls != p_ls || ee != p_ee || le != p_le) {
        // Current matching region is different from the previous one.
        p_reg.clear(); p_set.clear();
        p_es = es, p_ls = ls, p_ee = ee, p_le = le;
        if (es != ls)
          opt_region<Block2, Blocks2>(y, es, dual(y, ls), p_reg);
        p_l = man_region<Block2, Blocks2>(y, ls, ee, p_reg);
        assert (p_l >= 0);
        if (ee != le)
          opt_region<Block2, Blocks2>(y, dual(y, ee), le, p_reg);
        for (unsigned i = 0; i < p_reg.size(); ++i)
          p_set.include(p_reg[i].S);
      }
      int k = xi.u - p_l;
      if (k < 0)
        return false;
      long u = 0;  
      for (int i = 0; i < p_reg.length(); ++i) {
        if (xi.S.disjoint(p_set))
          continue;
        // p_reg[i].u - p.reg[i].l + p_l <= xi.u.
        u += min(k + p_reg[i].l, p_reg[i].u);
      }
      // std::cerr << xi << ' ' << p_l << ' ' << u << '\n';
      assert (xi.l <= u);
      if (u == 0) {
        up.push(std::make_pair(i, NSBlocks(1)));
        continue;
      }
      if (p_l == 0 || p_l < xi.l || u > xi.u) {
        string kpref = p_reg.known_pref(), ksuff = p_reg.known_suff();
        if (xi.l <= p_l && xlen == 1) {
          // Special case where x is a single block: the soundness of the
          // propagation is preserved by constraining the min/max length.
          for (unsigned i = 0; i < p_reg.size(); ++i) {
            p_reg[i].S.intersect(xi.S);
            p_reg[i].u = min(p_reg[i].u, xi.u);
          }
          p_reg.normalize();
          if (kpref != "" || ksuff != "" || p_reg.logdim() < x.at(0).logdim()) {
            up.push(std::make_pair(0, p_reg));
            // std::cerr << "1) x'_i: " << p_reg << "\n";
            return true;
          }
        }
        // Possibly crushing the matching region.
        int n = xi.S.size();
        xi.S.intersect(h, p_set);
        if (p_l > xi.l || u < xi.u || kpref != "" || ksuff != ""
        || (int) xi.S.size() < n) {
          NSBlocks v;
          int ll = max(p_l, xi.l), uu = min(u, xi.u), nn = kpref.size();
          if (nn > 0) {
            v = NSBlocks(kpref);
            ll -= nn;
            uu -= nn;
          }
          v.push_back(NSBlock(xi.S, ll, uu));
          nn = ksuff.size();
          if (nn > 0) {
            v.back().l -= nn;
            v.back().u -= nn;
            v.extend(NSBlocks(ksuff));
          }
          up.push(std::make_pair(i, v));
          // std::cerr << "2) x'_i: " << v << "\n";
        }
        continue;
      }
      // The matching region _must_ contain at least a character.
      NSBlocks w;
      for (int i = 0; i < p_reg.length(); ++i) {
        NSIntSet s(p_reg[i].S);
        s.intersect(xi.S);
        if (!s.empty()) {
          // p_reg[i].u - p.reg[i].l + p_l <= xi.u.
          int lb = p_reg[i].l, ub = min(k + lb, p_reg[i].u);
          w.push_back(NSBlock(s, lb, ub));
        }
        else
          assert (xi.l == 0 || p_reg[i].l == 0);
      }
      w.normalize();
      // std::cerr << "3) x'_i: " << w << "\n";
      if (w.size() != 1 || !(w[0] == xi))
        up.push(std::make_pair(i, w));
      assert (w.logdim() <= xi.logdim());
    }
    assert ((m.esp[0] == Position({0, 0}) || null(y.at(0))) &&
            (m.lep[xlen - 1] == first_bwd(y) || null(y.at(y.length() - 1))));
    return true;
  }

  // Matches each x-block against the blocks of y, but only checks feasibility 
  // (no refinement is performed).
  template <class Block1, class Blocks1, class Block2, class Blocks2>
  forceinline bool
  check_sweep(const Blocks1& x, const Blocks2& y) {
    // std::cerr << "check_sweep " << x << " vs " << y << std::endl;
    matching m;
    if (!init_x<Block1, Blocks1, Block2, Blocks2>(x, y, m))
      return false;
    if (x.length() == 0 || y.length() == 0)
      return true;
    int xlen = x.length();
    if (DashedString::_QUAD_SWEEP) {
      sweep_stack stack;
      // Update EST and EET for all x-blocks
      for (int i = xlen - 1; i >= 0; --i)
        stack.push(i);
      while (stack.size() > 0) {
        int i = stack.top();
        stack.pop();
        // Pushing EST forward for x[0], x[1], ...
        if (!push_esp<Block1, Blocks1, Block2, Blocks2>(x, y, m, i, stack))
          return false;
      }
      // Update LET and LST for all x-blocks
      for (int i = 0; i < xlen; ++i)
        stack.push(i);
      while (stack.size() > 0) {
        int i = stack.top();
        stack.pop();
        // Pushing LET backward for x[xlen-1], x[xlen-2], ... 
        if (!push_lep<Block1, Blocks1, Block2, Blocks2>(x, y, m, i, stack))
          return false;
      }
    }
    else {
      for (int i = 0; i < xlen; ++i)
        if (!push_esp<Block1, Block2, Blocks2>(x.at(i), y, m, i))
          return false;
      for (int i = xlen - 1; i >= 0; --i)
        if (!push_lep<Block1, Block2, Blocks2>(x.at(i), y, m, i))
          return false;
    }
    for (int i = 0; i < xlen; ++i) {
      Position es = m.esp[i]; // Fwd direction (positive offset).
      Position ls = i ? dual(y, m.lep[i - 1]) : m.esp[0]; // Fwd.
      Position le = m.lep[i]; // Bwd direction (negative offset).
      Position ee = i < xlen - 1 ? dual(y, m.esp[i + 1]) : m.lep[i]; // Bwd.
      // std::cerr<<"Block "<<i<<": "<<x.at(i)<<"  es: "<<es<<" ls: "<<ls
      //   <<" ee: "<<ee<<" le: "<<le<<'\n';
      if (!check_positions(y, es, ls, ee, le))
        return false;
    }
    // std::cerr << m.esp[0] << ' ' << m.lep[xlen-1] << "\n";
    assert ((m.esp[0] == Position({0, 0}) || null(y.at(0))) &&
            (m.lep[xlen - 1] == first_bwd(y) || null(y.at(y.length() - 1))));
    return true;
  }  


/*******************
* Refine functions *
*******************/
   
  forceinline void
  refine_eq(Space& h, DashedString& x, uvec up) {
    // std::cerr << "refine_eq " << x << "\n";
    x.changed(true);
    int m = 0;
    for (auto pair : up) {
      int n = pair.second.size();
      if (n > 1)
        m += n - 1;
    }
    if (m > 0) {
      // At least a block of x expands into >1 blocks.
      int n = x.length() + m;
      DSBlock* xx = h.alloc<DSBlock>(n);
      int i_x = 0, i_xx = 0, j = 0;
      for (auto pair : up) {
        for (; i_xx < pair.first + j; ++i_x, ++i_xx)
          xx[i_xx].init(h, x.at(i_x));
        ++i_x;
        for (auto b : pair.second) {
          xx[i_xx].init(h, b);
          ++i_xx;
        }
        if (pair.second.size() > 1)
          j += pair.second.size() - 1;
      }
      for (; i_xx < n; ++i_x, ++i_xx)
        xx[i_xx].init(h, x.at(i_x));
      x.blocks().become(h, xx, n);
      x.normalize(h);
      return;
    }
    for (auto pair : up) {
      if (pair.second.empty())
        x.at(pair.first).set_null(h);
      else
        x.at(pair.first).update(h, pair.second[0]);
    }
    x.normalize(h);
  }  

  forceinline void
  refine_concat(Space& h, DashedString& x, DashedString& y, uvec up) {
    x.changed(true);
    int lx = x.length(), k = 0, m = 0;
    for (k = 0; k < up.size(); ++k) {
      const std::pair<int, NSBlocks>& pair = up[k];
      if (pair.first >= lx)
        break;
      if (pair.second.size() > 1)
        m += pair.second.size() - 1;
    }
    if (m > 0) {
      // At least a block of x expands into >1 blocks.
      DSBlock* xx = h.alloc<DSBlock>(lx + m);
      int i_x = 0, i_xx = 0, j = 0;
      for (int i = 0; i < k; ++i) {
        const std::pair<int, NSBlocks>& pair = up[i];
        for (; i_xx < pair.first + j; ++i_x, ++i_xx)
          xx[i_xx].init(h, x.at(i_x));
        ++i_x;
        for (auto b : pair.second) {
          xx[i_xx].init(h, b);
          ++i_xx;
        }
        if (pair.second.size() > 1)
          j += pair.second.size() - 1;
      }
      for (; i_xx < lx + m; ++i_x, ++i_xx)
        xx[i_xx].init(h, x.at(i_x));
      x.blocks().become(h, xx, lx + m);
      x.normalize(h);      
    }
    else {
      for (auto pair : up) {
        if (pair.first >= lx)
          break;
        if (pair.second.empty())
          x.at(pair.first).set_null(h);
        else
          x.at(pair.first).update(h, pair.second[0]);
      }
      x.normalize(h);
    }
    if (k == up.size())
      return;
    y.changed(true);
    m = 0;    
    for (int i = k; i < up.size(); ++i) {
      int n = up[i].second.size();
      if (n > 1)
        m += n - 1;
    }
    if (m > 0) {
      // At least a block of y expands into >1 blocks.
      DSBlock* yy = h.alloc<DSBlock>(y.length() + m);
      int i_y = 0, i_yy = 0, j = 0;
      for (int i = k; i < up.size(); ++i) {
        const std::pair<int, NSBlocks>& pair = up[i];
        for (; i_yy < pair.first - lx + j; ++i_y, ++i_yy)
          yy[i_yy].init(h, y.at(i_y));
        ++i_y;
        for (auto b : pair.second) {
          yy[i_yy].init(h, b);
          ++i_yy;
        }
        if (pair.second.size() > 1)
          j += pair.second.size() - 1;
      }
      for (; i_yy < y.length() + m; ++i_y, ++i_yy)
        yy[i_yy].init(h, y.at(i_y));
      y.blocks().become(h, yy, m + y.length());
      y.normalize(h);
    }
    else {
      for (; k < up.size(); ++k) {
        const std::pair<int, NSBlocks>& pair = up[k];
        if (pair.second.empty())
          y.at(pair.first - lx).set_null(h);
        else
          y.at(pair.first - lx).update(h, pair.second[0]);
      }
      y.normalize(h);
    }
  }

  forceinline void
  update(NSBlocks& x, int i, const NSBlocks& v) {
    if (v.size() == 1) {
      if (!(v[0] == x[i]))
        x[i] = v[0];
      return;
    }
    x.erase(x.begin() + i);
    if (!v.empty())
      x.insert(x.begin() + i, v.begin(), v.end());
  }

  forceinline void
  refine_concat(Space& home, NSBlocks& xn, vec<DashedString*> x, uvec up) {
    // std::cerr << "refine_concat " << xn << '\n';
    // for (auto px:x) std::cerr<<"--> "<<*px<<'\n';
    int l = x.size() - 1;
    x[l]->changed(true);
    int p[l], q[up.size()];
    p[0] = x[0]->length();
    for (int k = 1; k < x.size(); ++k)
      p[k] = p[k - 1] + x[k]->length();
    // p[k] = \sum_{i = 0}^k |x[k]|
    int i = 0, j = 0;
    for (; i < up.size(); ++i) {
      while (p[j] <= up[i].first)
        ++j;
      q[i] = j;
    }
    i = j = 0;
    for (auto pair : up) {
      int n = pair.first + j, m = pair.second.size() - 1;
      for (int k = 0; k < l; ++k)
        if (n < p[k]) {
          x[k]->changed(true);
          break;
        }
      update(xn, n, pair.second);
      if (m != 0) {
        j += m;
        for (int k = q[i]; k < l; ++k)
          p[k] += m;
      }
      ++i;
    }
    if (!x[0]->known()) {
      NSBlocks xx(xn.slice(0, p[0]));
      x[0]->norm_update(home, xx);
    }
    for (int k = 1; k < l; ++k) {
      if (!x[k]->known()) {
        NSBlocks xx(xn.slice(p[k - 1], p[k]));
        x[k]->norm_update(home, xx);
      }
    }
    if (!x[l]->known()) {
      NSBlocks xx(xn.slice(p[l - 1], xn.size()));
      x[l]->norm_update(home, xx);
    }
  }

  forceinline void
  refine_rev(Space& h, DashedString& x, uvec up) {
    // std::cerr << "refine_rev: " << x << "\n";
    x.changed(true);
    int m = 0;
    for (auto pair : up) {
      int n = pair.second.size();
      if (n > 1)
        m += n - 1;
    }
    if (m > 0) {
      int lx = x.length();
      int n = lx + m;
      DSBlock* xx = h.alloc<DSBlock>(n);
      int i_x = 0, i_xx = 0, j = 0;
      for (auto pair : up) {
        for (; i_xx < pair.first + j; ++i_x, ++i_xx) 
          xx[n - i_xx - 1].init(h, x.at(lx - i_x - 1));
        ++i_x;
        for (auto b : pair.second) {
          xx[n - i_xx - 1].init(h, b);
          ++i_xx;
        }
        if (pair.second.size() > 1)
          j += pair.second.size() - 1;
      }
      for (; i_xx < n; ++i_x, ++i_xx)
        xx[n - i_xx - 1].init(h, x.at(lx - i_x - 1));
      x.blocks().become(h, xx, n);
      x.normalize(h);
      return;
    }
    int lx = x.length();
    for (auto pair : up) {
      if (pair.second.empty())
        x.at(lx - pair.first - 1).set_null(h);
      else
        x.at(lx - pair.first - 1).update(h, pair.second[0]);
    }
    x.normalize(h);
  }

  
/******************
* Sweep functions *
******************/

  forceinline bool
  sweep_equate(Space& h, DashedString& x, DashedString& y) {
    uvec upx;
    DSBlocks& by = y.blocks();
    if (!sweep_x<DSBlock, DSBlocks, DSBlock, DSBlocks>(h, x.blocks(), by, upx))
      return false;
    if (upx.size() > 0)
      refine_eq(h, x, upx);
    uvec upy;
    if (!sweep_x<DSBlock, DSBlocks, DSBlock, DSBlocks>(h, by, x.blocks(), upy))
      return false;
    if (upy.size() > 0)
      refine_eq(h, y, upy);
    return true;
  }

  forceinline bool
  sweep_equate(Space& h, DashedString& x, const NSBlocks& y) {
    if (!check_sweep<NSBlock, NSBlocks, DSBlock, DSBlocks>(y, x.blocks()))
      return false;
    uvec upx;
    if (!sweep_x<DSBlock, DSBlocks, NSBlock, NSBlocks>(h, x.blocks(), y, upx))
      return false;
    if (upx.size() > 0)
      refine_eq(h, x, upx);
    return true;
  }

  forceinline bool
  sweep_concat(Space& h, ConcatView& xy,
    DashedString& x, DashedString& y, DashedString& z
  ) {
    uvec up1;
    if (z.known()) {
      if (!check_sweep<char, string, DSBlock, ConcatView>(z.val(), xy))
        return false;
    }
    else {
      if (!sweep_x
      <DSBlock, DSBlocks, DSBlock, ConcatView>(h, z.blocks(), xy, up1))
        return false;
      if (up1.size() > 0)
        refine_eq(h, z, up1);
    }
    uvec up2;
    if (!sweep_x
    <DSBlock, ConcatView, DSBlock, DSBlocks>(h, xy, z.blocks(), up2))
      return false;
    if (up2.size() > 0)
      refine_concat(h, x, y, up2);
    return true;
  }

  forceinline bool
  sweep_concat(Space& h, NSBlocks& xn, vec<DashedString*> x, DashedString& y) {
    uvec up1;
    if (!sweep_x
    <DSBlock, DSBlocks, NSBlock, NSBlocks>(h, y.blocks(), xn, up1))
      return false;
    if (!y.known() && up1.size() > 0)
      refine_eq(h, y, up1);
    uvec up2;
    if (!sweep_x
    <NSBlock, NSBlocks, DSBlock, DSBlocks>(h, xn, y.blocks(), up2))
      return false;
    if (up2.size() > 0)
      refine_concat(h, xn, x, up2);
    return true;
  }

  forceinline bool
  sweep_reverse(Space& h, ReverseView& xr, DashedString& x, DashedString& y) {
    uvec up1;
    if (!sweep_x
    <DSBlock, DSBlocks, DSBlock, ReverseView>(h, y.blocks(), xr, up1))
      return false;
    if (up1.size() > 0)
      refine_eq(h, y, up1);
    uvec up2;
    if (!sweep_x
    <DSBlock, ReverseView, DSBlock, DSBlocks>(h, xr, y.blocks(), up2))
      return false;
    if (up2.size() > 0)
      refine_rev(h, x, up2);
    return true;
  }

}}
