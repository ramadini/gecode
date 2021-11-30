#include <gecode/int/arithmetic.hh>

namespace Gecode { namespace String { namespace RelOp {

  // x[3] is the string resulting from x[0] by replacing the first occurrence of 
  // x[1] with x[2].

  template <class View>
  forceinline ExecStatus
  Replace<View>::post(Home home, ViewArray<View>& x, bool a, bool l) {
    if (x[1].same(x[2])) {
      eq(home, x[0], x[3]);
      return ES_OK;
    }
    else if (x[0].same(x[1])) {
      eq(home, x[2], x[3]);
      return ES_OK;
    }
    else if (x[0].same(x[3])) {
      find(home, x[0], x[1], IntVar(home, 0, 0));
      return ES_OK;
    }
    else if (x[1].same(x[3]))
      find(home, x[0], x[1], IntVar(home, 1, x[0].max_length()));
    if (!a) {
      // Enforcing |y| = |x| + [find(x,q) > 0]*(|q'|-|q|).
      IntVar lx(home,  x[0].min_length(), x[0].max_length());
      IntVar lq(home,  x[1].min_length(), x[1].max_length());
      IntVar lq1(home, x[2].min_length(), x[2].max_length());
      IntVar ly(home,  x[3].min_length(), x[3].max_length());
      // b = [find(q, x) > 0].
      IntVar z(home, 0, lx.max());
      find(home, x[0], x[1], z);
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
    assert (x[0].assigned() && x[1].assigned());
    if (x[2].assigned()) {
      if (x[1].max_length() == 0) {
        if (x[2].max_length() == 0) {
          // x[1] = x[2] = "" -> x[0] = x[3].
          GECODE_ME_CHECK(x[3].equate(home, 
                                      ConstDashedView(x[0][0],x[0].size())));
          return home.ES_SUBSUMED(*this);
        }
        std::vector<int> w0 = x[0].val();
        std::vector<int> w2 = x[2].val();
        int w3[(w0.size()+1)* w2.size()];
        int k = 0;
        for (size_t i = 0; i < w0.size(); ++i) {          
          for (size_t j = 0; j < w2.size(); ++j)
            w3[k++] = w2[j];
          w3[k++] = w0[i];
        }
        for (size_t j = 0; j < w2.size(); ++j)
          w3[k++] = w2[j];
        // x[1] = "" -> x[3] = x[2] x[0][0] x[2] x[0][1] ... x[0][-1] x[2].
        GECODE_ME_CHECK(x[3].equate(home, ConstStringView(home,w3,k)));
      }
      else {
        std::vector<int> w0 = x[0].val();
        std::vector<int> w1 = x[1].val();
        std::vector<int> w2 = x[2].val();
        std::vector<int>::iterator pos = 
           std::search(w0.begin(), w0.end(), w1.begin(), w1.end());
        std::vector<int> idx;
        while (pos != w0.end()) { 
          idx.push_back(pos-w0.begin());
          pos = std::search(pos + w1.size(), w0.end(), w1.begin(), w1.end());
        }
         int k = 0, i0 = 0;
         int w3[w0.size() + idx.size() * (w2.size()-w1.size())];
         for (auto i : idx) {
           for (int j = i0; j < i; ++j)
             w3[k++] = w0[j];
           for (auto c : w2)
             w3[k++] = c;
           i0 = i + w1.size();
         }
         for (size_t i = i0; i < w0.size(); ++i)
           w3[k++] = w0[i];
        GECODE_ME_CHECK(x[3].equate(home, ConstStringView(home,w3,k)));
      }
    }
    else {
      if (x[1].max_length() == 0) {
        if (x[0].max_length() == 0) {
          eq(home, x[2], x[3]);
          return home.ES_SUBSUMED(*this);
        }
        // x[1] = "" -> x[3] = x[2] x[0][0] x[2] x[0][1] ... x[0][-1] x[2].
        std::vector<int> w0 = x[0].val();
        int n = w0.size();
        StringVarArray v(home, n);
        concat(home, x[2], StringVar(home, w0[0]), v[0]);
        for (int i = 1; i < n; ++i) {
          StringVar tmp(home);
          concat(home, x[2], StringVar(home, w0[i]), tmp);
          concat(home, v[i-1], tmp, v[i]);
        }
        concat(home, v[n-1], x[2], x[3]);
      }
      else {
        std::vector<int> w0 = x[0].val();
        std::vector<int> w1 = x[1].val();
        std::vector<int>::iterator pos = 
          std::search(w0.begin(), w0.end(), w1.begin(), w1.end());
        if (pos == w0.end()) {
          eq(home, x[0], x[3]);
          return home.ES_SUBSUMED(*this);
        }
        std::vector<int> w2 = x[2].val();
//        std::vector<StringVar> vx(1, StringVar(home));
//        string s = sx.substr(0, pos);
//        if (s == "")
//          rel(home, x[2], STRT_EQ, vx[0]);
//        else
//          rel(home, StringVar(home, s), x[2], STRT_CAT, vx[0]);
//        size_t nx = sq.size(), pos1 = pos + nx;
//        pos = sx.find(sq, pos1);
//        while (pos != string::npos) {
//          StringVar last = vx.back();
//          vx.push_back(StringVar(home));
//          if (pos > pos1) {
//            StringVar z(home);
//            rel(home,
//              StringVar(home, sx.substr(pos1, pos - pos1)), x[2], STRT_CAT, z
//            );
//            rel(home, last, z, STRT_CAT, vx.back());
//          }
//          else
//            rel(home, last, x[2], STRT_CAT, vx.back());
//          pos1 = pos + nx;
//          pos = sx.find(sq, pos1);          
//        }
//        if (pos1 < sx.size())
//          rel(home, vx.back(), StringVar(home,sx.substr(pos1)), STRT_CAT, x[3]);
//        else
//          rel(home, vx.back(), STRT_EQ, x[3]);
      }
    }
//    std::cerr << "After decomp_all: " << x << "\n";
    return home.ES_SUBSUMED(*this);
  }

  // Checking if |y| = |x| + |q'| - |q| is consistent.
  template <class View>
  forceinline bool
  Replace<View>::check_card() const {
   int lx  = x[0].min_length(), lq = x[1].min_length(), lq1 = x[2].min_length(),
       ly  = x[3].min_length();
   long ux = x[0].max_length(), uq = x[1].max_length(), uq1 = x[2].max_length(),
        uy = x[3].max_length();
   return ly <= ux + uq1 - lq && uy >= lx + lq1 - uq;
  }
  
//  // For replace, it returns 1 if q must occur in x[0], 0 otherwise.
//  // For decomp_all, it returns the minimum number of occurrences of q in x[0].
//  template <class View>
//  forceinline int
//  Replace<View>::occur(string q) const {
//    int min_occur = 0;
//    string curr = "";
//    DashedString* p = x[0].pdomain();
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
  
//  // If x[1] must not occur in x[0], then x[0] = x[3]. Otherwise, we use the 
//  // earliest/latest start/end positions of x[1] in x[0] to possibly refine 
//  // x[3] via equation.
//  template <class View>
//  forceinline ExecStatus
//  Replace<View>::replace_q_x(Space& home, int min_occur) {
//    DashedString* px  = x[0].pdomain();
//    DashedString* pq  = x[1].pdomain();
//    DashedString* pq1 = x[2].pdomain();
//    Position pos[2];
//    if (check_find(*pq, *px, pos)) {
//      // Prefix: x[0][: es]
//      NSBlocks v;
//      Position es = pos[0], le = pos[1];
//      // std::cerr << "ES: " << es << ", LE: " << le << "\n";
//      if (es != Position({0, 0}))
//        v = prefix(0, es);
//      // Crush x[0][es : le], possibly adding x[2].
//      int u = x[3].max_length();
//      if (u > 0) {
//        NSBlock b;
//        b.S = x[0].may_chars();
//        b.S.include(x[2].may_chars());
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
//      // Suffix: x[0][le :]
//      if (le != last_fwd(px->blocks()))
//        v.extend(suffix(0, le));
//      v.normalize();
//      // std::cerr << "1c) Equating " << x[3] << " with " << v << " => \n";
//      GECODE_ME_CHECK(x[3].dom(home, v));
//      //std::cerr << x[3] << "\n";
//    }
//    else {
//      if (min_occur > 0)
//        return ES_FAILED;
//      rel(home, x[0], STRT_EQ, x[3]);
//      return home.ES_SUBSUMED(*this);
//    }
//    return ES_OK;
//  }
  
//  // If x[2] must not occur in x[3], then find(x[1], x[0]) = 0 /\ x[0] = x[3].
//  // Otherwise, we use the earliest/latest start/end positions of x[2] in x[3]
//  // to possibly refine x[0] via equation.
//  template <class View>
//  forceinline ExecStatus
//  Replace<View>::replace_q1_y(Space& home, int min_occur) {
//    DashedString* pq  = x[1].pdomain();
//    DashedString* pq1 = x[2].pdomain();
//    DashedString* py  = x[3].pdomain();
//    Position pos[2];
//    if (check_find(*pq1, *py, pos)) {
//      // Prefix: x[3][: es].
//      NSBlocks v;
//      Position es = pos[0], le = pos[1];
//      // std::cerr << "ES: " << es << ", LE: " << le << "\n";
//      if (es != Position({0, 0}))
//        v = prefix(3, es);
//      // Crush x[3][es : ls], possibly adding x[1].
//      int u = x[0].max_length();
//      if (u > 0) {
//        NSBlock b;
//        b.S = x[1].may_chars();
//        b.S.include(x[3].may_chars());
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
//      // Suffix: x[3][le :]
//      if (le != last_fwd(py->blocks()))
//        v.extend(suffix(3, le));
//      v.normalize();
//      // std::cerr << "2) Equating " << x[0] << " with " << v << " => \n";
//      GECODE_ME_CHECK(x[0].dom(home, v));
//      //std::cerr << x[0] << "\n";
//    }
//    else {
//      if (min_occur > 0)
//        return ES_FAILED;
//      find(home, x[1], x[0], IntVar(home, 0, 0));
//      rel(home, x[0], STRT_EQ, x[3]);
//      return home.ES_SUBSUMED(*this);
//    }
//    return ES_OK;
//  }
  
  template <class View>
  forceinline ExecStatus
  Replace<View>::propagate(Space& home, const ModEventDelta&) {
    std::cerr<<"\nReplace" << (all ? "All" : last ? "Last" : "") << "::propagate: "<< x <<"\n";
    if (!all && !check_card()) {
      // x[1] not occurring in x[0].
      find(home, x[0], x[1], IntVar(home, 0, 0));
      eq(home, x[0], x[3]);
      return home.ES_SUBSUMED(*this);
    }
    int min_occur = 0;
    // Query string x[1] fixed.
    if (x[1].assigned()) {
      if (x[2].assigned() && check_equate_x(x[1],x[2])) {
        // x[1] = x[2] -> x[3] = x[0].
        rel(home, x[0], STRT_EQ, x[3]);
        return home.ES_SUBSUMED(*this);
      }
      if (x[1].max_length() == 0 && !all) {
        // x[1] = "".
        last ? concat(home, x[0], x[2], x[3]) : concat(home, x[2], x[0], x[3]);
        return home.ES_SUBSUMED(*this);
      }
      if (x[0].assigned()) {
        if (all)
          return decomp_all(home);
//        string sx = x[0].val();
//        size_t n = last ? sx.rfind(sq) : sx.find(sq);
//        if (n == string::npos)
//          rel(home, x[0], STRT_EQ, x[3]);
//        else {
//          string pref = sx.substr(0, n);
//          string suff = sx.substr(n + sq.size());
//          if (x[2].assigned())
//            GECODE_ME_CHECK(x[3].eq(home, pref + x[2].val() + suff));
//          else {
//            StringVar z(home);
//            rel(home, StringVar(home, pref), x[2], STRT_CAT, z);
//            rel(home, z, StringVar(home, suff), STRT_CAT, x[3]);
//          }
//        }
//        return home.ES_SUBSUMED(*this);
      }
//      min_occur = occur(sq);
    }
    if (x[0].assigned() && x[3].assigned()) {
      if (x[0].val() == x[3].val()) {
        find(home, x[1], x[0], IntVar(home, 0, 0));
        return home.ES_SUBSUMED(*this);
      }
      if (min_occur == 0)
        min_occur += 1;
    } 
//    // x[0] != x[3] => x[1] occur in x[0] /\ x[2] occur in x[3].
//    DashedString* px  = x[0].pdomain();
//    DashedString* pq  = x[1].pdomain();
//    DashedString* pq1 = x[2].pdomain();
//    DashedString* py  = x[3].pdomain();
//    if (min_occur == 0 && !px->check_equate(*py))
//      min_occur = 1;
//    if (min_occur > 0 && !all) {
//      // std::<<cerr << "min_occur = "<<min_occur<<": rewriting into concat!\n";
//      StringVar pref(home), suff(home);
//      StringVarArgs lhs, rhs;
//      lhs << pref << x[1] << suff;
//      rhs << pref << x[2] << suff;
//      gconcat(home, lhs, x[0]);
//      gconcat(home, rhs, x[3]);
//      find(home, x[1], last ? suff : pref, IntVar(home, 0, 0));
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
//      rel(home, x[0], STRT_EQ, x[3]);
//      return home.ES_SUBSUMED(*this);
//    }
//    if (home.failed())
//      return ES_FAILED;
//    // std::cerr<<"After replace: "<< x <<"\n";
//    assert (px->is_normalized() && pq->is_normalized() 
//        && pq1->is_normalized() && py->is_normalized());
//    switch (
//      x[0].assigned() + x[1].assigned() + x[2].assigned() + x[3].assigned()
//    ) {
//      case 4:
//      case 3:
//        // Force the re-execution of the propagation.
//        return x[0].assigned() ? propagate(home, m) : ES_FIX;
//      case 2:
//        return x[1].assigned() && x[0].assigned() ? propagate(home, m) : ES_FIX;
//      default:
//        return ES_FIX;
//    }
    return ES_FIX;
  }

}}}
