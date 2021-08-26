namespace Gecode { namespace String {
  template <>
  forceinline SweepIterator<ConcatView>::
  SweepIterator(const ConcatView& x, const Position& p)
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
  
  template <>
  forceinline const Position& 
  SweepIterator<ConcatView>::operator*(void) {
    return pos;
  }
  template <>
  forceinline int
  SweepIterator<ConcatView>::lb() const {
    return sv[pos.idx].lb();
  }
  
  template <>
  forceinline int
  SweepIterator<ConcatView>::ub() const {
    return sv[pos.idx].ub();
  }
  
  template <>
  forceinline bool
  SweepIterator<ConcatView>::disj(const Block& b) const {
    return sv[pos.idx].baseDisjoint(b);
  }
  template<>
  forceinline bool
  SweepIterator<ConcatView>::disj(int k) const {
    return !sv[pos.idx].baseContains(k);
  }
  
  template <>
  forceinline bool
  SweepIterator<ConcatView>::isOK() const {
    return pos.isNorm(sv);
  }
}}
namespace Gecode { namespace String {
  template <>
  forceinline SweepFwdIterator<ConcatView>::
  SweepFwdIterator(const ConcatView& x) 
  : SweepIterator<ConcatView>(x, Position(0,0)) {};
  
  template <>
  forceinline SweepFwdIterator<ConcatView>::
  SweepFwdIterator(const ConcatView& x, const Position& p) 
  : SweepIterator<ConcatView>(x, p) {};
  
  template <>
  forceinline bool
  SweepFwdIterator<ConcatView>::operator()(void) const {
    return sv.prec(pos, Position(sv.size(),0));
  }
  
  template <>
  forceinline void
  SweepFwdIterator<ConcatView>::nextBlock() {
    if (pos.idx >= sv.size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (SweepIterator<ConcatView>::isOK() || sv[pos.idx].isNull());
  }
  
  template <>
  forceinline bool
  SweepFwdIterator<ConcatView>::hasNextBlock(void) const {
    return pos.idx < sv.size();
  }
  
  template <>
  forceinline int
  SweepFwdIterator<ConcatView>::must_consume() const {
    return std::max(0, SweepIterator<ConcatView>::lb() - pos.off);
  }
  
  template <>
  forceinline int
  SweepFwdIterator<ConcatView>::may_consume() const {
    return SweepIterator<ConcatView>::ub() - pos.off;
  }
  
  template <>
  forceinline void
  SweepFwdIterator<ConcatView>::consume(int k) {
    if (k == 0)
      return;
    pos.off += k;
    if (pos.off >= sv[pos.idx].ub()) {
      if (pos.off > sv[pos.idx].ub())
        throw OutOfLimits("T::SweepBwdIterator::consume");
      pos.idx++;
      pos.off = 0;
    }
    assert (SweepIterator<ConcatView>::isOK());
  }
  
  template <>
  forceinline void
  SweepFwdIterator<ConcatView>::consumeMand(int k) {
    if (k == 0)
      return;
    pos.off += k; 
    if (pos.off > sv[pos.idx].lb())
      throw OutOfLimits("T::SweepBwdIterator::consume");
    if (pos.off == sv[pos.idx].ub()) {
      pos.idx++;
      pos.off = 0;
    }
    assert (SweepIterator<ConcatView>::isOK());
  }
}}
namespace Gecode { namespace String {
  
  template <>
  forceinline SweepBwdIterator<ConcatView>::
  SweepBwdIterator(const ConcatView& x)
  : SweepIterator<ConcatView>::SweepIterator(x, Position(x.size(), 0)) {};
  
  template <>
  forceinline SweepBwdIterator<ConcatView>::
  SweepBwdIterator(const ConcatView& x, const Position& p) 
  : SweepIterator<ConcatView>::SweepIterator(x, p) {};
  
  template <>
  forceinline int
  SweepBwdIterator<ConcatView>::lb() const {
    return sv[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <>
  forceinline int
  SweepBwdIterator<ConcatView>::ub() const {
    return sv[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <>
  forceinline bool
  SweepBwdIterator<ConcatView>::disj(const Block& b) const {
    return sv[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  template <>
  forceinline bool
  SweepBwdIterator<ConcatView>::disj(int k) const {
    return !sv[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(k);
  }
  
  template <>
  forceinline bool
  SweepBwdIterator<ConcatView>::operator()(void) const {
    return sv.prec(Position(0,0), pos);
  }
  
  template <>
  forceinline void
  SweepBwdIterator<ConcatView>::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (SweepIterator<ConcatView>::isOK() || sv[pos.idx].isNull());
  };
  
  template <>
  forceinline bool
  SweepBwdIterator<ConcatView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  template <>
  forceinline int
  SweepBwdIterator<ConcatView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].ub() : pos.off;
  }
  
  template <>
  forceinline int
  SweepBwdIterator<ConcatView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].lb() 
                                         : std::min(pos.off, sv[pos.idx].lb());
  }
  
  template <>
  forceinline void
  SweepBwdIterator<ConcatView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = sv[pos.idx].ub() - k;
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<ConcatView>::consume");
    assert (SweepIterator<ConcatView>::isOK());
  }
  
  template <>
  forceinline void
  SweepBwdIterator<ConcatView>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = sv[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, sv[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<ConcatView>::consumeMand");
    assert (SweepIterator<ConcatView>::isOK());
  }
}}
namespace Gecode { namespace String {

  forceinline 
  ConcatView::ConcatView(void) 
  : x0(*(new StringView())), x1(*(new StringView())), pivot(0) { GECODE_NEVER; }
  
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
  
  template <class T> forceinline void 
  ConcatView::update(Space&, const T&) const {
    GECODE_NEVER;
  }
  forceinline ModEvent
  ConcatView::bnd_length(Space&, int, int) const {
    GECODE_NEVER;
    return ME_STRING_NONE;
  }
  template <class T>
  forceinline bool
  ConcatView::contains(const T&) const {
    GECODE_NEVER
    return false;
  }  
  template <class T>
  forceinline bool
  ConcatView::equals(const T&) const {
    GECODE_NEVER
    return false;
  }
  forceinline ModEvent
  ConcatView::max_length(Space&, int) const {
    GECODE_NEVER;
    return ME_STRING_NONE;
  }

  forceinline int 
  ConcatView::size() const {
    return x0.size() + x1.size();
  }

  forceinline const Block&
  ConcatView::operator[](int i) const {
    if (i < pivot)
      return x0[i];
    else
      return x1[i-pivot];
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
    return x0.max_length() + x1.max_length();
  }
  
  
  forceinline int
  ConcatView::min_length() const {
    return x0.min_length() + x1.min_length();
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
  ConcatView::me(Space& home, const T& xk, int lb, int ub) const {
    int ux = xk.max_length();
    StringDelta d;
    if (xk.min_length() > lb || (ux < ub && ux < MAX_STRING_LENGTH))
      return xk.varimp()->notify(home, ME_STRING_CARD, d);
    if (ux == MAX_STRING_LENGTH && ub > MAX_STRING_LENGTH) {
      long u = 0L;
      for (int i = 0; i < xk.size(); ++i) {
        u += xk[i].ub();
        if (u >= ub)
          return xk.varimp()->notify(home, ME_STRING_BASE, d);
      }
      return xk.varimp()->notify(home, ME_STRING_CARD, d);
    }
    else
      return xk.varimp()->notify(home, ME_STRING_BASE, d);
  }
  
  template <class T>
  forceinline ModEvent
  ConcatView::equate(Space& home, const T& y) {
    if (assigned())
      return check_equate_x(*this,y) ? ME_STRING_NONE : ME_STRING_FAILED;
    int lb0 = x0.min_length(), lb1 = x1.min_length();
    long ub0 = x0.ubounds_sum(), ub1 = x1.ubounds_sum();
    Matching m[size()];
    int n, k;
    if (sweep_x(*this, y, m, k, n) && refine_x(home, *this, y, m, k, n)) {
      if (n == -1)
        return ME_STRING_NONE;
      return StringVarImp::me_combine(me<StringView>(home, x0, lb0, ub0),
                                      me<StringView>(home, x1, lb1, ub1));
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
      return x0.min_len_mand(bx, p, Position(pivot,0)) 
           + x1.min_len_mand(bx, Position(0,0), q-pivot);
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
    // FIXME: When only block by is involved.
    if (p.idx < pivot)
      x0.mand_region(home, x, idx, p, q);
    else
      x1.mand_region(home, x, idx, p-pivot, q-pivot);
  }
  
  forceinline int
  ConcatView::max_len_opt(const Block& bx, const Position& esp, 
                                           const Position& lep, int l1) const {
    if (equiv(esp,lep))
      return 0;
    assert(!prec(lep, esp));
    if (lep.idx < pivot)
      return x0.max_len_opt(bx, esp, lep, l1);
    else if (esp.idx >= pivot)
      return x1.max_len_opt(bx, esp-pivot, lep-pivot, l1);
    else
      return x0.max_len_opt(bx, esp, Position(pivot,0), l1) 
           + x1.max_len_opt(bx, Position(0,0), lep-pivot, l1);
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
  
  template <class T>
  forceinline void
  ConcatView::expandBlock(Space& home, const Block& bx, T& x) const {
    assert (!bx.isFixed());
    Set::GLBndSet s;
    Set::BndSetRanges i(bx.ranges());
    s.includeI(home, i);
    bool norm = false;
    int u = bx.ub();
    x.update(home, *this);
    if (u < x.max_length())
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
  ConcatView::crushBase(Space& home, ViewX& x, int idx, 
                        const Position& p, const Position& q) const {
    Set::GLBndSet s;
    for (int i = p.idx, j = q.idx - (q.off == 0); i <= j; ++i)
      (*this)[i].includeBaseIn(home, s);
    x.baseIntersectAt(home, idx, s);
  }
  
  forceinline int
  ConcatView::fixed_chars_pref(const Position& p, const Position& q) const {
    if (q.idx < pivot)
      return x0.fixed_chars_pref(p,q);
    else if (p.idx >= pivot)
      return x1.fixed_chars_pref(p-pivot,q-pivot);
    else {
      int k = x0.fixed_chars_pref(p,Position(pivot,0));
      return k == 0 ? 0 : k + x1.fixed_chars_pref(Position(0,0),q-pivot);
    }
  }
  
  forceinline int
  ConcatView::fixed_chars_suff(const Position& p, const Position& q) const {
    if (q.idx < pivot)
      return x0.fixed_chars_suff(p,q);
    else if (p.idx >= pivot)
      return x1.fixed_chars_suff(p-pivot,q-pivot);
    else {
      int k = x1.fixed_chars_suff(Position(0,0),q-pivot);
      return k == 0 ? 0 : k + x0.fixed_chars_suff(p,Position(pivot,0));
    }
  }
  
  forceinline double
  ConcatView::logdim() const {
    return x0.logdim() + x1.logdim();
  }
  
  forceinline void
  ConcatView::resize(Space& home, Block newBlocks[], int newSize,
                                                          int U[],  int uSize) {
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
    x->nullifyAt(home, i);
  }
  forceinline void
  ConcatView::lbAt(Space& home, int i, int l) {
    x->lbAt(home, i, l);
  }
  forceinline void
  ConcatView::ubAt(Space& home, int i, int u) {
    x->ubAt(home, i, u);
  }
  forceinline void
  ConcatView::updateCardAt(Space& home, int i, int l, int u) {
    x->updateCardAt(home, i, l, u);
  }
  forceinline void
  ConcatView::updateAt(Space& home, int i, const Block& b) {
    x->updateAt(home, i, b);
  }
  forceinline void
  ConcatView::baseIntersectAt(Space& home, int idx, const Set::BndSet& S) {
    x->baseIntersectAt(home, idx, S);
  }
  forceinline void
  ConcatView::baseIntersectAt(Space& home, int idx, const Block& b) {
    x->baseIntersectAt(home, idx, b);
  }
  
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConcatView& z) {
    os << z.lhs() << " ++ " << z.rhs();
    return os;
  };
     
}}

