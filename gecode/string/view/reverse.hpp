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
    // FIXME: Rewrite this.
    os << "( " << v << " )^-1";
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
    return sv.prec(q, p);
  }
  
  forceinline int
  ReverseView::ub_new_blocks(const Matching& m) const {
    if (prec(m.LSP, m.EEP))
      return prec(m.ESP, m.LSP) + m.EEP.idx - m.LSP.idx + (m.EEP.off > 0)
           + prec(m.EEP, m.LEP);
    else
      return 0;
  }

  forceinline int
  ReverseView::min_len_mand(const Block& b, const Position& p,
                                            const Position& q) const {
    return min_len_mand(b, q, p);
  }  
  
  forceinline void
  ReverseView::mand_region(Space& home, const Block& b, Block* bnew, int u,
                           const Position& p, const Position& q) const {
    return sv.mand_region(home, b, bnew, u, q, p);
  }
  
  template <class ViewX>
  forceinline void
  ReverseView::mand_region(Space& home, ViewX& x, int idx, 
                               const Position& p, const Position& q) const {
    return sv.mand_region(home, x, idx, q, p);
  }
  
  forceinline int
  ReverseView::max_len_opt(const Block& bx, const Position& esp, 
                                            const Position& lep, int) const {
    return sv.min_len_mand(bx, lep, esp);
  }
  
  forceinline void
  ReverseView::opt_region(Space& home, const Block& bx, Block& bnew,
                           const Position& p, const Position& q, int l1) const {
    return sv.opt_region(home, bx, bnew, q, p, l1);
  }
  
  template <class T>
  forceinline void
  ReverseView::expandBlock(Space& home, const Block& bx, T& x) const {
    sv.expandBlock(home, bx, x);
  }

  template <class ViewX>
  forceinline void
  ReverseView::crushBase(Space& home, ViewX& x, int idx, 
                        const Position& p, const Position& q) const {
    sv.crushBase(home, x, idx, q, p);
  }  
    
  forceinline int
  ReverseView::fixed_chars_pref(const Position& p, const Position& q) const {
    return sv.fixed_chars_pref(q, p);
  }
  
  forceinline int
  ReverseView::fixed_chars_suff(const Position& p, const Position& q) const {
    return sv.fixed_chars_suff(q, p);
  }
  
  forceinline std::vector<int>
  ReverseView::fixed_pref(const Position& p, const Position& q) const {
    return sv.fixed_pref(q, p);
  }
  
  forceinline std::vector<int>
  ReverseView::fixed_suff(const Position& p, const Position& q) const {
    return sv.fixed_suff(q, p);
  }
  
  forceinline double
  ReverseView::logdim() const {
    return sv.logdim();
  }  
    
//  template <class T> forceinline void 
//  ReverseView::gets(Space&, const T&) const {
//    GECODE_NEVER;
//  }
//  forceinline void
//  ReverseView::resize(Space&, Block[], int, int[], int) const {
//    GECODE_NEVER;                                                                
//  }
//  forceinline ModEvent ReverseView::nullify(Space&) { 
//    GECODE_NEVER; 
//    return ME_STRING_NONE;
//  }
//  forceinline void ReverseView::normalize(Space&) { GECODE_NEVER; }
//  
//  forceinline void
//  ReverseView::update(Space& home, ReverseView& w) {
//    ConstView<StringView>::update(home, w);
//    // dispose old ranges
//    assert (n == 0 && _val == NULL);
//    n = w.n;
//    _val = home.alloc<int>(n);
//    for (int i = 0; i < n; ++i)
//      _val[i] = w[i];
//  }
     
}}

