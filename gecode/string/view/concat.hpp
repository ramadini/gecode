namespace Gecode { namespace String {

  forceinline
  ConcatView::ConcatView(StringView& x, StringView& y)
  : x0(x), x1(y), pivot(x.size()) {}

  forceinline StringView
  ConcatView::lhs() const {
    return x0;  
  }

  forceinline StringView
  ConcatView::rhs() const {
    return x1;  
  }

  forceinline int
  ConcatView::size() const {
    return ubounded_sum(x0.size(), x1.size());
  }

  forceinline const Block&
  ConcatView::operator[](int i) const {
    return i < pivot ? x0[i] : x1[i-pivot];
  }
  
  forceinline std::vector<int> 
  ConcatView::val(void) const {
    if (!assigned())
      throw IllegalOperation("ConcatView::val(void)");
    std::vector<int> v(x0.val());
    std::vector<int> w(x1.val());
    v.insert(std::end(v), std::begin(w), std::end(w));
    return v;
  }
  
  forceinline int
  ConcatView::max_length() const {
    return ubounded_sum(x0.max_length(), x1.max_length());
  }
  
  
  forceinline int
  ConcatView::min_length() const {
    return ubounded_sum(x0.min_length(), x1.min_length());
  }
  
  forceinline bool
  ConcatView::assigned() const {
    return x0.assigned() && x1.assigned();
  }
  
  forceinline bool
  ConcatView::isOK() const {
    return x0.isOK() && x1.isOK();
  }
  
  forceinline bool
  ConcatView::isNull() const {
    return x0.isNull() && x1.isNull();
  }
  
  forceinline int
  ConcatView::lb_sum() const {
    assert (x0.lb_sum() + x1.lb_sum() < MAX_STRING_LENGTH);
    return ubounded_sum(x0.lb_sum(), x1.lb_sum());
  }
  
  forceinline long
  ConcatView::ub_sum() const {
    long s = x0.ub_sum() + x1.ub_sum();
    assert (s >= x0.ub_sum() && s >= x1.ub_sum());
    return s;
  }
  
  forceinline ModEvent
  ConcatView::nullify(Space& home) {
    if (x0.nullify(home) == ME_STRING_FAILED)
      return ME_STRING_FAILED;
    return x1.nullify(home);
  }
  
  forceinline void
  ConcatView::normalize(Space& home) {
    x0.normalize(home);
    x1.normalize(home);
  }
  
  forceinline SweepFwdIterator<ConcatView>
  ConcatView::fwd_iterator(void) const {
    return SweepFwdIterator<ConcatView>(*this);
  }
  
  forceinline SweepBwdIterator<ConcatView>
  ConcatView::bwd_iterator(void) const {
    return SweepBwdIterator<ConcatView>(*this);
  }
  
  template <class T>
  forceinline ModEvent
  ConcatView::equate(Space& home, const T& y) {
    assert (!assigned());
    int lbs0 = x0.lb_sum(), lbs1 = x1.lb_sum(), n0 = x0.size(), n1 = x1.size();
    bool a0 = x0.assigned(), a1 = x1.assigned();
    long ubs0 = x0.ub_sum(), ubs1 = x1.ub_sum();
    Matching m[size()];
    int n, k;
    if (sweep_x(*this, y, m, k, n) && refine_x(home, *this, y, m, k, n)) {
      if (n == -1)
        return ME_STRING_NONE;
      StringDelta d;
      ModEvent me0 = ME_STRING_NONE;
      if (!a0 && x0.assigned())
        me0 = x0.varimp()->notify(home, ME_STRING_VAL, d);
      else if (x0.lb_sum() > lbs0 || x0.ub_sum() < ubs0 || size() != n0)
        me0 = x0.varimp()->notify(home, ME_STRING_CARD, d);
      ModEvent me1 = ME_STRING_NONE;
      if (!a1 && x1.assigned())
        me1 = x1.varimp()->notify(home, ME_STRING_VAL, d);
      else if (x1.lb_sum() > lbs1 || x1.ub_sum() < ubs1 || size() != n1)
        me1 = x1.varimp()->notify(home, ME_STRING_CARD, d);
      if (me0 == ME_STRING_VAL)
        return me1 == ME_STRING_VAL ? me0 : ME_STRING_CARD;
      if (me1 == ME_STRING_VAL)
        return me0 == ME_STRING_VAL ? me1 : ME_STRING_CARD;
      return StringVarImp::me_combine(me0, me1);
    }
    else
      return ME_STRING_FAILED;  
  }
  
  template <class IterY>
  forceinline void
  ConcatView::stretch(int i, IterY& it) const {
    i < pivot ? x0.stretch(i, it) : x1.stretch(i-pivot, it);
  }
  
  template <class IterY>
  forceinline Position
  ConcatView::push(int i, IterY& it) const {
    return i < pivot ? x0.push(i, it) : x1.push(i-pivot, it);
  }
  
  forceinline bool
  ConcatView::equiv(const Position& p, const Position& q) const {
    return (q.idx <  pivot && x0.equiv(p,q))
        || (p.idx >= pivot && x1.equiv(p-pivot,q-pivot));
  }
  
  forceinline bool
  ConcatView::prec(const Position& p, const Position& q) const {
    if (p.idx > q.idx)
      return false;
    return q.idx < pivot ? x0.prec(p,q) 
                         : p.idx < pivot || x1.prec(p-pivot,q-pivot);
  }
  
  forceinline int
  ConcatView::ub_new_blocks(const Matching& m) const {
    if (prec(m.LSP, m.EEP))
      return prec(m.ESP, m.LSP) + m.EEP.idx - m.LSP.idx + (m.EEP.off > 0) 
           + prec(m.EEP, m.LEP);
    else
      return 0;
  }

  forceinline int
  ConcatView::min_len_mand(const Block& bx, const Position& p,
                                        const Position& q) const {
    if (!prec(p,q))
      return 0;
    else if (q.idx < pivot || (q.idx == pivot && q.off == 0))
      return x0.min_len_mand(bx, p, q);
    else if (p.idx >= pivot)
      return x1.min_len_mand(bx, p-pivot, q-pivot);
    else
      return ubounded_sum(x0.min_len_mand(bx, p, Position(pivot,0)),
                          x1.min_len_mand(bx, Position(0,0), q-pivot));
  }
  
  forceinline void
  ConcatView::mand_region(Space& home, const Block& bx, Block* bnew, int u,
                                const Position& p, const Position& q) const {
//    std::cerr << "ConcatView::mand_region" << bx << p << pivot << q << '\n';
    assert (prec(p,q));
    if (q.idx < pivot || (q.idx == pivot && q.off == 0))
      x0.mand_region(home, bx, bnew, u, p, q);
    else if (p.idx >= pivot)
      x1.mand_region(home, bx, bnew, u, p-pivot, q-pivot);
    else {
      x0.mand_region(home, bx, bnew, u, p, Position(pivot,0));
      x1.mand_region(home, bx, bnew+pivot-p.idx, u, Position(0,0), q-pivot);
    }
  }
  
  template <class ViewX>
  forceinline void
  ConcatView::mand_region(Space& home, ViewX& x, int idx,
                          const Position& p, const Position& q) const {
    // When only block by is involved.
    if (p.idx < pivot)
      x0.mand_region(home, x, idx, p, q);
    else
      x1.mand_region(home, x, idx, p-pivot, q-pivot);
  }
  
  forceinline long
  ConcatView::max_len_opt(const Block& bx, const Position& esp, 
                                           const Position& lep, int l1) const {
    if (equiv(esp,lep))
      return 0;
    assert(!prec(lep, esp));
    if (lep.idx < pivot)
      return x0.max_len_opt(bx, esp, lep, l1);
    else if (esp.idx >= pivot)
      return x1.max_len_opt(bx, esp-pivot, lep-pivot, l1);
    else {
      int s0 = x0.max_len_opt(bx, esp, Position(pivot,0), l1),
          s1 = x1.max_len_opt(bx, Position(0,0), lep-pivot, l1);
      return long(s0) + s1;
    }
  }
  
  forceinline void
  ConcatView::opt_region(Space& home, const Block& bx, Block& bnew,
                           const Position& p, const Position& q, int l1) const {
    assert(prec(p,q));
    if (q.idx < pivot || (q.idx == pivot && q.off == 0))
      x0.opt_region(home, bx, bnew, p, q, l1);
    else if (p.idx >= pivot)
      x1.opt_region(home, bx, bnew, p-pivot, q-pivot, l1);
    else {
      Block bnew0, bnew1;
      Position r = Position(pivot,0);
      if (p != r)
        x0.opt_region(home, bx, bnew0, p, r, l1);
      r = Position(0,0);
      Position q1 = q-pivot;
      if (r != q1)
        x1.opt_region(home, bx, bnew1, r, q1, l1);
      if (bnew0.isNull() && bnew1.isNull()) {
        bnew.nullify(home);
        return;
      }
      Set::GLBndSet s;
      bnew0.includeBaseIn(home, s);
      bnew1.includeBaseIn(home, s);      
      bnew.update(home, bx);
      bnew.baseIntersect(home, s);
      bnew.updateCard(home, 0, std::min(bx.ub(),ubounded_sum(bnew0.ub(),bnew1.ub())));
    }
  }
  
  template <class ViewX>
  forceinline void
  ConcatView::crushBase(Space& home, ViewX& x, int idx, 
                        const Position& p, const Position& q) const {
    Set::GLBndSet s;
    for (int i = p.idx, j = q.idx - (q.off == 0); i <= j; ++i)
      (*this)[i].includeBaseIn(home, s);
    x.baseIntersectAt(home, idx, s);
  }
  
  forceinline std::vector<int>
  ConcatView::fixed_pref(const Position& p, const Position& q, int& np) const {
    if (q.idx < pivot)
      return x0.fixed_pref(p,q,np);
    else if (p.idx >= pivot)
      return x1.fixed_pref(p-pivot,q-pivot,np);
    else {
      std::vector<int> v = x0.fixed_pref(p,Position(pivot,0),np);
      if (np <= 0)
        return v;
      const Block& b = x0[x0.size()-1];
      //FIXME: This check is sound but not complete. We must check that the end
      //       of x0 has been actually reached.
      if (b.baseSize() > 1 || !b.baseEquals(x1[0]))
        return v;
      int ns;
      std::vector<int> w = x1.fixed_pref(Position(0,0),q-pivot,ns);
      np = ns < 0 ? ns-np : ns+np;
      v.insert(v.end(), w.begin(), w.end());
      return v;
    }
  }
  
  forceinline std::vector<int>
  ConcatView::fixed_suff(const Position& p, const Position& q, int& ns) const {
    if (q.idx < pivot)
      return x0.fixed_suff(p,q,ns);
    else if (p.idx >= pivot)
      return x1.fixed_suff(p-pivot,q-pivot,ns);
    else {
      std::vector<int> v = x1.fixed_suff(Position(0,0),q-pivot,ns);
      if (ns <= 0)
        return v;
      const Block& b = x1[0];
      //FIXME: This check is sound but not complete. We must check that the
      //       beginning of x1 has been actually reached.
      if (b.baseSize() > 1 || !b.baseEquals(x0[x0.size()-1]))
        return v;
      int np;
      std::vector<int> w = x0.fixed_suff(p,Position(pivot,0),np);
      ns = np < 0 ? np-ns : np+ns;
      v.insert(v.end(), w.begin(), w.end());
      return v;
    }
  }
  
  forceinline double
  ConcatView::logdim() const {
    return x0.logdim() + x1.logdim();
  }
  
  forceinline void
  ConcatView::resize(Space& home, Block newBlocks[], int newSize,
                                            int U[], int uSize) {
    int i, j;
    for (i = 0, j = 0; i < uSize && U[i] < pivot; i += 2)
      j += U[i+1];
    if (j > 0)
      x0.resize(home, newBlocks, j, U, i);
    else
      x0.normalize(home);
    if (i < uSize) {
      for (int k = i; k < uSize; k += 2)
        U[k] -= pivot;
      x1.resize(home, newBlocks+j, newSize-j, U+i, uSize-i);
    }
    else
      x1.normalize(home);
    pivot = x0.size();
  }
  
  forceinline void
  ConcatView::nullifyAt(Space& home, int i) {
    i < pivot ? x0.nullifyAt(home, i) : x1.nullifyAt(home, i-pivot);
  }
  forceinline void
  ConcatView::lbAt(Space& home, int i, int l) {
    i < pivot ? x0.lbAt(home, i, l) : x1.lbAt(home, i-pivot, l);
  }
  forceinline void
  ConcatView::ubAt(Space& home, int i, int u) {
    i < pivot ? x0.ubAt(home, i, u) : x1.ubAt(home, i-pivot, u);
  }  
  forceinline void
  ConcatView::baseIntersectAt(Space& home, int i, const Set::BndSet& S) {
    i < pivot ? x0.baseIntersectAt(home, i, S) :
                x1.baseIntersectAt(home, i-pivot, S);
  }
  forceinline void
  ConcatView::baseIntersectAt(Space& home, int i, const Block& b) {
    i < pivot ? x0.baseIntersectAt(home, i, b) : 
                x1.baseIntersectAt(home, i-pivot, b);
  }
  forceinline void
  ConcatView::updateCardAt(Space& home, int i, int l, int u) {
    i < pivot ? x0.updateCardAt(home, i, l, u) :
                x1.updateCardAt(home, i-pivot, l, u);
  }
  forceinline void
  ConcatView::updateAt(Space& home, int i, const Block& b) {
    i < pivot ? x0.updateAt(home, i, b) : x1.updateAt(home, i-pivot, b);
  }
  
  template<class Char, class Traits>
  forceinline std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConcatView& z) {
    os << z.lhs() << " ++ " << z.rhs();
    return os;
  };
     
}}

