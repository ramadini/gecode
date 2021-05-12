namespace Gecode { namespace String {

  /*
   * Constructors and access
   *
   */
  
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

  template<class Char, class Traits>
  forceinline  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConstStringView& v) {
    for (int i = 0; i < v.n; ++i)
      os << v._val[i];
    return os;
  };
  
  forceinline bool
  ConstStringView::contains(const StringView& y) const {
    return equals(y);
  }
  
  forceinline bool
  ConstStringView::equals(const StringView& y) const {
    if (!y.assigned() || n != y.min_length())
      return false;
    std::vector<int> v = y.val();
    for (int i = 0; i < n; ++i)
      if (_val[i] != v[i])
        return false;
    return true;
  }
  
//  
//  forceinline ConstStringView::SweepFwdIterator
//  ConstStringView::fwd_iterator(void) const {
//    return SweepFwdIterator(*this);
//  }
//  
//  forceinline ConstStringView::SweepBwdIterator
//  ConstStringView::bwd_iterator(void) const {
//    return SweepBwdIterator(*this);
//  }
//  
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
    assert (prec(p,q) && n <= u);
    int p_i = p.idx, q_i = q.off > 0 ? q.idx : q.idx-1;
    bnew[0].update(home, (*this)[p_i]);
    for (int i = p_i+1, j = 1; i <= q_i; ++i, ++j)
      bnew[j].update(home, (*this)[i]);
  }
  
  forceinline void
  ConstStringView::mand_region(Space& home, Block& bx, const Block& by,
                             const Position&, const Position&) const {
    // FIXME: When only block by is involved.
    assert (by.isFixed());
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
        p_o = p.off, q_o = q.off > 0 ? q.off :1;
//    std::cerr << "p=(" << p_i << "," << p_o << "), q=(" << q_i << "," << q_o << ")\n";
    // Only one block involved
    const Block& bp = (*this)[p_i];
    int k = bx.ub() - l1;
    if (p_i == q_i) {
      bnew.update(home, bp);
      bnew.baseIntersect(home, bx);
      if (!bnew.isNull())
        bnew.updateCard(home, 0, std::min(q_o-p_o, 1+k));
      return;
    }
    // More than one block involved
    Gecode::Set::GLBndSet s;
    bp.includeBaseIn(home, s);
    int u = bp.ub() - p_o;
    for (int i = p_i+1; i < q_i; ++i) {
      const Block& bi = (*this)[i];
      bi.includeBaseIn(home, s);
      u = ub_sum(u, std::min(bi.ub(), 1+k));
    }
    const Block& bq = (*this)[q_i];
    bq.includeBaseIn(home, s);
    bnew.update(home, bx);
    bnew.baseIntersect(home, s);    
    if (!bnew.isNull())
      bnew.updateCard(home, 0, ub_sum(u, std::min(q_o, 1+k)));
  }
  
  forceinline void
  ConstStringView::expandBlock(Space& home, const Block&, Block* y) const {
    for (int i = 0; i < size(); i++)
      y[i].update(home, (*this)[i]);
  }

  forceinline void
  ConstStringView::crushBase(Space& home, Block& bx, const Position& p, 
                                                     const Position& q) const {
    Gecode::Set::GLBndSet s;
    for (int i = p.idx, j = q.idx - (q.off == 0); i <= j; ++i) {
      Gecode::Set::SetDelta d;
      int k = (*this)[i]; 
      s.include(home, k, k, d);
    }
    bx.baseIntersect(home, s);
  }
     
}}

