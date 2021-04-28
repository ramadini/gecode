namespace Gecode { namespace String {

  StringView::
  SweepIterator::SweepIterator(const StringView& x, const Position& p)
  : sv(x), pos(p) {
    if (!p.isNorm(x)) {
      if (p.off == sv[p.idx].ub()) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("StringView::SweepFwdIterator::SweepFwdIterator");
    }
    assert (isOK());
  };
  
  forceinline const Position& 
  StringView::SweepIterator::operator*(void) {
    return pos;
  }

  forceinline int
  StringView::SweepIterator::lb() const {
    return sv[pos.idx].lb();
  }
  
  forceinline int
  StringView::SweepIterator::ub() const {
    return sv[pos.idx].ub();
  }
  
  forceinline bool
  StringView::SweepIterator::disj(const Block& b) const {
    return sv[pos.idx].baseDisjoint(b);
  }
  
  forceinline bool
  StringView::SweepIterator::isOK() const {
    return pos.isNorm(sv);
  }

}}

namespace Gecode { namespace String {

  StringView::
  SweepFwdIterator::SweepFwdIterator(const StringView& x) 
  : SweepIterator(x, Position(0,0)) {};
  
  StringView::
  SweepFwdIterator::SweepFwdIterator(const StringView& x, const Position& p) 
  : SweepIterator(x, p) {};
  
  forceinline bool
  StringView::SweepFwdIterator::operator()(void) const {
    return sv.prec(pos, Position(sv.size(),0));
  }
  
  forceinline void
  StringView::SweepFwdIterator::nextBlock() {
    if (pos.idx >= sv.size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK());
  }
  
  forceinline bool
  StringView::SweepFwdIterator::hasNextBlock(void) const {
    return pos.idx < sv.size();
  }
  
  forceinline int
  StringView::SweepFwdIterator::must_consume() const {
    return std::max(0, lb() - pos.off);
  }
  
  forceinline int
  StringView::SweepFwdIterator::may_consume() const {
    return ub() - pos.off;
  }
  
  forceinline void
  StringView::SweepFwdIterator::consume(int k) {
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
  
  forceinline void
  StringView::SweepFwdIterator::consumeMand(int k) {
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
  
  StringView::SweepBwdIterator::SweepBwdIterator(const StringView& x) 
  : SweepIterator(x, Position(x.size(), 0)) {};
  
  StringView::
  SweepBwdIterator::SweepBwdIterator(const StringView& x, const Position& p) 
  : SweepIterator(x, p) {};
  
  forceinline int
  StringView::SweepBwdIterator::lb() const {
    return sv[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  forceinline int
  StringView::SweepBwdIterator::ub() const {
    return sv[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  forceinline bool
  StringView::SweepBwdIterator::disj(const Block& b) const {
    return  sv[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  
  forceinline bool
  StringView::SweepBwdIterator::operator()(void) const {
    return sv.prec(Position(0,0), pos);
  }
  
  forceinline void
  StringView::SweepBwdIterator::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (isOK());
  };
  
  forceinline bool
  StringView::SweepBwdIterator::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  forceinline int
  StringView::SweepBwdIterator::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].ub() : pos.off;
  }
  
  forceinline int
  StringView::SweepBwdIterator::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? sv[pos.idx-1].lb() 
                                         : std::min(pos.off, sv[pos.idx].lb());
  }
  
  forceinline void
  StringView::SweepBwdIterator::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0) {
      if (pos.idx > 0) {
        pos.idx--;
        pos.off = sv[pos.idx].ub() - k;
      }
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("StringView::SweepBwdIterator::consume");
    assert (isOK());
  }
  
  forceinline void
  StringView::SweepBwdIterator::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0)
      pos.idx--;
    pos.off = sv[pos.idx].lb() - k;
    if (pos.off < 0)
      throw OutOfLimits("StringView::SweepBwdIterator::consume");
    assert (isOK());
  }

}}

namespace Gecode { namespace String {

  /*
   * Constructors and access
   *
   */

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

  forceinline int 
  StringView::size() const {
    return x->size();
  }

  forceinline Block&
  StringView::operator[](int i) {
    return x->operator[](i);
  }
  forceinline const Block&
  StringView::operator[](int i) const {
    return x->operator[](i);
  }
  
  forceinline int
  StringView::max_length() const {
    return x->max_length();
  }
  
  forceinline int
  StringView::min_length() const {
    return x->min_length();
  }
  
  forceinline bool
  StringView::assigned() const {
    return x->assigned();
  }
  
  template<class Char, class Traits>
  forceinline  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringView& v) {
    os << v.varimp()->dom();
    return os;
  };
  
  forceinline void
  StringView::normalize(Space& home) {
    x->normalize(home);
  }
  
  forceinline StringView::SweepFwdIterator
  StringView::fwd_iterator(void) {
    return SweepFwdIterator(*this);
  }
  
  forceinline StringView::SweepBwdIterator
  StringView::bwd_iterator(void) {
    return SweepBwdIterator(*this);
  }
  
  forceinline bool
  StringView::equiv(const Position& p, const Position& q) const {
    return p == q 
       || (q.off == 0 && p.idx == q.idx-1 && p.off == (*this)[p.idx].ub());
  }
  
  forceinline bool
  StringView::prec(const Position& p, const Position& q) const {
    return (p.idx < q.idx-1)
        || (p.idx == q.idx && p.off < q.off)
        || (p.idx == q.idx-1 && (q.off == 0 || p.off != (*this)[p.idx].ub()));
  }
  
  forceinline int
  StringView::ub_new_blocks(const Matching& m) const {
    if (prec(m.LSP, m.EEP))
      return m.EEP.idx - m.LSP.idx + (m.EEP.off > 0) + prec(m.EEP, m.LEP);
    else
      return 0;
  }

  forceinline int
  StringView::min_len_mand(const Block& bx, const Position& lsp, 
                                            const Position& eep) const {
    if (!prec(lsp, eep))
      return 0;
    int h = lsp.idx, h1 = eep.off > 0 ? eep.idx : eep.idx-1, 
        k = lsp.off, k1 = eep.off > 0 ? eep.off : (*this)[h1].ub();
    if (h == h1)
      return nabla(bx, (*this)[h], std::max(0, bx.lb()-h1));
    int s = nabla(bx, (*this)[h], (*this)[h].lb() - k);
    for (int i = h+1; i < h1; i++) 
      s += nabla(bx, (*this)[i], (*this)[i].lb());
    return s + nabla(bx, (*this)[k], k1);
  }


  forceinline int
  StringView::max_len_opt(const Block& bx, const Position& esp, 
                                           const Position& lep, int l) const {
    if (equiv(esp,lep))
      return 0;
    assert(!prec(lep, esp));
    int p = esp.idx, p1 = lep.off > 0 ? lep.idx : lep.idx-1,
        q = esp.off, q1 = lep.off > 0 ? lep.off : (*this)[p1].ub();
    if (p == p1)
      return nabla(bx, (*this)[p], q1-q);
    int k = bx.ub()-l;
    int s = std::min(
      k + (*this)[p].lb(), nabla(bx, (*this)[p], (*this)[p].ub() - q)
    );
    for (int i = p+1; i < p1; i++) 
      s += std::min(k + (*this)[i].lb(), nabla(bx,(*this)[i], (*this)[i].ub()));
    return s + std::min((*this)[q].lb(), nabla(bx,(*this)[q], q1));
  }
  
  forceinline void
  StringView::expandBlock(Space& home, const Block& bx, Block* y) const {
    for (int i = 0; i < size(); i++) {
      y[i].update(home, (*this)[i]);
      y[i].baseIntersect(home, bx);
      if (y[i].ub() > bx.ub())
        y[i].ub(home, bx.ub());
    }
  }

  forceinline void
  StringView::crushBase(Space& home, Block& bx, const Position& esp, 
                                                 const Position& lep) const {
    Gecode::Set::GLBndSet s;
    int k = lep.idx - (lep.off == 0);
    for (int i = esp.idx; i <= k; ++i) {
      if ((*this)[i].baseSize() == 1) {
        int m = (*this)[i].baseMin();
        Gecode::Set::SetDelta d;
        s.include(home, m, m, d);
      }
      else {
        Gecode::Set::BndSetRanges r((*this)[i].ranges());
        s.includeI(home, r);
      }
    }
    bx.baseIntersect(home, s);
  }
   
  forceinline void
  StringView::opt_region(Space& home, const Block& bx, Block& bnew,
                              const Position& p, const Position& q) const {
    assert (prec(p, q));
    int p_i = p.idx, p_o = p.off, q_i = q.off > 0 ? q.idx : q.idx-1, 
                                  q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
    // Only one block involved
    if (p_i == q_i) {
      bnew.update(home, (*this)[p_i]);
      bnew.baseIntersect(home, bx);
      if (!bnew.isNull())
        bnew.updateCard(home, 0, q_o - p_o);
      return;
    }
    // More than one block involved
    Gecode::Set::GLBndSet s;
    int u = (*this)[p_i].ub() - p_o;
    for (int i = p_i+1; i < q_i; ++i) {
      if ((*this)[i].baseSize() == 1) {
        int m = (*this)[i].baseMin();
        Gecode::Set::SetDelta d;
        s.include(home, m, m, d);
      }
      else {
        Gecode::Set::BndSetRanges r((*this)[i].ranges());
        s.includeI(home, r);
      }
      u = ub_sum(u, (*this)[i].ub());
    }
    bnew.update(home, bx);
    bnew.baseIntersect(home, s);    
    if (!bnew.isNull())
      bnew.updateCard(home, 0, ub_sum(u, q_o));
  }
  
  forceinline void
  StringView::mand_region(Space& home, Block& bx, const Block& by,
                         const Position& lsp, const Position& eep) {
    // FIXME: When only 1 block involved.                         
    assert (lsp.idx == eep.idx || (lsp.idx == eep.idx-1 && eep.off == 0));
    bx.baseIntersect(home, by);
    if (!bx.isNull())
      bx.updateCard(home, std::max(bx.lb(), by.lb() - lsp.off),
                          std::min(bx.ub(), eep.off - lsp.off));
  }
  
  forceinline void
  StringView::mand_region(Space& home, const Block& bx, Block* bnew, int u,
                          const Position& p, const Position& q) const {
    assert (prec(p, q));
    int p_i = p.idx, p_o = p.off, q_i = q.off > 0 ? q.idx : q.idx-1, 
                                  q_o = q.off > 0 ? q.off : (*this)[q_i].ub();
    assert (p_i < q_i); // FIXME: When at least 2 blocks involved.
    // Head of the region.
    const Block& bp = (*this)[p_i];
    bnew[0].update(home, bp);
    bnew[0].baseIntersect(home, bx);
    if (!bnew[0].isNull())
      bnew[0].updateCard(home, std::max(0, bp.lb()-p_o), 
                               std::min(u, bp.ub()-p_o));
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
  
  forceinline void
  StringView::resize(Space& home, Block newBlocks[], int newSize, int U[], 
                                                                  int uSize) {
    DashedString d(home, size()+newSize);
    int j = 0, h = 0;
    for (int i = 0; i < uSize/2; ++i) {
      for (int k = j; k < U[i]; ++k)
        d[k].update(home, (*this)[k]);
      j = U[i] + U[i+1];
      for (int k = U[i]; k < j; ++k, ++h)
        d[k].update(home, newBlocks[h]);
    }
    d.normalize(home);
    update(home, d);                                                                
  }
     
}}











