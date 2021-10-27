namespace Gecode { namespace String {

  template <>
  forceinline SweepIterator<SweepFwd,ConstDashedView>::
  SweepIterator(const ConstDashedView& x) : view_ref(x), pos(Position(0,0)) {};
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstDashedView>::operator()(void) const {
    return view_ref.get().prec(pos, Position(view_ref.get().size(),0));
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConstDashedView>::nextBlock() {
    if (pos.idx >= view_ref.get().size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK() || view_ref.get()[pos.idx].isNull());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstDashedView>::hasNextBlock(void) const {
    return pos.idx < view_ref.get().size();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConstDashedView>::must_consume() const {
    return std::max(0, lb() - pos.off);
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConstDashedView>::may_consume() const {
    return ub() - pos.off;
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConstDashedView>::consume(int k) {
    if (k == 0)
      return;
    pos.off += k;
    if (pos.off >= view_ref.get()[pos.idx].ub()) {
      if (pos.off > view_ref.get()[pos.idx].ub())
        throw OutOfLimits("ConstDashedView::SweepBwdIterator::consume");
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConstDashedView>::consumeMand(int k) {
    if (k == 0)
      return;
    pos.off += k; 
    if (pos.off > view_ref.get()[pos.idx].lb())
      throw OutOfLimits("ConstDashedView::SweepBwdIterator::consume");
    if (pos.off == view_ref.get()[pos.idx].ub()) {
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstDashedView>::disj(const Block& b) const {
    return view_ref.get()[pos.idx].baseDisjoint(b);
  }  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstDashedView>::disj(int c) const {
    return !view_ref.get()[pos.idx].baseContains(c);
  }

}}

namespace Gecode { namespace String {
  
  template <>
  forceinline SweepIterator<SweepBwd,ConstDashedView>::
  SweepIterator(const ConstDashedView& x) : view_ref(x), pos(Position(x.size(),0)) {};

  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstDashedView>::lb() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstDashedView>::ub() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConstDashedView>::disj(const Block& b) const {
    return  view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConstDashedView>::disj(int c) const {
    return !(view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(c));
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConstDashedView>::operator()(void) const {
    return view_ref.get().prec(Position(0,0), pos);
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConstDashedView>::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (isOK() || view_ref.get()[pos.idx].isNull());
  };
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConstDashedView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstDashedView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].ub() : pos.off;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstDashedView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].lb() : 
            std::min(pos.off,view_ref.get()[pos.idx].lb());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConstDashedView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].ub() - k;
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ConstDashedView>::consume");
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConstDashedView>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, view_ref.get()[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ConstDashedView>::consumeMand");
    assert (isOK());
  }

}}

namespace Gecode { namespace String {
  
  forceinline
  ConstDashedView::ConstDashedView() : b0(NULL), n(0) {GECODE_NEVER;}
  
  forceinline
  ConstDashedView::ConstDashedView(const Block& b, int n0) : b0(&b), n(n0) {}
  
  forceinline int 
  ConstDashedView::size() const {
    return n;
  }

  forceinline bool
  ConstDashedView::assigned() const {
    for (int i = 0; i < n; ++i)
      if (!b0[i].isFixed())
        return false;
    return true;
  }

  forceinline const Block&
  ConstDashedView::operator[](int i) const {
    return *(b0+i);
  }

  forceinline bool
  ConstDashedView::isNull() const {
    return n == 1 && b0[0].isNull();
  }
  
  forceinline bool
  ConstDashedView::isOK() const {
    if (n <= 0)
      return false;
    for (int i = 0; i < n; i++)
      if (!b0[i].isOK())
        return false;
  }
  
  forceinline double
  ConstDashedView::logdim() const {
    double d = 0.0;
    for (int i = 0; i < n; ++i) {
      d += b0[i].logdim();
      if (std::isinf(d))
        return d;
    }
    return d;
  }

  template<class Char, class Traits>
  forceinline  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConstDashedView& d) {
    int n = d.size();
    for (int i = 0; i < n - 1; ++i)
      os << d[i] << " + ";
    os << d[n-1];
    return os;
  };

  forceinline SweepIterator<SweepFwd,ConstDashedView>
  ConstDashedView::fwd_iterator(void) const {
    return SweepIterator<SweepFwd,ConstDashedView>(*this);
  }
  
  forceinline SweepIterator<SweepBwd,ConstDashedView>
  ConstDashedView::bwd_iterator(void) const {
    return SweepIterator<SweepBwd,ConstDashedView>(*this);
  }

  forceinline bool
  ConstDashedView::equiv(const Position& p, const Position& q) const {
    return p == q
       || (q.off == 0 && p.idx == q.idx-1 && p.off == b0[p.idx].ub())
       || (p.off == 0 && q.idx == p.idx-1 && q.off == b0[q.idx].ub());
  }
  
  forceinline bool
  ConstDashedView::prec(const Position& p, const Position& q) const {
    return (p.idx < q.idx-1)
        || (p.idx == q.idx && p.off < q.off)
        || (p.idx == q.idx-1 && (q.off > 0 || p.off < b0[p.idx].ub()));
  }
  
  forceinline int
  ConstDashedView::ub_new_blocks(const Matching& m) const {    
    if (prec(m.LSP, m.EEP))
      return prec(m.ESP, m.LSP) + m.EEP.idx - m.LSP.idx + (m.EEP.off > 0)
           + prec(m.EEP, m.LEP);
    else
      return 0;
  }

  forceinline int
  ConstDashedView::min_len_mand(const Block& bx, const Position& p,
                                            const Position& q) const {
    if (!prec(p,q))
      return 0;    
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : b0[q_i].ub();
    const Block& bp = b0[p_i];
    if (p_i == q_i)
      return nabla(bx, bp, std::min(q_o, bp.lb()) - p_o);
    int m = nabla(bx, bp, bp.lb() - p_o);
    for (int i = p_i+1; i < q_i; i++) {
      const Block& bi = b0[i];
      m += nabla(bx, bi, bi.lb());
    }
    const Block& bq = b0[q_i];
    return m + nabla(bx, bq, std::min(bq.lb(), q_o));
  }
  
  forceinline void
  ConstDashedView::mand_region(Space& home, const Block& bx, Block* bnew, int u,
                                const Position& p, const Position& q) const {
    if (!prec(p, q)) {
      assert (b0[p.idx].isNull());
      return;
    };
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : b0[q_i].ub();
//    std::cerr << "LSP=(" << p_i << "," << p_o << "), EEP=(" << q_i << "," << q_o << ")\n";
    const Block& bp = b0[p_i];
    // Head of the region.    
    bnew[0].update(home, bp);
    bnew[0].baseIntersect(home, bx);    
    if (!bnew[0].isNull()) {
      if (p_i == q_i) {
        bnew[0].updateCard(home, std::max(0, std::min(q_o, bp.lb()) - p_o), 
                                 std::min(u, q_o-p_o));    
        return;
      }
      else
        bnew[0].updateCard(home, std::max(0, bp.lb()-p_o), 
                                 std::min(u, bp.ub()-p_o));
    }
    else if (p_i == q_i)
      return;
    // Central part of the region.
    int j = 1;
    for (int i = p_i+1; i < q_i; ++i, ++j) {
      Block& bj = bnew[j];
      bj.update(home, b0[i]);
      bj.baseIntersect(home, bx);
      if (!bj.isNull() && bj.ub() > u)
        bj.ub(home, u);
    }
    // Tail of the region.
    const Block& bq = b0[q_i];
    bnew[j].update(home, bq);
    bnew[j].baseIntersect(home, bx);
    if (!bnew[j].isNull())
      bnew[j].updateCard(home, std::min(bq.lb(), q_o), std::min(u, q_o));
  }
  
  template <class ViewX>
  forceinline void
  ConstDashedView::mand_region(Space& home, ViewX& x, int idx, 
                               const Position& p, const Position& q) const {
    // FIXME: When only block by is involved.
    assert (p.idx == q.idx || (p.idx == q.idx-1 && q.off == 0));
    const Block& by = b0[p.idx];
    x.baseIntersectAt(home, idx, by);
    const Block& bx = x[idx];
    if (!bx.isNull()) {
      int q_off = q.off > 0 ? q.off : b0[q.idx-1].ub();
      x.lbAt(home, idx, std::max(bx.lb(), std::min(q_off, by.lb()) - p.off)),
      x.ubAt(home, idx, std::min(bx.ub(), q_off - p.off));
    }
  }
  
  forceinline int
  ConstDashedView::max_len_opt(const Block& bx, const Position& esp, 
                               const Position& lep, int l1) const {
    if (equiv(esp,lep))
      return 0;
    assert(!prec(lep, esp));
    int p_i = esp.idx, q_i = lep.off > 0 ? lep.idx : lep.idx-1,
        p_o = esp.off, q_o = lep.off > 0 ? lep.off : b0[q_i].ub();
    const Block& bp = b0[p_i];
    int k = bx.ub() - l1;
    if (p_i == q_i)
      return nabla(bx, bp, std::min(q_o-p_o, ubounded_sum(bp.lb(),k)));
    int m = nabla(bx, bp, std::min(bp.ub() - p_o, ubounded_sum(bp.lb(),k)));
    for (int i = p_i+1; i < q_i; i++) {
      const Block& bi = b0[i];
      m = ubounded_sum(m, nabla(bx, bi, std::min(bi.ub(), ubounded_sum(bi.lb(),k))));
    }
    const Block& bq = b0[q_i];
    return ubounded_sum(m, nabla(bx, bq, std::min(q_o, ubounded_sum(bq.lb(),k))));
  }
  
  forceinline void
  ConstDashedView::opt_region(Space& home, const Block& bx, Block& bnew,
                        const Position& p, const Position& q, int l1) const {
    assert(prec(p,q));
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : b0[q_i].ub();
//    std::cerr << "p=(" << p_i << "," << p_o << "), q=(" << q_i << "," << q_o << ")\n";
    // Only one block involved
    const Block& bp = b0[p_i];
    int k = bx.ub() - l1;
    if (p_i == q_i) {
      bnew.update(home, bp);
      bnew.baseIntersect(home, bx);
      if (!bnew.isNull())
        bnew.updateCard(home, 0, std::min(q_o-p_o, ubounded_sum(bp.lb(),k)));
      return;
    }
    // More than one block involved
    Set::GLBndSet s;
    bp.includeBaseIn(home, s);
    int u = bp.ub() - p_o;
    for (int i = p_i+1; i < q_i; ++i) {
      const Block& bi = b0[i];
      bi.includeBaseIn(home, s);
      u = ubounded_sum(u, std::min(bi.ub(), ubounded_sum(bi.lb(), k)));
    }
    const Block& bq = b0[q_i];
    bq.includeBaseIn(home, s);
    bnew.update(home, bx);
    bnew.baseIntersect(home, s);
    if (!bnew.isNull())
      bnew.updateCard(home, 0, 
        std::min(bx.ub(), ubounded_sum(u, std::min(q_o, ubounded_sum(bq.lb(), k)))));
  }
  
  template <class T>
  forceinline void
  ConstDashedView::expandBlock(Space& home, const Block& bx, T& x) const {
    assert (!bx.isFixed());
    Set::GLBndSet s;
    Set::BndSetRanges i(bx.ranges());
    s.includeI(home, i);
    bool norm = false;
    int u = bx.ub();
    x.gets(home, DashedString(home, b0, n));
    int maxl = 0;
    for (int i = 0; i < n; ++i)
      maxl = ubounded_sum(maxl, x[i].ub());
    if (u < maxl)
      x.varimp()->max_length(home, u);
    for (int i = 0; i < x.size(); i++) {
      x.baseIntersectAt(home, i, s);
      norm |= x[i].isNull() || (i > 0 && x[i].baseEquals(x[i-1]));
    }
    if (norm)
      x.normalize(home);
  }

  template <class ViewX>
  forceinline void
  ConstDashedView::crushBase(Space& home, ViewX& x, int idx, 
                        const Position& p, const Position& q) const {
//    std::cerr << "Crushing " << *this << " from " << p << " to " << q << "\n";
    Set::GLBndSet s;
    for (int i = p.idx, j = q.idx - (q.off == 0); i <= j; ++i)
      b0[i].includeBaseIn(home, s);
    x.baseIntersectAt(home, idx, s);
  }
  
  forceinline std::vector<int>
  ConstDashedView::fixed_pref(const Position& p, const Position& q, int& np) const {
//    std::cerr << "fixed_pref of " << *this << " between " << p << " and " << q << "\n"; 
    np = 0;
    std::vector<int> v;
    if (!prec(p,q))
      return v;
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
//    std::cerr << "p=(" << p_i << "," << p_o << "), q=(" << q_i << "," << q_o << ")\n";
    const Block& bp = (*this)[p_i];
    if (bp.lb() == 0 || p_o > bp.lb() || bp.baseSize() > 1)
      return v;
    v.push_back(bp.baseMin());
    if (p_i == q_i) {
      np = std::max(0, std::min(bp.lb(), q_o) - p_o);
      assert (np > 0);
      v.push_back(np);
      if (p_i < size()-1)
        np = -np;
      return v;
    }
    np = bp.lb() - p_o;
    v.push_back(np);
    if (bp.lb() < bp.ub()) {
      if (p_i < size()-1)
        np = -np;
      return v;
    }
    for (int i = p_i+1; i < q_i; ++i) {
      const Block& bi = (*this)[i];
      int l = bi.lb();
      if (l == 0 || bi.baseSize() > 1) {
        if (p_i < size()-1)
          np = -np;
        return v;
      }
      v.push_back(bi.baseMin());
      v.push_back(l);
      np += l;
      if (l < bi.ub()) {
        if (i < size()-1)
          np = -np;
        return v;
      }
    }
    const Block& bq = (*this)[q_i];
    int l = std::min(bq.lb(), q_o);
    if (l == 0 || bq.baseSize() > 1) {
      if (p_i < size()-1)
        np = -np;
      return v;
    }
    v.push_back(bq.baseMin());
    v.push_back(l);
    np += l;
    if (q_i < size()-1)
      np = -np;
    return v;
  }

  forceinline std::vector<int>
  ConstDashedView::fixed_suff(const Position& p, const Position& q, int& ns) const {
//    std::cerr << "fixed_suff of " << *this << " between" << p << " and " << q << "\n"; 
    ns = 0;
    std::vector<int> v;
    if (!prec(p,q))
      return v;
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
//    std::cerr << "p=(" << p_i << "," << p_o << "), q=(" << q_i << "," << q_o << ")\n";
    const Block& bq = (*this)[q_i];
    if (bq.lb() == 0 || bq.baseSize() > 1)
      return v;
    v.push_back(bq.baseMin());
    if (p_i == q_i) {
      ns = std::max(0, std::min(bq.lb(), q_o) - p_o);
      v.push_back(ns);
      if (q_i < size()-1)
        ns = -ns;
      return v;
    }
    ns = std::min(bq.lb(), q_o);
    v.push_back(ns);
    if (bq.lb() < bq.ub()) {
      if (q_i < size()-1)
        ns = -ns;
      return v;
    }
    for (int i = q_i-1; i > p_i; --i) {
      const Block& bi = (*this)[i];
      int l = bi.lb();
      if (l == 0 || bi.baseSize() > 1) {        
        if (i < size()-1)
          ns = -ns;
        return v;
      }
      v.push_back(bi.baseMin());
      v.push_back(l);
      ns += l;
      if (l < bi.ub()) {
        if (i < size()-1)
          ns = -ns;
        return v;        
      }
    }
    const Block& bp = (*this)[p_i];
    int l = bp.lb() - p_o;
    if (l == 0 || bp.baseSize() > 1) {
      if (p_i < size()-1)
        ns = -ns;
      return v;
    }
    v.push_back(bp.baseMin());
    v.push_back(l);
    ns += l;
    if (p_i < size()-1)
      ns = -ns;
    return v;
  }
  
}}

