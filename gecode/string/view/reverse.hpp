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
  
  forceinline SweepBwdIterator<StringView>
  ReverseView::fwd_iterator(void) const {
    return sv.bwd_iterator();
  }
  
  forceinline SweepFwdIterator<StringView>
  ReverseView::bwd_iterator(void) const {
    return sv.fwd_iterator();
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

