namespace Gecode { namespace String {  

  template <>
  forceinline SweepIterator<ConstStringView>::
  SweepIterator(const ConstStringView& x, const Position& p) : sv(x), pos(p) {
    if (!p.isNorm(x)) {
      if (p.off == 1) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("SweepIterator<ConstStringView>::SweepIterator");
    }
    assert (pos.isNorm(sv));
  };
  
  template <>
  forceinline const Position& 
  SweepIterator<ConstStringView>::operator*(void) {
    return pos;
  }

  template <>
  forceinline int
  SweepIterator<ConstStringView>::lb() const {
    return 1;
  }
  
  template <>
  forceinline int
  SweepIterator<ConstStringView>::ub() const {
    return 1;
  }
  
  template <>
  forceinline bool
  SweepIterator<ConstStringView>::disj(const Block& b) const {
    return !b.baseContains(sv[pos.idx]);
  }
  template <>
  forceinline bool
  SweepIterator<ConstStringView>::disj(int c) const {
    return c != sv[pos.idx];
  }
  
  template <>
  forceinline bool
  SweepIterator<ConstStringView>::isOK() const {
    return pos.isNorm(sv);
  }

}}

namespace Gecode { namespace String {

  template <>
  forceinline SweepFwdIterator<ConstStringView>::
  SweepFwdIterator(const ConstStringView& x) : SweepIterator(x, Position(0,0)) {};
  
  template <>
  forceinline SweepFwdIterator<ConstStringView>::
  SweepFwdIterator(const ConstStringView& x, const Position& p) 
  : SweepIterator(x, p) {};
  
  template <>
  forceinline bool
  SweepFwdIterator<ConstStringView>::operator()(void) const {
    return sv.prec(pos, Position(sv.size(),0));
  }
  
  template <>
  forceinline void
  SweepFwdIterator<ConstStringView>::nextBlock() {
    if (pos.idx >= sv.size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK());
  }
  
  template <>
  forceinline bool
  SweepFwdIterator<ConstStringView>::hasNextBlock(void) const {
    return pos.idx < sv.size();
  }
  
  template <>
  forceinline int
  SweepFwdIterator<ConstStringView>::must_consume() const {
    return 1 - pos.off;
  }
  
  template <>
  forceinline int
  SweepFwdIterator<ConstStringView>::may_consume() const {
    return must_consume();
  }
  
  template <>
  forceinline void
  SweepFwdIterator<ConstStringView>::consume(int k) {
    if (k == 0)
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepFwdIterator<ConstStringView>::consumeMand(int k) {
    consume(k);
  }
  
}}

namespace Gecode { namespace String {
  
  template <>
  forceinline SweepBwdIterator<ConstStringView>::
  SweepBwdIterator(const ConstStringView& x)
  : SweepIterator(x, Position(x.size(), 0)) {};
  
  template <>
  forceinline SweepBwdIterator<ConstStringView>::
  SweepBwdIterator(const ConstStringView& x, const Position& p) 
  : SweepIterator(x, p) {};
  
  template <>
  forceinline int
  SweepBwdIterator<ConstStringView>::lb() const {
    return 1;
  }
  
  template <>
  forceinline int
  SweepBwdIterator<ConstStringView>::ub() const {
    return 1;
  }
  
  template <>
  forceinline bool
  SweepBwdIterator<ConstStringView>::disj(const Block& b) const {
    return !b.baseContains(sv[pos.off > 0 ? pos.idx : pos.idx-1]);
  }
  
  template <>
  forceinline bool
  SweepBwdIterator<ConstStringView>::operator()(void) const {
    return sv.prec(Position(0,0), pos);
  }
  
  template <>
  forceinline void
  SweepBwdIterator<ConstStringView>::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (isOK());
  };
  
  template <>
  forceinline bool
  SweepBwdIterator<ConstStringView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  template <>
  forceinline int
  SweepBwdIterator<ConstStringView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? 1 : pos.off;
  }
  
  template <>
  forceinline int
  SweepBwdIterator<ConstStringView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? 1 : std::min(pos.off, 1);
  }
  
  template <>
  forceinline void
  SweepBwdIterator<ConstStringView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = 0;
    }
    else
      pos.off--;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<ConstStringView>::consume");
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepBwdIterator<ConstStringView>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = 0;
    }
    else
      pos.off--;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<ConstStringView>::consumeMand");
    assert (isOK());
  }
}}

namespace Gecode { namespace String {

  forceinline ConstStringView::ConstStringView() : _val(), n(0) {};
  
  forceinline
  ConstStringView::ConstStringView(Space& home, int w[], int k) {
    if (k > Limits::MAX_STRING_LENGTH)
      throw OutOfLimits("ConstStringView::ConstStringView");
    n = k;
    _val = home.alloc<int>(n);
    for (int i = 0; i < n; ++i) {
      int wi = w[i];
      if (wi < 0 || wi >= Limits::MAX_ALPHABET_SIZE)
        throw OutOfLimits("ConstStringView::ConstStringView");
      _val[i] = wi;
    }
  }  
  
  forceinline bool
  ConstStringView::isNull() const {
    return n == 0;
  }
  
  forceinline int 
  ConstStringView::size() const {
    return n;
  }
 
  forceinline std::vector<int> 
  ConstStringView::val(void) const {
    return std::vector<int>(_val, _val + n);
  }
  
  forceinline int
  ConstStringView::max_length() const {
    return n;
  }
  
  forceinline int
  ConstStringView::min_length() const {
    return n;
  }
  
  forceinline int
  ConstStringView::ubounds_sum() const {
    return n;
  }
  
  forceinline ModEvent
  ConstStringView::bnd_length(Space&, int l, int u) const {
    return l == u && u == n ? ME_STRING_NONE : ME_STRING_FAILED;
  }
  
  forceinline bool
  ConstStringView::assigned() const {
    return true;
  }
  
  forceinline int
  ConstStringView::operator[](int i) const {
    return _val[i];
  }

  template<class Char, class Traits>
  forceinline std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConstStringView& v) {
    for (int i = 0; i < v.n; ++i)
      os << int2str(v._val[i]);
    return os;
  };
  
  template <class T>
  forceinline bool
  ConstStringView::contains(const T&) const {
    GECODE_NEVER
    return false;
  }
  template <class T>
  forceinline bool
  ConstStringView::equals(const T&) const {
    GECODE_NEVER
    return false;
  }
  
  forceinline SweepFwdIterator<ConstStringView>
  ConstStringView::fwd_iterator(void) const {
    return SweepFwdIterator<ConstStringView>(*this);
  }
  
  forceinline SweepBwdIterator<ConstStringView>
  ConstStringView::bwd_iterator(void) const {
    return SweepBwdIterator<ConstStringView>(*this);
  }
  
  template <class T>
  forceinline ModEvent
  ConstStringView::equate(Space&, const T& y) const {
    return check_equate_x(*this, y) ? ME_STRING_NONE : ME_STRING_FAILED;
  }
  
  template <class IterY>
  forceinline Position
  ConstStringView::push(int i, IterY& it) const {
//  std::cerr << "Pushing " << bx << " from " << *it << '\n';
    Position p = *it;
    // No. of chars. that must be consumed
    int cx = (*this)[i], k = 1;
    while (k > 0) {
//  std::cerr << "p=" << p << ", it=" << *it << ", k=" << k << std::endl;
      if (!it.hasNextBlock())
        return *it;
      if (it.disj(cx)) {
        // Skipping block, possibly resetting k
        if (it.lb() > 0) {
          it.nextBlock();
          p = *it;
          k = 1;
        }
        else
          it.nextBlock();
      }
      else {
        it.consume(1);
        return p;
      }
    }
    return p;
  }
  
  template <class IterY>
  forceinline void
  ConstStringView::stretch(int i, IterY& it) const {
//    std::cerr << "Streching " << cx << " from " << *it << '\n';
    int cx = (*this)[i], k = 1;
    while (it.hasNextBlock()) {
      // Min. no. of chars that must be consumed.
      int m = it.must_consume();
//      std::cerr << "it=" << *it << ", k=" << k << ", m=" << m << std::endl;      
      if (m == 0)
        it.nextBlock();
      else if (it.disj(cx))
        return;
      else if (k < m) {
        it.consumeMand(k);
        return;
      }
      else {
        k = 0;
        it.nextBlock();
      }
    }
  };
  
  forceinline bool
  ConstStringView::equiv(const Position& p, const Position& q) const {
    return p == q || (q.off == 0 && p.off == 1 && p.idx == q.idx-1)
                  || (p.off == 0 && q.off == 1 && q.idx == p.idx-1);
  }
  
  forceinline bool
  ConstStringView::prec(const Position& p, const Position& q) const {
    return (p.idx < q.idx-1)
        || (p.idx == q.idx && p.off < q.off)
        || (p.idx == q.idx-1 && (q.off > 0 || p.off == 0));
  }
  
  forceinline int
  ConstStringView::ub_new_blocks(const Matching& m) const {
    if (prec(m.LSP, m.EEP))
      return prec(m.ESP, m.LSP) + m.EEP.idx - m.LSP.idx + (m.EEP.off > 0) 
           + prec(m.EEP, m.LEP);
    else
      return 0;
  }

  forceinline int
  ConstStringView::min_len_mand(const Block&, const Position& p,
                                              const Position& q) const {
    return std::max(0, q.idx - p.idx + (q.off > 0));
  }  
  
  forceinline void
  ConstStringView::mand_region(Space& home, Block&, Block* bnew, int u,
                                const Position& p, const Position& q) const {
    assert (prec(p,q) && u <= n);
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1;
    for (int i = p_i, j = 0; i <= q_i; ++i, ++j)
      bnew[j].update(home, Block((*this)[i]));
  }
  
  forceinline void
  ConstStringView::mand_region(Space& home, Block& bx, const Block& by,
                               const Position&, const Position&) const {
    bx.update(home, by);
  }
  
  forceinline int
  ConstStringView::max_len_opt(const Block& bx, const Position& esp, 
                                                const Position& lep, int) const {
    return min_len_mand(bx, esp, lep);
  }
  
  forceinline void
  ConstStringView::opt_region(Space& home, const Block& bx, Block& bnew,
                           const Position& p, const Position& q, int l1) const {
    assert(prec(p,q));
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : 1;
//    std::cerr << "p=(" << p_i << "," << p_o << "), q=(" << q_i << "," << q_o << ")\n";
    int k = bx.ub() - l1;
    int bp = (*this)[p_i];
    if (p_i == q_i) {
      bnew.update(home, Block(bp));
      bnew.baseIntersect(home, bx);
      if (!bnew.isNull())
        bnew.updateCard(home, 0, std::min(q_o-p_o, k+1));
      return;
    }
    // More than one block involved
    Gecode::Set::GLBndSet s;
    Gecode::Set::SetDelta d;
    s.include(home, bp, bp, d);
    int u = 1 - p_o;
    for (int i = p_i+1; i < q_i; ++i) {
      int bi = (*this)[i];
      s.include(home, bi, bi, d);
      u = ub_sum(u, std::min(1, k+1));
    }
    int bq = (*this)[q_i];
    s.include(home, bq, bq, d);
    bnew.update(home, bx);
    bnew.baseIntersect(home, s);    
    if (!bnew.isNull())
      bnew.updateCard(home, 0, ub_sum(u, std::min(q_o, k+1)));
  }
  
  forceinline void
  ConstStringView::expandBlock(Space& home, const Block&, Block* y) const {
    for (int i = 0; i < n; i++)
      y[i].update(home, Block((*this)[i]));
  }

  forceinline void
  ConstStringView::crushBase(Space& home, Block& bx, const Position& p, 
                                                     const Position& q) const {
    Gecode::Set::GLBndSet s;
    for (int i = p.idx, j = q.idx - (q.off == 0); i <= j; ++i) {
      Gecode::Set::SetDelta d;
      int c = (*this)[i]; 
      s.include(home, c, c, d);
    }
    bx.baseIntersect(home, s);
  }  
    
  template <class T> forceinline ModEvent 
  ConstStringView::gets(Space&, const T&) const {
    GECODE_NEVER;
    return ME_STRING_NONE;
  }
  forceinline void
  ConstStringView::resize(Space&, Block[], int, int[], int) const {
    GECODE_NEVER;                                                                
  }
  forceinline ModEvent ConstStringView::nullify(Space&) { 
    GECODE_NEVER; 
    return ME_STRING_NONE;
  }
  forceinline void ConstStringView::normalize(Space&) { GECODE_NEVER; }
     
}}

