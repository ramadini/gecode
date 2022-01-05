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
  
  forceinline bool
  ConstStringView::isNorm() const {
    for (int i = 1; i < n; i++)
      if (_val[i-1] == _val[i])
        return false;
    return true;
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
    std::vector<int> w = v.val();
    os << "";
    for (int i : w)
      os << int2str(i);
    return os << "";
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
  ConstStringView::max_new_blocks(const Matching& m) const {
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
  ConstStringView::mand_region(Space& home, const Block&, Block* bnew, int u,
                                const Position& p, const Position& q) const {
    assert (prec(p,q) && u <= n);
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1;
    for (int i = p_i, j = 0; i <= q_i; ++i, ++j)
      bnew[j].update(home, Block((*this)[i]));
  }
  
  template <class ViewX>
  forceinline void
  ConstStringView::mand_region(Space& home, ViewX& x, int idx, 
                               const Position& p, const Position&) const {
    x.updateAt(home, idx, (*this)[p.idx]);
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
    Set::GLBndSet s;
    Set::SetDelta d;
    s.include(home, bp, bp, d);
    int u = 1 - p_o;
    for (int i = p_i+1; i < q_i; ++i) {
      int bi = (*this)[i];
      s.include(home, bi, bi, d);
      u = ubounded_sum(u, std::min(1, k+1));
    }
    int bq = (*this)[q_i];
    s.include(home, bq, bq, d);
    bnew.update(home, bx);
    bnew.baseIntersect(home, s);    
    if (!bnew.isNull())
      bnew.updateCard(home, 0, ubounded_sum(u, std::min(q_o, k+1)));
  }

  template <class ViewX>
  forceinline void
  ConstStringView::crushBase(Space& home, ViewX& x, int idx, 
                        const Position& p, const Position& q) const {
    Set::GLBndSet s;
    for (int i = p.idx, j = q.idx - (q.off == 0); i <= j; ++i) {
      Set::SetDelta d;
      int c = (*this)[i]; 
      s.include(home, c, c, d);
    }
    x.baseIntersectAt(home, idx, s);
  }
  
  forceinline std::vector<int>
  ConstStringView::fixed_pref(const Position& p, 
                              const Position& q, int& np) const {
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1, 
        p_o = p.off, q_o = q.off > 0 ? q.off : 1;
    assert (p_i != q_i);
    np = (1 - p_o) + std::max(0, q_i - p_i - 1) + q_o;
    int i0 = p.off == 0 ? p.idx : p.idx+1, i1 = q.off == 0 ? q.idx : q.idx+1;
    std::vector<int> w(i1-i0, 1);
    for (int i = i0; i < i1; ++i)
      w[i] = _val[i];
    return w;
  }
  
  forceinline std::vector<int>
  ConstStringView::fixed_suff(const Position& p, 
                              const Position& q, int& ns) const {
    return fixed_pref(p, q, ns);
  }
  
  forceinline double
  ConstStringView::logdim() const {
    return 0;
  }  
    
  template <class T> forceinline void 
  ConstStringView::gets(Space&, const T&) const {
    GECODE_NEVER;
  }
  
  forceinline ModEvent
  ConstStringView::min_length(Space&, int l) {
    return l == n ? ME_STRING_NONE : ME_STRING_FAILED;
  }
  
  forceinline ModEvent
  ConstStringView::max_length(Space&, int u) {
    return u == n ? ME_STRING_NONE : ME_STRING_FAILED;
  }
  
  forceinline ModEvent
  ConstStringView::bnd_length(Space&, int l, int u) const {
    return l == u && u == n ? ME_STRING_NONE : ME_STRING_FAILED;
  }
  
  forceinline ModEvent ConstStringView::nullify(Space&) {
    return n == 0 ? ME_STRING_NONE : ME_STRING_FAILED;
  }
  
  forceinline void
  ConstStringView::update(Space& home, ConstStringView& w) {
    assert (n == 0 && _val == NULL);
    n = w.n;
    _val = home.alloc<int>(n);
    for (int i = 0; i < n; ++i)
      _val[i] = w[i];
  }
     
}}

