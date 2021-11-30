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

//  // Returns the prefix of x[k] until position p.
//  template <class View>
//  forceinline NSBlocks
//  Replace<View>::prefix(int k, const Position& p) const {
//    NSBlocks pref;
//    DashedString* px = x[k].pdomain();
//    for (int i = 0; i < p.idx; ++i)
//      pref.push_back(NSBlock(px->at(i)));
//    int off = p.off;
//    if (off > 0) {
//      const DSBlock& b = px->at(p.idx);
//      if (off < b.l)
//        pref.push_back(NSBlock(b.S, off, off));
//      else
//        pref.push_back(NSBlock(b.S, b.l, off));
//    }
//    return pref;
//  }
  
//  // Returns the suffix of x[k] from position p.
//  template <class View>
//  forceinline NSBlocks
//  Replace<View>::suffix(int k, const Position& p) const {
//    NSBlocks suff;
//    DashedString* px = x[k].pdomain();
//    int off = p.off;
//    if (off < px->at(p.idx).u) {
//      const DSBlock& b = px->at(p.idx);
//      suff.push_back(NSBlock(b.S, max(0, b.l - off), b.u - off));
//    }
//    for (int i = p.idx + 1; i < px->length(); ++i)
//      suff.push_back(NSBlock(px->at(i)));
//    return suff;
//  }
  
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
        std::vector<int> wq1 = x[RPL].val();
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
          StringVar last = v.back();
          v.push_back(StringVar(home));
          if (pos > pos1) {
            StringVar tmp(home);
            std::vector<int>::iterator p = pos1 - (pos - wx.begin());
            concat(home, StringVar(home,std::vector<int>(pos1,p)), x[RPL], tmp);
            concat(home, last, tmp, v.back());
          }
          else
            concat(home, last, x[RPL], v.back());
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
  
//  // For replace, it returns 1 if q must occur in x[ORI], 0 otherwise.
//  // For decomp_all, it returns the minimum number of occurrences of q in x[ORI].
//  template <class View>
//  forceinline int
//  Replace<View>::occur(string q) const {
//    int min_occur = 0;
//    string curr = "";
//    DashedString* p = x[ORI].pdomain();
//    for (int i = 0; i < p->length(); ++i) {
//      const DSBlock& b = p->at(i);
//      if (b.S.size() == 1) {
//        string w(b.l, b.S.min());
//        curr += w;
//        size_t i = curr.find(q);
//        while (i != string::npos) {
//          min_occur++;
//          if (!all)
//            return min_occur;
//          curr = curr.substr(i + q.size());
//          i = curr.find(q);
//        }
//        if (b.l < b.u)
//          curr = w;
//      }
//      else
//        curr = "";
//    }
//    return min_occur;
//  }
  
//  // If x[QRY] must not occur in x[ORI], then x[ORI] = x[OUT]. Otherwise, we use the 
//  // earliest/latest start/end positions of x[QRY] in x[ORI] to possibly refine 
//  // x[OUT] via equation.
//  template <class View>
//  forceinline ExecStatus
//  Replace<View>::replace_q_x(Space& home, int min_occur) {
//    DashedString* px  = x[ORI].pdomain();
//    DashedString* pq  = x[QRY].pdomain();
//    DashedString* pq1 = x[RPL].pdomain();
//    Position pos[2];
//    if (check_find(*pq, *px, pos)) {
//      // Prefix: x[ORI][: es]
//      NSBlocks v;
//      Position es = pos[0], le = pos[1];
//      // std::cerr << "ES: " << es << ", LE: " << le << "\n";
//      if (es != Position({0, 0}))
//        v = prefix(0, es);
//      // Crush x[ORI][es : le], possibly adding x[RPL].
//      int u = x[OUT].max_length();
//      if (u > 0) {
//        NSBlock b;
//        b.S = x[ORI].may_chars();
//        b.S.include(x[RPL].may_chars());
//        b.u = u;
//        v.push_back(b);
//        for (int i = 0; i < min_occur; ++i) {
//          for (int j = 0; j < pq1->length(); ++j)
//            v.push_back(NSBlock(pq1->at(j)));
//          v.push_back(b);
//        }
//      }
//      else
//        for (int i = 0; i < min_occur; ++i)
//          for (int j = 0; j < pq1->length(); ++j)
//            v.push_back(NSBlock(pq1->at(j)));
//      // Suffix: x[ORI][le :]
//      if (le != last_fwd(px->blocks()))
//        v.extend(suffix(0, le));
//      v.normalize();
//      // std::cerr << "1c) Equating " << x[OUT] << " with " << v << " => \n";
//      GECODE_ME_CHECK(x[OUT].dom(home, v));
//      //std::cerr << x[OUT] << "\n";
//    }
//    else {
//      if (min_occur > 0)
//        return ES_FAILED;
//      rel(home, x[ORI], STRT_EQ, x[OUT]);
//      return home.ES_SUBSUMED(*this);
//    }
//    return ES_OK;
//  }
  
//  // If x[RPL] must not occur in x[OUT], then find(x[QRY], x[ORI]) = 0 /\ x[ORI] = x[OUT].
//  // Otherwise, we use the earliest/latest start/end positions of x[RPL] in x[OUT]
//  // to possibly refine x[ORI] via equation.
//  template <class View>
//  forceinline ExecStatus
//  Replace<View>::replace_q1_y(Space& home, int min_occur) {
//    DashedString* pq  = x[QRY].pdomain();
//    DashedString* pq1 = x[RPL].pdomain();
//    DashedString* py  = x[OUT].pdomain();
//    Position pos[2];
//    if (check_find(*pq1, *py, pos)) {
//      // Prefix: x[OUT][: es].
//      NSBlocks v;
//      Position es = pos[0], le = pos[1];
//      // std::cerr << "ES: " << es << ", LE: " << le << "\n";
//      if (es != Position({0, 0}))
//        v = prefix(3, es);
//      // Crush x[OUT][es : ls], possibly adding x[QRY].
//      int u = x[ORI].max_length();
//      if (u > 0) {
//        NSBlock b;
//        b.S = x[QRY].may_chars();
//        b.S.include(x[OUT].may_chars());
//        b.u = u;
//        v.push_back(b);
//        for (int i = 0; i < min_occur; ++i) {
//          for (int j = 0; j < pq->length(); ++j)
//            v.push_back(NSBlock(pq->at(j)));
//          v.push_back(b);
//        }
//      }
//      else {
//        for (int i = 0; i < min_occur; ++i)
//          for (int j = 0; j < pq->length(); ++j)
//            v.push_back(NSBlock(pq->at(j)));
//      }
//      // Suffix: x[OUT][le :]
//      if (le != last_fwd(py->blocks()))
//        v.extend(suffix(3, le));
//      v.normalize();
//      // std::cerr << "2) Equating " << x[ORI] << " with " << v << " => \n";
//      GECODE_ME_CHECK(x[ORI].dom(home, v));
//      //std::cerr << x[ORI] << "\n";
//    }
//    else {
//      if (min_occur > 0)
//        return ES_FAILED;
//      find(home, x[QRY], x[ORI], IntVar(home, 0, 0));
//      rel(home, x[ORI], STRT_EQ, x[OUT]);
//      return home.ES_SUBSUMED(*this);
//    }
//    return ES_OK;
//  }
  
  template <class View>
  forceinline ExecStatus
  Replace<View>::propagate(Space& home, const ModEventDelta&) {
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
//        string sx = x[ORI].val();
//        size_t n = last ? sx.rfind(sq) : sx.find(sq);
//        if (n == string::npos)
//          rel(home, x[ORI], STRT_EQ, x[OUT]);
//        else {
//          string pref = sx.substr(0, n);
//          string suff = sx.substr(n + sq.size());
//          if (x[RPL].assigned())
//            GECODE_ME_CHECK(x[OUT].eq(home, pref + x[RPL].val() + suff));
//          else {
//            StringVar z(home);
//            rel(home, StringVar(home, pref), x[RPL], STRT_CAT, z);
//            rel(home, z, StringVar(home, suff), STRT_CAT, x[OUT]);
//          }
//        }
//        return home.ES_SUBSUMED(*this);
      }
//      min_occur = occur(sq);
    }
    if (x[ORI].assigned() && x[OUT].assigned()) {
      if (x[ORI].val() == x[OUT].val()) {
        find(home, x[QRY], x[ORI], IntVar(home, 0, 0));
        return home.ES_SUBSUMED(*this);
      }
      if (min_occur == 0)
        min_occur += 1;
    } 
//    // x[ORI] != x[OUT] => x[QRY] occur in x[ORI] /\ x[RPL] occur in x[OUT].
//    DashedString* px  = x[ORI].pdomain();
//    DashedString* pq  = x[QRY].pdomain();
//    DashedString* pq1 = x[RPL].pdomain();
//    DashedString* py  = x[OUT].pdomain();
//    if (min_occur == 0 && !px->check_equate(*py))
//      min_occur = 1;
//    if (min_occur > 0 && !all) {
//      // std::<<cerr << "min_occur = "<<min_occur<<": rewriting into concat!\n";
//      StringVar pref(home), suff(home);
//      StringVarArgs lhs, rhs;
//      lhs << pref << x[QRY] << suff;
//      rhs << pref << x[RPL] << suff;
//      gconcat(home, lhs, x[ORI]);
//      gconcat(home, rhs, x[OUT]);
//      find(home, x[QRY], last ? suff : pref, IntVar(home, 0, 0));
//      return home.ES_SUBSUMED(*this);
//    }
//    // std::cerr << "min_occur: " << min_occur << "\n";
//    ExecStatus es = replace_q_x(home, min_occur);
//    if (es != ES_OK)
//      return es;
//    // std::cerr<<"After replace_q_x: "<< x <<"\n";
//    es = replace_q1_y(home, min_occur);
//    if (es != ES_OK)
//      return es;
//    // std::cerr<<"After replace_q1_y: "<< x <<"\n";
//    if (!all && !check_card()) {
//      rel(home, x[ORI], STRT_EQ, x[OUT]);
//      return home.ES_SUBSUMED(*this);
//    }
//    if (home.failed())
//      return ES_FAILED;
//    // std::cerr<<"After replace: "<< x <<"\n";
//    assert (px->is_normalized() && pq->is_normalized() 
//        && pq1->is_normalized() && py->is_normalized());
//    switch (
//      x[ORI].assigned() + x[QRY].assigned() + x[RPL].assigned() + x[OUT].assigned()
//    ) {
//      case 4:
//      case 3:
//        // Force the re-execution of the propagation.
//        return x[ORI].assigned() ? propagate(home, m) : ES_FIX;
//      case 2:
//        return x[QRY].assigned() && x[ORI].assigned() ? propagate(home, m) : ES_FIX;
//      default:
//        return ES_FIX;
//    }
    return ES_FIX;
  }

}}}
