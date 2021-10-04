namespace Gecode { namespace String {

  template <>
  forceinline SweepIterator<SweepFwd,ReverseView>::
  SweepIterator(const ReverseView& x) : SweepIterator(x, Position(x.size(), 0)) {};
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ReverseView>::lb() const {
    return sv[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ReverseView>::ub() const {
    return sv[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ReverseView>::disj(const Block& b) const {
    return  sv[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ReverseView>::disj(int c) const {
    return !(sv[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(c));
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ReverseView>::operator()(void) const {
    return sv.prec(Position(0,0), pos);
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ReverseView>::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (isOK() || sv[pos.idx].isNull());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ReverseView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ReverseView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].lb() 
                                         : std::min(pos.off, sv[pos.idx].lb());
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ReverseView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].ub() : pos.off;
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ReverseView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = sv[pos.idx].ub() - k;
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ReverseView>::consume");
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ReverseView>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = sv[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, sv[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ReverseView>::consumeMand");
    assert (isOK());
  }

}}

namespace Gecode { namespace String {

  template <>
  forceinline SweepIterator<SweepBwd,ReverseView>::
  SweepIterator(const ReverseView& x) : SweepIterator(x, Position(0,0)) {};
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ReverseView>::operator()(void) const {
    return sv.prec(pos, Position(sv.size(),0));
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ReverseView>::nextBlock() {
    if (pos.idx >= sv.size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK() || sv[pos.idx].isNull());
  };
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ReverseView>::hasNextBlock(void) const {
    return pos.idx < sv.size();
  };
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ReverseView>::may_consume() const {
    return ub() - pos.off;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ReverseView>::must_consume() const {
    return std::max(0, lb() - pos.off);
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ReverseView>::consume(int k) {
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
  SweepIterator<SweepBwd,ReverseView>::consumeMand(int k) {
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
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ReverseView>::disj(const Block& b) const {
    return sv[pos.idx].baseDisjoint(b);
  }  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ReverseView>::disj(int c) const {
    return !sv[pos.idx].baseContains(c);
  }

}}

namespace Gecode { namespace String {

  forceinline ReverseView::ReverseView() : sv(*(new StringView())) {
    GECODE_NEVER;
  }
  
  forceinline
  ReverseView::ReverseView(const StringView& x) : sv(x) {}
  
  forceinline bool
  ReverseView::isNull() const {
    return sv.isNull();
  }
  
  forceinline int 
  ReverseView::size() const {
    return sv.size();
  }
 
  forceinline std::vector<int> 
  ReverseView::val(void) const {
    return sv.val();
  }
  
  forceinline int
  ReverseView::max_length() const {
    return sv.max_length();
  }
  
  forceinline int
  ReverseView::min_length() const {
    return sv.min_length();
  }
  
  forceinline bool
  ReverseView::assigned() const {
    return sv.assigned();
  }
  
  forceinline const Block&
  ReverseView::operator[](int i) const {
    return sv[sv.size()-i-1];
  }

  template<class Char, class Traits>
  forceinline std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ReverseView& v) {
    int n = v.size();
    for (int i = 0; i < n-1; ++i)
      os << v[i] << " + ";
    return os << v[n-1];
  };
  
  template <class T>
  forceinline bool
  ReverseView::contains(const T&) const {
    GECODE_NEVER
    return false;
  }
  template <class T>
  forceinline bool
  ReverseView::equals(const T&) const {
    GECODE_NEVER
    return false;
  }
  
  forceinline SweepIterator<SweepFwd,ReverseView>
  ReverseView::fwd_iterator(void) const {
    return SweepIterator<SweepFwd,ReverseView>(*this);
  }
  
  forceinline SweepIterator<SweepBwd,ReverseView>
  ReverseView::bwd_iterator(void) const {
    return SweepIterator<SweepBwd,ReverseView>(*this);
  }
  
  template <class IterY>
  forceinline Position
  ReverseView::push(int i, IterY& it) const {
    return sv.push(i, it);
  }
  
  template <class IterY>
  forceinline void
  ReverseView::stretch(int i, IterY& it) const {
    sv.stretch(i, it);
  };
  
  forceinline bool
  ReverseView::equiv(const Position& p, const Position& q) const {
    return sv.equiv(p, q);
  }
  
  forceinline bool
  ReverseView::prec(const Position& p, const Position& q) const {
    return sv.prec(p, q);
  }
  
  forceinline int
  ReverseView::ub_new_blocks(const Matching& m) const {
    if (prec(m.EEP, m.LSP))
      return prec(m.LSP, m.ESP) + m.LSP.idx - m.EEP.idx + (m.LSP.off > 0)
           + prec(m.LEP, m.EEP);
    else
      return 0;
  }

  forceinline int
  ReverseView::min_len_mand(const Block& b, const Position& p,
                                            const Position& q) const {
    return sv.min_len_mand(b, q, p);
  }  
  
  forceinline void
  ReverseView::mand_region(Space& home, const Block& bx, Block* bnew, int u,
                           const Position& p, const Position& q) const {
    if (!prec(q,p)) {
      assert ((*this)[q.idx].isNull());
      return;
    };
    int q_i = q.idx, p_i = p.off > 0 ? p.idx : p.idx-1, 
        q_o = q.off, p_o = p.off > 0 ? p.off : (*this)[p_i].ub();
//    std::cerr << "LSP=(" << p_i << "," << p_o << "), EEP=(" << q_i << "," << q_o << ")\n";
    const Block& bq = (*this)[q_i];
    // Head of the region.    
    bnew[0].update(home, bq);
    bnew[0].baseIntersect(home, bx);    
    if (!bnew[0].isNull()) {
      if (p_i == q_i) {
        bnew[0].updateCard(home, std::max(0, std::min(p_o, bq.lb()) - q_o), 
                                 std::min(u, p_o-q_o));    
        return;
      }
      else
        bnew[0].updateCard(home, std::max(0, bq.lb()-q_o), 
                                 std::min(u, bq.ub()-q_o));
    }
    else if (p_i == q_i)
      return;
    // Central part of the region.
    int j = 1;
    for (int i = q_i+1; i < p_i; ++i, ++j) {
      Block& bj = bnew[j];
      bj.update(home, (*this)[i]);
      bj.baseIntersect(home, bx);
      if (!bj.isNull() && bj.ub() > u)
        bj.ub(home, u);
    }
    // Tail of the region.
    const Block& bp = (*this)[p_i];
    bnew[j].update(home, bp);
    bnew[j].baseIntersect(home, bx);
    if (!bnew[j].isNull())
      bnew[j].updateCard(home, std::min(bp.lb(), p_o), std::min(u, p_o));
  }
  
  template <class ViewX>
  forceinline void
  ReverseView::mand_region(Space& home, ViewX& x, int idx, 
                               const Position& p, const Position& q) const {
    return sv.mand_region(home, x, idx, q, p);
  }
  
  forceinline int
  ReverseView::max_len_opt(const Block& bx, const Position& esp, 
                                            const Position& lep, int i) const {
    return sv.max_len_opt(bx, lep, esp, i);
  }
  
  forceinline void
  ReverseView::opt_region(Space& home, const Block& bx, Block& bnew,
                           const Position& p, const Position& q, int l1) const {
    assert(prec(q,p));
    int q_i = q.idx, p_i = p.off > 0 ? p.idx : p.idx-1, 
        q_o = q.off, p_o = p.off > 0 ? p.off : (*this)[p_i].ub();
//    std::cerr << "p=(" << p_i << "," << p_o << "), q=(" << q_i << "," << q_o << ")\n";
    // Only one block involved
    const Block& bq = (*this)[q_i];
    int k = bx.ub() - l1;
    if (q_i == p_i) {
      bnew.update(home, bq);
      bnew.baseIntersect(home, bx);
      if (!bnew.isNull())
        bnew.updateCard(home, 0, std::min(p_o-q_o, ubounded_sum(bq.lb(),k)));
      return;
    }
    // More than one block involved
    Set::GLBndSet s;
    bq.includeBaseIn(home, s);
    int u = bq.ub() - q_o;
    for (int i = q_i+1; i < p_i; ++i) {
      const Block& bi = (*this)[i];
      bi.includeBaseIn(home, s);
      u = ubounded_sum(u, std::min(bi.ub(), ubounded_sum(bi.lb(), k)));
    }
    const Block& bp = (*this)[p_i];
    bp.includeBaseIn(home, s);
    bnew.update(home, bx);
    bnew.baseIntersect(home, s);
    if (!bnew.isNull())
      bnew.updateCard(home, 0, std::min(bx.ub(), 
                     ubounded_sum(u, std::min(p_o, ubounded_sum(bp.lb(), k)))));
  }
  
  template <class T>
  forceinline void
  ReverseView::expandBlock(Space& home, const Block& bx, T& x) const {
    assert (!bx.isFixed());
    Set::GLBndSet s;
    Set::BndSetRanges i(bx.ranges());
    s.includeI(home, i);
    bool norm = false;
    int u = bx.ub();
    x.gets_rev(home, *this);
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
  ReverseView::crushBase(Space& home, ViewX& x, int idx, 
                        const Position& p, const Position& q) const {
    sv.crushBase(home, x, idx, q, p);
  }  
    
  forceinline int
  ReverseView::fixed_chars_pref(const Position& p, const Position& q) const {
    return sv.fixed_chars_suff(q, p);
  }
  
  forceinline int
  ReverseView::fixed_chars_suff(const Position& p, const Position& q) const {
    return sv.fixed_chars_pref(q, p);
  }
  
  forceinline std::vector<int>
  ReverseView::fixed_pref(const Position& p, const Position& q) const {
    return sv.fixed_suff(q, p);
  }
  
  forceinline std::vector<int>
  ReverseView::fixed_suff(const Position& p, const Position& q) const {
    return sv.fixed_pref(q, p);
  }
  
  forceinline double
  ReverseView::logdim() const {
    return sv.logdim();
  }
     
}}

