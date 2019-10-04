#ifndef __GECODE_STRING_NS_DASHED_HH
#define __GECODE_STRING_NS_DASHED_HH

namespace Gecode { namespace String {

  struct NSRange {

    int l, u;

    NSRange* next;

    NSRange(): l(-1), u(-1), next(NULL) {};

    NSRange(int a): l(a), u(a), next(NULL) {};

    NSRange(int a, int b): l(a), u(b), next(NULL) {};

    NSRange(int a, int b, NSRange* p): l(a), u(b), next(p) {};

  };

}}

namespace Gecode { namespace String {

  class NSIntSet {

  private:

    NSRange* _fst;

    NSRange* _lst;

    int _size;

    int _len;

  public:

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
    
    bool consistent() const;

    NSIntSet& operator=(const NSIntSet& that);

    bool operator==(const NSIntSet& that) const;
    bool operator==(const DSIntSet& that) const;

    NSRange* first() const;

    NSRange* last() const;

    int min() const;

    int max() const;

    int size() const;

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

    NSIntSet comp() const;

    int nth(int n) const;

    ~NSIntSet();

    friend std::ostream& operator<<(std::ostream& os, const NSIntSet& that);

    static NSIntSet top();

    iterator begin() const;

    iterator end() const;

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
        char mi = unsigned(p->l);
        char ma = unsigned(p->u);
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

    forceinline
    NSIntSet::NSIntSet(): _fst(NULL), _lst(NULL), _size(0), _len(0) {}

    forceinline
    NSIntSet::NSIntSet(const DSIntSet& s)
    : _fst(NULL), _lst(NULL), _size(s.size()), _len(0) {
      if (!s.empty()) {
        int len = 0;
        Gecode::Set::BndSetRanges i(s);
        NSRange* p = new NSRange(i.min(), i.max());
        _fst = p;
        ++i;
        ++len;
        while (i()) {
          NSRange* q = new NSRange(i.min(), i.max());
          p->next = q;
          p = q;
          ++i;
          ++len;
        }
        _lst = p;
        _len = len;
      }
    }

    forceinline
    NSIntSet::NSIntSet(int l) {
      // std::cerr << "NSIntSet::NSIntSet(int l) " << l << "\n";
      if (l < 0)
        throw OutOfLimitsDS("NSIntSet::NSIntSet");
      NSRange* p = new NSRange(l, l);
      _fst = _lst = p;
      _size = 1;
      _len = 1;
    }

    forceinline
    NSIntSet::NSIntSet(int l, int u) {
      // std::cerr << "NSIntSet::NSIntSet(int l, int u)" << l << " " << u << "\n";
      if (l < 0)
        throw OutOfLimitsDS("NSIntSet::NSIntSet");
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
      int l = 0, s = 0;
      for (NSRange* p = _fst; p; p = p->next) {
        if (p->l > p->u)
          return false;
        ++l;
        s += p->u - p->l + 1;    
      }
      return l == _len && s == _size;     
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
      // std::cerr << "NSIntSet::empty()\n";
      return _fst == NULL && _lst == NULL && _len == 0 && _size == 0;
    }

    forceinline bool
    NSIntSet::disjoint(const NSIntSet& that) const {
      if (this->empty() || that.empty()
      ||  this->max() < that.min() || this->min() > that.max())
        return true;
      NSRange* i1 = this->_fst;
      NSRange* i2 = that ._fst;
      while (i1 && i2) {
        if ((i1->l <= i2->l && i2->l <= i1->u)
        ||  (i1->l <= i2->u && i2->u <= i1->u)
        ||  (i2->l <= i1->l && i1->l <= i2->u)
        ||  (i2->l <= i1->u && i1->u <= i2->u))
          return false;
      else
        if (i1->u > i2->u)
          i2 = i2->next;
        else
          i1 = i1->next;
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
	    // std::cerr << "NSIntSet::intersectNS " << *this << " " << that << "\n";
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
      NSRange* first = NULL;
      NSRange* last = NULL;
      int size = 0, len = 0;
      while (i1 && i2) {
        if (i1->l > i2->u)
          i2 = i2->next;
        else if (i1->u < i2->l)
          i1 = i1->next;
        else {
          int l = std::max(i1->l, i2->l);
          int u = std::min(i1->u, i2->u);
          size += u - l + 1;
          ++len;
          NSRange* p = new NSRange(l, u);
          if (first)
            last->next = p;
          else
            first = p;
          last = p;
          if (i1->u > i2->u)
            i2 = i2->next;
          else
            i1 = i1->next;
        }
      }
      this->clear();
      if (last)
        last->next = NULL;
      this->_fst  = first;
      this->_lst  = last;
      this->_len  = len;
      this->_size = size;
      // std::cerr << "intersected " << *this << "\n";
    }

    forceinline void
    NSIntSet::intersect(const DSIntSet& that) {
      // std::cerr << "NSIntSet::intersectDS " << *this << " " << that << "\n";
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
      NSRange* first = NULL;
      NSRange* last = NULL;
      int size = 0, len = 0;
      while (i1 && i2) {
        if (i1->l > i2->max())
          i2 = i2->next();
        else if (i1->u < i2->min())
          i1 = i1->next;
        else {
          int l = std::max(i1->l, i2->min());
          int u = std::min(i1->u, i2->max());
          size += u - l + 1;
          ++len;
          NSRange* p = new NSRange(l, u);
          if (first)
            last->next = p;
          else
            first = p;
          last = p;
          if (i1->u > i2->max())
            i2 = i2->next();
          else
            i1 = i1->next;
        }
      }
      this->clear();
      if (last)
        last->next = NULL;
      this->_fst  = first;
      this->_lst  = last;
      this->_len  = len;
      this->_size = size;
      // std::cerr << "intersected " << *this << "\n";
    }

    forceinline NSIntSet
    NSIntSet::comp() const {
      NSIntSet c;
      if (*this == top())
        return c;
      if (empty())
        return top();
      c._fst = new NSRange();
      NSRange* p = _fst;
      if (min() > 0)
        c._fst->l = 0;
      else {
        c._fst->l = _fst->u + 1;
        p = p->next;
      }
      NSRange* q = c._fst;
      for (; p; p = p->next) {
        q->u = p->l - 1;
        c._size += q->u - q->l + 1;
        c._len++;
        if (p->u == DashedString::_MAX_STR_ALPHA)
          return c;
        q->next = new NSRange(p->u + 1);
        q = q->next;
	      c._lst = q;
      }
      q->u = DashedString::_MAX_STR_ALPHA;
      c._size += q->u - q->l + 1;
      c._len++;
      c._lst = q;
      return c;
    }

    forceinline void
    NSIntSet::exclude(const NSIntSet& that) {
      this->intersect(that.comp());
    }

    forceinline void
    NSIntSet::include(const NSIntSet& that) {
      //std::cerr<<"NSIntSet::includeNS "<<*this<<' '<<that<<std::endl;
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
      //std::cerr << "Included " << *this << "\n";
    }

    forceinline void
    NSIntSet::include(const DSIntSet& that) {
      // std::cerr<<"NSIntSet::includeDS "<<*this<<' '<<that<<std::endl;
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
    	    //std::cerr << "NSRange* p " << p->l << " " << p->u << "\n";
          NSRange* q = new NSRange(p->l, p->u);
          last->next = q;
          last = q;
        }
      else if (i2)
        for (RangeList* p = i2; p; p = p->next()) {
    	    //std::cerr << "RangeList* p " << p->min() << ".." << p->max() << "\n";
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
      // std::cerr << "Included " << *this << "\n";
    }

    forceinline void
    NSIntSet::add(int n) {
      //std::cerr<<"add "<<n<<" to "<<*this<<"\n";
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
      if (this->empty())
        return;
      for (NSRange* p = this->_fst; p; p = p->next) {
        if (p->l + n > DashedString::_MAX_STR_ALPHA) {
          for (NSRange* q = p->next; p; q = p->next) {
            this->_len--;
            this->_size -= p->u - p->l + 1;
            delete p;
            p = q;
          }
          return;
        }
        p->l += n;
        p->u += n;
        if (p->l < 0) {
          if (p->u < 0) {
            this->_len--;
            this->_size -= p->u - p->l + 1;
            this->_fst = p->next;
            delete p;
          }
          else {
            p->l = 0;
            this->_size += n;
          }
        }
        if (p->u + n >= DashedString::_MAX_STR_ALPHA) {
          this->_size -= p->u + n - DashedString::_MAX_STR_ALPHA;
          p->u = DashedString::_MAX_STR_ALPHA;
          return;
        }
      }
    }

    forceinline
    NSIntSet::NSIntSet(const NSIntSet& that)
    : _fst(NULL), _lst(NULL), _size(0), _len(0) {
      *this = that;
    }

    forceinline NSIntSet&
    NSIntSet::operator=(const NSIntSet& that) {
      // std::cerr << *this << " = " << that << '\n';
      if (this != &that) {
        if (that.empty()) {
          this->clear();
          return *this;
        }
        if (this->empty()) {
          NSRange* p = new NSRange(that._fst->l, that._fst->u);
          this->_fst = p;
          for (NSRange* q = that._fst->next; q; q = q->next) {
            NSRange* r = new NSRange(q->l, q->u);
            p->next = r;
            p = r;
          }
          this->_lst = p;
          this->_size = that._size;
          this->_len  = that._len;
          return *this;
        }
        NSRange* p = this->_fst;
        NSRange* q = that._fst;
        NSRange* r = NULL;
        for (; p && q; p = p->next, q = q->next) {
          p->l = q->l;
          p->u = q->u;
          r = p;
        }
        if (p)
          for (; p; p = q) {
            q = p->next;
            delete p;
          }
        else
          for (; q; q = q->next) {
            NSRange* s = new NSRange(q->l, q->u);
            r->next = s;
            r = s;
          }
        r->next = NULL;
        this->_lst = r;
        this->_size = that._size;
        this->_len  = that._len;
      }
      return *this;
    }

    forceinline void
    NSIntSet::clear() {
      // std::cerr << "NSIntSet::clear " << *this << '\n';
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

  struct NSBlock {

    NSIntSet S;

    int l, u;

    forceinline static NSBlock top();

    forceinline bool
    isTop() const {
      return l == 0 && u == DashedString::_MAX_STR_LENGTH &&
        S.size() == (int) DashedString::_MAX_STR_ALPHA + 1;
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
        return log(u - l + 1);
      else {
        long double s = S.size();
        return u * log(s) + log(1.0 - std::pow(s, l - u - 1)) - log(1 - 1 / s);
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

  class NSBlocks : public std::vector<NSBlock> {

  public:

    forceinline
    NSBlocks(): std::vector<NSBlock>() {}

    forceinline explicit
    NSBlocks(const DSBlocks& blocks): std::vector<NSBlock>() {
      int n = 0;
      for (int i = 0; i < blocks.length(); ++i) {
        const NSBlock& b = NSBlock(blocks.at(i));
        n += b.l;
        if (n > DashedString::_MAX_STR_LENGTH)
          throw OutOfLimitsDS("NSBlocks::NSBlocks");
        this->push_back(b);
      }
    }
    
    forceinline explicit
    NSBlocks(const DashedView& dv): std::vector<NSBlock>() {
      for (int i = 0; i < dv.length(); ++i)
        push_back(NSBlock(dv.at(i)));
    }
    
    forceinline explicit
    NSBlocks(const DashedString& ds): NSBlocks(ds.blocks()) {}

    forceinline
    NSBlocks(const std::vector<NSBlock>& blocks)
    : std::vector<NSBlock>(blocks) {}

    forceinline
    NSBlocks(int n): std::vector<NSBlock>(n) {
      if (n < 0 || n > DashedString::_MAX_STR_LENGTH)
        throw OutOfLimitsDS("NSBlocks::NSBlocks");
    }

    forceinline
    NSBlocks(int n, const NSBlock& b): std::vector<NSBlock>(n, b) {
      if (n < 0 || n > DashedString::_MAX_STR_LENGTH)
        throw OutOfLimitsDS("NSBlocks::NSBlocks");
    }

    forceinline explicit
    NSBlocks(const std::string& s): std::vector<NSBlock>() {
      int n = s.size();
      if (n > DashedString::_MAX_STR_LENGTH)
        throw OutOfLimitsDS("NSBlocks::NSBlocks");
      if (n == 0) {
        push_back(NSBlock());
        return;
      }
      unsigned prev_c = s[0] + 1;
      for (unsigned c : s) {
        if (c == prev_c) {
          this->back().u++;
          this->back().l++;
        }
        else
          push_back(NSBlock(NSIntSet(c), 1, 1));
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

    forceinline string
    val() const {
      if (!known())
        throw UnknownValDS("DashedString::val");
      if (null())
        return "";
      string s;
      for (unsigned i = 0; i < size(); ++i) {
        const NSBlock& b = at(i);
        s += string(b.l, unsigned(b.S.min()));
      }
      return s;
    }

    forceinline int
    length() const {
      return size();
    }

    template <typename Block, typename Blocks>
    forceinline bool
    contains(const Blocks& that) const {
      if (this->isTop())
        return true;
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
      NSBlocks v(this->size() + 1);
      for (unsigned i = 0; i < this->size(); ++i)
        v[i + 1] = this->at(i);
      *this = v;
      this->front() = b;
    }

    forceinline void
    normalize() {
      // std::cerr << *this << "\n";
      int n = size(), i = 0;
      long M = DashedString::_MAX_STR_LENGTH;
      while (i < n) {
        //std::cerr << at(i) << std::endl;
        if (at(i).null()) {
          erase(begin() + i);
          n--;
          continue;
        }
        while (i < n - 1 && (at(i + 1).null() || at(i).S == at(i + 1).S)) {
          if (!at(i + 1).S.empty()) {
            at(i).l = std::min(long(at(i).l) + at(i + 1).l, M);
            at(i).u = std::min(long(at(i).u) + at(i + 1).u, M);
          }
          erase(begin() + i + 1);
          n--;
        }
        ++i;
      }
      if (empty())
        *this = NSBlocks(1, NSBlock());
      // std::cerr << *this << "\n";
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

    forceinline void
    concat(const NSBlocks& y, NSBlocks& z) const {
      z = *this;
      z.insert(z.end(), y.begin(), y.end());
    }

    forceinline void
    extend(const NSBlocks& x) {
      this->insert(this->end(), x.begin(), x.end());
    }

    forceinline void
    reverse() {
      int n = size(), m = n / 2;
      for (int i = 0; i < m; ++i) {
        int j = n - i - 1;
        NSBlock b = this->at(i);
        this->at(i) = this->at(j);
        this->at(j) = b;
      }
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
      if (null())
        return "";
      string pref = "";
      for (unsigned i = 0; i < this->size(); ++i) {
        const NSBlock& b = at(i);
        if (b.S.size() > 1)
          return pref;
        pref += string(b.l, unsigned(b.S.min()));
        if (b.l < b.u)
          return pref;
      }
      return pref;
    }

    forceinline string
    known_suff() const {
      if (null())
        return "";
      string suff = "";
      for (int i = this->size() - 1; i >= 0; --i) {
        const NSBlock& b = at(i);
        if (b.S.size() > 1)
          return suff;
        suff = string(b.l, unsigned(b.S.min())) + suff;
        if (b.l < b.u)
          return suff;
      }
      return suff;
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
