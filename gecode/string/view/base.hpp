namespace Gecode { namespace String {
  
  template <class T>
  forceinline const Block&
  DashedViewBase<T>::operator[](int i) const {
    return (*ptr)[i];
  }
  template <class T>
  forceinline int
  DashedViewBase<T>::size() const {
    return ptr->size();
  }
  
  template <class T>
  forceinline DashedViewBase<T>::DashedViewBase() : ptr(nullptr) {}
  template <class T>
  forceinline DashedViewBase<T>::DashedViewBase(const T* x) : ptr(x) {}
  
  template <class T>
  forceinline bool
  DashedViewBase<T>::equiv(const Position& p, const Position& q) const {
    return p == q 
       || (q.off == 0 && p.idx == q.idx-1 && p.off == (*this)[p.idx].ub())
       || (p.off == 0 && q.idx == p.idx-1 && q.off == (*this)[q.idx].ub());
  }
  
  template <class T>
  forceinline bool
  DashedViewBase<T>::prec(const Position& p, const Position& q) const {
    return (p.idx < q.idx-1)
        || (p.idx == q.idx && p.off < q.off)
        || (p.idx == q.idx-1 && (q.off > 0 || p.off < (*this)[p.idx].ub()));
  }
  
  template <class T>
  template <class IterY>
  forceinline Position
  DashedViewBase<T>::push(int i, IterY& it) const {
//    std::cerr << "Pushing " << (*this)[i] << " from " << *it << '\n';
    Position p = *it;    
    const Block& bx = (*this)[i];
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
//          std::cerr << "p=" << p << ", it=" << *it << "\n";
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
  
  template <class T>
  template <class IterY>
  forceinline void
  DashedViewBase<T>::stretch(int i, IterY& it) const {
//    std::cerr << "Streching " << (*this)[i] << " from " << *it << '\n';
    const Block& bx = (*this)[i];
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
  
  template <class T>
  forceinline int
  DashedViewBase<T>::ub_new_blocks(const Matching& m) const {    
    if (prec(m.LSP, m.EEP))
      return prec(m.ESP, m.LSP) + m.EEP.idx - m.LSP.idx + (m.EEP.off > 0)
           + prec(m.EEP, m.LEP);
    else
      return 0;
  }
  
  template <class T>
  forceinline int
  DashedViewBase<T>::min_len_mand(const Block& bx, const Position& p,
                                            const Position& q) const {
    if (!prec(p,q))
      return 0;    
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
    const Block& bp = (*this)[p_i];
    if (p_i == q_i)
      return nabla(bx, bp, std::min(q_o, bp.lb()) - p_o);
    int m = nabla(bx, bp, bp.lb() - p_o);
    for (int i = p_i+1; i < q_i; i++) {
      const Block& bi = (*this)[i];
      m += nabla(bx, bi, bi.lb());
    }
    const Block& bq = (*this)[q_i];
    return m + nabla(bx, bq, std::min(bq.lb(), q_o));
  }

  template <class T>
  forceinline int
  DashedViewBase<T>::max_len_opt(const Block& bx, const Position& esp, 
                                           const Position& lep, int l1) const {
    if (equiv(esp,lep))
      return 0;
    assert(!prec(lep, esp));
    int p_i = esp.idx, q_i = lep.off > 0 ? lep.idx : lep.idx-1,
        p_o = esp.off, q_o = lep.off > 0 ? lep.off : (*this)[q_i].ub();
    const Block& bp = (*this)[p_i];
    int k = bx.ub() - l1;
    if (p_i == q_i)
      return nabla(bx, bp, std::min(q_o-p_o, ubounded_sum(bp.lb(),k)));
    int m = nabla(bx, bp, std::min(bp.ub() - p_o, ubounded_sum(bp.lb(),k)));
    for (int i = p_i+1; i < q_i; i++) {
      const Block& bi = (*this)[i];
      m = ubounded_sum(m, nabla(bx, bi, std::min(bi.ub(), ubounded_sum(bi.lb(),k))));
    }
    const Block& bq = (*this)[q_i];
    return ubounded_sum(m, nabla(bx, bq, std::min(q_o, ubounded_sum(bq.lb(),k))));
  }
  
  template <class T>
  template <class ViewX>
  forceinline void
  DashedViewBase<T>::crushBase(Space& home, ViewX& x, int idx, 
                        const Position& p, const Position& q) const {
//    std::cerr << "Crushing " << *this << " from " << p << " to " << q << "\n";
    Set::GLBndSet s;
    for (int i = p.idx, j = q.idx - (q.off == 0); i <= j; ++i)
      (*this)[i].includeBaseIn(home, s);
    x.baseIntersectAt(home, idx, s);
  }
  
  template <class T>
  forceinline void
  DashedViewBase<T>::opt_region(Space& home, const Block& bx, Block& bnew,
                         const Position& p, const Position& q, int l1) const {
    assert(prec(p,q));
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
//    std::cerr << "p=(" << p_i << "," << p_o << "), q=(" << q_i << "," << q_o << ")\n";
    // Only one block involved
    const Block& bp = (*this)[p_i];
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
      const Block& bi = (*this)[i];
      bi.includeBaseIn(home, s);
      u = ubounded_sum(u, std::min(bi.ub(), ubounded_sum(bi.lb(), k)));
    }
    const Block& bq = (*this)[q_i];
    bq.includeBaseIn(home, s);
    bnew.update(home, bx);
    bnew.baseIntersect(home, s);
    if (!bnew.isNull())
      bnew.updateCard(home, 0, 
        std::min(bx.ub(), ubounded_sum(u, std::min(q_o, ubounded_sum(bq.lb(), k)))));
  }
  
  template <class T>
  forceinline void
  DashedViewBase<T>::mand_region(Space& home, const Block& bx, Block* bnew, int u,
                                const Position& p, const Position& q) const {
    if (!prec(p, q)) {
      assert ((*this)[p.idx].isNull());
      return;
    };
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
//    std::cerr << "LSP=(" << p_i << "," << p_o << "), EEP=(" << q_i << "," << q_o << ")\n";
    const Block& bp = (*this)[p_i];
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
      bj.update(home, (*this)[i]);
      bj.baseIntersect(home, bx);
      if (!bj.isNull() && bj.ub() > u)
        bj.ub(home, u);
    }
    // Tail of the region.
    const Block& bq = (*this)[q_i];
    bnew[j].update(home, bq);
    bnew[j].baseIntersect(home, bx);
    if (!bnew[j].isNull())
      bnew[j].updateCard(home, std::min(bq.lb(), q_o), std::min(u, q_o));
  }
  
  template <class T>
  template <class ViewX>
  forceinline void
  DashedViewBase<T>::mand_region(Space& home, ViewX& x, int idx, 
                          const Position& p, const Position& q) const {
    // FIXME: When only block by is involved.
    int q_i = q.off > 0 ? q.idx : q.idx-1;
    const Block& by = (*this)[q_i];
    x.baseIntersectAt(home, idx, by);
    const Block& bx = x[idx];
    if (!bx.isNull()) {
      int p_o = p.off, q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
      x.lbAt(home, idx, std::max(bx.lb(), std::min(q_o, by.lb()) - p_o));
      x.ubAt(home, idx, std::min(bx.ub(), q_o - p_o));
    }
  }  
  
  template <class T>
  forceinline std::vector<int>
  DashedViewBase<T>::fixed_pref(const Position& p, const Position& q, int& np) const {
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

  template <class T>
  forceinline std::vector<int>
  DashedViewBase<T>::fixed_suff(const Position& p, const Position& q, int& ns) const {
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

