#ifndef __GECODE_STRING_NS_DASHED_HH
#define __GECODE_STRING_NS_DASHED_HH

#ifdef GECODE_HAS_FAULT_INJECTION
#include <atomic>
#include <new>
#endif

namespace Gecode { namespace String {

  /// Linked inclusive range node owned by NSIntSet
  struct NSRange {

#ifdef GECODE_HAS_FAULT_INJECTION
    /// Test-only count of live raw range allocations
    inline static std::atomic<int> fault_live_ranges{0};

    static void* operator new(size_t size) {
      Support::FailPoint::check(Support::FailPoint::Phase::NSIntSetRange);
      void* range = ::operator new(size);
      fault_live_ranges.fetch_add(1, std::memory_order_relaxed);
      return range;
    }

    static void operator delete(void* range) {
      fault_live_ranges.fetch_sub(1, std::memory_order_relaxed);
      ::operator delete(range);
    }

    static int fault_live_allocations(void) {
      return fault_live_ranges.load(std::memory_order_relaxed);
    }
#endif

    int l, u;

    NSRange* next;

    NSRange(): l(-1), u(-1), next(NULL) {};

    NSRange(int a): l(a), u(a), next(NULL) {};

    NSRange(int a, int b): l(a), u(b), next(NULL) {};

    NSRange(int a, int b, NSRange* p): l(a), u(b), next(p) {};

  };

}}

namespace Gecode { namespace String {

  /**
   * \brief Ordinary-lifetime finite set of string symbols
   *
   * The set owns a normalized linked list of disjoint inclusive ranges.
   * Unlike DSIntSet it is independent of a Gecode Space and is suitable for
   * temporary analysis, automata labels, and refinements.
   */
  class NSIntSet {

  private:

    NSRange* _fst;

    NSRange* _lst;

    int _size;

    int _len;

  public:

    /// Forward iterator over individual symbols in ascending order
    class iterator {

    private:

      NSRange* node;
      int val;

    public:

      iterator(const NSIntSet& x) {
        if (x.empty()) {
          node = NULL;
          val = -1;
        }
        else {
          node = x._fst;
          val = x._fst->l;
        }
      };

      iterator(NSRange* n): node(n) {
        if (n == NULL)
          val = -1;
        else
          val = n->l;
      };

      void
      operator++() {
        if (val == node->u) {
          node = node->next;
          if (node != NULL)
            val = node->l;
          else
            val = -1;
        }
        else
          val++;
      }

      bool
      operator()() const {
        return node != NULL;
      }

      int
      operator*() const {
        return val;
      }

    };

    NSIntSet();

    explicit NSIntSet(const DSIntSet& s);

    NSIntSet(int l);

    NSIntSet(int l, int u);

    NSIntSet(const NSIntSet& that);

    NSIntSet(NSIntSet&& that) noexcept;

    /// Check range ordering, cached counts, and endpoint invariants
    bool consistent() const;

    NSIntSet& operator=(const NSIntSet& that);

    NSIntSet& operator=(NSIntSet&& that) noexcept;

    bool operator==(const NSIntSet& that) const;
    bool operator==(const DSIntSet& that) const;

    NSRange* first() const;

    NSRange* last() const;

    int min() const;

    int max() const;

    /// Return the number of individual symbols
    int size() const;

    /// Return the number of stored ranges
    int length() const;

    bool empty() const;

    bool disjoint(const NSIntSet& that) const;
    bool disjoint(const DSIntSet& that) const;

    bool contains(const NSIntSet& that) const;
    bool contains(const DSIntSet& that) const;
    bool contains(int x) const;

    bool in(int x) const;

    void clear();

    void remove(int x);

    void add(int x);

    void shift(int x);

    void intersect(const NSIntSet& that);
    void intersect(const DSIntSet& that);
    void intersect(Space& home, const NSIntSet& that);
    void intersect(Space& home, const DSIntSet& that);

    void include(const NSIntSet& that);
    void include(const DSIntSet& that);

    void exclude(const NSIntSet& that);

    /// Return the complement within the configured active symbol universe
    NSIntSet comp() const;

    /// Return the one-based \a n-th symbol, or zero when out of range
    int nth(int n) const;

    std::string toString() const;

    ~NSIntSet();

    friend std::ostream& operator<<(std::ostream& os, const NSIntSet& that);

    static NSIntSet top();

    iterator begin() const;

    iterator end() const;

    forceinline bool
    operator<(const NSIntSet& that) const {
        return this->min() < that.min();
    }

  };

}}

namespace Gecode { namespace String {

    forceinline NSIntSet
    NSIntSet::top() {
      return NSIntSet(0, DashedString::_MAX_STR_ALPHA);
    }

    forceinline std::ostream&
    operator<<(std::ostream& os, const NSIntSet& s) {
      NSRange* p = s._fst;
      os << '{';
      while (p != NULL) {
        const string mi =
          StringVal::from_symbols({p->l}).to_utf8();
        const string ma =
          StringVal::from_symbols({p->u}).to_utf8();
        if (mi == ma)
          os << mi;
        else
          os << mi << ".." << ma;
        p = p->next;
        if (p)
          os << ",";
      }
      return os << "}";
    }

    forceinline std::string
    NSIntSet::toString() const {
      NSRange* p = _fst;
      std::ostringstream os;
      os << "{";
      while (p != NULL) {
        int mi = p->l;
        int ma = p->u;
        if (mi == ma)
          os << mi;
        else
          os << mi << ".." << ma;
        p = p->next;
        if (p)
          os << ",";
      }
      os << "}";
      return os.str();
    }

    forceinline
    NSIntSet::NSIntSet(): _fst(NULL), _lst(NULL), _size(0), _len(0) {}

    forceinline
    NSIntSet::NSIntSet(const DSIntSet& s)
    : _fst(NULL), _lst(NULL), _size(0), _len(0) {
      NSIntSet copy;
      for (Gecode::Set::BndSetRanges i(s); i(); ++i) {
        NSRange* node = new NSRange(i.min(), i.max());
        if (copy._lst)
          copy._lst->next = node;
        else
          copy._fst = node;
        copy._lst = node;
        ++copy._len;
      }
      copy._size = s.size();
      *this = std::move(copy);
    }

    forceinline
    NSIntSet::NSIntSet(int l) {
      if (l < 0)
        l = static_cast<unsigned char>(l);
      NSRange* p = new NSRange(l, l);
      _fst = _lst = p;
      _size = 1;
      _len = 1;
    }

    forceinline
    NSIntSet::NSIntSet(int l, int u) {
      if (l < 0)
        l = static_cast<unsigned char>(l);
      if (u < 0)
        u = static_cast<unsigned char>(u);
      if (l > u) {
        _fst  = _lst = NULL;
        _size = _len = 0;
      }
      else {
        NSRange* p = new NSRange(l, u);
        _fst = _lst = p;
        _size = u - l + 1;
        _len = 1;
      }
    }

    forceinline bool
    NSIntSet::consistent() const {
      if ((_fst == NULL) || (_lst == NULL))
        return (_fst == NULL) && (_lst == NULL) &&
          (_len == 0) && (_size == 0);
      if ((_len <= 0) || (_size <= 0))
        return false;
      int length = 0, size = 0;
      NSRange* previous = NULL;
      NSRange* last = NULL;
      for (NSRange* p = _fst; p; p = p->next) {
        if ((p->l > p->u) ||
            (previous && (previous->u >= p->l - 1)))
          return false;
        ++length;
        size += p->u - p->l + 1;
        previous = p;
        last = p;
      }
      return (last == _lst) && (_lst->next == NULL) &&
        (length == _len) && (size == _size);
    }

    forceinline NSIntSet::iterator
    NSIntSet::begin() const {
      return iterator(_fst);
    }

    forceinline NSIntSet::iterator
    NSIntSet::end() const {
      return iterator(_lst);
    }

    forceinline NSRange*
    NSIntSet::first() const {
      return _fst;
    }

    forceinline NSRange*
    NSIntSet::last() const {
      return _lst;
    }

    forceinline int
    NSIntSet::size() const {
      return _size;
    }

    forceinline int
    NSIntSet::length() const {
      return _len;
    }

    forceinline int
    NSIntSet::min() const {
      return _fst->l;
    }

    forceinline int
    NSIntSet::max() const {
      return _lst->u;
    }

    forceinline bool
    NSIntSet::empty() const {
      return _fst == NULL && _lst == NULL && _len == 0 && _size == 0;
    }

    forceinline bool
    NSIntSet::disjoint(const NSIntSet& that) const {
      NSRange* i1 = _fst;
      NSRange* i2 = that._fst;
      while (i1 && i2) {
        if (i2->u < i1->l)
          i2 = i2->next;
        else if (i1->u < i2->l)
          i1 = i1->next;
        else
          return false;
      }
      return true;
    }

    forceinline bool
    NSIntSet::disjoint(const DSIntSet& that) const {
      return that.disjoint(*this);
    }

    forceinline bool
    NSIntSet::contains(const NSIntSet& that) const {
      if (that.empty())
        return true;
      if (this->size() < that.size())
        return false;
      NSRange* i1 = this->_fst;
      NSRange* i2 = that ._fst;
      while (i1 && i2)
        if (i1->l > i2->u)
          return false;
        else if (i1->l <= i2->l && i2->u <= i1->u)
          i2 = i2->next;
        else
          i1 = i1->next;
      return !i2;
    }

    forceinline bool
    NSIntSet::contains(const DSIntSet& that) const {
      if (that.empty())
        return true;
      if (this->size() < (int) that.size())
        return false;
      NSRange* i1 = this->_fst;
      Gecode::Set::BndSetRanges i2(that);
      while (i1 && i2())
        if (i1->l > i2.max())
          return false;
        else if (i1->l <= i2.min() && i2.max() <= i1->u)
          ++i2;
        else
          i1 = i1->next;
      return !i2();
    }

    forceinline bool
    NSIntSet::contains(int x) const {
      if (this->empty() || x < min() || x > max())
        return false;
      for (NSRange* p = _fst; p; p = p->next)
        if (x < p->l)
          return false;
        else if (p->l <= x && x <= p->u)
          return true;
      return false;
    }

    forceinline bool
    NSIntSet::in(int x) const {
      return this->contains(x);
    }

    forceinline void
    NSIntSet::remove(int x) {
      if (empty() || x < min() || x > max())
        return;
      NSRange* prev = NULL;
      for (NSRange* p = _fst; p != NULL; p = p->next) {
        if (p->l == x) {
          _size--;
          if (p->u == x) {
            _len--;
            if (prev) {
              prev->next = p->next;
              if (_lst == p)
                _lst = prev;
            }
            else if (p->next)
              _fst = p->next;
            else {
              _fst = NULL;
              _lst = NULL;
            }
            delete p;
          }
          else
            p->l++;
          return;
        }
        else if (p->u == x) {
          _size--;
          p->u--;
        }
        else if (p->l < x && x < p->u) {
          _size--;
          _len++;
          NSRange* q = new NSRange(x + 1, p->u, p->next);
          p->u = x - 1;
          p->next = q;
          if (_lst == p)
            _lst = q;
          return;
        }
        prev = p;
      }
    }

    forceinline void
    NSIntSet::intersect(Space&, const NSIntSet& that) {
      intersect(that);
    }

    forceinline void
    NSIntSet::intersect(Space&, const DSIntSet& that) {
      intersect(that);
    }

    forceinline void
    NSIntSet::intersect(const NSIntSet& that) {
      if (empty())
        return;
      if (that.empty()
      || this->min() > that.max() || that.min() > this->max()) {
        this->clear();
        return;
      }
      if (this->_len == 1 && that._len == 1) {
        NSRange* p = this->_fst;
        int l = std::max(p->l, that.min());
        int u = std::min(p->u, that.max());
        p->l = l;
        p->u = u;
        this->_size = u - l + 1;
        return;
      }
      NSRange* i1 = this->_fst;
      NSRange* i2 = that ._fst;
      NSIntSet intersection;
      while (i1 && i2) {
        if (i1->l > i2->u)
          i2 = i2->next;
        else if (i1->u < i2->l)
          i1 = i1->next;
        else {
          int l = std::max(i1->l, i2->l);
          int u = std::min(i1->u, i2->u);
          NSRange* p = new NSRange(l, u);
          if (intersection._lst)
            intersection._lst->next = p;
          else
            intersection._fst = p;
          intersection._lst = p;
          intersection._size += u - l + 1;
          ++intersection._len;
          if (i1->u > i2->u)
            i2 = i2->next;
          else
            i1 = i1->next;
        }
      }
      *this = std::move(intersection);
    }

    forceinline void
    NSIntSet::intersect(const DSIntSet& that) {
      if (empty())
        return;
      if (that.empty()
      || this->min() > that.max() || that.min() > this->max()) {
        this->clear();
        return;
      }
      RangeList* i2 = that.ranges();
      if (this->_len == 1 && !i2->next()) {
        NSRange* p = this->_fst;
        int l = std::max(p->l, i2->min());
        int u = std::min(p->u, i2->max());
        p->l = l;
        p->u = u;
        this->_size = u - l + 1;
        return;
      }
      NSRange* i1 = this->_fst;
      NSIntSet intersection;
      while (i1 && i2) {
        if (i1->l > i2->max())
          i2 = i2->next();
        else if (i1->u < i2->min())
          i1 = i1->next;
        else {
          int l = std::max(i1->l, i2->min());
          int u = std::min(i1->u, i2->max());
          NSRange* p = new NSRange(l, u);
          if (intersection._lst)
            intersection._lst->next = p;
          else
            intersection._fst = p;
          intersection._lst = p;
          intersection._size += u - l + 1;
          ++intersection._len;
          if (i1->u > i2->max())
            i2 = i2->next();
          else
            i1 = i1->next;
        }
      }
      *this = std::move(intersection);
    }

    forceinline NSIntSet
    NSIntSet::comp() const {
      NSIntSet c;
      const int max = DashedString::_MAX_STR_ALPHA;
      int next = 0;
      for (NSRange* range = _fst; range; range = range->next) {
        if (next < range->l) {
          NSRange* gap = new NSRange(next, range->l - 1);
          if (c._lst)
            c._lst->next = gap;
          else
            c._fst = gap;
          c._lst = gap;
          c._size += gap->u - gap->l + 1;
          ++c._len;
        }
        if (range->u == max) {
          next = max + 1;
          break;
        }
        next = range->u + 1;
      }
      if (next <= max) {
        NSRange* gap = new NSRange(next, max);
        if (c._lst)
          c._lst->next = gap;
        else
          c._fst = gap;
        c._lst = gap;
        c._size += gap->u - gap->l + 1;
        ++c._len;
      }
      return c;
    }

    forceinline void
    NSIntSet::exclude(const NSIntSet& that) {
      if (empty() || that.empty())
        return;
      if (this == &that) {
        clear();
        return;
      }
      NSIntSet difference;
      NSRange* excluded = that._fst;
      for (NSRange* range = _fst; range; range = range->next) {
        int next = range->l;
        while (excluded && excluded->u < next)
          excluded = excluded->next;
        while (excluded && excluded->l <= range->u) {
          if (next < excluded->l) {
            NSRange* kept = new NSRange(next, excluded->l - 1);
            if (difference._lst)
              difference._lst->next = kept;
            else
              difference._fst = kept;
            difference._lst = kept;
            difference._size += kept->u - kept->l + 1;
            ++difference._len;
          }
          if (excluded->u >= range->u) {
            next = range->u + 1;
            break;
          }
          next = std::max(next, excluded->u + 1);
          excluded = excluded->next;
        }
        if (next <= range->u) {
          NSRange* kept = new NSRange(next, range->u);
          if (difference._lst)
            difference._lst->next = kept;
          else
            difference._fst = kept;
          difference._lst = kept;
          difference._size += kept->u - kept->l + 1;
          ++difference._len;
        }
      }
      *this = std::move(difference);
    }

    forceinline void
    NSIntSet::include(const NSIntSet& that) {
      if (empty()) {
        *this = that;
        return;
      }
      if (that.empty() || this->contains(that))
        return;
      if (this->_len == 1) {
        if (this->size() == 1) {
          int n = this->min();
          *this = that;
          this->add(n);
          return;
        }
        else if (this->min() <= that.min() && this->max() >= that.max())
          return;
      }
      if (that._len == 1) {
        if (that.size() == 1) {
          this->add(that.min());
          return;
        }
        else if (that.min() <= this->min() && that.max() >= this->max()) {
          *this = that;
          return;
        }
      }
      if (this->min() > that.max()) {
        NSRange* p = new NSRange(that._fst->l, that._fst->u);
        NSRange* f = this->_fst;
        this->_fst = p;
        NSRange* q = p;
        for (p = that._fst->next; p; p = p->next) {
          NSRange* r = q;
          q = new NSRange(p->l, p->u);
          r->next = q;
        }
        if (f->l == q->u + 1) {
          q->u = f->u;
          q->next = f->next;
          if (!f->next)
            this->_lst = q;
          delete f;
          this->_len--;
        }
        else
          q->next = f;
        this->_len += that._len;
        this->_size += that._size;
        return;
      }
      if (this->max() < that.min()) {
        NSRange* f;
        if (this->max() == that.min() - 1) {
          this->_lst->u = that._fst->u;
          f = that._fst->next;
          this->_len--;
        }
        else
          f = that._fst;
        for (NSRange* p = f; p; p = p->next) {
          NSRange* q = new NSRange(p->l, p->u);
          this->_lst->next = q;
          this->_lst = q;
        }
        this->_len += that._len;
        this->_size += that._size;
        return;
      }
      NSRange* i1 = this->_fst;
      NSRange* i2 = that._fst;
      NSRange* first = NULL;
      NSRange* last = NULL;
      while (i1 && i2) {
        if (i2->u < i1->l - 1) {
          NSRange* p = new NSRange(i2->l, i2->u);
          if (first)
            last->next = p;
          else
            first = p;
          last = p;
          i2 = i2->next;
        }
        else if (i1->u < i2->l - 1) {
          NSRange* p = new NSRange(i1->l, i1->u);
          if (first)
            last->next = p;
          else
            first = p;
          last = p;
          i1 = i1->next;
        }
        else {
          int l = std::min(i1->l, i2->l);
          int u = std::max(i1->u, i2->u);
          NSRange* p = new NSRange(l, u);
          if (!first)
            first = p;
          else
            last->next = p;
          last = p;
          if (i1->u == i2->u) {
            i1 = i1->next;
            i2 = i2->next;
          }
          else if (i1->u > i2->u)
            i2 = i2->next;
          else
            i1 = i1->next;
        }
      }
      if (i1)
        for (NSRange* p = i1; p; p = p->next) {
          NSRange* q = new NSRange(p->l, p->u);
          last->next = q;
          last = q;
        }
      else if (i2)
        for (NSRange* p = i2; p; p = p->next) {
          NSRange* q = new NSRange(p->l, p->u);
          last->next = q;
          last = q;
        }
      int size = 0, len = 0;
      NSRange* p = first;
      for (; p; p = p->next) {
        NSRange* q = p->next;
        while (q && p->u >= q->l - 1) {
          p->u = q->u;
          p->next = q->next;
          delete q;
          q = p->next;
        }
        ++len;
        size += p->u - p->l + 1;
        last = p;
      }
      this->clear();
      this->_fst = first;
      this->_lst = last;
      this->_len = len;
      this->_size = size;
    }

    forceinline void
    NSIntSet::include(const DSIntSet& that) {
      if (empty()) {
        *this = NSIntSet(that);
        return;
      }
      if (that.empty() || this->contains(that))
        return;
      if (this->_len == 1) {
        if (this->size() == 1) {
          int n = this->min();
          *this = NSIntSet(that);
          this->add(n);
          return;
        }
        else if (this->min() <= that.min() && this->max() >= that.max())
          return;
      }
      if (!that.ranges()->next()) {
        if (that.size() == 1) {
          this->add(that.min());
          return;
        }
        else if (that.min() <= this->min() && that.max() >= this->max()) {
          *this = NSIntSet(that);
          return;
        }
      }
      if (this->min() > that.max()) {
        NSRange* p = new NSRange(that.ranges()->min(), that.ranges()->max());
        NSRange* f = this->_fst;
        this->_fst = p;
        NSRange* q = p;
        int k = 1;
        for (RangeList* p = that.ranges()->next(); p; p = p->next()) {
          NSRange* r = q;
          q = new NSRange(p->min(), p->max());
          r->next = q;
          ++k;
        }
        if (f->l == q->u + 1) {
          q->u = f->u;
          q->next = f->next;
          if (!f->next)
            this->_lst = q;
          delete f;
          --k;
        }
        else
          q->next = f;
        this->_len += k;
        this->_size += that.size();
        return;
      }
      if (this->max() < that.min()) {
        RangeList* f;
        int k = 0;
        if (this->max() == that.min() - 1) {
          this->_lst->u = that.ranges()->max();
          f = that.ranges()->next();
        }
        else
          f = that.ranges();
        for (RangeList* p = f; p; p = p->next()) {
          NSRange* q = new NSRange(p->min(), p->max());
          this->_lst->next = q;
          this->_lst = q;
          k++;
        }
        this->_len += k;
        this->_size += that.size();
        return;
      }
      NSRange* i1 = this->_fst;
      RangeList* i2 = that.ranges();
      NSRange* first = NULL;
      NSRange* last = NULL;
      while (i1 && i2) {
        if (i2->max() < i1->l - 1) {
          NSRange* p = new NSRange(i2->min(), i2->max());
          if (first)
            last->next = p;
          else
            first = p;
          last = p;
          i2 = i2->next();
        }
        else if (i1->u < i2->min() - 1) {
          NSRange* p = new NSRange(i1->l, i1->u);
          if (first)
            last->next = p;
          else
            first = p;
          last = p;
          i1 = i1->next;
        }
        else {
          int l = std::min(i1->l, i2->min());
          int u = std::max(i1->u, i2->max());
          NSRange* p = new NSRange(l, u);
          if (!first)
            first = p;
          else
            last->next = p;
          last = p;
          if (i1->u == i2->max()) {
            i1 = i1->next;
            i2 = i2->next();
          }
          else if (i1->u > i2->max())
            i2 = i2->next();
          else
            i1 = i1->next;
        }
      }
      if (i1)
        for (NSRange* p = i1; p; p = p->next) {
          NSRange* q = new NSRange(p->l, p->u);
          last->next = q;
          last = q;
        }
      else if (i2)
        for (RangeList* p = i2; p; p = p->next()) {
          NSRange* q = new NSRange(p->min(), p->max());
          last->next = q;
          last = q;
        }
      int size = 0, len = 0;
      NSRange* p = first;
      for (; p; p = p->next) {
        NSRange* q = p->next;
        while (q && p->u >= q->l - 1) {
          p->u = q->u;
          p->next = q->next;
          delete q;
          q = p->next;
        }
        ++len;
        size += p->u - p->l + 1;
        last = p;
      }
      this->clear();
      this->_fst = first;
      this->_lst = last;
      this->_len = len;
      this->_size = size;
    }

    forceinline void
    NSIntSet::add(int n) {
      if (this->empty()) {
        this->_fst = new NSRange(n, n);
        this->_lst = this->_fst;
        this->_len = this->_size = 1;
        return;
      }
      if (n < this->min()) {
        if (n == this->min() - 1)
          _fst->l--;
        else {
          _len++;
          NSRange* p = _fst;
          _fst = new NSRange(n, n, p);
        }
        _size++;
        return;
      }
      if (n > this->max()) {
        if (n == this->max() + 1)
          _lst->u++;
        else {
          _len++;
          NSRange* p = _lst;
          _lst = new NSRange(n, n);
          p->next = _lst;
        }
        _size++;
        return;
      }
      NSRange* p = _fst;
      NSRange* q = p;
      // NOTE: This can be done in O(log n) with a doubly-linked list.
      for (; q; p = q) {
        if (p->l <= n && n <= p->u)
          return;
        q = p->next;
        if (q && p->u < n && n < q->l) {
          if (p->u == n - 1) {
            if (q->l == n + 1) {
              p->next = q->next;
              p->u = q->u;
              if (q == _lst)
                _lst = p;
              delete q;
              _len--;
            }
            else
              p->u++;
          }
          else if (q->l == n + 1)
            q->l--;
          else {
            NSRange* r = new NSRange(n, n, q);
            p->next = r;
            _len++;
          }
          _size++;
          return;
        }
      }
    }

    forceinline void
    NSIntSet::shift(int n) {
      if (this->empty() || n == 0)
        return;
      NSRange* first = NULL;
      NSRange* last = NULL;
      int size = 0;
      int len = 0;
      const long max = DashedString::_MAX_STR_ALPHA;
      for (NSRange* range = _fst; range; ) {
        NSRange* next = range->next;
        const long l = long(range->l) + n;
        const long u = long(range->u) + n;
        if (u < 0 || l > max) {
          delete range;
        } else {
          range->l = std::max(l, 0L);
          range->u = std::min(u, max);
          range->next = NULL;
          if (last)
            last->next = range;
          else
            first = range;
          last = range;
          size += range->u - range->l + 1;
          ++len;
        }
        range = next;
      }
      _fst = first;
      _lst = last;
      _size = size;
      _len = len;
    }

    forceinline
    NSIntSet::NSIntSet(const NSIntSet& that)
    : _fst(NULL), _lst(NULL), _size(0), _len(0) {
      NSIntSet copy;
      for (NSRange* range = that._fst; range; range = range->next) {
        NSRange* node = new NSRange(range->l, range->u);
        if (copy._lst)
          copy._lst->next = node;
        else
          copy._fst = node;
        copy._lst = node;
      }
      copy._size = that._size;
      copy._len = that._len;
      *this = std::move(copy);
    }

    forceinline
    NSIntSet::NSIntSet(NSIntSet&& that) noexcept
    : _fst(that._fst), _lst(that._lst),
      _size(that._size), _len(that._len) {
      that._fst = that._lst = NULL;
      that._size = that._len = 0;
    }

    forceinline NSIntSet&
    NSIntSet::operator=(const NSIntSet& that) {
      if (this != &that) {
        NSIntSet copy(that);
        *this = std::move(copy);
      }
      return *this;
    }

    forceinline NSIntSet&
    NSIntSet::operator=(NSIntSet&& that) noexcept {
      if (this != &that) {
        clear();
        _fst = that._fst;
        _lst = that._lst;
        _size = that._size;
        _len = that._len;
        that._fst = that._lst = NULL;
        that._size = that._len = 0;
      }
      return *this;
    }

    forceinline void
    NSIntSet::clear() {
      if (empty())
        return;
      NSRange* q = NULL;
      for (NSRange* p = _fst; p; p = q) {
        q = p->next;
        delete p;
      }
      _fst = _lst = NULL;
      _len = _size = 0;
    }

    forceinline bool
    NSIntSet::operator==(const DSIntSet& s) const {
      if (this->size() != (int) s.size())
        return false;
      Gecode::Set::BndSetRanges i(s);
      NSRange* p = _fst;
      while (p && i()) {
        if (p->l != i.min() || p->u != i.max())
          return false;
        p = p->next;
        ++i;
      }
      return !p && !i();
    }

    forceinline bool
    NSIntSet::operator==(const NSIntSet& that) const {
      if (_size != that._size)
        return false;
      NSRange* i1 = this->_fst;
      NSRange* i2 = that ._fst;
      while (i1 && i2) {
        if (i1->l != i2->l || i1->u != i2->u)
          return false;
        i1 = i1->next;
        i2 = i2->next;
      }
      return !i1 && !i2;
    }

    forceinline int
    NSIntSet::nth(int n) const {
      if (n < 1 || n > _size)
        return 0;
      for (NSRange* p = _fst; p; p = p->next) {
        int m = n - p->u + p->l - 1;
        if (m <= 0)
          return p->l + n - 1;
        n = m;
      }
      GECODE_NEVER;
      return -1;
    }

    forceinline
    NSIntSet::~NSIntSet() {
      clear();
    }

}}

namespace Gecode { namespace String {

  /**
   * \brief Ordinary-lifetime dashed block
   *
   * The block represents between \a l and \a u consecutive symbols, each
   * drawn from \a S. A null block represents the empty contribution.
   */
  struct NSBlock {

    NSIntSet S;

    int l, u;

    forceinline static NSBlock top();

    forceinline bool
    isTop() const {
      return l == 0 && u == DashedString::_MAX_STR_LENGTH &&
        S.size() == (int) DashedString::_MAX_STR_ALPHA + 1 &&
        S.min() == 0 && S.max() == DashedString::_MAX_STR_ALPHA;
    }

    forceinline
    NSBlock(): S(), l(0), u(0) {}

    forceinline
    NSBlock(unsigned x): S(x), l(1), u(1) {}

    forceinline
    NSBlock(const NSIntSet& s, int a): S(s) {
      if (a < 0)
        a = 0;
      if (a > DashedString::_MAX_STR_LENGTH)
        a = DashedString::_MAX_STR_LENGTH;
      l = a;
      u = a;
    }

    forceinline
    NSBlock(NSIntSet&& s, int a): S(std::move(s)) {
      if (a < 0)
        a = 0;
      if (a > DashedString::_MAX_STR_LENGTH)
        a = DashedString::_MAX_STR_LENGTH;
      l = a;
      u = a;
    }

    forceinline
    NSBlock(const NSIntSet& s, int a, int b): S(), l(0), u(0) {
       if (b == 0 || a > b || s.size() == 0)
        return;
       if (a < 0)
         a = 0;
       if (b > DashedString::_MAX_STR_LENGTH)
         b = DashedString::_MAX_STR_LENGTH;
       l = a;
       u = b;
       S = s;
    }

    forceinline
    NSBlock(NSIntSet&& s, int a, int b): S(), l(0), u(0) {
       if (b == 0 || a > b || s.size() == 0)
        return;
       if (a < 0)
         a = 0;
       if (b > DashedString::_MAX_STR_LENGTH)
         b = DashedString::_MAX_STR_LENGTH;
       l = a;
       u = b;
       S = std::move(s);
    }

    forceinline explicit
    NSBlock(const DSIntSet& s, int a, int b): S(), l(0), u(0) {
       if (b == 0 || a > b || s.size() == 0)
        return;
       if (a < 0)
         a = 0;
       if (b > DashedString::_MAX_STR_LENGTH)
         b = DashedString::_MAX_STR_LENGTH;
       l = a;
       u = b;
       S = NSIntSet(s);
    }

    forceinline explicit
    NSBlock(const DSBlock& block): S(block.S), l(block.l), u(block.u) {}

    template <class Block>
    forceinline void
    update(Space&, const Block& block) {
      *this = block;
    }

    forceinline bool
    known() const {
      return l == u && S.size() <= 1;
    }

    forceinline bool
    null() const {
      return u == 0 || l > u || S.empty();
    }

    forceinline double
    logdim() const {
      if (null())
      return 0;
      if (S.size() == 1)
        return std::log(u - l + 1);
      else {
        long double s = S.size();
        return u * std::log(s) + std::log(1.0 - std::pow(s, l - u - 1))
          - std::log(1 - 1 / s);
      }
    }

    forceinline bool
    operator==(const NSBlock& that) const {
      return l == that.l && u == that.u && S == that.S;
    }
    forceinline bool
    operator==(const DSBlock& that) const {
      return l == that.l && u == that.u && S == that.S;
    }

  };

  forceinline std::ostream&
  operator<<(std::ostream& os, const NSBlock& s) {
    return os << s.S << "^(" << s.l << ", "<< s.u << ")";
  }

  forceinline NSBlock
  NSBlock::top() {
    return NSBlock(NSIntSet::top(), 0, DashedString::_MAX_STR_LENGTH);
  }


}}


namespace Gecode { namespace String {

  /**
   * \brief Ordinary-lifetime sequence of dashed blocks
   *
   * NSBlocks owns all character ranges through normal C++ lifetime rules.
   * It is used for temporary analysis and must be normalized before replacing
   * a space-owned DashedString domain.
   */
  class NSBlocks : public std::vector<NSBlock> {

  public:

    forceinline
    NSBlocks(): std::vector<NSBlock>() {}

    forceinline explicit
    NSBlocks(const DSBlocks& blocks): std::vector<NSBlock>() {
      reserve(blocks.length());
      int n = 0;
      for (int i = 0; i < blocks.length(); ++i) {
        emplace_back(blocks.at(i));
        n += back().l;
        if (n > DashedString::_MAX_STR_LENGTH)
          throw OutOfLimitsDS("NSBlocks::NSBlocks");
      }
    }

    forceinline explicit
    NSBlocks(const DashedView& dv): std::vector<NSBlock>() {
      reserve(dv.length());
      for (int i = 0; i < dv.length(); ++i)
        emplace_back(dv.at(i));
    }

    forceinline explicit
    NSBlocks(const DashedString& ds): NSBlocks(ds.blocks()) {}

    forceinline
    NSBlocks(const std::vector<NSBlock>& blocks)
    : std::vector<NSBlock>(blocks) {}

    forceinline explicit
    NSBlocks(const std::vector<StringSymbol>& symbols)
    : std::vector<NSBlock>() {
      if (symbols.size() >
          static_cast<std::vector<StringSymbol>::size_type>(
            DashedString::_MAX_STR_LENGTH))
        throw OutOfLimitsDS("NSBlocks::NSBlocks");
      for (std::vector<StringSymbol>::const_iterator i = symbols.begin();
           i != symbols.end(); ++i) {
        if (!empty() && back().S.size() == 1 && back().S.min() == *i) {
          ++back().l;
          ++back().u;
        }
        else
          emplace_back(NSIntSet(*i), 1, 1);
      }
      if (empty())
        emplace_back();
    }

    forceinline
    NSBlocks(std::vector<NSBlock>&& blocks)
    : std::vector<NSBlock>(std::move(blocks)) {}

    forceinline
    NSBlocks(int n)
    : std::vector<NSBlock>(n) {}

    forceinline
    NSBlocks(int n, const NSBlock& b)
    : std::vector<NSBlock>(n, b) {}

    forceinline explicit
    NSBlocks(const std::string& s): std::vector<NSBlock>() {
      int n = s.size();
      if (n > DashedString::_MAX_STR_LENGTH)
        throw OutOfLimitsDS("NSBlocks::NSBlocks");
      if (n == 0) {
        emplace_back();
        return;
      }
      unsigned prev_c = static_cast<unsigned char>(s[0]) + 1;
      for (unsigned char c : s) {
        if (c == prev_c) {
          this->back().u++;
          this->back().l++;
        }
        else
          emplace_back(NSIntSet(c), 1, 1);
        prev_c = c;
      }
    }

    forceinline bool
    isTop() const {
      return size() == 1 && at(0).isTop();
    }

    forceinline bool
    null() const {
      return size() == 1 && this->front().null();
    }

    forceinline bool
    known() const {
      for (unsigned i = 0; i < size(); ++i) {
        if (!at(i).known())
          return false;
      }
      return true;
    }

    /// Extract assigned legacy bytes, returning false for non-byte symbols
    forceinline bool
    try_val_bytes(string& value) const {
      if (!known())
        throw UnknownValDS("NSBlocks::try_val_bytes");
      value.clear();
      string::size_type length = 0;
      for (const NSBlock& block : *this)
        length += block.l;
      value.reserve(length);
      for (const NSBlock& block : *this) {
        if (block.null())
          continue;
        const int symbol = block.S.min();
        if (symbol < 0 || symbol > 255) {
          value.clear();
          return false;
        }
        value.append(block.l,
          static_cast<char>(static_cast<unsigned char>(symbol)));
      }
      return true;
    }

    /// Return assigned legacy bytes or throw for non-byte symbols
    forceinline string
    val() const {
      string value;
      if (!try_val_bytes(value))
        throw OutOfLimitsDS("NSBlocks::val");
      return value;
    }

    /// Return the assigned encoding-neutral scalar-value sequence
    forceinline StringVal
    val_symbols() const {
      if (!known())
        throw UnknownValDS("NSBlocks::val_symbols");
      std::vector<StringSymbol> symbols;
      for (const NSBlock& block : *this)
        if (!block.null())
          symbols.insert(symbols.end(), block.l, block.S.min());
      return StringVal::from_symbols(std::move(symbols));
    }

    forceinline int
    length() const {
      return size();
    }

    forceinline NSIntSet
    may_chars() const {
      NSIntSet set;
      for (const NSBlock& block : *this)
        set.include(block.S);
      return set;
    }

    template <typename Block, typename Blocks>
    forceinline bool
    contains(const Blocks& that) const {
      if (this->isTop()) {
        long upper = 0;
        for (int i = 0; i < that.length(); ++i) {
          const Block& block = that.at(i);
          if (block.null())
            continue;
          upper += block.u;
          if (upper > DashedString::_MAX_STR_LENGTH ||
              !this->at(0).S.contains(block.S))
            return false;
        }
        return true;
      }
      int n1 = this->length(), n2 = that.length();
      if (n1 < n2)
        return false;
      for (int i = 0; i < n2; ++i) {
        const NSBlock& b1 = this->at(i);
        const Block& b2 = that .at(i);
        if (!b1.S.contains(b2.S) || b1.l > b2.l || b1.u < b2.u)
          return false;
      }
      for (int i = n2; i < n1; ++i)
        if (this->at(i).l > 0)
          return false;
      return true;
    }

    forceinline void
    push_front(const NSBlock& b) {
      NSBlock front(b);
      insert(begin(), std::move(front));
    }

    /// Drop null blocks and merge adjacent blocks with equal symbol sets
    forceinline void
    normalize() {
      int n = size();
      int write = 0;
      const long M = DashedString::_MAX_STR_LENGTH;
      for (int read = 0; read < n; ++read) {
        NSBlock& block = at(read);
        if (block.null()) {
          if (write > 0 && !block.S.empty()) {
            NSBlock& previous = at(write - 1);
            previous.l = std::min(long(previous.l) + block.l, M);
            previous.u = std::min(long(previous.u) + block.u, M);
          }
          continue;
        }
        if (write > 0 && at(write - 1).S == block.S) {
          NSBlock& previous = at(write - 1);
          previous.l = std::min(long(previous.l) + block.l, M);
          previous.u = std::min(long(previous.u) + block.u, M);
        } else {
          if (write != read)
            at(write) = std::move(block);
          ++write;
        }
      }
      resize(write);
      if (write == 0)
        emplace_back();
      assert (is_normalized());
    }

    forceinline bool
    is_normalized() const {
      int n = size();
      for (int i = 1; i < n; ++i) {
        const NSBlock& b = at(i);
        if (b.null() || b.S == at(i - 1).S)
          return false;
      }
      const NSBlock& b = at(0);
      return b.null() ? n == 1 && b.l == 0 && b.u == 0 : b.l <= b.u;
    }

    forceinline NSBlocks
    slice(int i, int j) const {
      return NSBlocks(std::vector<NSBlock>(begin() + i, begin() + j));
    }

    /**
     * Concatenate this sequence and \a y into \a z.
     *
     * Aliasing \a z with \a y is supported.
     */
    forceinline void
    concat(const NSBlocks& y, NSBlocks& z) const {
      if (&z == &y) {
        NSBlocks right(y);
        z = *this;
        z.insert(z.end(), std::make_move_iterator(right.begin()),
          std::make_move_iterator(right.end()));
        return;
      }
      z = *this;
      z.insert(z.end(), y.begin(), y.end());
    }

    forceinline void
    extend(const NSBlocks& x) {
      if (this == &x) {
        NSBlocks copy(x);
        this->insert(this->end(), std::make_move_iterator(copy.begin()),
          std::make_move_iterator(copy.end()));
        return;
      }
      this->insert(this->end(), x.begin(), x.end());
    }

    forceinline void
    extend(NSBlocks&& x) {
      if (this == &x) {
        extend(static_cast<const NSBlocks&>(x));
        return;
      }
      this->insert(this->end(),
        std::make_move_iterator(x.begin()), std::make_move_iterator(x.end()));
    }

    forceinline void
    extend(const DSBlocks& blocks) {
      for (int i = 0; i < blocks.length(); ++i)
        emplace_back(blocks.at(i));
    }

    forceinline void
    reverse() {
      int n = size(), m = n / 2;
      for (int i = 0; i < m; ++i)
        std::swap(this->at(i), this->at(n - i - 1));
    }

    forceinline double
    logdim() const {
      double d = 0.0;
       for (unsigned i = 0; i < this->size(); ++i) {
         const NSBlock& b = at(i);
         d += b.logdim();
         if (std::isinf(d))
           return d;
       }
       return d;
     }

    forceinline string
    known_pref() const {
      unsigned end = 0;
      string::size_type length = 0;
      for (; end < this->size(); ++end) {
        const NSBlock& b = at(end);
        if (b.null())
          continue;
        if (b.S.size() > 1)
          break;
        length += b.l;
        if (b.l < b.u) {
          ++end;
          break;
        }
      }
      string pref;
      pref.reserve(length);
      for (unsigned i = 0; i < end; ++i) {
        const NSBlock& b = at(i);
        if (!b.null()) {
          const int symbol = b.S.min();
          if (symbol < 0 || symbol > 255)
            throw OutOfLimitsDS("NSBlocks::known_pref");
          pref.append(b.l,
            static_cast<char>(static_cast<unsigned char>(symbol)));
        }
      }
      return pref;
    }

    forceinline std::vector<StringSymbol>
    known_pref_symbols() const {
      unsigned end = 0;
      std::vector<StringSymbol>::size_type length = 0;
      for (; end < this->size(); ++end) {
        const NSBlock& b = at(end);
        if (b.null())
          continue;
        if (b.S.size() > 1)
          break;
        length += static_cast<std::vector<StringSymbol>::size_type>(b.l);
        if (b.l < b.u) {
          ++end;
          break;
        }
      }
      std::vector<StringSymbol> prefix;
      prefix.reserve(length);
      for (unsigned i = 0; i < end; ++i) {
        const NSBlock& b = at(i);
        if (!b.null())
          prefix.insert(prefix.end(), b.l, b.S.min());
      }
      return prefix;
    }

    forceinline string
    known_suff() const {
      int begin = this->size();
      string::size_type length = 0;
      for (int i = begin - 1; i >= 0; --i) {
        const NSBlock& b = at(i);
        if (b.null())
          continue;
        if (b.S.size() > 1)
          break;
        begin = i;
        length += b.l;
        if (b.l < b.u)
          break;
      }
      string suff;
      suff.reserve(length);
      for (int i = begin; i < static_cast<int>(this->size()); ++i) {
        const NSBlock& b = at(i);
        if (!b.null()) {
          const int symbol = b.S.min();
          if (symbol < 0 || symbol > 255)
            throw OutOfLimitsDS("NSBlocks::known_suff");
          suff.append(b.l,
            static_cast<char>(static_cast<unsigned char>(symbol)));
        }
      }
      return suff;
    }

    forceinline std::vector<StringSymbol>
    known_suff_symbols() const {
      int begin = this->size();
      std::vector<StringSymbol>::size_type length = 0;
      for (int i = begin - 1; i >= 0; --i) {
        const NSBlock& b = at(i);
        if (b.null())
          continue;
        if (b.S.size() > 1)
          break;
        begin = i;
        length += static_cast<std::vector<StringSymbol>::size_type>(b.l);
        if (b.l < b.u)
          break;
      }
      std::vector<StringSymbol> suffix;
      suffix.reserve(length);
      for (int i = begin; i < static_cast<int>(this->size()); ++i) {
        const NSBlock& b = at(i);
        if (!b.null())
          suffix.insert(suffix.end(), b.l, b.S.min());
      }
      return suffix;
    }

    friend std::ostream& operator<<(std::ostream& os, const NSBlocks& v);

  };

  forceinline std::ostream&
  operator<<(std::ostream& os, const NSBlocks& v) {
    if (v.empty())
      return os << "[]";
    os << "[";
    int n = v.size() - 1;
    for (int i = 0; i < n; ++i)
      os << v[i] << ", ";
    return os << v[n] << ")]";
  }

}}

#endif

// STATISTICS: string-var
