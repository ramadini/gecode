namespace Gecode { namespace String {

  /*
   * Constructors and access
   *
   */
  
//  forceinline
//  ConstStringView::ConstStringView(Space& home, int d[], int n) : w(home) {
//    if (n > Limits::MAX_STRING_LENGTH)
//      throw OutOfLimits("ConstStringView::ConstStringView");
//    w = Gecode::Support::DynamicArray<int,Space>(home, n);
//    for (int i = 0; i < n; ++i) {
//      int di = d[i];
//      if (di < 0 || di >= Limits::MAX_ALPHABET_SIZE)
//        throw OutOfLimits("ConstStringView::ConstStringView");
//      w[i] = di;
//    }
//  }
 
//  forceinline int 
//  ConstStringView::size() const {
//    return w.n;
//  }
//  
//  forceinline std::vector<int> 
//  ConstStringView::val(void) const {
//    return x->val();
//  }
//  
//  forceinline int
//  ConstStringView::max_length() const {
//    return x->max_length();
//  }
//  
//  forceinline int
//  ConstStringView::min_length() const {
//    return x->min_length();
//  }
//  
//  forceinline bool
//  ConstStringView::assigned() const {
//    return x->assigned();
//  }
//  
//  forceinline bool
//  ConstStringView::isOK() const {
//    return x->isOK();
//  }
//  
//  forceinline bool
//  ConstStringView::contains(const StringView& y) const {
//    return x->contains(*y.x);
//  }
//  
//  forceinline bool
//  ConstStringView::equals(const StringView& y) const {
//    return x->equals(*y.x);
//  }
//  
//  forceinline ModEvent
//  ConstStringView::min_length(Space& home, int l) {
//    return x->min_length(home, l);
//  }
//  
//  forceinline ModEvent
//  ConstStringView::max_length(Space& home, int u) {
//    return x->max_length(home, u);
//  }
//  
//  forceinline ModEvent
//  ConstStringView::bnd_length(Space& home, int l, int u) {
//    return x->bnd_length(home, l, u);
//  }
//  
//  template<class Char, class Traits>
//  forceinline  std::basic_ostream<Char,Traits>&
//  operator <<(std::basic_ostream<Char,Traits>& os, const StringView& v) {
//    os << *v.varimp();
//    return os;
//  };
//  
//  forceinline void
//  ConstStringView::normalize(Space& home) {
//    x->normalize(home);
//  }
//  
//  forceinline ConstStringView::SweepFwdIterator
//  ConstStringView::fwd_iterator(void) const {
//    return SweepFwdIterator(*this);
//  }
//  
//  forceinline ConstStringView::SweepBwdIterator
//  ConstStringView::bwd_iterator(void) const {
//    return SweepBwdIterator(*this);
//  }
//  
//  forceinline bool
//  ConstStringView::equiv(const Position& p, const Position& q) const {
//    return p == q 
//       || (q.off == 0 && p.idx == q.idx-1 && p.off == (*this)[p.idx].ub())
//       || (p.off == 0 && q.idx == p.idx-1 && q.off == (*this)[q.idx].ub());
//  }
//  
//  forceinline bool
//  ConstStringView::prec(const Position& p, const Position& q) const {
//    return (p.idx < q.idx-1)
//        || (p.idx == q.idx && p.off < q.off)
//        || (p.idx == q.idx-1 && (q.off > 0 || p.off < (*this)[p.idx].ub()));
//  }
//  
//  forceinline int
//  ConstStringView::ub_new_blocks(const Matching& m) const {
//    if (prec(m.LSP, m.EEP))
//      return prec(m.ESP, m.LSP) + m.EEP.idx - m.LSP.idx + (m.EEP.off > 0) 
//           + prec(m.EEP, m.LEP);
//    else
//      return 0;
//  }

//  forceinline int
//  ConstStringView::min_len_mand(const Block& bx, const Position& p,
//                                            const Position& q) const {
//    if (!prec(p,q))
//      return 0;    
//    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
//        p_o = p.off, q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
//    const Block& bp = (*this)[p_i];
//    if (p_i == q_i)
//      return nabla(bx, bp, std::min(q_o, bp.lb()) - p_o);
//    int m = nabla(bx, bp, bp.lb() - p_o);
//    for (int i = p_i+1; i < q_i; i++) {
//      const Block& bi = (*this)[p_i];
//      m += nabla(bx, bi, bi.lb());
//    }
//    const Block& bq = (*this)[q_i];
//    return m + nabla(bx, bq, std::min(bq.lb(), q_o));
//  }  
//  
//  forceinline void
//  ConstStringView::mand_region(Space& home, Block& bx, Block* bnew, int u,
//                                const Position& p, const Position& q) const {
//    assert (prec(p, q));
//    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
//        p_o = p.off, q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
////    std::cerr << "LSP=(" << p_i << "," << p_o << "), EEP=(" << q_i << "," << q_o << ")\n";
//    const Block& bp = (*this)[p_i];
//    // Head of the region.    
//    bnew[0].update(home, bp);
//    bnew[0].baseIntersect(home, bx);    
//    if (!bnew[0].isNull()) {
//      if (p_i == q_i) {
//        bnew[0].updateCard(home, std::max(0, std::min(q_o, bp.lb()) - p_o), 
//                                 std::min(u, q_o-p_o));    
//        return;
//      }
//      else
//        bnew[0].updateCard(home, std::max(0, bp.lb()-p_o), 
//                                 std::min(u, bp.ub()-p_o));
//    }
//    // Central part of the region.
//    int j = 1;
//    for (int i = p_i+1; i < q_i; ++i, ++j) {
//      Block& bj = bnew[j];
//      bj.update(home, (*this)[i]);
//      bj.baseIntersect(home, bx);
//      if (!bj.isNull() && bj.ub() > u)
//        bj.ub(home, u);
//    }
//    // Tail of the region.
//    const Block& bq = (*this)[q_i];
//    bnew[j].update(home, bq);
//    bnew[j].baseIntersect(home, bx);
//    if (!bnew[j].isNull())
//      bnew[j].updateCard(home, std::min(bq.lb(), q_o), std::min(u, q_o));
//  }
//  
//  forceinline void
//  ConstStringView::mand_region(Space& home, Block& bx, const Block& by,
//                             const Position& p, const Position& q) const {
//    // FIXME: When only block by is involved.
//    assert (p.idx == q.idx || (p.idx == q.idx-1 && q.off == 0));
//    int q_off = q.off > 0 ? q.off : (*this)[q.idx-1].ub();
//    bx.baseIntersect(home, by);
//    if (!bx.isNull())
//      bx.updateCard(home, std::max(bx.lb(), std::min(q_off, by.lb()) - p.off),
//                          std::min(bx.ub(), q_off - p.off));
//  }
//  
//  forceinline int
//  ConstStringView::max_len_opt(const Block& bx, const Position& esp, 
//                                           const Position& lep, int l1) const {
//    if (equiv(esp,lep))
//      return 0;
//    assert(!prec(lep, esp));
//    int p_i = esp.idx, q_i = lep.off > 0 ? lep.idx : lep.idx-1,
//        p_o = esp.off, q_o = lep.off > 0 ? lep.off : (*this)[q_i].ub();
//    const Block& bp = (*this)[p_i];
//    int k = bx.ub() - l1;
//    if (p_i == q_i)
//      return nabla(bx, bp, std::min(q_o-p_o, bp.lb()+k));
//    int m = nabla(bx, bp, std::min(bp.ub() - p_o, bp.lb()+k));
//    for (int i = p_i+1; i < q_i; i++) {
//      const Block& bi = (*this)[i];
//      m = ub_sum(m, nabla(bx, bi, std::min(bi.ub(), bi.lb()+k)));
//    }
//    const Block& bq = (*this)[q_i];
//    return ub_sum(m, nabla(bx, bq, std::min(q_o, bq.lb()+k)));
//  }
//  
//  forceinline void
//  ConstStringView::opt_region(Space& home, const Block& bx, Block& bnew,
//                           const Position& p, const Position& q, int l1) const {
//    assert(prec(p,q));
//    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
//        p_o = p.off, q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
////    std::cerr << "p=(" << p_i << "," << p_o << "), q=(" << q_i << "," << q_o << ")\n";
//    // Only one block involved
//    const Block& bp = (*this)[p_i];
//    int k = bx.ub() - l1;
//    if (p_i == q_i) {
//      bnew.update(home, bp);
//      bnew.baseIntersect(home, bx);
//      if (!bnew.isNull())
//        bnew.updateCard(home, 0, std::min(q_o-p_o, bp.lb()+k));
//      return;
//    }
//    // More than one block involved
//    Gecode::Set::GLBndSet s;
//    bp.includeBaseIn(home, s);
//    int u = bp.ub() - p_o;
//    for (int i = p_i+1; i < q_i; ++i) {
//      const Block& bi = (*this)[i];
//      bi.includeBaseIn(home, s);
//      u = ub_sum(u, std::min(bi.ub(), bi.lb()+k));
//    }
//    const Block& bq = (*this)[q_i];
//    bq.includeBaseIn(home, s);
//    bnew.update(home, bx);
//    bnew.baseIntersect(home, s);    
//    if (!bnew.isNull())
//      bnew.updateCard(home, 0, ub_sum(u, std::min(q_o, bq.lb()+k)));
//  }
//  
//  forceinline void
//  ConstStringView::expandBlock(Space& home, const Block& bx, Block* y) const {
//    for (int i = 0; i < size(); i++) {
//      y[i].update(home, (*this)[i]);
//      y[i].baseIntersect(home, bx);
//      if (y[i].ub() > bx.ub())
//        y[i].ub(home, bx.ub());
//    }
//  }

//  forceinline void
//  ConstStringView::crushBase(Space& home, Block& bx, const Position& p, 
//                                                const Position& q) const {
//    Gecode::Set::GLBndSet s;
//    for (int i = p.idx, j = q.idx - (q.off == 0); i <= j; ++i)
//      (*this)[i].includeBaseIn(home, s);
//    bx.baseIntersect(home, s);
//  }
     
}}

