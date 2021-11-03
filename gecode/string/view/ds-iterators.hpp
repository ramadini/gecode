namespace Gecode { namespace String {

  /// Iterator used by sweep-based equation algorithms for pushing/stretching 
  /// a block against the specified View.
  /// The current position of the iterator _must always_ be normalized 
  /// w.r.t. the specified view.
  template <class View>
  class SweepIterator {
  protected:
    /// The view on which we iterate
    union {
      std::reference_wrapper<const View> view_ref;
      std::reference_wrapper<const StringView> base_ref;
    };
    /// The current position on the view, always normalized w.r.t. it
    Position pos;
  public:
    /// Constructors
    SweepIterator(const View& x);
    SweepIterator(const View& x, const Position& p);
    /// Move iterator to the beginning of the next block (if possible)
    virtual void nextBlock(void) = 0;
    /// Test whether iterator is still within the dashed string or done
    bool hasNextBlock(void) const;
    /// Min. no. of chars that must be consumed from current position within current block
    int must_consume(void) const;
    /// Max. no. of chars that may be consumed from current position within current block
    int may_consume(void) const;
    /// Consume \a k characters from current position within current block
    void consume(int k);
    /// Consume \a k mandatory characters from current position within current block
    void consumeMand(int k);
    /// Returns const reference to the current position
    const Position& operator *(void) const;
    /// Returns true iff we are not done in the iteration.
    bool operator()(void) const;
    /// Returns the lower bound of the current block
    int lb(void) const;
    /// Returns the upper bound of the current block
    int ub(void) const;
    /// Check if the base of the current block is disjoint with that of \a b
    bool disj(const Block& b) const;
    /// Check if the base of the current block is disjoint with singleton {c}
    bool disj(int c) const;
  };
  
  template <class View> struct SweepFwdIterator : public SweepIterator<View> {
    using SweepIterator<View>::pos;
    using SweepIterator<View>::view_ref;
    SweepFwdIterator(const View& x);
    SweepFwdIterator(const View& x, const Position& p);
    void nextBlock(void);
    bool hasNextBlock(void) const;
    int must_consume(void) const;
    int may_consume(void) const;
    void consume(int k);
    void consumeMand(int k);
    bool operator()(void) const;
    int lb(void) const;
    int ub(void) const;
    bool disj(const Block& b) const;
    bool disj(int c) const;
    bool isOK(void) const;
  };
  template <class View> struct SweepBwdIterator : public SweepIterator<View> {
    using SweepIterator<View>::pos;
    using SweepIterator<View>::view_ref;
    SweepBwdIterator(const View& x);
    SweepBwdIterator(const View& x, const Position& p);
    void nextBlock(void);
    bool hasNextBlock(void) const;
    int must_consume(void) const;
    int may_consume(void) const;
    void consume(int k);
    void consumeMand(int k);
    bool operator()(void) const;
    int lb(void) const;
    int ub(void) const;
    bool disj(const Block& b) const;
    bool disj(int c) const;
    bool isOK(void) const;
  };
    
}}

namespace Gecode { namespace String {

  template <class View>
  forceinline SweepIterator<View>::
  SweepIterator(const View& x, const Position& p) : view_ref(x), pos(p) {
    if (!p.isNorm(x)) {
      if (p.off == view_ref.get()[p.idx].ub()) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("SweepIterator<View>::SweepIterator");
    }
    assert (pos.isNorm(view_ref.get()));
  };
  
  template <>
  forceinline SweepIterator<ConstStringView>::
  SweepIterator(const ConstStringView& x, const Position& p) 
  : view_ref(x), pos(p) {
    if (!p.isNorm(x)) {
      if (p.off == 1) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("SweepIterator<View>::SweepIterator");
    }
    assert (pos.isNorm(view_ref.get()));
  };
  
  template <>
  forceinline SweepIterator<ReverseView>::
  SweepIterator(const ReverseView& x, const Position& p) 
  : base_ref(x.baseView()), pos(p) {
    if (!p.isNorm(x.baseView())) {
      if (p.off == base_ref.get()[p.idx].ub()) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("SweepIterator<StringView>::SweepIterator");
    }
    assert (pos.isNorm(base_ref.get()));
  };
  
  template <class View>
  forceinline const Position& 
  SweepIterator<View>::operator*(void) const {
    return pos;
  }
  
}}


namespace Gecode { namespace String {

  template <class View>
  forceinline SweepFwdIterator<View>::SweepFwdIterator(const View& x) 
  : SweepIterator<View>(x, Position(0,0)) {};
  
  template <class View>
  forceinline SweepFwdIterator<View>::
  SweepFwdIterator(const View& x, const Position& p) 
  : SweepIterator<View>(x, p) {};
  
  template <class View>
  forceinline int
  SweepFwdIterator<View>::lb() const {
    return view_ref.get()[pos.idx].lb();
  }
  
  template <class View>
  forceinline int
  SweepFwdIterator<View>::ub() const {
    return view_ref.get()[pos.idx].ub();
  }
  
  template <class View>
  forceinline bool
  SweepFwdIterator<View>::operator()(void) const {
    return view_ref.get().prec(pos, Position(view_ref.get().size(),0));
  }
  
  template <class View>
  forceinline void
  SweepFwdIterator<View>::nextBlock() {
    if (pos.idx >= view_ref.get().size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK() || view_ref.get()[pos.idx].isNull());
  }
  
  template <class View>
  forceinline bool
  SweepFwdIterator<View>::hasNextBlock(void) const {
    return pos.idx < view_ref.get().size();
  }
  
  template <class View>
  forceinline int
  SweepFwdIterator<View>::must_consume() const {
    return std::max(0, lb() - pos.off);
  }
  
  template <class View>
  forceinline int
  SweepFwdIterator<View>::may_consume() const {
    return ub() - pos.off;
  }
  
  template <class View>
  forceinline void
  SweepFwdIterator<View>::consume(int k) {
    if (k == 0)
      return;
    pos.off += k;
    if (pos.off >= view_ref.get()[pos.idx].ub()) {
      if (pos.off > view_ref.get()[pos.idx].ub())
        throw OutOfLimits("StringView::SweepBwdIterator::consume");
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <class View>
  forceinline void
  SweepFwdIterator<View>::consumeMand(int k) {
    if (k == 0)
      return;
    pos.off += k; 
    if (pos.off > view_ref.get()[pos.idx].lb())
      throw OutOfLimits("StringView::SweepBwdIterator::consume");
    if (pos.off == view_ref.get()[pos.idx].ub()) {
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <class View>
  forceinline bool
  SweepFwdIterator<View>::disj(const Block& b) const {
    return view_ref.get()[pos.idx].baseDisjoint(b);
  }  
  template <class View>
  forceinline bool
  SweepFwdIterator<View>::disj(int c) const {
    return !view_ref.get()[pos.idx].baseContains(c);
  }
  
  template <class View>
  forceinline bool
  SweepFwdIterator<View>::isOK() const {
    return pos.isNorm(view_ref.get());
  }

}}

namespace Gecode { namespace String {
  
  template <class View>
  forceinline SweepBwdIterator<View>::SweepBwdIterator(const View& x) 
  : SweepIterator<View>(x, Position(x.size(),0)) {};

  template <class View>
  forceinline SweepBwdIterator<View>::
  SweepBwdIterator(const View& x, const Position& p) 
  : SweepIterator<View>(x, p) {};

  template <class View>
  forceinline int
  SweepBwdIterator<View>::lb() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <class View>
  forceinline int
  SweepBwdIterator<View>::ub() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <class View>
  forceinline bool
  SweepBwdIterator<View>::disj(const Block& b) const {
    return  view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  template <class View>
  forceinline bool
  SweepBwdIterator<View>::disj(int c) const {
    return !(view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(c));
  }
  
  template <class View>
  forceinline bool
  SweepBwdIterator<View>::operator()(void) const {
    return view_ref.get().prec(Position(0,0), pos);
  }
  
  template <class View>
  forceinline void
  SweepBwdIterator<View>::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (isOK() || view_ref.get()[pos.idx].isNull());
  };
  
  template <class View>
  forceinline bool
  SweepBwdIterator<View>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  template <class View>
  forceinline int
  SweepBwdIterator<View>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].ub() : pos.off;
  }
  
  template <class View>
  forceinline int
  SweepBwdIterator<View>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].lb() : 
            std::min(pos.off,view_ref.get()[pos.idx].lb());
  }
  
  template <class View>
  forceinline void
  SweepBwdIterator<View>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].ub() - k;
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<View>::consume");
    assert (isOK());
  }
  
  template <class View>
  forceinline void
  SweepBwdIterator<View>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, view_ref.get()[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<View>::consumeMand");
    assert (isOK());
  }
  
  template <class View>
  forceinline bool
  SweepBwdIterator<View>::isOK() const {
    return pos.isNorm(view_ref.get());
  }

}}


namespace Gecode { namespace String {
  template <>
  forceinline void
  SweepFwdIterator<ConstStringView>::nextBlock() {
    if (pos.idx >= view_ref.get().size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK());
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
  
  template <>
  forceinline int
  SweepFwdIterator<ConstStringView>::lb() const {
    return 1;
  }
  
  template <>
  forceinline int
  SweepFwdIterator<ConstStringView>::ub() const {
    return 1;
  }
  
  template <>
  forceinline bool
  SweepFwdIterator<ConstStringView>::disj(int c) const {
    return c != view_ref.get()[pos.idx];
  }
  
  template <>
  forceinline bool
  SweepFwdIterator<ConstStringView>::disj(const Block& b) const {
    return !b.baseContains(view_ref.get()[pos.idx]);
  }
  
}}


namespace Gecode { namespace String {
  
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
  SweepBwdIterator<ConstStringView>::operator()(void) const {
    return view_ref.get().prec(Position(0,0), pos);
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
  
  template <>
  forceinline bool
  SweepBwdIterator<ConstStringView>::disj(const Block& b) const {
    return !b.baseContains(view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1]);
  }
  
  template <>
  forceinline bool
  SweepBwdIterator<ConstStringView>::disj(int k) const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1] != k;
  }

}}


namespace Gecode { namespace String {

  template <>
  forceinline SweepFwdIterator<ReverseView>::
  SweepFwdIterator(const ReverseView& x, const Position& p) 
  : SweepIterator<ReverseView>(x,p) {};
  
  template <>
  forceinline bool
  SweepFwdIterator<ReverseView>::isOK() const {
    return pos.isNorm(base_ref.get());
  }

  template <>
  forceinline SweepFwdIterator<ReverseView>::
  SweepFwdIterator(const ReverseView& x) 
  : SweepIterator(x.baseView(), Position(x.size(), 0)) {};
  
  template <>
  forceinline int
  SweepFwdIterator<ReverseView>::lb() const {
    return base_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <>
  forceinline int
  SweepFwdIterator<ReverseView>::ub() const {
    return base_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <>
  forceinline bool
  SweepFwdIterator<ReverseView>::disj(const Block& b) const {
    return  base_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  template <>
  forceinline bool
  SweepFwdIterator<ReverseView>::disj(int c) const {
    return !(base_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(c));
  }
  
  template <>
  forceinline bool
  SweepFwdIterator<ReverseView>::operator()(void) const {
    return base_ref.get().prec(Position(0,0), pos);
  }
  
  template <>
  forceinline void
  SweepFwdIterator<ReverseView>::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (isOK() || base_ref.get()[pos.idx].isNull());
  }
  
  template <>
  forceinline bool
  SweepFwdIterator<ReverseView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  }
  
  template <>
  forceinline int
  SweepFwdIterator<ReverseView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? base_ref.get()[pos.idx-1].lb() 
          : std::min(pos.off, base_ref.get()[pos.idx].lb());
  }
  
  template <>
  forceinline int
  SweepFwdIterator<ReverseView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? base_ref.get()[pos.idx-1].ub() : pos.off;
  }
  
  template <>
  forceinline void
  SweepFwdIterator<ReverseView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = base_ref.get()[pos.idx].ub() - k;
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<ReverseView>::consume");
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepFwdIterator<ReverseView>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = base_ref.get()[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, base_ref.get()[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepBwdIterator<ReverseView>::consumeMand");
    assert (isOK());
  }

}}

namespace Gecode { namespace String {

  template <>
  forceinline SweepBwdIterator<ReverseView>::
  SweepBwdIterator(const ReverseView& x, const Position& p) 
  : SweepIterator(x,p) {
    if (!p.isNorm(x.baseView())) {
      if (p.off == base_ref.get()[p.idx].ub()) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("SweepIterator<StringView>::SweepIterator");
    }
    assert (pos.isNorm(base_ref.get()));
  };
  
  template <>
  forceinline bool
  SweepBwdIterator<ReverseView>::isOK() const {
    return pos.isNorm(base_ref.get());
  }

  template <>
  forceinline SweepBwdIterator<ReverseView>::
  SweepBwdIterator(const ReverseView& x) 
  : SweepIterator(x.baseView(), Position(0,0)) {};
  
  template <>
  forceinline int
  SweepBwdIterator<ReverseView>::lb() const {
    return base_ref.get()[pos.idx].lb();
  }
  
  template <>
  forceinline int
  SweepBwdIterator<ReverseView>::ub() const {
    return base_ref.get()[pos.idx].ub();
  }
  
  template <>
  forceinline bool
  SweepBwdIterator<ReverseView>::operator()(void) const {
    return base_ref.get().prec(pos, Position(base_ref.get().size(),0));
  }
  
  template <>
  forceinline void
  SweepBwdIterator<ReverseView>::nextBlock() {
    if (pos.idx >= base_ref.get().size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK() || base_ref.get()[pos.idx].isNull());
  };
  
  template <>
  forceinline bool
  SweepBwdIterator<ReverseView>::hasNextBlock(void) const {
    return pos.idx < base_ref.get().size();
  };
  
  template <>
  forceinline int
  SweepBwdIterator<ReverseView>::may_consume() const {
    return ub() - pos.off;
  }
  
  template <>
  forceinline int
  SweepBwdIterator<ReverseView>::must_consume() const {
    return std::max(0, lb() - pos.off);
  }
  
  template <>
  forceinline void
  SweepBwdIterator<ReverseView>::consume(int k) {
    if (k == 0)
      return;
    pos.off += k;
    if (pos.off >= base_ref.get()[pos.idx].ub()) {
      if (pos.off > base_ref.get()[pos.idx].ub())
        throw OutOfLimits("StringView::SweepBwdIterator::consume");
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepBwdIterator<ReverseView>::consumeMand(int k) {
    if (k == 0)
      return;
    pos.off += k; 
    if (pos.off > base_ref.get()[pos.idx].lb())
      throw OutOfLimits("StringView::SweepBwdIterator::consume");
    if (pos.off == base_ref.get()[pos.idx].ub()) {
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <>
  forceinline bool
  SweepBwdIterator<ReverseView>::disj(const Block& b) const {
    return base_ref.get()[pos.idx].baseDisjoint(b);
  }  
  template <>
  forceinline bool
  SweepBwdIterator<ReverseView>::disj(int c) const {
    return !base_ref.get()[pos.idx].baseContains(c);
  }

}}

