namespace Gecode { namespace String {

  forceinline ReverseView::ReverseView() : x0(*(new StringView())) {
    GECODE_NEVER;
  }
  
  forceinline
  ReverseView::ReverseView(const StringView& x) : x0(x) {}
  
  forceinline bool
  ReverseView::isNull() const {
    return x0.isNull();
  }
  
  forceinline int 
  ReverseView::size() const {
    return x0.size();
  }

  forceinline int
  ReverseView::max_length() const {
    return x0.max_length();
  }
  
  forceinline int
  ReverseView::min_length() const {
    return x0.min_length();
  }
  
  forceinline bool
  ReverseView::assigned() const {
    return x0.assigned();
  }
  
  forceinline const Block&
  ReverseView::operator[](int i) const {
    return x0[x0.size()-i-1];
  }

  forceinline const StringView&
  ReverseView::baseView() const {
    return x0;
  }

  template<class Char, class Traits>
  forceinline std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ReverseView& v) {
    return os << "( " << v.baseView() << " )^-1"; 
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

  forceinline bool
  ReverseView::equiv(const Position& p, const Position& q) const {
    return x0.equiv(q,p);
  }
  
  forceinline bool
  ReverseView::prec(const Position& p, const Position& q) const {
    return x0.prec(q,p);
  }
  
  forceinline int
  ReverseView::ub_new_blocks(const Matching& m) const {
    if (x0.prec(m.EEP, m.LSP))
      return x0.prec(m.LSP, m.ESP) + m.LSP.idx - m.EEP.idx + (m.LSP.off > 0)
           + x0.prec(m.LEP, m.EEP);
    else
      return 0;
  }

  forceinline int
  ReverseView::min_len_mand(const Block& b, const Position& p,
                                            const Position& q) const {
    return x0.min_len_mand(b, q, p);
  }  
  
  forceinline void
  ReverseView::mand_region(Space& home, const Block& bx, Block* bnew, int u,
                           const Position& p, const Position& q) const {
//    std::cerr << "p: " << p << ", q: " << q << '\n';
    if (!prec(p,q)) {
      assert ((*this)[q.idx].isNull());
      return;
    };
    int q_i = q.idx, p_i = p.off > 0 ? p.idx : p.idx-1, 
        q_o = q.off, p_o = p.off > 0 ? p.off : x0[p_i].ub();
//    std::cerr << "LSP=(" << p_i << "," << p_o << "), EEP=(" << q_i << "," << q_o << "), u = "<<u<<"\n";
    const Block& bp = x0[p_i];
//    std::cerr << "bp: " << bp << "\n";
    // Head of the region.    
    bnew[0].update(home, bp);
    bnew[0].baseIntersect(home, bx);    
    if (!bnew[0].isNull()) {
      if (p_i == q_i) {
        bnew[0].updateCard(home, std::max(0, std::min(p_o, bp.lb())-q_o), 
                                 std::min(u, p_o-q_o));
        return;
      }
      else
        bnew[0].updateCard(home, std::min(bp.lb(), p_o), std::min(u, p_o));
    }
    else if (p_i == q_i)
      return;
    // Central part of the region.
    int j = 1;
    for (int i = p_i-1; i > q_i; --i, ++j) {
      Block& bj = bnew[j];
      bj.update(home, x0[i]);
      bj.baseIntersect(home, bx);
      if (!bj.isNull() && bj.ub() > u)
        bj.ub(home, u);
    }
    // Tail of the region.
    const Block& bq = x0[q_i];
//    std::cerr << "bq: " << bq << "\n";
    bnew[j].update(home, bq);
    bnew[j].baseIntersect(home, bx);
    if (!bnew[j].isNull())
      bnew[j].updateCard(home, std::max(0, bq.lb()-q_o), 
                               std::min(u, bq.ub()-q_o));
  }
  
  template <class ViewX>
  forceinline void
  ReverseView::mand_region(Space& home, ViewX& x, int idx, 
                               const Position& p, const Position& q) const {
    return x0.mand_region(home, x, idx, q, p);
  }
  
  forceinline int
  ReverseView::max_len_opt(const Block& bx, const Position& esp, 
                                            const Position& lep, int i) const {
    return x0.max_len_opt(bx, lep, esp, i);
  }
  
  forceinline void
  ReverseView::opt_region(Space& home, const Block& bx, Block& bnew,
                           const Position& p, const Position& q, int l1) const {
    assert(prec(p,q));
    int q_i = q.idx, p_i = p.off > 0 ? p.idx : p.idx-1, 
        q_o = q.off, p_o = p.off > 0 ? p.off : x0[p_i].ub();
//    std::cerr << "ESP: (" << p_i << "," << p_o << "), LEP=(" << q_i << "," << q_o << ")\n";
    // Only one block involved
    const Block& bp = x0[p_i];
//    std::cerr << "bp: " << bp << "\n";
    int k = bx.ub() - l1;
    if (q_i == p_i) {
      bnew.update(home, bp);
      bnew.baseIntersect(home, bx);
      if (!bnew.isNull())
        bnew.updateCard(home, 0, std::min(p_o-q_o, ubounded_sum(bp.lb(), k)));
      return;
    }
    // More than one block involved
    Set::GLBndSet s;
    bp.includeBaseIn(home, s);
    int u = std::min(bx.ub(), std::min(p_o, ubounded_sum(bp.lb(), k)));
    for (int i = p_i-1; i > q_i; --i) {
      const Block& bi = x0[i];
      bi.includeBaseIn(home, s);
      u = ubounded_sum(u, std::min(bi.ub(), ubounded_sum(bi.lb(), k)));
    }
    const Block& bq = x0[q_i];
//    std::cerr << "bq: " << bq << "\n";
    bq.includeBaseIn(home, s);
    bnew.update(home, bx);
    bnew.baseIntersect(home, s);
    if (!bnew.isNull())
      bnew.updateCard(home, 0, ubounded_sum(u, std::min(bx.ub(), bq.ub()-q_o)));
  }

  template <class ViewX>
  forceinline void
  ReverseView::crushBase(Space& home, ViewX& x, int idx, 
                        const Position& p, const Position& q) const {
    x0.crushBase(home, x, idx, q, p);
  }
  
  forceinline std::vector<int>
  ReverseView::fixed_pref(const Position& p, const Position& q, int& n) const {
    return x0.fixed_suff(q, p, n);
  }
  
  forceinline std::vector<int>
  ReverseView::fixed_suff(const Position& p, const Position& q, int& n) const {
    return x0.fixed_pref(q, p, n);
  }
  
  forceinline double
  ReverseView::logdim() const {
    return x0.logdim();
  }
  
  forceinline bool
  ReverseView::isOK() const {
    return x0.isOK();
  }
  
}}

