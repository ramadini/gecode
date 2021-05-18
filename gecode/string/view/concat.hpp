//namespace Gecode { namespace String {

//  template <>
//  forceinline SweepIterator<ConcatView>::
//  SweepIterator(const StringView& x, const Position& p) : sv(x), pos(p) {
//    if (!p.isNorm(x)) {
//      if (p.off == sv[p.idx].ub()) {
//        pos.idx++;
//        pos.off = 0;
//      }
//      else
//        throw OutOfLimits("SweepIterator<ConcatView>::SweepIterator");
//    }
//    assert (pos.isNorm(sv));
//  };
//  
//  template <>
//  forceinline const Position& 
//  SweepIterator<ConcatView>::operator*(void) {
//    return pos;
//  }

//  template <>
//  forceinline int
//  SweepIterator<ConcatView>::lb() const {
//    return sv[pos.idx].lb();
//  }
//  
//  template <>
//  forceinline int
//  SweepIterator<ConcatView>::ub() const {
//    return sv[pos.idx].ub();
//  }
//  
//  template <>
//  forceinline bool
//  SweepIterator<ConcatView>::disj(const Block& b) const {
//    return sv[pos.idx].baseDisjoint(b);
//  }  
//  template <>
//  forceinline bool
//  SweepIterator<ConcatView>::disj(int c) const {
//    return !sv[pos.idx].baseContains(c);
//  }
//  
//  template <>
//  forceinline bool
//  SweepIterator<ConcatView>::isOK() const {
//    return pos.isNorm(sv);
//  }

//}}

//namespace Gecode { namespace String {

//  template <>
//  forceinline SweepFwdIterator<ConcatView>::
//  SweepFwdIterator(const StringView& x) : SweepIterator(x, Position(0,0)) {};
//  
//  template <>
//  forceinline SweepFwdIterator<ConcatView>::
//  SweepFwdIterator(const StringView& x, const Position& p) 
//  : SweepIterator(x, p) {};
//  
//  template <>
//  forceinline bool
//  SweepFwdIterator<ConcatView>::operator()(void) const {
//    return sv.prec(pos, Position(sv.size(),0));
//  }
//  
//  template <>
//  forceinline void
//  SweepFwdIterator<ConcatView>::nextBlock() {
//    if (pos.idx >= sv.size())
//      return;
//    pos.idx++;
//    pos.off = 0;
//    assert (isOK() || sv[pos.idx].isNull());
//  }
//  
//  template <>
//  forceinline bool
//  SweepFwdIterator<ConcatView>::hasNextBlock(void) const {
//    return pos.idx < sv.size();
//  }
//  
//  template <>
//  forceinline int
//  SweepFwdIterator<ConcatView>::must_consume() const {
//    return std::max(0, lb() - pos.off);
//  }
//  
//  template <>
//  forceinline int
//  SweepFwdIterator<ConcatView>::may_consume() const {
//    return ub() - pos.off;
//  }
//  
//  template <>
//  forceinline void
//  SweepFwdIterator<ConcatView>::consume(int k) {
//    if (k == 0)
//      return;
//    pos.off += k;
//    if (pos.off >= sv[pos.idx].ub()) {
//      if (pos.off > sv[pos.idx].ub())
//        throw OutOfLimits("ConcatView<View0,View1>::SweepBwdIterator::consume");
//      pos.idx++;
//      pos.off = 0;
//    }
//    assert (isOK());
//  }
//  
//  template <>
//  forceinline void
//  SweepFwdIterator<ConcatView>::consumeMand(int k) {
//    if (k == 0)
//      return;
//    pos.off += k; 
//    if (pos.off > sv[pos.idx].lb())
//      throw OutOfLimits("ConcatView<View0,View1>::SweepBwdIterator::consume");
//    if (pos.off == sv[pos.idx].ub()) {
//      pos.idx++;
//      pos.off = 0;
//    }
//    assert (isOK());
//  }

//}}

//namespace Gecode { namespace String {
//  
//  template <>
//  forceinline SweepBwdIterator<ConcatView>::
//  SweepBwdIterator(const StringView& x)
//  : SweepIterator(x, Position(x.size(), 0)) {};
//  
//  template <>
//  forceinline SweepBwdIterator<ConcatView>::
//  SweepBwdIterator(const StringView& x, const Position& p) 
//  : SweepIterator(x, p) {};
//  
//  template <>
//  forceinline int
//  SweepBwdIterator<ConcatView>::lb() const {
//    return sv[pos.off > 0 ? pos.idx : pos.idx-1].lb();
//  }
//  
//  template <>
//  forceinline int
//  SweepBwdIterator<ConcatView>::ub() const {
//    return sv[pos.off > 0 ? pos.idx : pos.idx-1].ub();
//  }
//  
//  template <>
//  forceinline bool
//  SweepBwdIterator<ConcatView>::disj(const Block& b) const {
//    return  sv[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
//  }
//  template <>
//  forceinline bool
//  SweepBwdIterator<ConcatView>::disj(int c) const {
//    return !(sv[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(c));
//  }
//  
//  template <>
//  forceinline bool
//  SweepBwdIterator<ConcatView>::operator()(void) const {
//    return sv.prec(Position(0,0), pos);
//  }
//  
//  template <>
//  forceinline void
//  SweepBwdIterator<ConcatView>::nextBlock() {
//    if (pos.idx == 0 && pos.off == 0)
//      return;
//    if (pos.off > 0)
//      pos.off = 0;
//    else
//      pos.idx--;
//    assert (isOK() || sv[pos.idx].isNull());
//  };
//  
//  template <>
//  forceinline bool
//  SweepBwdIterator<ConcatView>::hasNextBlock(void) const {
//    return pos.idx > 0 || pos.off > 0;
//  };
//  
//  template <>
//  forceinline int
//  SweepBwdIterator<ConcatView>::may_consume() const {
//    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].ub() : pos.off;
//  }
//  
//  template <>
//  forceinline int
//  SweepBwdIterator<ConcatView>::must_consume() const {
//    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].lb() 
//                                         : std::min(pos.off, sv[pos.idx].lb());
//  }
//  
//  template <>
//  forceinline void
//  SweepBwdIterator<ConcatView>::consume(int k) {
//    if (k == 0)
//      return;
//    if (pos.off == 0 && pos.idx > 0) {
//      pos.idx--;
//      pos.off = sv[pos.idx].ub() - k;
//    }
//    else
//      pos.off -= k;
//    if (pos.off < 0)
//      throw OutOfLimits("SweepBwdIterator<ConcatView>::consume");
//    assert (isOK());
//  }
//  
//  template <>
//  forceinline void
//  SweepBwdIterator<ConcatView>::consumeMand(int k) {
//    if (k == 0)
//      return;
//    if (pos.off == 0 && pos.idx > 0) {
//      pos.idx--;
//      pos.off = sv[pos.idx].lb() - k;
//    }
//    else
//      pos.off = std::min(pos.off, sv[pos.idx].lb()) - k;
//    if (pos.off < 0)
//      throw OutOfLimits("SweepBwdIterator<ConcatView>::consumeMand");
//    assert (isOK());
//  }

//}}

namespace Gecode { namespace String {

  template <class View0, class View1>
  forceinline 
  ConcatView<View0,View1>::ConcatView(void) : x0(), x1(), pivot(0) {}
  
  template <class View0, class View1>
  forceinline
  ConcatView<View0,View1>::ConcatView(View0& x, View1& y)
  : x0(x), x1(y), pivot(x.size()) {}

  template <class View0, class View1>
  forceinline int 
  ConcatView<View0,View1>::size() const {
    return x0.size() + x1.size();
  }

  template <class View0, class View1>
  forceinline Block&
  ConcatView<View0,View1>::operator[](int i) {
    return i < pivot ? x0[i] : x1[i];
  }
  template <class View0, class View1>
  forceinline const Block&
  ConcatView<View0,View1>::operator[](int i) const {
    return i < pivot ? x0[i] : x1[i];
  }
  
  template <class View0, class View1>
  forceinline std::vector<int> 
  ConcatView<View0,View1>::val(void) const {
    std::vector<int> v(size());
    for (int i = 0; i < x0.size(); ++i)
      v[i] = x0[i];
    for (int i = 0, j = x0.size(); i < x1.size(); ++i, ++j)
      v[j] = x1[i];
    return v;
  }
  
  template <class View0, class View1>
  forceinline int
  ConcatView<View0,View1>::max_length() const {
    return x0.max_length() + x1.max_length();
  }
  
  template <class View0, class View1>
  forceinline int
  ConcatView<View0,View1>::min_length() const {
    return x0.min_length() + x1.min_length();
  }
  
  template <class View0, class View1>
  forceinline bool
  ConcatView<View0,View1>::assigned() const {
    return x0.assigned() && x1.assigned();
  }
  
  template <class View0, class View1>
  forceinline bool
  ConcatView<View0,View1>::isOK() const {
    return x0.isOK() && x1.isOK();
  }
  
  template <class View0, class View1>
  forceinline void
  ConcatView<View0,View1>::normalize(Space& home) {
    x0.normalize(home);
    x1.normalize(home);
  }
//  
//  forceinline SweepFwdIterator<ConcatView>
//  ConcatView<View0,View1>::fwd_iterator(void) const {
//    return SweepFwdIterator<ConcatView>(*this);
//  }
//  
//  forceinline SweepBwdIterator<ConcatView>
//  ConcatView<View0,View1>::bwd_iterator(void) const {
//    return SweepBwdIterator<ConcatView>(*this);
//  }
  
  template <class View0, class View1>
  forceinline bool
  ConcatView<View0,View1>::equiv(const Position& p, const Position& q) const {
    return (q.idx <  pivot && x0.equiv(p,q))
        || (p.idx >= pivot && x1.equiv(p,q));
  }
  
  template <class View0, class View1>
  forceinline bool
  ConcatView<View0,View1>::prec(const Position& p, const Position& q) const {
    return p.idx < q.idx 
        || (q.idx <  pivot && x0.prec(p,q))
        || (p.idx >= pivot && x1.prec(p,q));
  }
  
  template <class View0, class View1>
  forceinline int
  ConcatView<View0,View1>::ub_new_blocks(const Matching& m) const {
    if (prec(m.LSP, m.EEP))
      return prec(m.ESP, m.LSP) + m.EEP.idx - m.LSP.idx + (m.EEP.off > 0) 
           + prec(m.EEP, m.LEP);
    else
      return 0;
  }

  template <class View0, class View1>
  forceinline int
  ConcatView<View0,View1>::min_len_mand(const Block& bx, const Position& p,
                                        const Position& q) const {
    if (!prec(p,q))
      return 0;
    else if (q.idx < pivot)
      return x0.min_len_mand(bx, p, q);
    else if (p.idx >= pivot)
      return x1.min_len_mand(bx, p, q);
    else
      return x0.min_len_mand(bx, p, Position(pivot,0)) 
           + x1.min_len_mand(bx, Position(pivot,0), q);
  }
//  
//  forceinline void
//  ConcatView<View0,View1>::mand_region(Space& home, Block& bx, Block* bnew, int u,
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
//  ConcatView<View0,View1>::mand_region(Space& home, Block& bx, const Block& by,
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
//  ConcatView<View0,View1>::max_len_opt(const Block& bx, const Position& esp, 
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
//  ConcatView<View0,View1>::opt_region(Space& home, const Block& bx, Block& bnew,
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
//  ConcatView<View0,View1>::expandBlock(Space& home, const Block& bx, Block* y) const {
//    for (int i = 0; i < size(); i++) {
//      y[i].update(home, (*this)[i]);
//      y[i].baseIntersect(home, bx);
//      if (y[i].ub() > bx.ub())
//        y[i].ub(home, bx.ub());
//    }
//  }

//  forceinline void
//  ConcatView<View0,View1>::crushBase(Space& home, Block& bx, const Position& p, 
//                                                const Position& q) const {
//    Gecode::Set::GLBndSet s;
//    for (int i = p.idx, j = q.idx - (q.off == 0); i <= j; ++i)
//      (*this)[i].includeBaseIn(home, s);
//    bx.baseIntersect(home, s);
//  }
//  
//  forceinline void
//  ConcatView<View0,View1>::resize(Space& home, Block newBlocks[], int newSize, int U[], 
//                                                                  int uSize) {
//    DashedString d(home, size()+newSize);
//    int j = 0, h = 0;
//    for (int i = 0; i < uSize; i += 2) {
//      for (int k = j; k < U[i]; ++k)
//        d[k].update(home, (*this)[k]);
//      j = h + U[i] + U[i+1];
//      for (int k = h + U[i]; k < j; ++k, ++h)
//        d[k].update(home, newBlocks[h]);
//    }
//    for (h = U[uSize-2]+1; h < size(); ++j, ++h)
//      d[j].update(home, (*this)[h]);
//    d.normalize(home);
//    x->update(home, d);
//  }

  template<class Char, class Traits, class X, class Y>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConcatView<X,Y>& z) {
    os << z.x0 << " ++ " << z.x1;
    return os;
  };
     
}}

