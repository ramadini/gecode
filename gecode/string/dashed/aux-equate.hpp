#include <gecode/string/dashed/vec.h>

namespace Gecode { namespace String {  
  
  typedef vec<std::pair<int, NSBlocks>> uvec;
  
  // Helpers.
  forceinline int lower(const DSBlock& b) { return b.l; }
  forceinline int lower(const NSBlock& b) { return b.l; }
  forceinline int lower(char) { return 1; }
  forceinline int upper(const DSBlock& b) { return b.u; }
  forceinline int upper(const NSBlock& b) { return b.u; }
  forceinline int upper(char) { return 1; }
  forceinline bool disjoint(char c1, char c2) { return c1 != c2; }
  forceinline bool disjoint(char c, const DSBlock& b) { return !b.S.in(c); }
  forceinline bool disjoint(const DSBlock& b, char c) { return !b.S.in(c); }
  forceinline bool disjoint(char c, const NSBlock& b) { return !b.S.in(c); }
  forceinline bool disjoint(const NSBlock& b, char c) { return !b.S.in(c); }
  forceinline bool disjoint(const DSBlock& b1, const DSBlock& b2) { 
    return b1.S.disjoint(b2.S); 
  }
  forceinline bool disjoint(const DSBlock& b1, const NSBlock& b2) { 
    return b1.S.disjoint(b2.S); 
  }
  forceinline bool disjoint(const NSBlock& b1, const DSBlock& b2) { 
    return b1.S.disjoint(b2.S); 
  }
  forceinline bool disjoint(const NSBlock& b1, const NSBlock& b2) { 
    return b1.S.disjoint(b2.S); 
  }

  struct Position {
    int idx; // Block
    int off; // How far into the block?

    forceinline bool
    operator==(const Position& x) const {
      return idx == x.idx && off == x.off;
    }

    forceinline bool
    operator!=(const Position& x) const {
      return idx != x.idx || off != x.off;
    }
    
  };
  forceinline std::ostream&
  operator<<(std::ostream& os, const Position& p) {
    os << p.idx << "->" << p.off;
    return os;
  }

  struct Fwd {

    forceinline static int
    prev(int x) {
      return x - 1;
    }

    forceinline static int
    next(int x) {
      return x + 1;
    }

    forceinline static bool
    lt(const Position& x, const Position& y) {
      if (x.idx == y.idx)
        return x.off < y.off;
      return x.idx < y.idx;
    }
    
    forceinline static bool
    le(const Position& x, const Position& y, int u) {
      return x == y || (x.idx == y.idx + 1 && (x.off == 0 && y.off == u)) || 
        lt(x, y);
    }

  };

  struct Bwd {

    forceinline static int
    prev(int x) {
      return x + 1;
    }

    forceinline static int
    next(int x) {
      return x - 1;
    }

    forceinline static bool
    lt(const Position& x, const Position& y) {
      if (x.idx == y.idx)
        return x.off < y.off;
      return x.idx > y.idx;
    }
    
    forceinline static bool
    le(const Position& x, const Position& y, int u) {
      return x == y || (x.idx == y.idx - 1 && x.off == 0 && y.off == u) 
        || lt(x, y);        
    }

  };

  // Macro to shift position P to the start of the following block, aborting if 
  // we reach end position E.
  #define NEXT(P, E) \
  do { \
    (P).idx = Succ::next((P).idx); \
    (P).off = 0 ; \
    if(!Succ::lt(P, E)) { (P) = (E); return (E); } \
  } while(0)

  template <class Blocks>
  forceinline Position
  first_bwd(const Blocks& x) {
    return x.length() == 0 ? Position {0, 0} 
                           : Position { (int) x.length() - 1, 0 };
  }

  template <class Blocks>
  forceinline Position
  last_fwd(const Blocks& x) {
    return x.length() == 0 ? Position {0, 0} 
                           : Position { (int) x.length() - 1, upper(x.back()) };
  }

  template <class Blocks>
  forceinline Position
  last_bwd(const Blocks& x) {
    return Position { 0, upper(x.front()) };
  }
  
  struct matching {
    vec<Position> esp;
    vec<Position> lep;
  };

  struct sweep_stack: public vec<int> {

    void sweep_push(int i) {
      if (size() > 0 && last() == i)
        return;
      push(i);
    }
    
    int& top() { return last(); }
    
    private:
      int& last() { return vec<int>::last(); };

  };  
  
  // For figuring out lsp & eep, need to switch between prefixes and suffixes.
  template <class Blocks>
  forceinline Position
  dual(const Blocks& x, const Position& p) {
    if (p.idx < 0 || (int) x.length() <= p.idx)
      return p;
    return Position { p.idx, upper(x.at(p.idx)) - p.off };
  }

  struct DashedView {
    virtual const DSBlock& at(int) const = 0;
    virtual DSBlock& at(int i) = 0;
    virtual int length() const = 0;
    int size() const { return length(); }
    const DSBlock& front() const { return at(0); }
    const DSBlock& back() const  { return at(length()-1); }    
  };

  class ConcatView : public DashedView {
  private:
    int l;
    int lx;
    DashedString& x;
    DashedString& y;
    friend std::ostream& operator<<(std::ostream& os, const ConcatView& that);
  public:
    
    ConcatView(DashedString& a, DashedString& b)
    : l(a.length() + b.length()), lx(a.length()), x(a), y(b) {};

    forceinline const DSBlock&
    at(int i) const {
      return i < lx ? x.blocks().at(i) : y.blocks().at(i - lx);
    }
    forceinline DSBlock&
    at(int i) {
      return i < lx ? x.blocks().at(i) : y.blocks().at(i - lx);
    }

    forceinline int
    length() const {
      return l;
    }

  };
  forceinline std::ostream&
  operator<<(std::ostream& os, const ConcatView& xy) {
    os << xy.x << "  ++  " << xy.y;
    return os;
  }

  class ReverseView : public DashedView {
    const int n;
    DashedString& x;
    friend std::ostream& operator<<(std::ostream& os, const ReverseView& that);
  public:
    ReverseView(DashedString& a): n(a.length()), x(a) {};

    forceinline const DSBlock&
    at(int i) const {
      return x.at(n - i - 1);
    }
    forceinline DSBlock&
    at(int i) {
      return x.at(n - i - 1);
    }

    forceinline int
    length() const {
      return n;
    }

  };
  forceinline std::ostream&
  operator<<(std::ostream& os, const ReverseView& x) {
    for (int i = x.n - 1; i >= 0; --i)
      os << x.x.at(i);
    return os;
  }
  
}}
