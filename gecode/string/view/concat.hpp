namespace Gecode { namespace String {
  template <>
  forceinline SweepIterator<SweepFwd,ConcatView>::
  SweepIterator(const ConcatView& x) 
  : SweepIterator<SweepFwd,ConcatView>(x, Position(0,0)) {};
 
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConcatView>::operator()(void) const {
    return view_ref.get().prec(pos, Position(view_ref.get().size(),0));
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConcatView>::nextBlock() {
    if (pos.idx >= view_ref.get().size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK() || view_ref.get()[pos.idx].isNull());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConcatView>::hasNextBlock(void) const {
    return pos.idx < view_ref.get().size();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConcatView>::must_consume() const {
    return std::max(0, lb() - pos.off);
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConcatView>::may_consume() const {
    return ub() - pos.off;
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConcatView>::consume(int k) {
    if (k == 0)
      return;
    pos.off += k;
    if (pos.off >= view_ref.get()[pos.idx].ub()) {
      if (pos.off > view_ref.get()[pos.idx].ub())
        throw OutOfLimits("T::SweepBwdIterator::consume");
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConcatView>::consumeMand(int k) {
    if (k == 0)
      return;
    pos.off += k; 
    if (pos.off > view_ref.get()[pos.idx].lb())
      throw OutOfLimits("T::SweepBwdIterator::consume");
    if (pos.off == view_ref.get()[pos.idx].ub()) {
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConcatView>::disj(const Block& b) const {
    return view_ref.get()[pos.idx].baseDisjoint(b);
  }
  
  template<>
  forceinline bool
  SweepIterator<SweepFwd,ConcatView>::disj(int k) const {
    return !view_ref.get()[pos.idx].baseContains(k);
  }
  
}}

namespace Gecode { namespace String {
  
  template <>
  forceinline SweepIterator<SweepBwd,ConcatView>::
  SweepIterator(const ConcatView& x) : SweepIterator<SweepBwd,ConcatView>
  ::SweepIterator(x, Position(x.size(), 0)) {};
 
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConcatView>::lb() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConcatView>::ub() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConcatView>::disj(const Block& b) const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConcatView>::disj(int k) const {
    return !view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(k);
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConcatView>::operator()(void) const {
    return view_ref.get().prec(Position(0,0), pos);
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConcatView>::nextBlock() {
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
  SweepIterator<SweepBwd,ConcatView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConcatView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].ub() : pos.off;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConcatView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].lb() 
                                         : std::min(pos.off, view_ref.get()[pos.idx].lb());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConcatView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].ub() - k;
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ConcatView>::consume");
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConcatView>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, view_ref.get()[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ConcatView>::consumeMand");
    assert (isOK());
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
  ConcatView::gets(Space&, const T&) const {
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
    return ubounded_sum(x0.lb_sum(), x1.lb_sum());
  }
  
  forceinline long
  ConcatView::ub_sum() const {
    return ubounded_sum(x0.ub_sum(), x1.lb_sum());
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
  
  forceinline SweepIterator<SweepFwd,ConcatView>
  ConcatView::fwd_iterator(void) const {
    return SweepIterator<SweepFwd,ConcatView>(*this);
  }
  
  forceinline SweepIterator<SweepBwd,ConcatView>
  ConcatView::bwd_iterator(void) const {
    return SweepIterator<SweepBwd,ConcatView>(*this);
  }
  
  template <class T>
  forceinline ModEvent
  ConcatView::equate(Space& home, const T& y) {
    assert (!assigned());
    int lbs0 = x0.lb_sum(), lbs1 = x1.lb_sum(), n0 = x0.size(), n1 = x1.size();    
    bool a0 = x0.assigned(), a1 = x1.assigned();
    long ubs0 = x0.ub_sum(), ubs1 = x1.ub_sum();
    // FIXME: logdim is avoidable if we pass the pivot to sweep_x, and we add an
    //        extra parameter to refine_x to discriminate between x0/x1.
    double ld0 = x0.logdim(), ld1 = x1.logdim();
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
      else if (x0.logdim() < ld0)
        me0 = x0.varimp()->notify(home, ME_STRING_BASE, d);
      ModEvent me1 = ME_STRING_NONE;
      if (!a1 && x1.assigned())
        me1 = x1.varimp()->notify(home, ME_STRING_VAL, d);
      else if (x1.lb_sum() > lbs1 || x1.ub_sum() < ubs1 || size() != n1)
        me1 = x1.varimp()->notify(home, ME_STRING_CARD, d);
      else if (x1.logdim() < ld1)
        me1 = x1.varimp()->notify(home, ME_STRING_BASE, d);
//      std::cerr << me0 << ' ' << me1 << '\n';
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
      return ubounded_sum(x0.max_len_opt(bx, esp, Position(pivot,0), l1),
                          x1.max_len_opt(bx, Position(0,0), lep-pivot, l1));
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
    x.gets(home, *this);
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
//    std::cerr << *this << ' ' << p << ' ' << q << '\n';
    if (q.idx < pivot)
      return x0.fixed_chars_pref(p,q);
    else if (p.idx >= pivot)
      return x1.fixed_chars_pref(p-pivot,q-pivot);
    else {
      int k = x0.fixed_chars_pref(p,Position(pivot,0));
      const Block& b = x0[x0.size()-1];
      return k < x0.min_length() || b.baseSize() > 1 || !b.baseEquals(x1[0]) ? 
             k : k + x1.fixed_chars_pref(Position(0,0),q-pivot);
    }
  }
  
  forceinline std::vector<int>
  ConcatView::fixed_pref(const Position& p, const Position& q) const {
    if (q.idx < pivot)
      return x0.fixed_pref(p,q);
    else if (p.idx >= pivot)
      return x1.fixed_pref(p-pivot,q-pivot);
    else {
      std::vector<int> v = x0.fixed_pref(p,Position(pivot,0));
      const Block& b = x0[x0.size()-1];
      if ((int) v.size() < x0.min_length() || b.baseSize() > 1 
                                           || !b.baseEquals(x1[0])) {
        std::vector<int> w = x1.fixed_pref(Position(0,0),q-pivot);
        v.insert(v.end(), w.begin(), w.end());
      }
      return v;
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
      const Block& b = x1[0];
      return k < x1.min_length() || b.baseSize() > 1 
                                 || !b.baseEquals(x0[x0.size()-1]) ?
             k : k + x0.fixed_chars_suff(p,Position(pivot,0));
    }
  }
  
  forceinline std::vector<int>
  ConcatView::fixed_suff(const Position& p, const Position& q) const {
    if (q.idx < pivot)
      return x0.fixed_suff(p,q);
    else if (p.idx >= pivot)
      return x1.fixed_suff(p-pivot,q-pivot);
    else {
      std::vector<int> v = x1.fixed_suff(Position(0,0),q-pivot);
      const Block& b = x1[0];
      if ((int) v.size() < x1.min_length() || b.baseSize() > 1 
                                           || !b.baseEquals(x0[x0.size()-1]))
        return v;
      std::vector<int> w = x0.fixed_suff(p,Position(pivot,0));
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
  
  forceinline ModEvent
  ConcatView::nullifyAt(Space& home, int i) {
    return i < pivot ? x0.nullifyAt(home, i) : x1.nullifyAt(home, i-pivot);
  }
  forceinline ModEvent
  ConcatView::lbAt(Space& home, int i, int l) {
    return i < pivot ? x0.lbAt(home, i, l) : x1.lbAt(home, i-pivot, l);
  }
  forceinline ModEvent
  ConcatView::ubAt(Space& home, int i, int u) {
    return i < pivot ? x0.ubAt(home, i, u) : x1.ubAt(home, i-pivot, u);
  }  
  forceinline ModEvent
  ConcatView::baseIntersectAt(Space& home, int i, const Set::BndSet& S) {
    return i < pivot ? x0.baseIntersectAt(home, i, S) : 
                       x1.baseIntersectAt(home, i-pivot, S);
  }
  forceinline ModEvent
  ConcatView::baseIntersectAt(Space& home, int i, const Block& b) {
    return i < pivot ? x0.baseIntersectAt(home, i, b) : 
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

