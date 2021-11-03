namespace Gecode { namespace String {

  forceinline
  StringView::StringView(void) {}
  forceinline
  StringView::StringView(const StringVar& y)
    : VarImpView<StringVar>(y.varimp()), DashedViewBase(&(y.varimp()->dom())) {}
  forceinline
  StringView::StringView(StringVarImp* y)
    : VarImpView<StringVar>(y), DashedViewBase(&(y->dom())) {}

  forceinline void
  StringView::gets(Space& home, const DashedString& d) {
    x->gets(home, d);
  }  
  forceinline void
  StringView::gets(Space& home, const std::vector<int>& w) {
    x->gets(home, w);
  }
  forceinline void
  StringView::gets(Space& home, const StringView& y) {
    x->gets(home, *y.x);
  }
  forceinline void
  StringView::gets(Space& home, const ConstStringView& y) {
    x->gets(home, y.val());
  }
  forceinline void
  StringView::gets(Space& home, const ConcatView& y) {
    x->gets(home, *y.lhs().x, *y.rhs().x);
  }
  
  forceinline void
  StringView::gets_rev(Space& home, const StringView& y) {
    x->gets_rev(home, *y.x);
  }
  
  forceinline ModEvent
  StringView::nullify(Space& home) {
    return x->nullify(home);
  }
  
  template <class T>
  forceinline bool
  StringView::same(const T& y) const {
    return varimp() == y.varimp();
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
  
  forceinline int
  StringView::lb_sum() const {
    return x->lb_sum();
  }
  
  forceinline long
  StringView::ub_sum() const {
    return x->ub_sum();
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
    return os << *v.varimp();
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
    assert (!assigned());
    int lbs = lb_sum();
    long ubs = ub_sum();
    Matching m[size()];
    int n, k, s = size();
    if (sweep_x(*this, y, m, k, n) && refine_x(home, *this, y, m, k, n)) {
      if (n == -1)
        return ME_STRING_NONE;      
      StringDelta d;
      int lbs0 = lb_sum(); long ubs0 = ub_sum();     
      if (assigned())
        return x->notify(home, ME_STRING_VAL, d);
      return x->notify(home, lbs0 > lbs || ubs0 < ubs || size() != s? 
                             ME_STRING_CARD : ME_STRING_BASE, d);
    }
    else
      return ME_STRING_FAILED;  
  }
  
  forceinline void
  StringView::resize(Space& home, Block newBlocks[], int newSize, int U[], 
                                                                  int uSize) {
//    std::cerr << "Resizing " << *this << '\n';
//    for (int i = 0; i < uSize; i ++) std::cerr << "U[" << i << "] = " << U[i] << "\n"; 
//    for (int i = 0; i < newSize; i ++) std::cerr << "newBlocks[" << i << "] = " << newBlocks[i] << "\n";
    Region r;
    int n = size()+newSize-uSize/2;
    Block* d = r.alloc<Block>(n);
    int off = 0, idxOld = 0, idxNew = 0;
    for (int idxU = 0; idxU < uSize; idxU += 2) {
      for (; idxOld < size() && idxOld < U[idxU]; ++idxOld, ++idxNew)
        d[idxNew].update(home, (*this)[idxOld]);
      ++idxOld;
      for (int k = 0; k < U[idxU+1]; ++idxNew, ++k)
        d[idxNew].update(home, newBlocks[k + off]);
      off += U[idxU+1];
    }
    for (; idxOld < size(); ++idxOld, ++idxNew) {
      const Block& b = (*this)[idxOld];
      if (!(d[idxNew].equals(b)))
        d[idxNew].update(home, b);
    }
    int u = max_length();
    x->gets(home, DashedString(home, d, n));
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
  StringView::baseIntersectAt(Space& home, int idx, const Set::BndSet& S) {
    x->baseIntersectAt(home, idx, S);
  }
  forceinline void
  StringView::baseIntersectAt(Space& home, int idx, const Block& b) {
    x->baseIntersectAt(home, idx, b);
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
  StringView::update(Space& home, StringView& y) {
    VarImpView<StringVar>::update(home,y);
    DashedViewBase::ptr = &(x->dom());
  }
  
}}

