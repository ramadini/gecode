namespace Gecode { namespace String {

  template <class T>
  forceinline SweepIterator<T>::
  SweepIterator(const T& x, const Position& p)
  : sv(x), pos(p) {
    if (!p.isNorm(x)) {
      if (p.off == sv[p.idx].ub()) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("SweepIterator<ConcatView>::SweepIterator");
    }
    assert (pos.isNorm(sv));
  };
  
  template <class T>
  forceinline const Position& 
  SweepIterator<T>::operator*(void) {
    return pos;
  }

  template <class T>
  forceinline int
  SweepIterator<T>::lb() const {
    return sv[pos.idx].lb();
  }
  
  template <class T>
  forceinline int
  SweepIterator<T>::ub() const {
    return sv[pos.idx].ub();
  }
  
  template <class T>
  forceinline bool
  SweepIterator<T>::disj(const Block& b) const {
    const CBlock& c = sv[pos.idx];
    return c.isFixed() ? !b.baseContains(c.val()) : c.block().baseDisjoint(b);
  }
  template<class T>
  forceinline bool
  SweepIterator<T>::disj(int k) const {
    const CBlock& c = sv[pos.idx];
    return c.isFixed() ? c.val() != k : !c.block().baseContains(k);
  }
  
  template <class T>
  forceinline bool
  SweepIterator<T>::isOK() const {
    return pos.isNorm(sv);
  }

}}

namespace Gecode { namespace String {

  template <class T>
  forceinline SweepFwdIterator<T>::
  SweepFwdIterator(const T& x) : SweepIterator<T>(x, Position(0,0)) {};
  
  template <class T>
  forceinline SweepFwdIterator<T>::
  SweepFwdIterator(const T& x, const Position& p) 
  : SweepIterator<T>(x, p) {};
  
  template <class T>
  forceinline bool
  SweepFwdIterator<T>::operator()(void) const {
    return sv.prec(pos, Position(sv.size(),0));
  }
  
  template <class T>
  forceinline void
  SweepFwdIterator<T>::nextBlock() {
    if (pos.idx >= sv.size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (SweepIterator<T>::isOK() || sv[pos.idx].isNull());
  }
  
  template <class T>
  forceinline bool
  SweepFwdIterator<T>::hasNextBlock(void) const {
    return pos.idx < sv.size();
  }
  
  template <class T>
  forceinline int
  SweepFwdIterator<T>::must_consume() const {
    return std::max(0, SweepIterator<T>::lb() - pos.off);
  }
  
  template <class T>
  forceinline int
  SweepFwdIterator<T>::may_consume() const {
    return SweepIterator<T>::ub() - pos.off;
  }
  
  template <class T>
  forceinline void
  SweepFwdIterator<T>::consume(int k) {
    if (k == 0)
      return;
    pos.off += k;
    if (pos.off >= sv[pos.idx].ub()) {
      if (pos.off > sv[pos.idx].ub())
        throw OutOfLimits("T::SweepBwdIterator::consume");
      pos.idx++;
      pos.off = 0;
    }
    assert (SweepIterator<T>::isOK());
  }
  
  template <class T>
  forceinline void
  SweepFwdIterator<T>::consumeMand(int k) {
    if (k == 0)
      return;
    pos.off += k; 
    if (pos.off > sv[pos.idx].lb())
      throw OutOfLimits("T::SweepBwdIterator::consume");
    if (pos.off == sv[pos.idx].ub()) {
      pos.idx++;
      pos.off = 0;
    }
    assert (SweepIterator<T>::isOK());
  }

}}

namespace Gecode { namespace String {
  
  template <class T>
  forceinline SweepBwdIterator<T>::
  SweepBwdIterator(const T& x)
  : SweepIterator<T>::SweepIterator(x, Position(x.size(), 0)) {};
  
  template <class T>
  forceinline SweepBwdIterator<T>::
  SweepBwdIterator(const T& x, const Position& p) 
  : SweepIterator<T>::SweepIterator(x, p) {};
  
  template <class T>
  forceinline int
  SweepBwdIterator<T>::lb() const {
    return sv[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <class T>
  forceinline int
  SweepBwdIterator<T>::ub() const {
    return sv[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <class T>
  forceinline bool
  SweepBwdIterator<T>::disj(const Block& b) const {
    const CBlock& c = sv[pos.off > 0 ? pos.idx : pos.idx-1];
    return c.isFixed() ? !b.baseContains(c.val()) : c.block().baseDisjoint(b);
  }
  template <class T>
  forceinline bool
  SweepBwdIterator<T>::disj(int k) const {
    const CBlock& c = sv[pos.off > 0 ? pos.idx : pos.idx-1];
    return c.isFixed() ? c.val() != k : !c.block().baseContains(k);
  }
  
  template <class T>
  forceinline bool
  SweepBwdIterator<T>::operator()(void) const {
    return sv.prec(Position(0,0), pos);
  }
  
  template <class T>
  forceinline void
  SweepBwdIterator<T>::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (SweepIterator<T>::isOK() || sv[pos.idx].isNull());
  };
  
  template <class T>
  forceinline bool
  SweepBwdIterator<T>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  template <class T>
  forceinline int
  SweepBwdIterator<T>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].ub() : pos.off;
  }
  
  template <class T>
  forceinline int
  SweepBwdIterator<T>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].lb() 
                                         : std::min(pos.off, sv[pos.idx].lb());
  }
  
  template <class T>
  forceinline void
  SweepBwdIterator<T>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = sv[pos.idx].ub() - k;
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<T>::consume");
    assert (SweepIterator<T>::isOK());
  }
  
  template <class T>
  forceinline void
  SweepBwdIterator<T>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = sv[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, sv[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<T>::consumeMand");
    assert (SweepIterator<T>::isOK());
  }

}}

namespace Gecode { namespace String {

  template <class View0, class View1>
  forceinline 
  ConcatView<View0,View1>::ConcatView(void) : x0(), x1(), pivot(0) {}
  
  template <class View0, class View1>
  forceinline
  ConcatView<View0,View1>::ConcatView(View0& x, View1& y)
  : x0(x), x1(y), pivot(x.size()) {}

  template <class View0, class View1>
  forceinline View0
  ConcatView<View0,View1>::lhs() const {
    return x0;  
  }

  template <class View0, class View1>
  forceinline View1
  ConcatView<View0,View1>::rhs() const {
    return x1;  
  }

  template <class View0, class View1>
  forceinline int 
  ConcatView<View0,View1>::size() const {
    return x0.size() + x1.size();
  }

  template <class View0, class View1>
  forceinline CBlock
  ConcatView<View0,View1>::operator[](int i) const {
    if (i < pivot)
      return CBlock(x0[i]);
    else
      return CBlock(x1[i-pivot]);
  }
  
  template <class View0, class View1>
  forceinline std::vector<int> 
  ConcatView<View0,View1>::val(void) const {
    if (!assigned())
      throw IllegalOperation("ConcatView<View0,View1>::val(void)");
    std::vector<int> v(x0.val());
    std::vector<int> w(x1.val());
    v.insert(std::end(v), std::begin(w), std::end(w));
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
  
  template <class View0, class View1>
  forceinline SweepFwdIterator<ConcatView<View0,View1>>
  ConcatView<View0,View1>::fwd_iterator(void) const {
    return SweepFwdIterator<ConcatView<View0,View1>>(*this);
  }
  
  template <class View0, class View1>
  forceinline SweepBwdIterator<ConcatView<View0,View1>>
  ConcatView<View0,View1>::bwd_iterator(void) const {
    return SweepBwdIterator<ConcatView<View0,View1>>(*this);
  }
  
  template <class View0, class View1>
  forceinline bool
  ConcatView<View0,View1>::equiv(const Position& p, const Position& q) const {
    return (q.idx <  pivot && x0.equiv(p,q))
        || (p.idx >= pivot && x1.equiv(p-pivot,q-pivot));
  }
  
  template <class View0, class View1>
  forceinline bool
  ConcatView<View0,View1>::prec(const Position& p, const Position& q) const {
    return p.idx < q.idx 
        || (q.idx <  pivot && x0.prec(p,q))
        || (p.idx >= pivot && x1.prec(p-pivot,q-pivot));
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
      return x1.min_len_mand(bx, p-pivot, q-pivot);
    else
      return x0.min_len_mand(bx, p, Position(pivot,0)) 
           + x1.min_len_mand(bx, Position(0,0), q-pivot);
  }
  
  template <class View0, class View1>
  forceinline void
  ConcatView<View0,View1>::mand_region(Space& home, Block& bx, Block* bnew, int u,
                                const Position& p, const Position& q) const {
    assert (prec(p,q));
    if (q.idx < pivot)
      x0.mand_region(home, bx, bnew, u, p, q);
    else if (p.idx >= pivot)
      x1.mand_region(home, bx, bnew, u, p-pivot, q-pivot);
    else {
      x0.mand_region(home, bx, bnew, u, p, Position(pivot,0));
      x1.mand_region(home, bx, bnew+pivot-p.idx, u, Position(0,0), q-pivot);
    }
  }
  
  template <class View0, class View1>
  forceinline void
  ConcatView<View0,View1>::mand_region(Space& home, Block& bx, const Block& by,
                             const Position& p, const Position& q) const {
    // FIXME: When only block by is involved.
    if (p.idx < pivot)
      x0.mand_region(home, bx, by, p, q);
    else
      x1.mand_region(home, bx, by, p-pivot, q-pivot);
  }
  
  template <class View0, class View1>
  forceinline int
  ConcatView<View0,View1>::max_len_opt(const Block& bx, const Position& esp, 
                                           const Position& lep, int l1) const {
    if (equiv(esp,lep))
      return 0;
    assert(!prec(lep, esp));
    if (lep.idx < pivot)
      return x0.max_len_opt(bx, esp, lep, l1);
    else if (esp.idx >= pivot)
      return x1.max_len_opt(bx, esp, lep, l1);
    else
      return x0.max_len_opt(bx, esp, Position(pivot,0), l1) 
           + x1.max_len_opt(bx, Position(0,0), lep-pivot, l1);
  }
  
  template <class View0, class View1>
  forceinline void
  ConcatView<View0,View1>::opt_region(Space& home, const Block& bx, Block& bnew,
                           const Position& p, const Position& q, int l1) const {
    assert(prec(p,q));
    if (q.idx < pivot)
      x0.opt_region(home, bx, bnew, p, q, l1);
    else if (p.idx >= pivot)
      x1.opt_region(home, bx, bnew, p-pivot, q-pivot, l1);
    else {
      Block& bnew0;
      Block& bnew1;
      x0.opt_region(home, bx, bnew0, p, Position(pivot,0), l1);
      x1.opt_region(home, bx, bnew1, Position(0,0), q-pivot, l1);
      Gecode::Set::GLBndSet s;
      bnew0.includeBaseIn(home, s);
      bnew1.includeBaseIn(home, s);      
      bnew.update(home, bx);
      bnew.baseIntersect(home, s);
      bnew.updateCard(home, 0, std::min(bx.ub(),ub_sum(bnew0.ub(),bnew1.ub())));
    }
  }
  
  template <class View0, class View1>
  forceinline void
  ConcatView<View0,View1>::expandBlock(Space& home, const Block& bx, Block* y) 
  const {
    for (int i = 0; i < size(); i++) {
      y[i].update(home, (*this)[i]);
      y[i].baseIntersect(home, bx);
      if (y[i].ub() > bx.ub())
        y[i].ub(home, bx.ub());
    }
  }

  template <class View0, class View1>
  forceinline void
  ConcatView<View0,View1>::crushBase(Space& home, Block& bx, const Position& p, 
                                                      const Position& q) const {
    Gecode::Set::GLBndSet s;
    for (int i = p.idx, j = q.idx - (q.off == 0); i <= j; ++i)
      (*this)[i].includeBaseIn(home, s);
    bx.baseIntersect(home, s);
  }
  
  template <class View0, class View1>
  forceinline void
  ConcatView<View0,View1>::resize(Space& home, Block newBlocks[], int newSize,
                                                          int U[],  int uSize) {
    int i, j;
    for (i = 0, j = 0; U[i] < pivot; i += 2)
      j += U[i+1];
    if (j > 0)
      x0.resize(home, newBlocks, j, U, i);
    if (i < uSize) {
      for (; i < uSize; i += 2)
        U[i] -= pivot;
      x1.resize(home, newBlocks+j, newSize-j, U+i, uSize-i);
    }
    pivot = x0.size();
  }

  template<class Char, class Traits, class X, class Y>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConcatView<X,Y>& z) {
    os << z.x0 << " ++ " << z.x1;
    return os;
  };
     
}}

