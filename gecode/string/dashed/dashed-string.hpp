#include <gecode/string/dashed/ns-dashed-string.hpp>

namespace Gecode { namespace String {

  forceinline double min(double a, double b) { return std::min<double>(a, b); }
  forceinline double max(double a, double b) { return std::max<double>(a, b); }
  forceinline unsigned char2int(unsigned char c) { return (unsigned) c; }
  forceinline unsigned char int2char(unsigned i) { return (unsigned char) i; }
  forceinline int divl(int a, int b) { return b == 0 ? 0 : a / b; }
  forceinline int divu(int a, int b) { return b == 0 ?
    String::DashedString::_MAX_STR_LENGTH : ceil(a / b);
  }

}}

namespace Gecode { namespace String {

  forceinline IntSet
  DSIntSet::toIntSet() const {
    if (empty())
      return IntSet();
    int n = 0;
    for (RangeList* c = fst(); c != NULL; c = c->next())
      n++;
    int a[n][2], i = 0;
    for (RangeList* c = fst(); c != NULL; c = c->next()) {
      a[i][0] = c->min();
      a[i][1] = c->max();
      i++;
    }
    IntSet is(a, n);
    return is;
  }

  forceinline void
  DSIntSet::init(Space& h, const DSIntSet& d) {
	  // std::cerr << "DSIntSet::init " << *this << " with " << d << "\n";
    _size = d.size();
    if (_size == 0) {
      fst(NULL);
      lst(NULL);
      return;
    }
    int n=0;
    for (RangeList* c = d.fst(); c != NULL; c = c->next())
      n++;
    RangeList* r = h.alloc<RangeList>(n);
    fst(r); lst(r+n-1);
    {
      RangeList* c = d.fst();
      for (int i = 0; i < n; i++) {
        r[i].min(c->min());
        r[i].max(c->max());
        r[i].next(&r[i + 1]);
        c = c->next();
      }
    }
    r[n-1].next(NULL);
  }

  forceinline void
  DSIntSet::init(Space& h, const NSIntSet& that) {
	//std::cerr << "NSIntSet::init " << *this << " with " << s << "\n";
    _size = that.size();
    if (_size == 0) {
      fst(NULL);
      lst(NULL);
      return;
    }
    int n = that.length();
    RangeList* r = h.alloc<RangeList>(n);
    fst(r); lst(r+n-1);
    {
      NSRange* c = that.first();
      for (int i = 0; i < n; i++) {
        r[i].min(c->l);
        r[i].max(c->u);
        r[i].next(&r[i + 1]);
        c = c->next;
      }
    }
    r[n-1].next(NULL);
  }

  forceinline void
  DSIntSet::update(Space& h, const NSIntSet& that) {
    //std::cerr << "DSIntSet::update " << *this << " with NS " << that << '\n';
    if (that.empty()) {
      dispose(h);
      fst(NULL);
      lst(NULL);
      _size = 0;
      return;
    }
    if (empty()) {
      init(h, that);
      return;
    }
    RangeList* p = fst();
    NSRange* q = that.first();
    RangeList* r = NULL;
    for (; p && q; p = p->next(), q = q->next) {
      p->min(q->l);
      p->max(q->u);
      r = p;
    }
    if (p)
      p->dispose(h, lst());
    else
      for (; q; q = q->next) {
        RangeList* s = new (h) RangeList(q->l, q->u);
        r->next(s);
        r = s;
      }
    r->next(NULL);
    lst(r);
    _size = that.size();
    //std::cerr << "DSIntSet::updated " << *this << ' ' << "\n";
  }
  
  forceinline void
  DSIntSet::update(Space& h, const DSIntSet& that) {
	//std::cerr << "DSIntSet::update " << *this << " with DS " << that <<'\n';
    BndSet::update(h, that);
    //std::cerr << "DSIntSet::updated " << *this << "\n";
  }

  forceinline void
  DSIntSet::update(Space& h, char c) {
    int i = char2int(c);
    if (ranges() && ranges()->next() == NULL) {
      ranges()->min(i);
      ranges()->max(i);
      _size = 1;
      return;
    }
    //std::cerr << "DSIntSet::update " << *this << " with " << c << '\n';
    if (!empty())
      dispose(h);
    RangeList* s = new (h) RangeList(i, i);
    fst(s);
    lst(s);
    fst()->next(NULL);
    _size = 1;
  }

  forceinline bool
  DSIntSet::operator!=(const DSIntSet& that) const {
    return !(*this == that);
  }

  forceinline bool
  DSIntSet::operator==(const DSIntSet& that) const {
    if (size() != that.size())
      return false;
    Gecode::Set::BndSetRanges i1(*this);
    Gecode::Set::BndSetRanges i2(that);
    while (i1() && i2()) {
      if (i1.min() != i2.min() || i1.max() != i2.max())
        return false;
      ++i1;
      ++i2;
    }
    return !i1() && !i2();
  }

  forceinline bool
  DSIntSet::operator==(const NSIntSet& that) const {
    return that == *this;
  }

  forceinline bool
  DSIntSet::contains(const DSIntSet& that) const {
    if (that.empty())
      return true;
    if (_size < that.size())
      return false;
    Gecode::Set::BndSetRanges i1(*this);
    Gecode::Set::BndSetRanges i2(that);
    while (i1() && i2())
      if (i1.min() > i2.min() || i1.max() < i2.max())
        ++i1;
      else
        ++i2;
    return !i2();
  }

  forceinline bool
  DSIntSet::contains(const NSIntSet& that) const {
    if (that.empty())
      return true;
    if ((int) _size < that.size())
      return false;
    Gecode::Set::BndSetRanges i1(*this);
    NSRange* i2 = that.first();
    while (i1() && i2)
      if (i1.min() > i2->l || i1.max() < i2->u)
        ++i1;
      else
        i2 = i2->next;
    return !i2;
  }

  forceinline bool
  DSIntSet::disjoint(const DSIntSet& that) const {
//  std::cerr << "DSIntSet::disjoint " << *this << ' ' << that << std::endl;
    if (empty() || that.empty()
    ||  max() < that.min() || min() > that.max())
      return true;
    Gecode::Set::BndSetRanges i1(*this);
    Gecode::Set::BndSetRanges i2(that);
    while (i1() && i2())
      if ((i1.min() <= i2.min() && i2.min() <= i1.max())
      ||  (i1.min() <= i2.max() && i2.max() <= i1.max())
      ||  (i2.min() <= i1.min() && i1.min() <= i2.max())
      ||  (i2.min() <= i1.max() && i1.max() <= i2.max()))
        return false;
      else
        if (i1.max() > i2.max())
          ++i2;
        else
          ++i1;
    return true;
  }

  forceinline bool
  DSIntSet::disjoint(const NSIntSet& that) const {
    if (empty() || that.empty()
    ||  max() < that.min() || min() > that.max())
      return true;
    Gecode::Set::BndSetRanges i1(*this);
    NSRange* i2 = that.first();
    while (i1() && i2)
      if ((i1.min() <= i2->l && i2->l <= i1.max())
      ||  (i1.min() <= i2->u && i2->u <= i1.max())
      ||  (i2->l <= i1.min() && i1.min() <= i2->u)
      ||  (i2->l <= i1.max() && i1.max() <= i2->u))
        return false;
      else
        if (i1.max() > i2->u)
          i2 = i2->next;
        else
          ++i1;
    return true;
  }

  forceinline void
  DSIntSet::remove(Space& h, int x) {
    if (empty() || x < min() || x > max())
      return;
    RangeList* prev = NULL;
    for (RangeList* p =fst(); p != NULL; p = p->next()) {
       if (p->min() == x) {
         _size--;
         if (p->max() == x) {
           if (prev) {
             prev->next(p->next());
             if (lst() == p)
               lst(prev);
           }
           else if (p->next())
             fst(p->next());
           else {
             fst(NULL);
             lst(NULL);
           }
           delete p;
         }
         else
           p->min(p->min() + 1);
         return;
       }
       else if (p->max() == x) {
         _size--;
         p->max(p->max() - 1);
       }
       else if (p->min() < x && x < p->max()) {
         _size--;
         RangeList* q = new (h) RangeList(x + 1, p->max(), p->next());
         p->max(x - 1);
         p->next(q);
         if (lst() == p)
           lst(q);
         return;
       }
       prev = p;
     }
  }

  forceinline void
  DSIntSet::intersect(Space& h, const DSIntSet& that) {
    Gecode::Set::BndSetRanges x(that);
    intersectI(h, x);
  }

  forceinline void
  DSIntSet::intersect(Space& h, const NSIntSet& that) {
    if (empty())
      return;
    if (that.empty() || min() > that.max() || that.min() > max()) {
      dispose(h);
      fst(NULL);
      lst(NULL);
      _size = 0;
      return;
    }
    if (fst() == lst() && that.length() == 1) {
      RangeList* p = fst();
      int l = std::max(p->min(), that.min());
      int u = std::min(p->max(), that.max());
      p->min(l);
      p->max(u);
      _size = u - l + 1;
      return;
    }
    Gecode::Set::BndSetRanges i1(*this);
    NSRange* i2 = that.first();
    RangeList* first = NULL;
    RangeList* last = NULL;
    int size = 0;
    while (i1() && i2) {
      if (i1.min() > i2->u)
        i2 = i2->next;
      else if (i1.max() < i2->l)
        ++i1;
      else {
        int l = std::max(i1.min(), i2->l);
        int u = std::min(i1.max(), i2->u);
        size += u - l + 1;
        RangeList* p = new (h) RangeList(l, u);
        if (first)
          last->next(p);
        else
          first = p;
        last = p;
        if (i1.max() > i2->u)
          i2 = i2->next;
        else
          ++i1;
      }
    }
    dispose(h);
    if (last)
      last->next(NULL);
    fst(first);
    lst(last);
    _size = size;
  }
  
  forceinline std::ostream&
  operator<<(std::ostream& os, const DSIntSet& s) {
    Gecode::Set::BndSetRanges i(s);
    os << "{";
    while (i()) {
      char mi = int2char(i.min());
      char ma = int2char(i.max());
      if (mi == ma)
        os << mi;
      else
        os << mi << ".." << ma;
      ++i;
      if (i())
        os << ",";
    }
    return os << "}"; //[" << s._size << ']';
  }

}}


namespace Gecode { namespace String {

  forceinline DSBlock
  DSBlock::top(Space& h) {
    return DSBlock(h, DSIntSet(h, 0, DashedString::_MAX_STR_ALPHA), 0,
      DashedString::_MAX_STR_LENGTH);
  }

  forceinline bool
  DSBlock::isTop() const {
    return l == 0 && u == DashedString::_MAX_STR_LENGTH &&
           S.size() == (unsigned) DashedString::_MAX_STR_ALPHA + 1;
  }

  forceinline void
  DSBlock::init(Space& h, const DSBlock& b) {
    l = b.l;
    u = b.u;
    S.init(h, b.S);
  }

  forceinline void
  DSBlock::init(Space& h, const NSBlock& b) {
    l = b.l;
    u = b.u;
    S.init(h, b.S);
  }

  forceinline void
  DSBlock::set_null(Space& h) {
    S.dispose(h);
    l = u = 0;
  }

  forceinline int
  DSBlock::min() const {
    return S.min();
  }

  forceinline int
  DSBlock::max() const {
    return S.max();
  }

  forceinline bool
  DSBlock::known() const {
    return null() || (l == u && S.size() <= 1);
  }

  forceinline double
  DSBlock::logdim() const {
    if (null())
      return 0;
    if (S.size() == 1)
      return log(u - l + 1);
    else {
      long double s = S.size();
      return u * log(s) + log(1.0 - std::pow(s, l - u - 1)) - log(1 - 1 / s);
    }
  }

  forceinline bool
  DSBlock::contains(const DSBlock& b) const {
    return l <= b.l && u >= b.u && S.contains(b.S);
  }


  forceinline bool
  DSBlock::null() const {
    return l > u || u == 0 || S.empty();
  }

  forceinline void
  DSBlock::update(Space& h, const DSBlock& that) {
    //std::cerr << "DSBlock::update " << *this << " <= " << that << '\n';
    if (this != &that) {
      if (S.empty()) {
        init(h, that);
        return;
      }
      S.update(h, that.S);
      l = that.l;
      u = that.u;
    }
  }

  forceinline void
  DSBlock::update(Space& h, const NSBlock& that) {
    //std::cerr << "DSBlock::update " << *this << " <= " << that << '\n';
    if (S.empty()) {
      init(h, that);
      return;
    }
    S.update(h, that.S);
    l = that.l;
    u = that.u;
  }

  template <typename Block>
  forceinline bool
  DSBlock::operator==(const Block& that) const {
    return l == that.l && u == that.u && S == that.S;
  }

  forceinline std::ostream&
  operator<<(std::ostream& os, const DSBlock& b) {
    if (b.null())
      return os << "";
    if (b.known())
      return os << "\"" <<  string(b.l, b.S.min()) << "\"";
    else
      return os << b.S << "^(" << b.l << ", " << b.u << ")";
  }

}}

namespace Gecode { namespace String {

  forceinline int
  DSBlocks::length() const {
    return _size;
  }

  forceinline void
  DSBlocks::size(int i) {
    _size = i;
  }

  forceinline bool
  DSBlocks::null() const {
    return at(0).null();
  }

  forceinline double
  DSBlocks::logdim() const {
    double d = 0.0;
    for (int i = 0; i < _size; ++i) {
      const DSBlock& b = at(i);
      d += b.logdim();
      if (std::isinf(d))
        return d;
    }
    return d;
  }

  forceinline DSBlock&
  DSBlocks::at(int i) {
    return *(x + i);
  }
  forceinline const DSBlock&
  DSBlocks::at(int i) const {
    return *(x + i);
  }

  forceinline const DSBlock&
  DSBlocks::front() const {
    return at(0);
  }
  forceinline DSBlock&
  DSBlocks::front() {
    return at(0);
  }

  forceinline DSBlock&
  DSBlocks::back() {
    return at(_size - 1);
  }
  forceinline const DSBlock&
  DSBlocks::back() const {
    return at(_size - 1);
  }

  forceinline NSIntSet
  DSBlocks::may_chars() const {
    NSIntSet set;
    for (int i = 0; i < _size; ++i)
      set.include(at(i).S);
    return set;
  }

  forceinline void
  DSBlocks::init(Space& h, const DSBlocks& d) {
    _size = d._size;
    for (int i = 0; i < _size; ++i)
      x[i].init(h, d.at(i));
  }

  forceinline void
  DSBlocks::init(Space& h, const NSBlocks& v) {
    _size = v.size();
    for (int i = 0; i < _size; ++i) {
      x[i].init(h, v[i]);
    }
  }

  forceinline void
  DSBlocks::update(Space& h, const DSBlocks& d) {
    // std::cerr<<"DSArray::update DSBlocks"<<std::endl;
    if (*this == d)
      return;
    int m = d._size;
    if (m <= _size) {
      for (int i = 0; i < m; ++i)
        x[i].update(h, d.at(i));
      if (m < _size) {
        for (int i = m; i < _size; ++i)
          x[i].S.dispose(h);
        a.free(x + m, _size - m);
      }
      n = m;
      _size = m;
      // std::cerr<<"Updated: "<<*this<<std::endl;
      return;
    }
    for (int i = 0; i < _size; ++i)
      x[i].S.dispose(h);
    a.free(x, n);
    x = a.template alloc<DSBlock>(m);
    for (int i = 0; i < m; ++i)
      x[i].init(h, d.at(i));
    n = m;
    _size = m;
    // std::cerr<<"Updated: "<<*this<<std::endl;
  }
  
  forceinline void
  DSBlocks::update(Space& h, const NSBlocks& v, int& l, int& u) {
    // std::cerr<<"DSArray::update NSBlocks"<<std::endl;
    int m = v.size();
    long ll = 0, uu = 0;
    if (m <= _size) {
      for (int i = 0; i < m; ++i) {
        x[i].update(h, v[i]);
        ll += x[i].l;
        uu += x[i].u;
      }
      if (m < _size) {
        for (int i = m; i < _size; ++i)
          x[i].S.dispose(h);
        a.free(x + m, _size - m);
      }
      n = m;
      _size = m;
      if (ll > DashedString::_MAX_STR_LENGTH)
        h.fail();
      if (uu > DashedString::_MAX_STR_LENGTH)
        uu = DashedString::_MAX_STR_LENGTH;
      l = (int) ll;
      u = (int) uu;
      // std::cerr<<"Updated: "<<*this<<std::endl;
      return;
    }
    for (int i = 0; i < _size; ++i)
      x[i].S.dispose(h);
    a.free(x, n);
    x = a.template alloc<DSBlock>(m);
    for (int i = 0; i < m; ++i) {
      x[i].init(h, v[i]);
      ll += x[i].l;
      uu += x[i].u;
    }
    n = m;
    _size = m;
    if (ll > DashedString::_MAX_STR_LENGTH)
      h.fail();
    if (uu > DashedString::_MAX_STR_LENGTH)
      uu = DashedString::_MAX_STR_LENGTH;
    l = (int) ll;
    u = (int) uu;
    // std::cerr<<"Updated: "<<*this<<std::endl;
  }
  
  forceinline void
  DSBlocks::update(Space& h, const string& s) {
    // std::cerr<<"DSArray::update " << *this << " with " << s <<std::endl;
    if (s.empty()) {
      for (int i = 0; i < _size; ++i)
        x[i].S.dispose(h);
      a.free(x, n);
      x = a.template alloc<DSBlock>(1);
      n = 1;
      _size = 1;
      return;
    }
    int m = 0, j = 0, ns = s.size();
    unsigned prev_c = s[0] + 1;
    for (int i = 0; i < ns; ++i) {
      unsigned s_i = char2int(s[i]);
      if (s_i != prev_c) {
        m++;
        prev_c = s_i;
      }
    }
    prev_c = s[0] + 1;
    if (m <= _size) {
      for (int i = 0; i < ns; ++i) {
        unsigned s_i = char2int(s[i]);
        if (s_i == prev_c) {
          x[j - 1].l++;
          x[j - 1].u++;
        }
        else {
          prev_c = s_i;
          x[j].S.update(h, prev_c);
          x[j].l = 1;
          x[j].u = 1;
          ++j;
          // std::cerr << "x_j: " << x[j] << '\n';
        }
      }
      for (int i = m; i < _size; ++i)
        x[i].S.dispose(h);
      a.free(x + m, _size - m);
      n = m;
      _size = m;
      return;
    }
    for (int i = 0; i < _size; ++i)
      x[i].S.dispose(h);
    a.free(x, n);
    x = a.template alloc<DSBlock>(m);
    prev_c = s[0] + 1;
    for (int i = 0; i < ns; ++i) {
      unsigned s_i = char2int(s[i]);
      if (s_i == prev_c) {
        x[j - 1].l++;
        x[j - 1].u++;
      }
      else {        
        prev_c = s_i;
        x[j].S.init(h, prev_c);
        x[j].l = 1;
        x[j].u = 1;
        ++j;
      }
    }
    n = m;
    _size = m;
  }

  forceinline void
  DSBlocks::insert(Space& h, int k, const DSBlock& b) {
    // std::cerr << "Inserting " << b << " at position " << k << " of " << *this << '\n';
    DSBlock* y = h.alloc<DSBlock>(n + 1);
    for (int i = 0; i < k; i++)
      y[i].update(h, x[i]);
    y[k].update(h, b);
    for (int i = k; i < n; i++)
      y[i + 1].update(h, x[i]);
    for (int i = 0; i < _size; ++i)
      x[i].S.dispose(h);
    a.free(x, n);
    x = y;
    ++n;
    _size = n;
    // std::cerr << "Inserted: " << *this << "\n";
  }
  
  forceinline void
  DSBlocks::become(Space& h, DSBlock* y, int m) {
    for (int i = 0; i < _size; ++i)
      x[i].S.dispose(h);
    a.free(x, n);
    x = y;
    n = m;
    _size = m;
  }

  forceinline void
  DSBlocks::set_null(Space& h) {
    if (_size > 1) {
      for (int i = 0; i < _size; ++i)
        x[i].S.dispose(h);
      a.free(x, n);
      x = a.alloc<DSBlock>(1);
      n = 1;
      _size = 1;
    }
    else
      x->S.excludeAll(h);
  }

  forceinline void
  DSBlocks::normalize(Space& h, int& l, int& u) {
    // std::cerr<<"normalize "<<*this<<std::endl;
    int m = _size;
    long ll = 0, uu = 0, M = DashedString::_MAX_STR_LENGTH;
    for (int i = 0; i < _size; ) {
      if (x[i].null()) {
        --m;
        ++i;
        continue;
      }
      ll += x[i].l;
      uu += x[i].u;
      int j = i + 1;  
      while (j < _size && (x[j].null() || x[i].S == x[j].S)) {
        if (!x[j].null()) {
          ll += x[j].l;
          uu += x[j].u;
          x[i].l += x[j].l;
          x[i].u = std::min(x[i].u + (long) x[j].u, M);
          x[j].set_null(h);
        }
        --m;
        ++j;
      }
      i = j;
    }    
    // std::cerr << "First pass: " << *this <<' '<<m<<' '<<ll<<' '<<uu<< "\n";
    if (m == 0) {
      a.free(x, n);
      n = 1;
      _size = 1;      
      x = h.alloc<DSBlock>(1);
      l = u = 0;
      return;
    }
    if (ll > M)
      h.fail();
    if (uu > M)
      uu = M;
    assert (ll <= uu && uu > 0);
    if (m < _size) {
      for (int i = 0; i < _size; ++i) {
        if (x[i].null()) {
          int j = i + 1;
          while (j < _size && x[j].null())
            j++;
          if (j == _size)
            break;
          if (i > 0 && x[i - 1].S == x[j].S) {
            x[i - 1].l += x[j].l;
            x[i - 1].u += x[j].u;
            --m;
          }
          else
            x[i].init(h, x[j]);
          x[j].set_null(h);
        }
      }
      a.free(x + m, _size - m);
      n = m;
      _size = m;
    }
    l = ll;
    u = uu;      
    if (_size == 1) {
      if (at(0).l < l)
        at(0).l = l;
      if (at(0).u > u)
        at(0).u = u;
    }
    // std::cerr<< "normalized: " << *this << ' ' << l << ' ' << u << "\n";
  }

  template <typename Blocks>
  forceinline bool
  DSBlocks::operator==(const Blocks& that) const {
    if (length() != that.length())
      return false;
    for (int i = 0; i < length(); ++i)
      if (!(at(i) == that.at(i)))
        return false;
    return true;
  }

  forceinline std::ostream&
  operator<<(std::ostream& os, const DSBlocks& s) {
    if (s._size == 0)
      return os << "[]";
    os << "[";
    for (int i = 0; i < s._size - 1; ++i)
      os << s.x[i] << ", ";
    return os << s.x[s._size - 1] << "]";
  }

}}

#include <gecode/string/dashed/sweep-equate.hpp>
#include <gecode/string/dashed/aux-find.hpp>
#include <gecode/string/dashed/aux-lex.hpp>

namespace Gecode { namespace String {

  // Use dynamic programming to check the completeness of sweep (debug-only).
  static bool
  deep_check_dp(
    const NSBlocks& x, int i, const NSBlock& x_i, 
    const NSBlocks& y, int j, const NSBlock& y_j, set6 nogoods
  ) {
    // std::cerr << "\tdeep_check_dp: "<<x<<' '<<i<<' '<<x_i<<"  "<<y<<' '<<j<<' '<<y_j<<' '<<'\n';
    int l_i = lower(x_i), u_i = upper(x_i), l_j = lower(y_j), u_j = upper(y_j);
    tpl6 ng(i, l_i, u_i, j, l_j, u_j);
    if (nogoods.find(ng) != nogoods.end())
      return false;
    int lx = x.length(), ly = y.length();
    if (i == lx) {
      if (l_j > 0)
        return false;
      for (int k = j + 1; k < ly; ++k) {
        if (lower(y.at(k)) > 0) {
          nogoods.insert(ng);
          return false;
        }
      }
      throw true;
    }
    if (j == ly) {
      if (l_i > 0)
        return false;
      for (int k = i + 1; k < lx; ++k)
        if (lower(x.at(k)) > 0) {
          nogoods.insert(ng);
          return false;
        }
      throw true;
    }
    bool ndisj = !disjoint(x_i, y_j);
    if (!ndisj && l_i > 0 && l_j > 0) {
      nogoods.insert(ng);
      return false;
    }
    bool c_i, c_j;
    if (l_i == 0 || (ndisj && l_i <= u_j)) {
      const NSBlock& r_i = i + 1 < lx ? x.at(i + 1) : x.at(i);
      NSBlock r_j = NSBlock(y_j);
      if (ndisj) {
        r_j.l = max(0, l_j - u_i);
        r_j.u = u_j - l_i;
      }
      c_i = deep_check_dp(x, i + 1, r_i, y, j, r_j, nogoods);
    }
    else
      c_i = false;
    if (l_j == 0 || (ndisj && l_j <= u_i)) {      
      NSBlock r_i = NSBlock(x_i);
      if (ndisj) {
        r_i.l = max(0, l_i - u_j);
        r_i.u = u_i - l_j;
      }
      const NSBlock& r_j = j + 1 < ly ? y.at(j + 1) : y.at(j);    
      c_j = deep_check_dp(x, i, r_i, y, j + 1, r_j, nogoods);
    }
    else
      c_j = false;
    if (!(c_i || c_j)) {
      nogoods.insert(ng);
      return false;
    }
    throw true;
  }
  template <class Block1, class Blocks1, class Block2, class Blocks2>
  static bool
  deep_check(const Blocks1& x, const Blocks2& y) {
    // std::cerr << "deep_check " << x << " " << y << "\n";
    set6 ng;
    int lx = x.length(), ly = y.length();
    if (lx == 0 || ly == 0)
      return lx == ly;
    bool ok = false;
    NSBlocks xx(x), yy(y);
    try {
      ok = deep_check_dp(xx, 0, xx[0], yy, 0, yy[0], ng);
    }
    catch (bool b) {
      ok = b;
    }
    return ok;
  }

  forceinline DSBlocks&
  DashedString::blocks() {
    return _blocks;
  }

  forceinline const DSBlocks&
  DashedString::blocks() const {
    return _blocks;
  }

  forceinline int
  DashedString::first_na_block() const {
    for (int i = 0; i < _blocks.length(); ++i)
      if (!at(i).known())
        return i;
    return -1;
  }

  forceinline int
  DashedString::smallest_na_block() const {
    int j = -1;
    double d = std::numeric_limits<double>::infinity();
    for (int i = 0; i < _blocks.length(); ++i) {
      const DSBlock& bi = at(i);
      if (!bi.known() && bi.logdim() < d) {
        j = i;
        d = bi.logdim();
      }
    }
    return j;
  }

  forceinline void
  DashedString::_commit0(
    Space& h, Branch::Level lev, Branch::Value val, int idx
  ) {
    DSBlock& block = at(idx);
    switch (lev) {
      case Branch::LENGTH: {
        // std::cerr << "Branch::LENGTH\n";
        switch (val) {
          case Branch::MIN:
            refine_ub(h, _min_length);
            return;
          case Branch::MAX:
            refine_lb(_max_length);
            return;
          default:
            GECODE_NEVER;
        }
      }
      case Branch::CARD: {
        // std::cerr << "Branch::CARD\n";
        switch (val) {
          case Branch::MIN:
            block.u = block.l;
            refine_card(h, _min_length, _max_length);
            return;
          case Branch::MAX:
            block.l = block.u;
            refine_card(h, _min_length, _max_length);
            return;
          default:
            GECODE_NEVER;
        }
      }
      case Branch::BASE: {
        // std::cerr << "Branch::BASE\n";
        int l = block.l;
        const DSIntSet& S = block.S;
        DSIntSet s;
        switch (val) {
          case Branch::MIN:
            s.init(h, S.min());
            break;
          case Branch::MAX:
            s.init(h, S.max());
            break;
          case Branch::MUSTMIN:
            if (_MUST_CHARS.disjoint(S))
              s.init(h, S.min());
            else {
              NSIntSet t(_MUST_CHARS);
              t.intersect(S);
              s.init(h, t.min());
            }
            break;
          case Branch::MUSTMAX:
            if (_MUST_CHARS.disjoint(S))
              s.init(h, S.max());
            else {
              NSIntSet t(_MUST_CHARS);
              t.intersect(S);
              s.init(h, t.max());
            }
            break;
          default:
            GECODE_NEVER;
        }
        bool norm = (idx > 0 && at(idx - 1).S == s) ||
          (l == 1 && idx < length() - 1 && at(idx + 1).S == s);
        if (l == 1 && !norm) {
          at(idx).S.update(h, s);
          return;
        }
        _blocks.insert(h, idx, DSBlock(h, s, 1, 1));
        _blocks.at(idx + 1).l--;
        _blocks.at(idx + 1).u--;
        if (norm)
          normalize(h);
        return;
      }
      default:
        GECODE_NEVER;
    }
  }

  forceinline void
  DashedString::_commit1(
    Space& h, Branch::Level lev, Branch::Value val, int idx
  ) {
    DSBlock& block = at(idx);
    switch (lev) {
      case Branch::LENGTH: {
        if (_min_length == _max_length)
          h.fail();
        // std::cerr << "Branch::LENGTH\n";
        switch (val) {
          case Branch::MIN:
            refine_lb(_min_length + 1);
            return;
          case Branch::MAX:
            refine_ub(h, _max_length - 1);
            return;
          default:
            GECODE_NEVER;
        }
      }
      case Branch::CARD: {
        // std::cerr << "Branch::CARD\n";
        if (block.l == block.u)
          h.fail();
        switch (val) {
          case Branch::MIN:
             block.l++;
             refine_card(h, _min_length, _max_length);
             return;
          case Branch::MAX:
             block.u--;
             refine_card(h, _min_length, _max_length);
             return;
          default:
            GECODE_NEVER;
        }
      }
      case Branch::BASE: {
        // std::cerr << "Branch::BASE\n";
        DSIntSet S1(h, block.S);
        if (S1.size() == 1)
          h.fail();
        int l = block.l;
        Gecode::Set::SetDelta d;
        switch (val) {
          case Branch::MIN: {
            int m = S1.min();
            S1.exclude(h, m, m, d);
            break;
          }
          case Branch::MAX: {
            int m = S1.max();
            S1.exclude(h, m, m, d);
            break;
          }
          case Branch::MUSTMIN: {
            int m;
            if (_MUST_CHARS.disjoint(S1))
              m = S1.min();
            else {
              NSIntSet t(_MUST_CHARS);
              t.intersect(S1);
              m = t.min();
            }
            S1.exclude(h, m, m, d);
            break;
          }
          case Branch::MUSTMAX: {
            int m;
            if (_MUST_CHARS.disjoint(S1))
              m = S1.max();
            else {
              NSIntSet t(_MUST_CHARS);
              t.intersect(S1);
              m = t.max();
            }
            S1.exclude(h, m, m, d);
            break;
          }
          default:
            GECODE_NEVER;
        }
        bool norm = (idx > 0 && S1 == at(idx-1).S) ||
          (l == 1 && idx < length() - 1 && S1 == at(idx+1).S);
        if (l == 1 && !norm) {
          at(idx).S.update(h, S1);
          return;
        }
        _blocks.insert(h, idx, DSBlock(h, S1, 1, 1));
        _blocks.at(idx + 1).l--;
        _blocks.at(idx + 1).u--;
        if (norm)
          normalize(h);
        return;
      }
      default:
        GECODE_NEVER;
    }
  }

  forceinline void
  DashedString::commit(
    Space& h, Branch::Level l, Branch::Value v, Branch::Block b, unsigned a
  ) {
    // std::cerr<<"\nDashedString::commit "<< *this <<" ["<<_min_length<< ", "
    // << _max_length << "] -- " << l <<' '<< v << ' ' << b << ' '<< a <<'\n';
    int i;
    switch (b) {
      case Branch::FIRST:
        i = first_na_block();
        break;
      case Branch::SMALLEST:
        i = smallest_na_block();
        break;
      default:
        GECODE_NEVER;
    }
    a == 0 ? _commit0(h, l, v, i) : _commit1(h, l, v, i);
    _changed = true;
    // std::cerr<<"Set: "<<*this<<" ["<<_min_length<<", "<< _max_length <<"]\n";   
    assert (is_normalized());
  }
  
  forceinline bool
  DashedString::is_normalized() const {
    // std::cerr << *this << ' ' << _min_length << ' ' << _max_length << "\n";
    if (_min_length > _max_length)
      return false;
    int n = length();
    // std::cerr << at(0).S << ' ' << at(0).l << ' ' << at(0).u << '\n';
    if (n == 1 && (_min_length != at(0).l || _max_length != at(0).u))
      return false;
    for (int i = 1; i < n; ++i) {
      const DSBlock& b = at(i);
      if (b.null() || b.S == at(i - 1).S)
        return false;
    }
    const DSBlock& b = at(0);
    return b.null() ? n == 1 && b.l == 0 && b.u == 0 : b.l <= b.u;
  }

  forceinline bool
  DashedString::find(Space& h, DashedString& x, int& lb, int& ub, bool mod) {
    // std::cerr << "\nDashedString::find: " << x << " in " << *this <<
    // " from [" << lb << ", " << ub << "]\n";
    assert (0 <= lb && lb <= ub);
    _changed = x._changed = false;
    if (!sweep_find(h, x, *this, lb, ub, mod))
      return false;
    // std::cerr << "After find: " << x << " in " << *this << "\n";
    if (mod) {
      bool again;
      do {
        again = false;
        // l(x) <= l(y) - i + 1.
        if (x._max_length > _max_length - lb + 1) {
          x._max_length = _max_length - lb + 1;
          if (x._max_length < x._min_length)
            return false;
          x._changed = again = true;
        }
        // l(y) >= l(y) + i - 1.
        if (_min_length < x._min_length + lb - 1) {
          _min_length = x._min_length + lb - 1;
          if (_max_length < _min_length)
            return false;
          _changed = again = true;
        }
        // i <= l(y) - l(x) + 1.
        if (ub > _max_length - x._min_length + 1) {
          ub = _max_length - x._min_length + 1;
          if (ub < lb)
            return false;
          again = true;
        }
      } while (again);
    }
    if (_changed && !refine_card(h, _min_length, _max_length))
      return false;
    if (x._changed && !x.refine_card(h, x._min_length, x._max_length))
      return false;
    return true;
  }

  forceinline NSBlocks
  DashedString::pow(int n) const {
    if (n <= 0 || null())
      return NSBlocks();
    int nx = _blocks.length();
    NSBlocks y(n * nx);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < nx; ++j)
        y[i*nx + j] = NSBlock(at(j));
    return y;
  }
 
  forceinline bool
  DashedString::refine_card_eq(Space& h, DashedString& that) {
    int mil = max(_min_length, that._min_length),
        mal = min(_max_length, that._max_length);
    if (mil > _min_length) {
      if (!refine_lb(mil))
        return false;
    }
    else if (mil > that._min_length) {
      if (!that.refine_lb(mil))
        return false;
    }
    if (mal < _max_length) {
      if (!refine_ub(h, mal))
        return false;
    }
    else if (mal < that._max_length) {
      if (!that.refine_ub(h, mal))
        return false;
    }
    return true;
  }
 
 
  forceinline bool
  DashedString::refine_card_cat(Space& h, DashedString& x, DashedString& y) {
    long lx = x._min_length, ly = y._min_length, lz = _min_length,
         ux = x._max_length, uy = y._max_length, uz = _max_length;
    bool again;
    do {
      again = false;
      lx = max(lx, lz - uy); ly = max(ly, lz - ux), lz = max(lz, lx + ly);
      ux = min(ux, uz - ly); uy = min(uy, uz - lx), uz = min(uz, ux + uy);
      if (ux < lx || uy < ly || uz < lz)
        return false;
      if (lx > x._min_length) { x._min_length = lx; again = x._changed = true; }
      if (ux < x._max_length) { x._max_length = ux; again = x._changed = true; }
      if (ly > y._min_length) { y._min_length = ly; again = y._changed = true; }
      if (uy < y._max_length) { y._max_length = uy; again = y._changed = true; }
      if (lz > _min_length)   {   _min_length = lz; again =   _changed = true; }
      if (uz < _max_length)   {   _max_length = uz; again =   _changed = true; }
    } while (again);
    if ((x._changed && !x.refine_card(h, lx, ux)) ||
        (y._changed && !y.refine_card(h, ly, uy)) ||
        (  _changed &&   !refine_card(h, lz, uz)))
      return false;    
    return true;
  }
  
  forceinline bool
  DashedString::refine_card_pow(Space& h, DashedString& x, int n1, int n2) {
    long lx = x._min_length, ly = max(lx * n1, _min_length),
         ux = x._max_length, uy = min(ux * n2, _max_length);
    bool again;
    do {
      again = false;
      ly = max(ly, lx * n1), uy = min(uy, ux * n2),
      lx = max(lx, divl(ly, n2)), ux = min(ux, divu(uy, n1)),
      n1 = max(n1, divl(ly, ux)), n2 = min(n2, divu(uy, lx));
      if (n1 > n2 || lx > ux || ly > uy)
        return false;
      if (lx > x._min_length) { x._min_length = lx; again = x._changed = true; }
      if (ux < x._max_length) { x._max_length = ux; again = x._changed = true; }
      if (ly > _min_length)   {   _min_length = ly; again =   _changed = true; }
      if (uy < _max_length)   {   _max_length = uy; again =   _changed = true; }
    } while (again);
    if ((x._changed && !x.refine_card(h, lx, ux)) || 
        (  _changed &&   !refine_card(h, ly, uy)))
      return false;
    return true;
  }
  
  forceinline bool
  DashedString::concat(Space& h, DashedString& x, DashedString& y) {
    // std::cerr<<"DashedString::concat "<<*this<<" = "<<x<<" ++ "<<y<<std::endl;    
    _changed = x._changed = y._changed = false;
    if (null())
      return equate(h, x) && equate(h, y);
    if (x.null())
      return equate(h, y);    
    if (y.null())
      return equate(h, x);    
    if (known()) {
      string sz = val();
      int nz = sz.size();
      if (x.known()) {
        string sx = x.val();
        if (y.known())
          return sz == sx + y.val();
        int nx = sx.size();
        if (nz < nx || sz.substr(0, nx) != sx)
          return false;
        string suff = sz.substr(nx, nz - nx);
        if (!check_sweep<DSBlock, DSBlocks, char, string>(y._blocks, suff))
          return false;
        y.update(h, suff);
        y._changed = true;
        return true;
      }
      else if (y.known()) {
        string sy = y.val();
        int ny = sy.size();
        if (nz < ny || sz.substr(nz - ny, ny) != sy)
          return false;
        string pref = sz.substr(0, nz - ny);
        if (!check_sweep<DSBlock, DSBlocks, char, string>(x._blocks, pref))
          return false;
        x.update(h, pref);
        x._changed = true;
        return true;
      }
    }    
    if (x.known() && y.known()) {
      string xy = x.val() + y.val();
      int n = xy.size();
      if (n < _min_length || n > _max_length)
        return false;
      if (!check_sweep<DSBlock, DSBlocks, char, string>(_blocks, xy))
        return false;
      update(h, xy);
      _changed = true;
      return true;
    }
    if (x._min_length > 0 && y._min_length > 0 && contains(x, y)) {
      // std::cerr << *this << " contains " << x << "  ++  " << y << "\n";
      int norm = x.blocks().back().S == y.at(0).S ? 1 : 0;
      double d = logdim();
      NSBlocks xy(x.length() + y.length() - norm);
      for (int i = 0; i < x.length(); ++i)
        xy[i] = NSBlock(x.at(i));
      if (norm) {
        xy[x.length() - 1].l += y.at(0).l;
        xy[x.length() - 1].u += y.at(0).u;
      }
      for (int i = norm; i < y.length(); ++i)
        xy[i + x.length() - norm] = NSBlock(y.at(i));
      if (d != xy.logdim()) {
        update(h, xy);
        _changed = true;
      }
    }
    else {
      ConcatView xy(x, y);
      if (!sweep_concat(h, xy, x, y, *this))
        return false;
    }
    if (!refine_card_cat(h, x, y))
      return false;
    return true;
  }

  forceinline bool
  DashedString::increasing(Space& h, bool strict) {
    _changed = false;
    // Scanning forward.
    int c = -1;
    for (int i = 0; i < _blocks.length(); ++i) {
      DSBlock& b = _blocks.at(i);
      if (b.S.max() < c)
        return false;
      else if (b.S.max() == c) {
        if (strict) {
          if (b.l > 0)
            return false;
          else {
            _changed = true;
            b.u = 0;
          }
        }
        else if (b.S.size() > 1) {
          _changed = true;
          b.S.update(h, c);
        }
      }
      // b.S.max() > c.
      else {
        int m = b.S.min();
        if (m <= c) {
          _changed = true;
          Gecode::Set::SetDelta d;
          if (strict)
            b.S.exclude(h, m, c, d);
          else if (m != c)
            b.S.exclude(h, m, c - 1, d);
        }
      }
      if (b.l > 0 && !b.null()) {
        if (strict)
          c = b.S.minN(b.l - 1);
        else
          c = b.S.min();
      }
    }
    if (_changed)
      normalize(h);
    // Scanning backward.
    bool changed = false;
    c = _MAX_STR_ALPHA;
    for (int i = _blocks.length() - 1; i >= 0; --i) {
      DSBlock& b = _blocks.at(i);
      if (b.S.min() > c)
        return false;
      else if (b.S.min() == c) {
        if (strict) {
          if (b.l > 0)
            return false;
          else {
            changed = true;
            b.u = 0;
          }
        }
        else if (b.S.size() > 1) {
          changed = true;
          b.S.update(h, c);
        }
      }
      // b.S.min() < c.
      else {
        int m = b.S.max();
        if (m >= c) {
          changed = true;
          Gecode::Set::SetDelta d;
          if (strict)
            b.S.exclude(h, c, m, d);
          else if (m != c)
            b.S.exclude(h, c + 1, m, d);
        }
      }
      if (b.l > 0 && !b.null())
        c = b.S.max();
    }
    if (changed)
      normalize(h);
    return true;
  }

  forceinline NSBlocks
  DashedString::lex_least(const std::vector<float>& succ) const {
    NSBlocks v;
    for (int i = 0; i < _blocks.length(); ++i) {
      const NSBlock& b = lex_min(at(i), succ[i]);
      if (i && at(i - 1).S == b.S) {
        v.back().l += b.l;
        v.back().u += b.u;
      }
      else
        v.push_back(b);
    }
    assert (v.known());
    return v;
  }

  forceinline NSBlocks
  DashedString::lex_greatest(const std::vector<float>& succ) const {
    NSBlocks v;
    for (int i = 0; i < _blocks.length(); ++i) {
      const NSBlock& b = lex_max(at(i), succ[i]);
      if (i && v.back().S == b.S) {
        v.back().l += b.l;
        v.back().u += b.u;
      }
      else
        v.push_back(b);
    }
    assert (v.known());
    return v;
  }

  forceinline bool
  DashedString::lex(Space& h, DashedString& that, bool lt) {
    // lt ? std::cerr << *this << " < "  << that << "\n"
    //    : std::cerr << *this << " <= " << that << "\n";
    if (_min_length == 0)
      return (!lt || that.max_length() > 0);
    if (that.max_length() == 0)
      return false;
    _changed = that._changed = false;
    std::vector<float> y_succ = min_succ(that);
    // std::cerr<<"y_succ = ["; for (auto v:y_succ) std::cerr<<v<< ", "; std::cerr<<"]\n";
    NSBlocks y_max = that.lex_greatest(y_succ);
    if (known()) {
      string sx = val(), sy = y_max.val();
      if ((lt && sx >= sy) || (!lt && sx > sy))
        return false;
    }
    else {
      // Checking this <= max(that), by possibly refining this.
      Position pos{0, 0};
      std::vector<float> x_succ = min_succ(*this);
      bool sub = false;
      for (int j = 0; j < length(); ++j) {
        DSBlock& xj = at(j);
        if (!lex_step_xy(xj, x_succ[j], y_max, pos, sub))
          return false;
        if (sub) {
          if (pos.idx < 0) {
              xj.l--;
            xj.u--;
            if (xj.u == 0)
              _changed = true;
            else {
              NSIntSet s(x_succ[j], xj.S.max());
              s.intersect(xj.S);
              if (j > 1 && s == at(j - 1).S) {
                at(j).l++;
                at(j).u++;
              }
              else
                blocks().insert(h, j, DSBlock(h, DSIntSet(h, s), 1, 1));
            }
            break;
          }
          break;
        }
        else if (xj.l > 0 && !xj.known()) {
          _changed = true;
          xj.update(h, lex_min(xj, x_succ[j]));
        }
      }
      if (lt && !sub && length() >= that.length())
        return false;
      if (_changed)
        normalize(h);
      // std::cerr << "After lex: x = " << *this << " (" << sub << ")\n";
    }
    std::vector<float> x_succ = min_succ(*this);
    NSBlocks x_min = lex_least(x_succ);
    if (that.known()) {
      string sx = x_min.val(), sy = that.val();
      if ((lt && sx >= sy) || (!lt && sx > sy))
        return false;
    }
    else {
      // Checking that >= min(this), by possibly refining that.
      Position pos{0, 0};
      y_succ = max_succ(that);
      bool sub = false;
      for (int j = 0; j < that.length(); ++j) {
        DSBlock& yj = that.at(j);
        if (!lex_step_yx(yj, y_succ[j], x_min, pos, sub))
          return false;        
        if (sub) {
          if (pos.idx < 0) {
            if (yj.l > 0)
              yj.l--;
            yj.u--;
            if (yj.u == 0)
              that._changed = true;
            NSIntSet s(y_succ[j], yj.S.max());
            s.intersect(yj.S);
            if (j > 1 && s == that.at(j - 1).S) {
              that.at(j).l++;
              that.at(j).u++;
            }
            else
              that.blocks().insert(h, j, DSBlock(h, DSIntSet(h, s), 1, 1));
            if (that._changed)
              that.normalize(h);
            return true;
          }
          break;
        }
        else if (yj.l > 0 && !yj.known()) {
          yj.update(h, lex_max(yj, y_succ[j]));
          that._changed = true;
        }
      }
      if (lt && !sub && length() >= that.length())
        return false;
      if (that._changed)
        that.normalize(h);
      // std::cerr << "After lex: y = " << that << '\n';
    }
    return true;
  }

  forceinline bool
  DashedString::check_lex(const DashedString& that, bool lt) const {
    // std::cout << "check_lex " << *this << " " << that << "\n";
    if (_min_length == 0)
      return (!lt || that.max_length() > 0);
    if (that.max_length() == 0)
      return false;
    std::vector<float> y_succ = min_succ(that);
    NSBlocks y_max = that.lex_greatest(y_succ);
    if (known()) {
      string sx = val(), sy = y_max.val();
      if ((lt && sx >= sy) || (!lt && sx > sy))
        return false;
    }
    else {
      // Checking this <= max(that).
      Position pos{0, 0};
      std::vector<float> x_succ = min_succ(*this);
      bool sub = false;
      for (int j = 0; j < length(); ++j) {
        if (!lex_step_xy(at(j), x_succ[j], y_max, pos, sub))
          return false;
        if (sub)
          break;
      }
      if (lt && !sub && length() >= that.length())
        return false;
    }
    std::vector<float> x_succ = min_succ(*this);
    NSBlocks x_min = lex_least(x_succ);
    if (that.known()) {
      string sx = x_min.val(), sy = that.val();
      if ((lt && sx >= sy) || (!lt && sx > sy))
        return false;
    }
    else {
      // Checking that >= min(this).
      Position pos{0, 0};
      y_succ = max_succ(that);
      bool sub = false;
      for (int j = 0; j < that.length(); ++j) {
        if (!lex_step_yx(that.at(j), y_succ[j], x_min, pos, sub))
          return false;
        if (sub)
          break;
      }
      if (lt && !sub && length() >= that.length())
        return false;
    }
    return true;
  }

  forceinline bool
  DashedString::rev(Space& h, DashedString& x) {
    //std::cerr<<"DashedString::reverse "<<*this<<" = "<<x<<"^-1"<<std::endl;
    _changed = x._changed = false;
    if (!refine_card_eq(h, x))
      return false;
    ReverseView xrev(x);
    if (!sweep_reverse(h, xrev, x, *this))
      return false;    
    if ((_changed || x._changed) && !refine_card_eq(h, x))
      return false;
    // std::cerr<<"reversed: "<<*this<<" "<<_changed<<std::endl;
    return true;
  }

  forceinline bool
  DashedString::pow(Space& h, DashedString& x, int& n1, int& n2) {
    // std::cerr<< "DashedString::pow " << *this << ' ' << x << ' ' << n1 << ' ' << n2 << '\n';
    assert (n1 <= n2);
    _changed = x._changed = false;
    if (n1 == 1 && n2 == 1)
      return equate(h, x);    
    if (n2 == 0 || x.null()) {
      if (null())
        return true;
      if (_min_length > 0)
        return false;
      set_null(h);
      _changed = true;
      return true;
    }
    if (null()) {
      if (n1 > 0) {
        if (x._min_length > 0)
          return false;
        x._changed = true;
        x.set_null(h);
      }
      else if (x._min_length > 0)
        n2 = 0;
      return true;
    }
    NSBlocks xn = x.pow(n1);
    int u = min(_MAX_STR_LENGTH, long(x._max_length) * (n2 - n1));
    xn.concat(NSBlocks(1, NSBlock(x.may_chars(), 0, u)), xn);
    xn.normalize();
//    std::cerr << "xn: " << xn << "\n";
    if (!sweep_equate(h, *this, xn))
      return false;    
    if (!refine_card_pow(h, x, n1, n2))
      return false;
    return true;
  }

  forceinline DSBlock&
  DashedString::at(int i) {
    return _blocks.at(i);
  }
  forceinline const DSBlock&
  DashedString::at(int i) const {
    return _blocks.at(i);
  }

  forceinline int
  DashedString::length() const {
    return _blocks.length();
  }

  forceinline int
  DashedString::min_length() const {
    return _min_length;
  }

  forceinline int
  DashedString::max_length() const {
    return _max_length;
  }

  forceinline void
  DashedString::min_length(int i) {
    _min_length = min(_MAX_STR_LENGTH, i);
  }

  forceinline void
  DashedString::max_length(int i) {
    _max_length = min(_MAX_STR_LENGTH, i);
  }

  forceinline NSIntSet
  DashedString::must_chars() const {
    NSIntSet set;
    for (int i = 0; i < _blocks.length(); ++i) {
      const DSBlock& b = at(i);
      if (b.S.size() == 1 && b.l > 0)
        set.include(b.S);
    }
    return set;
  }

  forceinline NSIntSet
  DashedString::may_chars() const {
    NSIntSet set;
    for (int i = 0; i < _blocks.length(); ++i)
      set.include(at(i).S);
    return set;
  }

  forceinline double
  DashedString::logdim() const {
    return _blocks.logdim();
  }

  forceinline bool
  DashedString::known() const {
    if (_min_length != _max_length)
      return false;
    for (int i = 0; i < _blocks.length(); ++i)
      if (!at(i).known())
        return false;
    return true;
  }

  forceinline string
  DashedString::known_pref() const {
    string pref = "";
    for (int i = 0; i < _blocks.length(); ++i) {
      const DSBlock& b = at(i);
      if (b.S.size() > 1)
        return pref;
      pref += string(b.l, int2char(b.S.min()));
      if (b.l < b.u)
        return pref;
    }
    return pref;
  }

  forceinline string
  DashedString::known_suff() const {
    string suff = "";
    for (int i = _blocks.length() - 1; i >= 0; --i) {
      const DSBlock& b = at(i);
      if (b.S.size() > 1)
        return suff;
      suff = string(b.l, int2char(b.S.min())) + suff;
      if (b.l < b.u)
        return suff;
    }
    return suff;
  }

  forceinline bool
  DashedString::null() const {
    return _max_length == 0;
  }
  
  forceinline bool
  DashedString::isTop() const {
    return length() == 1 && _blocks.at(0).isTop();
  }
  
  forceinline bool
  DashedString::refine_lb(int ly) {
    // std::cerr << "refine_lb: " << *this << ' ' << ly << "\n";
    if (ly > _max_length)
      return false;
    if (known())
      return ly <= _min_length;
    int lx = 0;
    long ux = 0;
    for (int i = 0; i < _blocks.length(); ++i) {
      const DSBlock& bi = at(i);
      lx += bi.l;
      ux += bi.u;
    }
    if (ly < lx) {
      if (_min_length < lx) {
        _min_length = lx;
        _changed = true;
      }
      return true;
    }
    if (ux < ly)
      return false;
    int sl = 0;
    for (int i = 0; i < _blocks.length(); ++i) {
      DSBlock& bi = at(i);
      int l = bi.l, ll = l, u = bi.u;
      if (ux - u + l < ly) {
        ll = min(u, ly - ux + u);
        bi.l = ll;
      }
      sl += ll;
    }
    lx = max(ly, sl);
    if (_min_length < lx) {
      _min_length = lx;
      _changed = true;
    }    
    // std::cerr << "refined: " << *this << ' ' << _changed << ' ' << _min_length << "\n";
    return true;
  }

  forceinline bool
  DashedString::refine_ub(Space& h, int uy) {
    // std::cerr << "refine_ub: " << *this << ' ' << uy << ' ' << _max_length << "\n";
    if (uy < _min_length)
      return false;
    if (known())
      return uy >= _max_length;    
    int lx = 0;
    long ux = 0;
    bool norm = false;
    for (int i = 0; i < _blocks.length(); ++i) {
      const DSBlock& bi = at(i);
      lx += bi.l;
      ux += bi.u;
      norm |= bi.null();
    }
    if (ux < uy) {
      if (_max_length > ux) {
        _max_length = ux;
        _changed = true;
      }
      if (norm)
        normalize(h);
      return true;      
    }
    if (uy < lx)
      return false;
    long su = 0;    
    for (int i = 0; i < _blocks.length(); ++i) {
      DSBlock& bi = at(i);
      int l = bi.l, u = bi.u, uu = u;
      if (lx - l + u > uy) {
        uu = max(l, uy - lx + l);
        bi.u = uu;
      }
      su += uu;
      norm |= uu == 0;
    }    
    ux = min(uy, su);
    if (_max_length > ux) {
      _max_length = ux;
      _changed = true;      
    }
    if (norm)
      normalize(h);
    // std::cerr << "refined: " << *this << ' ' << _max_length << "\n";      
    assert (is_normalized());    
    return true;
  }

  forceinline bool
  DashedString::refine_card(Space& h, int l, int u) {
    return refine_lb(l) && refine_ub(h, u);
  }

  forceinline string
  DashedString::val() const {
    if (!known())
      throw UnknownValDS("DashedString::val");
    string s;
    for (int i = 0; i < _blocks.length(); ++i) {
      const DSBlock& b = at(i);
      s += string(b.l, int2char(b.S.min()));
    }
    return s;
  }

  forceinline bool
  DashedString::check_equate(const DashedString& that) const {
    if (_min_length > that._max_length
    ||  _max_length < that._min_length)
      return false;
    return check_sweep<DSBlock, DSBlocks, DSBlock, DSBlocks>(
      _blocks, that. _blocks
    );
  }
  forceinline bool
  DashedString::check_equate(const NSBlocks& that) const {
    return check_sweep<DSBlock, DSBlocks, NSBlock, NSBlocks>(
      _blocks, that
    );
  }

  forceinline bool
  DashedString::equate(Space& h, DashedString& that) {
    // std::cerr<<"DashedString::equate "<<*this<<' '<<that<<std::endl;    
    _changed = that._changed = false;
    if (!refine_card_eq(h, that))
      return false;
    if (known()) {
      string s = val();
      if (that.known())
        return s == that.val();
      else {
        if (check_sweep<char, string, DSBlock, DSBlocks>(s, that._blocks)) {
          that.update(h, s);
          that._changed = true;
          return true;
        }
        else
          return false;
      }
    }
    if (that.known()) {
      string s = that.val();
      if (check_sweep<DSBlock, DSBlocks, char, string>(_blocks, s)) {
        update(h, s);
        _changed = true;
        return true;
      }
      else
        return false;
    }
    if (contains(that)) {
      if (*this == that)
        return true;
      update(h, that);
      _changed = true;
      return true;
    }
    if (that.contains(*this)) {
      that.update(h, *this);
      that._changed = true;
      return true;
    }
    // std::cerr<<"refined: "<<*this<<' '<<that<<std::endl;
    if (!sweep_equate(h, *this, that))
      return false;
    if ((_changed || that._changed) && !refine_card_eq(h, that))
      return false;
    // std::cerr<<"DashedString::equated "<<*this<<' '<<that<<std::endl;
    return true;
  }

  forceinline bool
  DashedString::equate(Space& h, const NSBlocks& that) {
    // std::cerr<<"DashedString::equate "<<*this<<' '<<that<<std::endl;
    _changed = false;
    if (known()) {
      string s = val();
      if (that.known())
        return s == that.val();
      else
        return check_sweep<char, string, NSBlock, NSBlocks>(s, that);
    }
    if (that.known()) {
      string s = that.val();
      if (check_sweep<DSBlock, DSBlocks, char, string>(_blocks, s)) {
        update(h, s);
        _changed = true;
        return true;
      }
      else
        return false;
    }
    if (that.contains<DSBlock, DashedString>(*this))
      return true;
    if (contains(that)) {
      update(h, that);
      _changed = true;
      return true;
    }
    if (!sweep_equate(h, *this, that))
      return false;
    int l = 0;
    long u = 0;
    for (unsigned i = 0; i < that.size(); ++i) {
      const NSBlock& b = that[i];
      l += b.l;
      u += b.u;
    }
    if (!refine_card(h, max(_min_length, l), min(_max_length, u)))
      return false;
    // std::cerr<<"equated: "<<*this<<' '<<_changed<<std::endl;
    return true;
  }

  forceinline void
  DashedString::update(Space& h, string s) {
    _blocks.update(h, s);
    _min_length = _max_length = s.size();
    assert (is_normalized());
  }
  
  forceinline void
  DashedString::update(Space& h, const DashedString& that) {
    _blocks.update(h, that._blocks);
    _min_length = that._min_length;
    _max_length = that._max_length;
    _changed = that._changed;
    assert (is_normalized());
  }

  forceinline void
  DashedString::update(Space& h, const NSBlocks& that) {
    int l, u;
    _blocks.update(h, that, l, u);
    _min_length = l;
    _max_length = u;
    if (length() == 1) {
      if (at(0).l < _min_length)
        at(0).l = _min_length;
      if (at(0).u > _max_length)
        at(0).u = _max_length;
    }
    // std::cerr << *this << ' ' << l << ' ' << u << '\n';
    assert (is_normalized());
  }

  forceinline void
  DashedString::norm_update(Space& h, NSBlocks& v) {
    // std::cerr << "norm_update " << *this << " with v = " << v;
    v.normalize();
    int l, u;    
    _blocks.update(h, v, l, u);
    _min_length = l;
    _max_length = u;
    if (length() == 1) {
      if (at(0).l < _min_length)
        at(0).l = _min_length;
      if (at(0).u > _max_length)
        at(0).u = _max_length;
    }
    assert (is_normalized());
  }

  forceinline void
  DashedString::normalize(Space& h) {
    int l, u;
    _blocks.normalize(h, l, u);
    _min_length = max(_min_length, l);
    _max_length = min(_max_length, u);
    if (length() == 1) {
      if (at(0).l < _min_length)
        at(0).l = _min_length;
      if (at(0).u > _max_length)
        at(0).u = _max_length;
    }
    assert (is_normalized());
  }

  forceinline void
  DashedString::set_null(Space& h) {
    _blocks.update(h, "");
    _min_length = _max_length = 0;
  }

  forceinline bool
  DashedString::contains(const DashedString& that) const {
    // std::cerr << *this << " DashedString::contains" << that << "\n";
    if (_min_length > that._min_length 
    ||  _max_length < that._max_length)
      return false;
    if (isTop() || (that.null() && _min_length == 0))
      return true;
    int n1 = length(), n2 = that.length();
    if (n1 < n2)
      return false;
    for (int i = 0; i < n2; ++i) {
      const DSBlock& b1 = at(i);
      const DSBlock& b2 = that.at(i);
      if (!b1.S.contains(b2.S) || b1.l > b2.l || b1.u < b2.u)
        return false;
    }
    for (int i = n2; i < n1; ++i)
      if (at(i).l > 0)
        return false;
    return true;
  }
  forceinline bool
  DashedString::contains(const NSBlocks& that) const {
    // std::cerr << *this << " DashedString::contains" << that << "\n";
    if (isTop() || (that.null() && _min_length == 0))
      return true;
    int n1 = length(), n2 = that.length();
    if (n1 < n2)
      return false;
    for (int i = 0; i < n2; ++i) {
      const DSBlock& b1 = at(i);
      const NSBlock& b2 = that.at(i);
      if (!b1.S.contains(b2.S) || b1.l > b2.l || b1.u < b2.u)
        return false;
    }
    for (int i = n2; i < n1; ++i)
      if (at(i).l > 0)
        return false;
    return true;
  }
  forceinline bool
  DashedString::contains(const DashedString& x, const DashedString& y) const {
    // std::cerr << *this << " DashedString::contains" << that << "\n";
    if (isTop())
      return true;
    int n1 = length(), n2 = x.length() + y.length(), norm = 0;
    if (x.blocks().back().S == y.at(0).S) {
      norm = 1;
      n2--;
    }
    if (n1 < n2)
      return false;
    for (int i = 0; i < x.length() - norm; ++i) {
      const DSBlock& b1 = at(i);
      const DSBlock& b2 = x.at(i);
      if (!b1.S.contains(b2.S) || b1.l > b2.l || b1.u < b2.u)
        return false;
    }
    if (norm) {
      const DSBlock& b1 = at(x.length() - 1);
      const DSBlock& b2 = x.at(x.length() - 1);
      const DSBlock& b3 = y.at(0);
      if (!b1.S.contains(b2.S) || b1.l > b2.l + b3.l || b1.u < b2.u + b3.u)
        return false;
    }
    for (int i = norm; i < y.length(); ++i) {
      const DSBlock& b1 = at(i + x.length() - norm);
      const DSBlock& b2 = y.at(i);
      if (!b1.S.contains(b2.S) || b1.l > b2.l || b1.u < b2.u)
        return false;
    }
    for (int i = n2; i < n1; ++i)
      if (at(i).l > 0)
        return false;
    return true;
  }

  forceinline bool
  DashedString::equals(const DashedString& that) const {
    if (_min_length != that._min_length 
    ||  _max_length != that._max_length)
      return false;
    int n = length();
    if (n != that.length())
      return false;
    for (int i = 0; i < n; ++i) {
      const DSBlock& b1 = at(i);
      const DSBlock& b2 = that .at(i);
      if (!(b1.S == b2.S) || b1.l != b2.l || b1.u != b2.u)
        return false;
    }
    return true;
  }
  forceinline bool
  DashedString::equals(const NSBlocks& that) const {
    int n = length();
    if (n != that.length())
      return false;
    for (int i = 0; i < n; ++i) {
      const DSBlock& b1 = at(i);
      const NSBlock& b2 = that .at(i);
      if (!(b1.S == b2.S) || b1.l != b2.l || b1.u != b2.u)
        return false;
    }
    return true;
  }  

  forceinline bool
  DashedString::operator==(const DashedString& that) const {
    return equals(that);
  }
  forceinline bool
  DashedString::operator!=(const DashedString& that) const {
    return !(equals(that));
  }
  
  forceinline bool
  DashedString::changed() const {
    return _changed;
  }
  forceinline void
  DashedString::changed(bool b) {
    _changed = b;
  }

  forceinline std::ostream&
  operator<<(std::ostream& os, const DashedString& ds) {
    if (ds.known()) {
      if (ds.null())
        os << "\"\"";
      else {
        os << "\"";
        for (int i = 0; i < ds._blocks.length(); ++i) {
          const DSBlock& b = ds.at(i);
          os << std::string(b.l, int2char(b.S.min()));
        }
        os << "\"";
      }
    }
    else {
      int n = ds.length();
      for (int i = 0; i < n - 1; ++i)
        os << ds.at(i) << " + ";
      os << ds.at(n - 1);
    }
    return os;
  }

}}
