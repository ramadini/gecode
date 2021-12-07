#include <gecode/int/arithmetic.hh>

namespace Gecode { namespace String { namespace RelOp {

  // x[OUT] is the string resulting from x[ORI] by replacing [first|last|all] 
  // occurrence of x[QRY] with x[RPL].
  #define ORI 0
  #define QRY 1
  #define RPL 2
  #define OUT 3

  template <class View>
  forceinline ExecStatus
  Replace<View>::post(Home home, ViewArray<View>& x, bool a, bool l) {
    if (x[QRY].same(x[RPL])) {
      eq(home, x[ORI], x[OUT]);
      return ES_OK;
    }
    else if (x[ORI].same(x[QRY])) {
      eq(home, x[RPL], x[OUT]);
      return ES_OK;
    }
    else if (x[ORI].same(x[OUT])) {
      find(home, x[ORI], x[QRY], IntVar(home, 0, 0));
      return ES_OK;
    }
    else if (x[QRY].same(x[OUT]))
      find(home, x[ORI], x[QRY], IntVar(home, 1, x[ORI].max_length()));
    if (!a) {
      // Enforcing |y| = |x| + [find(x,q) > 0]*(|q'|-|q|).
      IntVar lx(home,  x[ORI].min_length(), x[ORI].max_length());
      IntVar lq(home,  x[QRY].min_length(), x[QRY].max_length());
      IntVar lq1(home, x[RPL].min_length(), x[RPL].max_length());
      IntVar ly(home,  x[OUT].min_length(), x[OUT].max_length());
      // b = [find(q, x) > 0].
      IntVar z(home, 0, lx.max());
      find(home, x[ORI], x[QRY], z);
      BoolVar b(home, 0, 1);
      rel(home, z, IRT_GR, 0, b);
      // d = |q'| - |q|.
      IntVar d(home, lq1.min() - lq.max(), lq1.max() - lq.min());
      IntArgs ia;
      ia << 1 << 1 << -1;
      IntVarArgs iv;
      iv << d << lq << lq1;
      linear(home, ia, iv, IRT_EQ, 0);
      // lz = int(b) * d.
      IntVar lz(home, std::min(d.min(), 0), std::max(0, d.max()));
      IntVar i(home, 0, 1);
      channel(home, b, i);
      Gecode::Int::Arithmetic::MultBnd::post(home, i, d, lz);
      // ly = lx + lz.
      IntArgs ia1;
      ia1 << 1 << -1 << 1;
      IntVarArgs iv1;
      iv1 << lx << ly << lz;
      linear(home, ia1, iv1, IRT_EQ, 0);
    }
    (void) new (home) Replace<View>(home, x, a, l);
    return ES_OK;
  }
  
  template <class View>
  forceinline ExecStatus
  Replace<View>::rewrite(Home home, ViewArray<View>& x, bool last) {
    std::cerr << "Rewriting replace\n";
    StringVar pref(home), suff(home), tmp(home), tmp1(home);
    concat(home, pref, x[QRY], tmp);
    concat(home, tmp, suff, x[ORI]);
    concat(home, pref, x[RPL], tmp1);
    concat(home, tmp1, suff, x[OUT]);
    find(home, last ? suff : pref, x[QRY], IntVar(home, 0, 0));
    return ES_OK;
  }

  template <class View>
  forceinline Actor*
  Replace<View>::copy(Space& home) {
    return new (home) Replace(home, *this);
  }

  template <class View>
  forceinline
  Replace<View>::Replace(Home home, ViewArray<View>& x, bool a, bool l)
  : NaryPropagator<View, PC_STRING_ANY>(home, x), all(a), last(l) {}

  template <class View>
  forceinline
  Replace<View>::Replace(Space& home, Replace& p)
  : NaryPropagator<View, PC_STRING_ANY>(home, p), all(p.all), last(p.last) {}

  // The array of blocks starting at pref becomes the prefix x[:p].
  template <class View>
  forceinline void
  Replace<View>::prefix(Space& home, const View& x, const Position& p, 
                                                  Block* pref, int& k) const {
    assert (p.isNorm(x));
    if (p == Position(0,0))
      return;
    for (int i = 0; i < p.idx; ++i)
      pref[k++].update(home, x[i]);
    int off = p.off;
    if (off > 0) {
      const Block& b = x[p.idx];
      if (off < b.lb())
        pref[k++].updateCard(home, off, off);
      else
        pref[k++].ub(home, off);
    }
  }
  
  // The array of blocks starting at suff becomes the prefix x[p:].
  template <class View>
  forceinline void
  Replace<View>::suffix(Space& home, const View& x, const Position& p, 
                                                  Block* suff, int& k) const {
    assert (p.isNorm(x));
    int off = p.off, idx = p.idx, nx = x.size();
    if (idx == nx && off == 0)
      return;
    if (off > 0) {
      const Block& b = x[idx];
      suff[k++].updateCard(home, std::max(0, b.lb()-off), b.ub()-off);
    }
    for (int i = idx + (off > 0); i < nx; ++i)
      suff[k++].update(home, x[i]);
  }
  
  // Decomposes decomp_all into basic constraints.
  template <class View>
  forceinline ExecStatus
  Replace<View>::decomp_all(Space& home) {
//    std::cerr << "decomp_all\n";
    assert (x[ORI].assigned() && x[QRY].assigned());
    if (x[RPL].assigned()) {
      if (x[QRY].max_length() == 0) {
        if (x[RPL].max_length() == 0) {
          // x[QRY] = x[RPL] = "" -> x[ORI] = x[OUT].
          GECODE_ME_CHECK(x[OUT].equate(home, 
                                      ConstDashedView(x[ORI][0],x[ORI].size())));
          return home.ES_SUBSUMED(*this);
        }
        std::vector<int> wx = x[ORI].val();
        std::vector<int> wq1 = x[RPL].val();
        int wy[(wx.size()+1)* wq1.size()];
        int k = 0;
        for (size_t i = 0; i < wx.size(); ++i) {          
          for (size_t j = 0; j < wq1.size(); ++j)
            wy[k++] = wq1[j];
          wy[k++] = wx[i];
        }
        for (size_t j = 0; j < wq1.size(); ++j)
          wy[k++] = wq1[j];
        // x[QRY] = "" -> x[OUT] = x[RPL] x[ORI][0] x[RPL] x[ORI][1] ... x[ORI][-1] x[RPL].
        GECODE_ME_CHECK(x[OUT].equate(home, ConstStringView(home,wy,k)));
      }
      else {
        std::vector<int> wx = x[ORI].val();
        std::vector<int> wq = x[QRY].val();
        std::vector<int> wq1 = x[RPL].val();
        std::vector<int>::iterator pos = 
           std::search(wx.begin(), wx.end(), wq.begin(), wq.end());
        std::vector<int> idx;
        while (pos != wx.end()) { 
          idx.push_back(pos-wx.begin());
          pos = std::search(pos + wq.size(), wx.end(), wq.begin(), wq.end());
        }
         int k = 0, i0 = 0;
         int wy[wx.size() + idx.size() * (wq1.size()-wq.size())];
         for (auto i : idx) {
           for (int j = i0; j < i; ++j)
             wy[k++] = wx[j];
           for (auto c : wq1)
             wy[k++] = c;
           i0 = i + wq.size();
         }
         for (size_t i = i0; i < wx.size(); ++i)
           wy[k++] = wx[i];
        GECODE_ME_CHECK(x[OUT].equate(home, ConstStringView(home,wy,k)));
      }
    }
    else {
      if (x[QRY].max_length() == 0) {
        if (x[ORI].max_length() == 0) {
          eq(home, x[RPL], x[OUT]);
          return home.ES_SUBSUMED(*this);
        }
        // x[QRY] = "" -> x[OUT] = x[RPL] x[ORI][0] x[RPL] x[ORI][1] ... x[ORI][-1] x[RPL].
        std::vector<int> wx = x[ORI].val();
        int n = wx.size();
        StringVarArray v(home, n);
        concat(home, x[RPL], StringVar(home, wx[0]), v[0]);
        for (int i = 1; i < n; ++i) {
          StringVar tmp(home);
          concat(home, x[RPL], StringVar(home, wx[i]), tmp);
          concat(home, v[i-1], tmp, v[i]);
        }
        concat(home, v[n-1], x[RPL], x[OUT]);
      }
      else {
        std::vector<int> wx = x[ORI].val();
        std::vector<int> wq = x[QRY].val();
        std::vector<int>::iterator pos = 
          std::search(wx.begin(), wx.end(), wq.begin(), wq.end());        
        if (pos == wx.end()) {
          eq(home, x[ORI], x[OUT]);
          return home.ES_SUBSUMED(*this);
        }
        std::vector<StringVar> v(1, StringVar(home));
        std::vector<int> w = std::vector<int>(wx.begin(), pos);
        if (w.empty())
          // x[ORI] starts with x[QRY]
          eq(home, x[RPL], v[0]);
        else
          concat(home, StringVar(home, w), x[RPL], v[0]);
        size_t nq = wq.size();
        std::vector<int>::iterator pos1 = pos + nq;
        pos = std::search(pos1, wx.end(), wq.begin(), wq.end());
        while (pos != wx.end()) {
          StringVar lst = v.back();
          v.push_back(StringVar(home));
          if (pos > pos1) {
            StringVar tmp(home);
            concat(home, StringVar(home,std::vector<int>(pos1,pos)), x[RPL], tmp);
            concat(home, lst, tmp, v.back());
          }
          else
            concat(home, lst, x[RPL], v.back());
          pos1 = pos + nq;
          pos = std::search(pos1, wx.end(), wq.begin(), wq.end());
        }
        if (pos1 < wx.end())
          concat(home, v.back(), 
                 StringVar(home, std::vector<int>(pos1,wx.end())), x[OUT]);
        else
          eq(home, v.back(), x[OUT]);
      }
    }
//    std::cerr << "After decomp_all: " << x << "\n";
    return home.ES_SUBSUMED(*this);
  }

  // Checking if |y| = |x| + |q'| - |q| is consistent.
  template <class View>
  forceinline bool
  Replace<View>::check_card() const {
   int lx  = x[ORI].min_length(), lq = x[QRY].min_length(), lq1 = x[RPL].min_length(),
       ly  = x[OUT].min_length();
   long ux = x[ORI].max_length(), uq = x[QRY].max_length(), uq1 = x[RPL].max_length(),
        uy = x[OUT].max_length();
   return ly <= ux + uq1 - lq && uy >= lx + lq1 - uq;
  }
  
  // For replace, it returns 1 if x[QRY] must occur in x[ORI], 0 otherwise.
  // For decomp_all, it returns the minimum number of occurrences of q in x[ORI].
  template <class View>
  forceinline int
  Replace<View>::occur(Space& home) const {
    assert (x[QRY].assigned());
    if (x[ORI].assigned() && !all)
      return find_fixed(x[QRY], x[ORI]) > 0;
    int min_occur = 0, k = 0;
    std::vector<int> curr;
    for (int i = 0, nq = x[ORI].size(); i < nq; ++i) {
      const Block& b = x[ORI][i];
      if (b.lb() > 0 && b.baseSize() == 1) {
        std::vector<int> w(b.lb(), b.baseMin());
        curr.insert(curr.end(), w.begin(), w.end());
        k += b.lb();
        int i = find_fixed(ConstStringView(home,&curr[0],k), x[QRY]);
        while (i > 0) {
          if (!all)
            return 1;
          min_occur++;
          i = find_fixed(ConstStringView(home,&curr[0]+i+nq-1,k-i), x[QRY]);
        }
        if (b.lb() < b.ub()) {
          curr = w;
          k = b.lb();
        }
      }
      else {
        curr.clear();
        k = 0;
      }
    }
    return min_occur;
  }
  
  // If x[QRY] must not occur in x[ORI], then x[ORI] = x[OUT]. 
  // Otherwise, we use the earliest/latest start/end positions of x[QRY] in 
  // x[ORI] to possibly refine x[OUT] via equation.
  template <class View>
  forceinline ExecStatus
  Replace<View>::refine_out(Space& home, int min_occur) {
//    std::cerr << "refine_out\n";
    Position pos[2];
    check_find(x[ORI], x[QRY], pos);
    if (pos[0].idx == -1) {
      if (min_occur > 0)
        return ES_FAILED;
      eq(home, x[ORI], x[OUT]);
      return home.ES_SUBSUMED(*this);
    }
    // Prefix: x[ORI][:es]
    Region r;
    Position es = pos[0], le = pos[1];
    assert (es.isNorm(x[ORI]) && le.isNorm(x[ORI]));
    int n = es.idx + (es.off > 0) + x[ORI].size() - le.idx +
            min_occur*x[RPL].size() + (x[OUT].max_length() > 0)*(min_occur+1);            
    std::cerr << "ES: " << es << ", LE: " << le << ", n: "<< n << "\n";
    if (n == 0)
      return ES_OK;
    int k = 0;
    Block* d = r.alloc<Block>(n);
    prefix(home, x[ORI], es, d, k);
    // Crush x[ORI][es : le], possibly adding x[RPL].
    if (x[OUT].max_length() > 0) {
      Set::GLBndSet s;
      for (auto i : {ORI, RPL}) {
        for (int j = 0; j < x[i].size(); ++j) {
          if (s.min() == 0 && s.max() == MAX_ALPHABET_SIZE-1)
            break;
          x[i][j].includeBaseIn(home, s);
        }
        if (i == ORI && min_occur > 0 && !x[QRY].assigned()) {
          int u = std::max(0, std::min(x[ORI].max_length(), 
                              x[ORI].max_length()-x[QRY].min_length()+1));
          Block b(home, CharSet(home,s), 0, u);
          GECODE_ME_CHECK(x[QRY].equate(home, ConstDashedView(b,1)));
        }
      }
      Block b(home, CharSet(home,s), 0, x[OUT].max_length());
      d[k++].update(home,b);
      for (int i = 0; i < min_occur; ++i) {
        for (int j = 0; j < x[RPL].size(); ++j)
          d[k++].update(home, x[RPL][j]);
        d[k++].update(home,b);
      }
    }
    else {
      for (int i = 0; i < min_occur; ++i)
        for (int j = 0; j < x[RPL].size(); ++j)
          d[k++].update(home, x[RPL][j]);
    }
    // Suffix: x[ORI][le:]
    suffix(home, x[ORI], le, d, k);
    ConstDashedView dom(d[0],k);
    std::cerr << "Equating x[OUT]: " << x[OUT] << " with " << dom << " => \n";
    if (x[OUT].assigned())
      return check_equate_x(x[OUT], dom) ? ES_OK : ES_FAILED;
    GECODE_ME_CHECK(x[OUT].equate(home, dom));
    // std::cerr << x[OUT] << "\n";
    return ES_OK;
  }
  
  // If x[RPL] must not occur in x[OUT], then find(x[QRY], x[ORI]) = 0 /\ x[ORI] = x[OUT].
  // Otherwise, we use the earliest/latest start/end positions of x[RPL] in x[OUT]
  // to possibly refine x[ORI] via equation.
  template <class View>
  forceinline ExecStatus
  Replace<View>::refine_ori(Space& home, int min_occur) {
//    std::cerr << "refine_ori\n";
    Position pos[2];
    check_find(x[OUT], x[RPL], pos);
    if (pos[0].idx == -1) {
      if (min_occur > 0)
        return ES_FAILED;
      find(home, x[ORI], x[QRY], IntVar(home, 0, 0));
      eq(home, x[ORI], x[OUT]);
      return home.ES_SUBSUMED(*this);
    }
    // Prefix: x[OUT][: es].
    Region r;
    Position es = pos[0], le = pos[1];
    assert (es.isNorm(x[OUT]) && le.isNorm(x[OUT]));
    int n = es.idx + (es.off > 0) + x[OUT].size() - le.idx +
            min_occur*x[QRY].size() + (x[ORI].max_length() > 0)*(min_occur+1);
//    std::cerr << "ES: " << es << ", LE: " << le << ", n: "<< n << "\n";
    if (n == 0)
      return ES_OK;
    int k = 0;
    Block* d = r.alloc<Block>(n);
    prefix(home, x[OUT], es, d, k);
    // Crush x[OUT][es : ls], possibly adding x[QRY].
    if (x[ORI].max_length() > 0) {
      Set::GLBndSet s;
      for (auto i : {OUT,QRY}) {
        for (int j = 0; j < x[i].size(); ++j) {
          if (s.min() == 0 && s.max() == MAX_ALPHABET_SIZE-1)
            break;
          x[i][j].includeBaseIn(home, s);
        }
        if (i == OUT && min_occur > 0 && !x[RPL].assigned()) {
          int u = std::max(0, std::min(x[OUT].max_length(), 
                              x[OUT].max_length()-x[RPL].min_length()+1));
          Block b(home, CharSet(home,s), 0, u);
          GECODE_ME_CHECK(x[RPL].equate(home, ConstDashedView(b,1)));
        }
      }
      Block b(home, CharSet(home,s), 0, x[ORI].max_length());
      d[k++].update(home,b);
      for (int i = 0; i < min_occur; ++i) {
        for (int j = 0; j < x[QRY].size(); ++j)
          d[k++].update(home, x[QRY][j]);
        d[k++].update(home,b);
      }
    }
    else {
      for (int i = 0; i < min_occur; ++i)
        for (int j = 0; j < x[QRY].size(); ++j)
          d[k++].update(home, x[QRY][j]);
    }
    // Suffix: x[OUT][le :]
    suffix(home, x[OUT], le, d, k);
    ConstDashedView dom(d[0],k);
    std::cerr << "Equating x[ORI]: " << x[ORI] << " with " << dom << " => \n";
    if (x[ORI].assigned())
      return check_equate_x(x[ORI], dom) ? ES_OK : ES_FAILED;
    GECODE_ME_CHECK(x[ORI].equate(home, dom));
    //std::cerr << x[ORI] << "\n";    
    return ES_OK;
  }
  
  template <class View>
  forceinline ExecStatus
  Replace<View>::propagate(Space& home, const ModEventDelta&) {
  again:
    std::cerr<<"\nReplace" << (all ? "All" : last ? "Last" : "") << "::propagate: "<< x <<"\n";
    if (!all && !check_card()) {
      // x[QRY] not occurring in x[ORI].
      find(home, x[ORI], x[QRY], IntVar(home, 0, 0));
      eq(home, x[ORI], x[OUT]);
      return home.ES_SUBSUMED(*this);
    }
    int min_occur = 0;
    // Query string x[QRY] fixed.
    if (x[QRY].assigned()) {
      if (x[RPL].assigned() && check_equate_x(x[QRY],x[RPL])) {
        // x[QRY] = x[RPL] -> x[OUT] = x[ORI].
        rel(home, x[ORI], STRT_EQ, x[OUT]);
        return home.ES_SUBSUMED(*this);
      }
      if (x[QRY].max_length() == 0 && !all) {
        // x[QRY] = "".
        last ? concat(home, x[ORI], x[RPL], x[OUT]) : concat(home, x[RPL], x[ORI], x[OUT]);
        return home.ES_SUBSUMED(*this);
      }
      if (x[ORI].assigned()) {
        if (all)
          return decomp_all(home);
        int n = last ? rfind_fixed(x[ORI],x[QRY]) : find_fixed(x[ORI],x[QRY]);
        std::cerr << "idx: " << n << "\n"; 
        if (n == 0)
          eq(home, x[ORI], x[OUT]);
        else {
          n--;
          std::vector<int> wx = x[ORI].val();
          std::vector<int> pref(wx.begin(), wx.begin() + n);
          std::vector<int> suff(wx.begin() + x[QRY].max_length() + n, wx.end());
          if (x[RPL].assigned()) {
            std::vector<int> w(pref);
            std::vector<int> wq1 = x[RPL].val();
            w.insert(w.end(), wq1.begin(), wq1.end());
            w.insert(w.end(), suff.begin(), suff.end());
            eq(home, x[OUT], w);
          }
          else {
            StringVar tmp(home);
            concat(home, StringVar(home, pref), x[RPL], tmp);
            concat(home, tmp, StringVar(home, suff), x[OUT]);
          }
        }
        return home.ES_SUBSUMED(*this);
      }
      min_occur = occur(home);
    }
    if (x[ORI].assigned() && x[OUT].assigned()) {
      if (x[ORI].val() == x[OUT].val()) {
        find(home, x[ORI], x[QRY], IntVar(home, 0, 0));
        return home.ES_SUBSUMED(*this);
      }
      if (min_occur == 0)
        min_occur = 1;
    } 
    // x[ORI] != x[OUT] => x[QRY] occur in x[ORI] /\ x[RPL] occur in x[OUT].
    if (min_occur == 0 && !check_equate_x(x[ORI],x[OUT]))
      min_occur = 1;
    if (min_occur > 0 && !all)
      GECODE_REWRITE(*this, rewrite(home, x, last));
    std::cerr << "min_occur: " << min_occur << "\n";
    GECODE_ES_CHECK(refine_out(home, min_occur));
//    std::cerr<<"After refine_out: "<< x <<"\n";
    GECODE_ES_CHECK(refine_ori(home, min_occur));
//    std::cerr<<"After refine_ori: "<< x <<"\n";
    if (!all && !check_card()) {
      eq(home, x[ORI], x[OUT]);
      return home.ES_SUBSUMED(*this);
    }
    assert (!home.failed());
    std::cerr<<"After replace: "<< x <<"\n";
    switch (x[ORI].assigned() + x[QRY].assigned() +
            x[RPL].assigned() + x[OUT].assigned()) {
      case 4:
      case 3:
        // Force the re-execution of the propagation.
        if (x[ORI].assigned())
          goto again;
        return ES_FIX;
      case 2:
        if (x[ORI].assigned() && x[QRY].assigned())
          goto again;
        return ES_FIX;
      default:
        return ES_FIX;
    }
    return ES_FIX;
  }

}}}
