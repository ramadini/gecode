namespace Gecode { namespace String {

  template <>
  forceinline SweepIterator<StringView>::
  SweepIterator(const StringView& x, const Position& p) : sv(x), pos(p) {
    if (!p.isNorm(x)) {
      if (p.off == sv[p.idx].ub()) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("SweepIterator<StringView>::SweepIterator");
    }
    assert (pos.isNorm(sv));
  };
  
  template <>
  forceinline const Position& 
  SweepIterator<StringView>::operator*(void) {
    return pos;
  }

  template <>
  forceinline int
  SweepIterator<StringView>::lb() const {
    return sv[pos.idx].lb();
  }
  
  template <>
  forceinline int
  SweepIterator<StringView>::ub() const {
    return sv[pos.idx].ub();
  }
  
  template <>
  forceinline bool
  SweepIterator<StringView>::disj(const Block& b) const {
    return sv[pos.idx].baseDisjoint(b);
  }  
  template <>
  forceinline bool
  SweepIterator<StringView>::disj(int c) const {
    return !sv[pos.idx].baseContains(c);
  }
  
  template <>
  forceinline bool
  SweepIterator<StringView>::isOK() const {
    return pos.isNorm(sv);
  }

}}

namespace Gecode { namespace String {

  template <>
  forceinline SweepFwdIterator<StringView>::
  SweepFwdIterator(const StringView& x) : SweepIterator(x, Position(0,0)) {};
  
  template <>
  forceinline SweepFwdIterator<StringView>::
  SweepFwdIterator(const StringView& x, const Position& p) 
  : SweepIterator(x, p) {};
  
  template <>
  forceinline bool
  SweepFwdIterator<StringView>::operator()(void) const {
    return sv.prec(pos, Position(sv.size(),0));
  }
  
  template <>
  forceinline void
  SweepFwdIterator<StringView>::nextBlock() {
    if (pos.idx >= sv.size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK() || sv[pos.idx].isNull());
  }
  
  template <>
  forceinline bool
  SweepFwdIterator<StringView>::hasNextBlock(void) const {
    return pos.idx < sv.size();
  }
  
  template <>
  forceinline int
  SweepFwdIterator<StringView>::must_consume() const {
    return std::max(0, lb() - pos.off);
  }
  
  template <>
  forceinline int
  SweepFwdIterator<StringView>::may_consume() const {
    return ub() - pos.off;
  }
  
  template <>
  forceinline void
  SweepFwdIterator<StringView>::consume(int k) {
    if (k == 0)
      return;
    pos.off += k;
    if (pos.off >= sv[pos.idx].ub()) {
      if (pos.off > sv[pos.idx].ub())
        throw OutOfLimits("StringView::SweepBwdIterator::consume");
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepFwdIterator<StringView>::consumeMand(int k) {
    if (k == 0)
      return;
    pos.off += k; 
    if (pos.off > sv[pos.idx].lb())
      throw OutOfLimits("StringView::SweepBwdIterator::consume");
    if (pos.off == sv[pos.idx].ub()) {
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }

}}

namespace Gecode { namespace String {
  
  template <>
  forceinline SweepBwdIterator<StringView>::
  SweepBwdIterator(const StringView& x)
  : SweepIterator(x, Position(x.size(), 0)) {};
  
  template <>
  forceinline SweepBwdIterator<StringView>::
  SweepBwdIterator(const StringView& x, const Position& p) 
  : SweepIterator(x, p) {};
  
  template <>
  forceinline int
  SweepBwdIterator<StringView>::lb() const {
    return sv[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <>
  forceinline int
  SweepBwdIterator<StringView>::ub() const {
    return sv[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <>
  forceinline bool
  SweepBwdIterator<StringView>::disj(const Block& b) const {
    return  sv[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  template <>
  forceinline bool
  SweepBwdIterator<StringView>::disj(int c) const {
    return !(sv[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(c));
  }
  
  template <>
  forceinline bool
  SweepBwdIterator<StringView>::operator()(void) const {
    return sv.prec(Position(0,0), pos);
  }
  
  template <>
  forceinline void
  SweepBwdIterator<StringView>::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (isOK() || sv[pos.idx].isNull());
  };
  
  template <>
  forceinline bool
  SweepBwdIterator<StringView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  template <>
  forceinline int
  SweepBwdIterator<StringView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].ub() : pos.off;
  }
  
  template <>
  forceinline int
  SweepBwdIterator<StringView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].lb() 
                                         : std::min(pos.off, sv[pos.idx].lb());
  }
  
  template <>
  forceinline void
  SweepBwdIterator<StringView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = sv[pos.idx].ub() - k;
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<StringView>::consume");
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepBwdIterator<StringView>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = sv[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, sv[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<StringView>::consumeMand");
    assert (isOK());
  }

}}

namespace Gecode { namespace String {

  forceinline
  StringView::StringView(void) {}
  forceinline
  StringView::StringView(const StringVar& y)
    : VarImpView<StringVar>(y.varimp()) {}
  forceinline
  StringView::StringView(StringVarImp* y)
    : VarImpView<StringVar>(y) {}

  forceinline void
  StringView::update(Space& home, const DashedString& d) {
    x->update(home, d);
  }  
  forceinline void
  StringView::update(Space& home, const std::vector<int>& w) {
    x->update(home, w);
  }
  forceinline void
  StringView::update(Space& home, const StringView& y) {
    if (x == nullptr) {
      x = new (home) StringVarImp(home, y.size());
      x->update(home, *y.x, false);
    }
    x->update(home, *y.x);
  }
  forceinline void
  StringView::update(Space& home, const ConstStringView& y) {
    x->update(home, y.val());
  }
  forceinline void
  StringView::update(Space& home, const ConcatView& y) {
    x->update(home, *y.lhs().x, *y.rhs().x);
  }
  
  forceinline void
  StringView::update_rev(Space& home, const StringView& y) {
    x->update_rev(home, *y.x);
  }
  
  forceinline ModEvent
  StringView::nullify(Space& home) {
    return x->nullify(home);
  }
  
  forceinline bool
  StringView::same(const StringView& y) const {
    return varimp() == y.varimp();
  }
  forceinline bool
  StringView::same(const ConstStringView&) const {
    return false;
  }

  forceinline int 
  StringView::size() const {
    return x->size();
  }

  forceinline const Block&
  StringView::operator[](int i) const {
    return x->operator[](i);
  }
  
  forceinline std::vector<int> 
  StringView::val(void) const {
    return x->val();
  }
  
  forceinline int
  StringView::max_length() const {
    return x->max_length();
  }
  
  forceinline int
  StringView::min_length() const {
    return x->min_length();
  }
  
  forceinline double
  StringView::logdim() const {
    return x->logdim();
  }
  
  forceinline long
  StringView::ubounds_sum() const {
    long ub = max_length();
    if (ub == Limits::MAX_STRING_LENGTH && min_length() < ub && size() > 1) {
      ub = 0;
      for (int i = 0; i < size(); ++i)
        ub += (*this)[i].ub();
    }
    return ub;
  }
  
  forceinline bool
  StringView::assigned() const {
    return x->assigned();
  }
  
  forceinline bool
  StringView::isNull() const {
    return x->isNull();
  }
  
  forceinline bool
  StringView::isOK() const {
    return x->isOK();
  }
  
  forceinline bool
  StringView::contains(const StringView& y) const {
    return x->contains(*y.x);
  }
  forceinline bool
  StringView::contains_rev(const StringView& y) const {
    return x->contains_rev(*y.x);
  }
  template <class T>
  forceinline bool
  StringView::contains(const T&) const {
    GECODE_NEVER
    return false;
  }
  
  forceinline bool
  StringView::equals(const StringView& y) const {
    return x->equals(*y.x);
  }
  forceinline bool
  StringView::equals_rev(const StringView& y) const {
    return x->equals_rev(*y.x);
  }
    
  template <class T>
  forceinline bool
  StringView::equals(const T&) const {
    GECODE_NEVER
    return false;
  }

  forceinline ModEvent
  StringView::bnd_length(Space& home, int l, int u) {
    return x->bnd_length(home, l, u);
  }
  
  forceinline ModEvent
  StringView::min_length(Space& home, int l) {
    return x->min_length(home, l);
  }
  
  forceinline ModEvent
  StringView::max_length(Space& home, int u) {
    return x->max_length(home, u);
  }
  
  template<class Char, class Traits>
  forceinline  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringView& v) {
    os << *v.varimp();
    return os;
  };
  
  forceinline ModEvent
  StringView::splitBlock(Space& home, int idx, int c, unsigned a) {
    StringDelta d;
    return x->notify(home, x->splitBlock(home, idx, c, a), d);
  }
  
  forceinline void
  StringView::normalize(Space& home) {
    x->normalize(home);
  }
  
  forceinline SweepFwdIterator<StringView>
  StringView::fwd_iterator(void) const {
    return SweepFwdIterator<StringView>(*this);
  }
  
  forceinline SweepBwdIterator<StringView>
  StringView::bwd_iterator(void) const {
    return SweepBwdIterator<StringView>(*this);
  }
  
  forceinline int
  StringView::leftmost_unfixed_idx(void) const {
    assert (!assigned());
    for (int i = 0; i < size(); ++i)
      if (!(*this)[i].isFixed())
        return i;
    GECODE_NEVER;
  }
  
  forceinline int
  StringView::rightmost_unfixed_idx(void) const {
    assert (!assigned());
    for (int i = size()-1; i >= 0; --i)
      if (!(*this)[i].isFixed())
        return i;
    GECODE_NEVER;
  }
  
  forceinline int
  StringView::smallest_unfixed_idx(void) const {
    assert (!assigned());
    double l = std::numeric_limits<double>::infinity();
    int j = -1;
    for (int i = 0; i < size(); ++i) {
      const Block& b = (*this)[i];
      if (!b.isFixed()) {
        double lb = b.logdim();
        if (lb < l) {
          l = lb;
          j = i;  
        }
      }
    }
    return j;    
  }
  
  template <class T>
  forceinline ModEvent
  StringView::equate(Space& home, const T& y) {
    assert (!assigned() || !y.assigned());
    int lb = min_length();
    long ub = this->ubounds_sum();
    Matching m[size()];
    int n, k;
    if (sweep_x(*this, y, m, k, n) && refine_x(home, *this, y, m, k, n)) {
      if (n == -1)
        return ME_STRING_NONE;
      StringDelta d;    
      if (assigned())
        return x->notify(home, ME_STRING_VAL, d);
      int ux = max_length();
      if (min_length() > lb || (ux < ub && ux < MAX_STRING_LENGTH))
        return x->notify(home, ME_STRING_CARD, d);
      if (ux == MAX_STRING_LENGTH && ub > MAX_STRING_LENGTH) {
        long u = 0L;
        for (int i = 0; i < size(); ++i) {
          u += (*this)[i].ub();
          if (u >= ub)
            return x->notify(home, ME_STRING_BASE, d);
        }
        return x->notify(home, ME_STRING_CARD, d);
      }
      else
        return x->notify(home, ME_STRING_BASE, d);
    }
    else
      return ME_STRING_FAILED;  
  }
  
  template <class IterY>
  forceinline Position
  StringView::push(int i, IterY& it) const {
//    std::cerr << "Pushing " << (*this)[i] << " from " << *it << '\n';
    Position p = *it;
    // No. of chars. that must be consumed
    const Block& bx = (*this)[i];
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
  
  template <class IterY>
  forceinline void
  StringView::stretch(int i, IterY& it) const {
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
  
  
  forceinline bool
  StringView::equiv(const Position& p, const Position& q) const {
    return p == q 
       || (q.off == 0 && p.idx == q.idx-1 && p.off == (*this)[p.idx].ub())
       || (p.off == 0 && q.idx == p.idx-1 && q.off == (*this)[q.idx].ub());
  }
  
  forceinline bool
  StringView::prec(const Position& p, const Position& q) const {
    return (p.idx < q.idx-1)
        || (p.idx == q.idx && p.off < q.off)
        || (p.idx == q.idx-1 && (q.off > 0 || p.off < (*this)[p.idx].ub()));
  }
  
  forceinline int
  StringView::ub_new_blocks(const Matching& m) const {    
    if (prec(m.LSP, m.EEP))
      return prec(m.ESP, m.LSP) + m.EEP.idx - m.LSP.idx + (m.EEP.off > 0)
           + prec(m.EEP, m.LEP);
    else
      return 0;
  }

  forceinline int
  StringView::min_len_mand(const Block& bx, const Position& p,
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
  
  forceinline void
  StringView::mand_region(Space& home, const Block& bx, Block* bnew, int u,
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
  
  template <class ViewX>
  forceinline void
  StringView::mand_region(Space& home, ViewX& x, int idx, 
                          const Position& p, const Position& q) const {
    // FIXME: When only block by is involved.
    assert (p.idx == q.idx || (p.idx == q.idx-1 && q.off == 0));
    const Block& by = (*this)[p.idx];
    x.baseIntersectAt(home, idx, by);
    const Block& bx = x[idx];
    if (!bx.isNull()) {
      int q_off = q.off > 0 ? q.off : (*this)[q.idx-1].ub();
      x.updateCardAt(home, idx, 
                     std::max(bx.lb(), std::min(q_off, by.lb()) - p.off),
                     std::min(bx.ub(), q_off - p.off));
    }
  }
  
  forceinline int
  StringView::max_len_opt(const Block& bx, const Position& esp, 
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
  
  forceinline void
  StringView::opt_region(Space& home, const Block& bx, Block& bnew,
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
  StringView::expandBlock(Space& home, const Block& bx, T& x) const {
    assert (!bx.isFixed());
    Set::GLBndSet s;
    Set::BndSetRanges i(bx.ranges());
    s.includeI(home, i);
    bool norm = false;
    int u = bx.ub();
    x.update(home, *this);
    if (u < max_length())
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
  StringView::crushBase(Space& home, ViewX& x, int idx, 
                        const Position& p, const Position& q) const {
    Set::GLBndSet s;
    for (int i = p.idx, j = q.idx - (q.off == 0); i <= j; ++i)
      (*this)[i].includeBaseIn(home, s);
    x.baseIntersectAt(home, idx, s);
  }
  
  forceinline int
  StringView::fixed_chars_pref(const Position& p, const Position& q) const {
    if (!prec(p,q))
      return 0;
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
    const Block& bp = (*this)[p_i];
    if (p_o > bp.lb() || bp.baseSize() > 1)
      return 0;
//    std::cerr << "p=(" << p_i << "," << p_o << "), q=(" << q_i << "," << q_o << ")\n";
    if (p_i == q_i)
      return std::max(0, std::min(bp.lb(), q_o) - p_o);
    int k = bp.lb() - p_o;
    if (bp.lb() < bp.ub())
      return k;
    for (int i = p_i+1; i < q_i; ++i) {
      const Block& bi = (*this)[i];
      if (bi.baseSize() > 1)
        return k;
      k += bi.lb();
      if (bi.ub() > bi.lb())
        return k;
    }
    const Block& bq = (*this)[q_i];
    if (bq.baseSize() > 1)
      return k;
    return k + std::min(bq.lb(), q_o);
  }
  
  forceinline int
  StringView::fixed_chars_suff(const Position& p, const Position& q) const {
    if (!prec(p,q))
      return 0;
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : (*this)[q_i].ub();    
//    std::cerr << "p=(" << p_i << "," << p_o << "), q=(" << q_i << "," << q_o << ")\n";
    const Block& bq = (*this)[q_i];    
    if (bq.baseSize() > 1)
      return 0;
    if (p_i == q_i)
      return std::max(0, std::min(bq.lb(), q_o) - p_o);
    int k = std::min(bq.lb(), q_o);
    for (int i = q_i-1; i > p_i; --i) {
      const Block& bi = (*this)[i];
      if (bi.baseSize() > 1)
        return k;
      k += bi.lb();
      if (bi.ub() > bi.lb())
        return k;
    }
    const Block& bp = (*this)[p_i];
    if (p_o > bp.lb() || bp.baseSize() > 1)
      return k;
    return k + bp.lb() - p_o;
  }
  
  forceinline void
  StringView::resize(Space& home, Block newBlocks[], int newSize, int U[], 
                                                                  int uSize) {
    DashedString d(home, size()+newSize-uSize/2);
//    for (int i = 0; i < uSize; i ++)
//      std::cerr << "U[" << i << "] = " << U[i] << "\n"; 
//    for (int i = 0; i < newSize; i ++)
//      std::cerr << "newBlocks[" << i << "] = " << newBlocks[i] << "\n"; 
    int off = 0, idxOld = 0, idxNew = 0;
    for (int idxU = 0; idxU < uSize; idxU += 2) {
      for (; idxOld < size() && idxOld < U[idxU]; ++idxOld, ++idxNew)
        d.updateAt(home, idxNew, (*this)[idxOld]);
      ++idxOld;
      for (int k = 0; k < U[idxU+1]; ++idxNew, ++k)
        d.updateAt(home, idxNew, newBlocks[k + off]);
      off += U[idxU+1];
    }
    for (; idxOld < size(); ++idxOld, ++idxNew) {
      const Block& b = (*this)[idxOld];
      if (!(d[idxNew].equals(b)))
        d.updateAt(home, idxNew, b);
    }
    int u = max_length();
    d.normalize(home);
    x->update(home, d);
    if (u < max_length())
      x->max_length(home, u);
  }
  
  forceinline void
  StringView::nullifyAt(Space& home, int i) {
    x->nullifyAt(home, i);
  }
  forceinline void
  StringView::lbAt(Space& home, int i, int l) {
    x->lbAt(home, i, l);
  }
  forceinline void
  StringView::ubAt(Space& home, int i, int u) {
    x->ubAt(home, i, u);
  }
  forceinline void
  StringView::updateCardAt(Space& home, int i, int l, int u) {
    x->updateCardAt(home, i, l, u);
  }
  forceinline void
  StringView::updateAt(Space& home, int i, const Block& b) {
    x->updateAt(home, i, b);
  }
  forceinline void
  StringView::baseIntersectAt(Space& home, int idx, const Set::BndSet& S) {
    std::cerr <<idx << ' ' << CharSet(home,S) << '\n';
    x->baseIntersectAt(home, idx, S);
  }
  forceinline void
  StringView::baseIntersectAt(Space& home, int idx, const Block& b) {
    x->baseIntersectAt(home, idx, b);
  }
  
}}

