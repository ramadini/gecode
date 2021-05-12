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
    if (n == 0)
      return true;
    int j = 0, k = MAX_STRING_LENGTH;
    for (int i = 0; i < n; ++i) {
      const Block& b = y[j];      
      if (b.baseContains(_val[i])) {
        k = std::min(b.lb(), k);
        if (k == 1) {
          ++j;
          k = MAX_STRING_LENGTH;
        }
        else
          k--;
      }
      else
        return false;  
    }
    return j == y.size() && k == MAX_STRING_LENGTH;
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
    for (int i = p_i, j = 0; i <= q_i; ++i, ++j)
      bnew[j].update(home, (*this)[i]);
  }
  
  forceinline void
  ConstStringView::mand_region(Space& home, Block& bx, const Block& by,
                             const Position&, const Position&) const {
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
        p_o = p.off, q_o = q.off > 0 ? q.off : 1;
//    std::cerr << "p=(" << p_i << "," << p_o << "), q=(" << q_i << "," << q_o << ")\n";
    Gecode::Set::GLBndSet s;
    for (int i = p_i; i <= q_i; ++i) {
      int c = (*this)[i]; 
      Gecode::Set::SetDelta d;
      s.include(home, c, c, d);
    }
    bnew.update(home, bx);
    bnew.baseIntersect(home, s);
    bnew.updateCard(home, 0, std::min(q_i - p_i + 1, bx.ub() - l1 + 1));
  }
  
  forceinline void
  ConstStringView::expandBlock(Space& home, const Block&, Block* y) const {
    for (int i = 0; i < n; i++)
      y[i].update(home, (*this)[i]);
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
     
}}

