namespace Gecode { namespace String {

  struct SweepFwd;
  struct SweepBwd;
  
  /// Iterator used by sweep-based equation algorithms for pushing/stretching 
  /// a block against the specified View in the specified direction Dir.
  /// FIXME: The current position of the iterator _must always_ be normalized 
  ///        w.r.t. the view regardless of the direction.
  template <class Dir, class View>
  class SweepIterator {
  protected:
    /// The view on which we iterate
    union {
      std::reference_wrapper<const View> view_ref;
      std::reference_wrapper<const StringView> base_ref;
    };
    /// The current position on the view, always normalized w.r.t. it
    Position pos;
    /// Check if the iterator position is in a consistent state;
    bool isOK(void) const;
  public:
    /// Constructors
    SweepIterator(const View& x);
    SweepIterator(const View& x, const Position& p);
    /// Move iterator to the beginning of the next block (if possible)
    void nextBlock(void);
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
    
}}

namespace Gecode { namespace String {

  template <class Dir, class View>
  forceinline SweepIterator<Dir,View>::
  SweepIterator(const View& x, const Position& p) : view_ref(x), pos(p) {
    if (!p.isNorm(x)) {
      if (p.off == view_ref.get()[p.idx].ub()) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("SweepIterator<StringView>::SweepIterator");
    }
    assert (pos.isNorm(view_ref.get()));
  };
  
  template <class Dir, class View>
  forceinline const Position& 
  SweepIterator<Dir,View>::operator*(void) const {
    return pos;
  }
  
  template <class Dir, class View>
  forceinline bool
  SweepIterator<Dir,View>::isOK() const {
    return pos.isNorm(view_ref.get());
  }

  template <class Dir, class View>
  forceinline int
  SweepIterator<Dir, View>::lb() const {
    return view_ref.get()[pos.idx].lb();
  }
  
  template <class Dir, class View>
  forceinline int
  SweepIterator<Dir, View>::ub() const {
    return view_ref.get()[pos.idx].ub();
  }

}}

namespace Gecode { namespace String {

  template <>
  forceinline SweepIterator<SweepFwd,StringView>::
  SweepIterator(const StringView& x) : view_ref(x), pos(Position(0,0)) {};
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,StringView>::operator()(void) const {
    return view_ref.get().prec(pos, Position(view_ref.get().size(),0));
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,StringView>::nextBlock() {
    if (pos.idx >= view_ref.get().size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK() || view_ref.get()[pos.idx].isNull());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,StringView>::hasNextBlock(void) const {
    return pos.idx < view_ref.get().size();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,StringView>::must_consume() const {
    return std::max(0, lb() - pos.off);
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,StringView>::may_consume() const {
    return ub() - pos.off;
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,StringView>::consume(int k) {
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
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,StringView>::consumeMand(int k) {
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
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,StringView>::disj(const Block& b) const {
    return view_ref.get()[pos.idx].baseDisjoint(b);
  }  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,StringView>::disj(int c) const {
    return !view_ref.get()[pos.idx].baseContains(c);
  }

}}

namespace Gecode { namespace String {
  
  template <>
  forceinline SweepIterator<SweepBwd,StringView>::
  SweepIterator(const StringView& x) : view_ref(x), pos(Position(x.size(),0)) {};

  template <>
  forceinline int
  SweepIterator<SweepBwd,StringView>::lb() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,StringView>::ub() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,StringView>::disj(const Block& b) const {
    return  view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  template <>
  forceinline bool
  SweepIterator<SweepBwd,StringView>::disj(int c) const {
    return !(view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(c));
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,StringView>::operator()(void) const {
    return view_ref.get().prec(Position(0,0), pos);
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,StringView>::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (isOK() || view_ref.get()[pos.idx].isNull());
  };
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,StringView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,StringView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].ub() : pos.off;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,StringView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].lb() : 
            std::min(pos.off,view_ref.get()[pos.idx].lb());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,StringView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].ub() - k;
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,StringView>::consume");
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,StringView>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, view_ref.get()[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,StringView>::consumeMand");
    assert (isOK());
  }

}}

namespace Gecode { namespace String {

  template <>
  forceinline SweepIterator<SweepFwd,ConstStringView>::
  SweepIterator(const ConstStringView& x, const Position& p) 
  : view_ref(x), pos(p) {
    if (!p.isNorm(x)) {
      if (p.off == 1) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("SweepIterator<StringView>::SweepIterator");
    }
    assert (pos.isNorm(view_ref.get()));
  };

  template <>
  forceinline SweepIterator<SweepFwd,ConstStringView>::
  SweepIterator(const ConstStringView& x) : SweepIterator(x, Position(0,0)) {};
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstStringView>::operator()(void) const {
    return view_ref.get().prec(pos, Position(view_ref.get().size(),0));
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConstStringView>::nextBlock() {
    if (pos.idx >= view_ref.get().size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstStringView>::hasNextBlock(void) const {
    return pos.idx < view_ref.get().size();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConstStringView>::must_consume() const {
    return 1 - pos.off;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConstStringView>::may_consume() const {
    return must_consume();
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConstStringView>::consume(int k) {
    if (k == 0)
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConstStringView>::consumeMand(int k) {
    consume(k);
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConstStringView>::lb() const {
    return 1;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConstStringView>::ub() const {
    return 1;
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstStringView>::disj(int c) const {
    return c != view_ref.get()[pos.idx];
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstStringView>::disj(const Block& b) const {
    return !b.baseContains(view_ref.get()[pos.idx]);
  }
  
  
}}

namespace Gecode { namespace String {
  
  template <>
  forceinline SweepIterator<SweepBwd,ConstStringView>::
  SweepIterator(const ConstStringView& x) 
  : view_ref(x), pos(Position(x.size(),0)) {};
  
  template <>
  forceinline SweepIterator<SweepBwd,ConstStringView>::
  SweepIterator(const ConstStringView& x, const Position& p) 
  : view_ref(x), pos(p) {
    if (!p.isNorm(x)) {
      if (p.off == 1) {
        pos.idx++;
        pos.off = 0;
      }
      else
        throw OutOfLimits("SweepIterator<StringView>::SweepIterator");
    }
    assert (pos.isNorm(view_ref.get()));
  };
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstStringView>::lb() const {
    return 1;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstStringView>::ub() const {
    return 1;
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConstStringView>::disj(const Block& b) const {
    return !b.baseContains(view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1]);
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConstStringView>::disj(int k) const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1] != k;
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConstStringView>::operator()(void) const {
    return view_ref.get().prec(Position(0,0), pos);
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConstStringView>::nextBlock() {
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
  SweepIterator<SweepBwd,ConstStringView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstStringView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? 1 : pos.off;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstStringView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? 1 : std::min(pos.off, 1);
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConstStringView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = 0;
    }
    else
      pos.off--;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ConstStringView>::consume");
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConstStringView>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = 0;
    }
    else
      pos.off--;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ConstStringView>::consumeMand");
    assert (isOK());
  }
}}

namespace Gecode { namespace String {
  template <>
  forceinline SweepIterator<SweepFwd,ConcatView>::
  SweepIterator(const ConcatView& x) 
  : SweepIterator<SweepFwd,ConcatView>(x, Position(0,0)) {};
 
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConcatView>::operator()(void) const {
    return view_ref.get().prec(pos, Position(view_ref.get().size(),0));
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConcatView>::nextBlock() {
    if (pos.idx >= view_ref.get().size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK() || view_ref.get()[pos.idx].isNull());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConcatView>::hasNextBlock(void) const {
    return pos.idx < view_ref.get().size();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConcatView>::must_consume() const {
    return std::max(0, lb() - pos.off);
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConcatView>::may_consume() const {
    return ub() - pos.off;
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConcatView>::consume(int k) {
    if (k == 0)
      return;
    pos.off += k;
    if (pos.off >= view_ref.get()[pos.idx].ub()) {
      if (pos.off > view_ref.get()[pos.idx].ub())
        throw OutOfLimits("T::SweepBwdIterator::consume");
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConcatView>::consumeMand(int k) {
    if (k == 0)
      return;
    pos.off += k; 
    if (pos.off > view_ref.get()[pos.idx].lb())
      throw OutOfLimits("T::SweepBwdIterator::consume");
    if (pos.off == view_ref.get()[pos.idx].ub()) {
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConcatView>::disj(const Block& b) const {
    return view_ref.get()[pos.idx].baseDisjoint(b);
  }
  
  template<>
  forceinline bool
  SweepIterator<SweepFwd,ConcatView>::disj(int k) const {
    return !view_ref.get()[pos.idx].baseContains(k);
  }
  
}}

namespace Gecode { namespace String {
  
  template <>
  forceinline SweepIterator<SweepBwd,ConcatView>::
  SweepIterator(const ConcatView& x) : SweepIterator<SweepBwd,ConcatView>
  ::SweepIterator(x, Position(x.size(), 0)) {};
 
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConcatView>::lb() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConcatView>::ub() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConcatView>::disj(const Block& b) const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConcatView>::disj(int k) const {
    return !view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(k);
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConcatView>::operator()(void) const {
    return view_ref.get().prec(Position(0,0), pos);
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConcatView>::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (isOK() || view_ref.get()[pos.idx].isNull());
  };
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConcatView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConcatView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].ub() : pos.off;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConcatView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].lb() 
                                         : std::min(pos.off, view_ref.get()[pos.idx].lb());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConcatView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].ub() - k;
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ConcatView>::consume");
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConcatView>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, view_ref.get()[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ConcatView>::consumeMand");
    assert (isOK());
  }
  
}}

namespace Gecode { namespace String {

  template <>
  forceinline SweepIterator<SweepFwd,ReverseView>::
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
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ReverseView>::isOK() const {
    return pos.isNorm(base_ref.get());
  }

  template <>
  forceinline SweepIterator<SweepFwd,ReverseView>::
  SweepIterator(const ReverseView& x) 
  : SweepIterator(x.baseView(), Position(x.size(), 0)) {};
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ReverseView>::lb() const {
    return base_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ReverseView>::ub() const {
    return base_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ReverseView>::disj(const Block& b) const {
    return  base_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ReverseView>::disj(int c) const {
    return !(base_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(c));
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ReverseView>::operator()(void) const {
    return base_ref.get().prec(Position(0,0), pos);
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
    assert (isOK() || base_ref.get()[pos.idx].isNull());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ReverseView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ReverseView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? base_ref.get()[pos.idx-1].lb() 
          : std::min(pos.off, base_ref.get()[pos.idx].lb());
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ReverseView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? base_ref.get()[pos.idx-1].ub() : pos.off;
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ReverseView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = base_ref.get()[pos.idx].ub() - k;
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
      pos.off = base_ref.get()[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, base_ref.get()[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ReverseView>::consumeMand");
    assert (isOK());
  }

}}

namespace Gecode { namespace String {

  template <>
  forceinline SweepIterator<SweepBwd,ReverseView>::
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
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ReverseView>::isOK() const {
    return pos.isNorm(base_ref.get());
  }

  template <>
  forceinline SweepIterator<SweepBwd,ReverseView>::
  SweepIterator(const ReverseView& x) 
  : SweepIterator(x.baseView(), Position(0,0)) {};
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ReverseView>::lb() const {
    return base_ref.get()[pos.idx].lb();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ReverseView>::ub() const {
    return base_ref.get()[pos.idx].ub();
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ReverseView>::operator()(void) const {
    return base_ref.get().prec(pos, Position(base_ref.get().size(),0));
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ReverseView>::nextBlock() {
    if (pos.idx >= base_ref.get().size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK() || base_ref.get()[pos.idx].isNull());
  };
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ReverseView>::hasNextBlock(void) const {
    return pos.idx < base_ref.get().size();
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
  SweepIterator<SweepBwd,ReverseView>::consumeMand(int k) {
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
  SweepIterator<SweepBwd,ReverseView>::disj(const Block& b) const {
    return base_ref.get()[pos.idx].baseDisjoint(b);
  }  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ReverseView>::disj(int c) const {
    return !base_ref.get()[pos.idx].baseContains(c);
  }

}}

namespace Gecode { namespace String {

  template <>
  forceinline SweepIterator<SweepFwd,ConstDashedView>::
  SweepIterator(const ConstDashedView& x) : view_ref(x), pos(Position(0,0)) {};
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstDashedView>::operator()(void) const {
    return view_ref.get().prec(pos, Position(view_ref.get().size(),0));
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConstDashedView>::nextBlock() {
    if (pos.idx >= view_ref.get().size())
      return;
    pos.idx++;
    pos.off = 0;
    assert (isOK() || view_ref.get()[pos.idx].isNull());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstDashedView>::hasNextBlock(void) const {
    return pos.idx < view_ref.get().size();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConstDashedView>::must_consume() const {
    return std::max(0, lb() - pos.off);
  }
  
  template <>
  forceinline int
  SweepIterator<SweepFwd,ConstDashedView>::may_consume() const {
    return ub() - pos.off;
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConstDashedView>::consume(int k) {
    if (k == 0)
      return;
    pos.off += k;
    if (pos.off >= view_ref.get()[pos.idx].ub()) {
      if (pos.off > view_ref.get()[pos.idx].ub())
        throw OutOfLimits("ConstDashedView::SweepBwdIterator::consume");
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepFwd,ConstDashedView>::consumeMand(int k) {
    if (k == 0)
      return;
    pos.off += k; 
    if (pos.off > view_ref.get()[pos.idx].lb())
      throw OutOfLimits("ConstDashedView::SweepBwdIterator::consume");
    if (pos.off == view_ref.get()[pos.idx].ub()) {
      pos.idx++;
      pos.off = 0;
    }
    assert (isOK());
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstDashedView>::disj(const Block& b) const {
    return view_ref.get()[pos.idx].baseDisjoint(b);
  }  
  template <>
  forceinline bool
  SweepIterator<SweepFwd,ConstDashedView>::disj(int c) const {
    return !view_ref.get()[pos.idx].baseContains(c);
  }

}}

namespace Gecode { namespace String {
  
  template <>
  forceinline SweepIterator<SweepBwd,ConstDashedView>::
  SweepIterator(const ConstDashedView& x) : view_ref(x), pos(Position(x.size(),0)) {};

  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstDashedView>::lb() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].lb();
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstDashedView>::ub() const {
    return view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].ub();
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConstDashedView>::disj(const Block& b) const {
    return  view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseDisjoint(b);
  }
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConstDashedView>::disj(int c) const {
    return !(view_ref.get()[pos.off > 0 ? pos.idx : pos.idx-1].baseContains(c));
  }
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConstDashedView>::operator()(void) const {
    return view_ref.get().prec(Position(0,0), pos);
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConstDashedView>::nextBlock() {
    if (pos.idx == 0 && pos.off == 0)
      return;
    if (pos.off > 0)
      pos.off = 0;
    else
      pos.idx--;
    assert (isOK() || view_ref.get()[pos.idx].isNull());
  };
  
  template <>
  forceinline bool
  SweepIterator<SweepBwd,ConstDashedView>::hasNextBlock(void) const {
    return pos.idx > 0 || pos.off > 0;
  };
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstDashedView>::may_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].ub() : pos.off;
  }
  
  template <>
  forceinline int
  SweepIterator<SweepBwd,ConstDashedView>::must_consume() const {
    return (pos.idx > 0 && pos.off == 0) ? view_ref.get()[pos.idx-1].lb() : 
            std::min(pos.off,view_ref.get()[pos.idx].lb());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConstDashedView>::consume(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].ub() - k;
    }
    else
      pos.off -= k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ConstDashedView>::consume");
    assert (isOK());
  }
  
  template <>
  forceinline void
  SweepIterator<SweepBwd,ConstDashedView>::consumeMand(int k) {
    if (k == 0)
      return;
    if (pos.off == 0 && pos.idx > 0) {
      pos.idx--;
      pos.off = view_ref.get()[pos.idx].lb() - k;
    }
    else
      pos.off = std::min(pos.off, view_ref.get()[pos.idx].lb()) - k;
    if (pos.off < 0)
      throw OutOfLimits("SweepIterator<SweepBwd,ConstDashedView>::consumeMand");
    assert (isOK());
  }

}}

