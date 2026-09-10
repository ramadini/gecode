#include <gecode/int/arithmetic.hh>

#include <algorithm>

namespace Gecode { namespace String {

  forceinline StringVal::size_type
  replace_npos(void) {
    return static_cast<StringVal::size_type>(-1);
  }

  forceinline StringVal::size_type
  replace_find_symbols(const StringVal& value, const StringVal& pattern,
                       StringVal::size_type from = 0) {
    if (from > value.size())
      return replace_npos();
    StringVal::const_iterator position = std::search(
      value.begin() + from, value.end(), pattern.begin(), pattern.end());
    return position == value.end() && !pattern.empty()
      ? replace_npos()
      : static_cast<StringVal::size_type>(position - value.begin());
  }

  forceinline StringVal::size_type
  replace_rfind_symbols(const StringVal& value, const StringVal& pattern) {
    StringVal::const_iterator position = std::find_end(
      value.begin(), value.end(), pattern.begin(), pattern.end());
    return position == value.end() && !pattern.empty()
      ? replace_npos()
      : static_cast<StringVal::size_type>(position - value.begin());
  }

  forceinline StringVal
  replace_slice_symbols(const StringVal& value, StringVal::size_type from,
                        StringVal::size_type length = replace_npos()) {
    assert(from <= value.size());
    const StringVal::size_type count =
      length == replace_npos() || length > value.size() - from
        ? value.size() - from : length;
    return StringVal::from_symbols(std::vector<StringSymbol>(
      value.begin() + from, value.begin() + from + count));
  }

  forceinline StringVal
  replace_symbol_range(const StringVal& value, StringVal::size_type position,
                       StringVal::size_type length,
                       const StringVal& replacement) {
    assert(position <= value.size() && length <= value.size() - position);
    std::vector<StringSymbol> result;
    result.reserve(value.size() - length + replacement.size());
    result.insert(result.end(), value.begin(), value.begin() + position);
    result.insert(result.end(), replacement.begin(), replacement.end());
    result.insert(result.end(), value.begin() + position + length, value.end());
    return StringVal::from_symbols(std::move(result));
  }

  // x[3] is the string resulting from x[0] by replacing the first occurrence of
  // x[1] with x[2].

  forceinline ExecStatus
  Replace::post(Home home, ViewArray<StringView>& x, bool a, bool l) {
    if (x[1].same(x[2])) {
      rel(home, x[0], STRT_EQ, x[3]);
      return ES_OK;
    }
    else if (x[0].same(x[1])) {
      rel(home, x[2], STRT_EQ, x[3]);
      return ES_OK;
    }
    else if (x[0].same(x[3])) {
      find(home, x[1], x[0], IntVar(home, 0, 0));
      return ES_OK;
    }
    else if (x[1].same(x[3]))
      rel(home, x[1], STRT_SUB, x[0]);
    if (!a) {
      // Enforcing |y| = |x| + [find(q, x) > 0]*(|q'|-|q|).
      IntVar lx(home,  x[0].min_length(), x[0].max_length());
      IntVar lq(home,  x[1].min_length(), x[1].max_length());
      IntVar lq1(home, x[2].min_length(), x[2].max_length());
      IntVar ly(home,  x[3].min_length(), x[3].max_length());
      // b = [find(q, x) > 0].
      IntVar z(home, 0, lx.max());
      find(home, x[1], x[0], z);
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
      IntVar lz(home, min(d.min(), 0), max(0, d.max()));
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
    (void) new (home) Replace(home, x, a, l);
    return ES_OK;
  }

  forceinline Actor*
  Replace::copy(Space& home) {
    return new (home) Replace(home, *this);
  }

  forceinline
  Replace::Replace(Home home, ViewArray<StringView>& x, bool a, bool l)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, x), all(a), last(l) {}

  forceinline
  Replace::Replace(Space& home, Replace& p)
  : NaryPropagator<StringView, PC_STRING_DOM>(home, p), all(p.all), last(p.last)
  {}

  // Decomposes decomp_all into basic constraints.
  forceinline ExecStatus
  Replace::decomp_all(Space& home) {
    // std::cerr << "decomp_all\n";
    string sx, sq;
    if (!x[0].domain().try_val_bytes(sx) ||
        !x[1].domain().try_val_bytes(sq))
      return decomp_all_symbols(home);
    if (x[2].assigned()) {
      string sq1;
      if (!x[2].domain().try_val_bytes(sq1))
        return decomp_all_symbols(home);
      if (sq == "") {
        if (sq1 == "") {
          rel(home, x[0], STRT_EQ, x[3]);
          return home.ES_SUBSUMED(*this);
        }
        string sz = sq1;
        for (auto& c : sx) {
          sz += c;
          sz += sq1;
        }
        GECODE_ME_CHECK(x[3].eq(home, sz));
      }
      else {
        size_t pos = sx.find(sq), n = sq.size(), n1 = sq1.size();
        while (pos != string::npos) {
          sx.replace(pos, n, sq1);
          pos = sx.find(sq, pos + n1);
        }
        GECODE_ME_CHECK(x[3].eq(home, sx));
      }
    }
    else {
      if (sq == "") {
        if (sx == "") {
          rel(home, x[2], STRT_EQ, x[3]);
          return home.ES_SUBSUMED(*this);
        }
        int n = sx.size();
        StringVarArray vx(home, n);
        rel(home, x[2], StringVar(home, string(1, sx[0])), STRT_CAT, vx[0]);
        for (int i = 1; i < n; ++i) {
          StringVar z(home);
          rel(home, x[2], StringVar(home, string(1, sx[i])), STRT_CAT, z);
          rel(home, vx[i - 1], z, STRT_CAT, vx[i]);
        }
        rel(home, vx[n - 1], x[2], STRT_CAT, x[3]);
      }
      else {
        size_t pos = sx.find(sq);
        if (pos == string::npos) {
          rel(home, x[0], STRT_EQ, x[3]);
          return home.ES_SUBSUMED(*this);
        }
        std::vector<StringVar> vx(1, StringVar(home));
        string s = sx.substr(0, pos);
        if (s == "")
          rel(home, x[2], STRT_EQ, vx[0]);
        else
          rel(home, StringVar(home, s), x[2], STRT_CAT, vx[0]);
        size_t nx = sq.size(), pos1 = pos + nx;
        pos = sx.find(sq, pos1);
        while (pos != string::npos) {
          StringVar last = vx.back();
          vx.push_back(StringVar(home));
          if (pos > pos1) {
            StringVar z(home);
            rel(home,
              StringVar(home, sx.substr(pos1, pos - pos1)), x[2], STRT_CAT, z
            );
            rel(home, last, z, STRT_CAT, vx.back());
          }
          else
            rel(home, last, x[2], STRT_CAT, vx.back());
          pos1 = pos + nx;
          pos = sx.find(sq, pos1);
        }
        if (pos1 < sx.size())
          rel(home, vx.back(), StringVar(home,sx.substr(pos1)), STRT_CAT, x[3]);
        else
          rel(home, vx.back(), STRT_EQ, x[3]);
      }
    }
    // std::cerr << "After decomp_all: " << x << "\n";
    return home.ES_SUBSUMED(*this);
  }

  // Decomposes replace-all when at least one assigned operand contains a
  // symbol that cannot be represented by the legacy byte path.
  forceinline ExecStatus
  Replace::decomp_all_symbols(Space& home) {
    const StringVal sx = x[0].val_symbols();
    const StringVal sq = x[1].val_symbols();
    NSIntSet intermediate_alphabet = x[0].may_chars();
    intermediate_alphabet.include(x[1].may_chars());
    intermediate_alphabet.include(x[2].may_chars());
    intermediate_alphabet.include(x[3].may_chars());
    const int intermediate_max_length = x[3].max_length();
    if (x[2].assigned()) {
      const StringVal sq1 = x[2].val_symbols();
      if (sq.empty()) {
        if (sq1.empty()) {
          rel(home, x[0], STRT_EQ, x[3]);
          return home.ES_SUBSUMED(*this);
        }
        std::vector<StringSymbol> result;
        result.reserve(sq1.size() * (sx.size() + 1) + sx.size());
        result.insert(result.end(), sq1.begin(), sq1.end());
        for (StringVal::const_iterator symbol = sx.begin();
             symbol != sx.end(); ++symbol) {
          result.push_back(*symbol);
          result.insert(result.end(), sq1.begin(), sq1.end());
        }
        GECODE_ME_CHECK(x[3].eq(
          home, StringVal::from_symbols(std::move(result))));
      }
      else {
        StringVal result = sx;
        StringVal::size_type position = replace_find_symbols(result, sq);
        while (position != replace_npos()) {
          result = replace_symbol_range(result, position, sq.size(), sq1);
          position = replace_find_symbols(
            result, sq, position + sq1.size());
        }
        GECODE_ME_CHECK(x[3].eq(home, result));
      }
    }
    else {
      if (sq.empty()) {
        if (sx.empty()) {
          rel(home, x[2], STRT_EQ, x[3]);
          return home.ES_SUBSUMED(*this);
        }
        const int n = static_cast<int>(sx.size());
        std::vector<StringVar> vx;
        vx.reserve(n);
        for (int i = 0; i < n; ++i)
          vx.push_back(StringVar(
            home, intermediate_alphabet, 0, intermediate_max_length));
        rel(home, x[2],
            StringVar(home, StringVal::from_symbols({sx[0]})),
            STRT_CAT, vx[0]);
        for (int i = 1; i < n; ++i) {
          StringVar z(
            home, intermediate_alphabet, 0, intermediate_max_length);
          rel(home, x[2], StringVar(home, StringVal::from_symbols(
                {sx[static_cast<StringVal::size_type>(i)]})), STRT_CAT, z);
          rel(home, vx[i - 1], z, STRT_CAT, vx[i]);
        }
        rel(home, vx[n - 1], x[2], STRT_CAT, x[3]);
      }
      else {
        StringVal::size_type position = replace_find_symbols(sx, sq);
        if (position == replace_npos()) {
          rel(home, x[0], STRT_EQ, x[3]);
          return home.ES_SUBSUMED(*this);
        }
        std::vector<StringVar> vx(1, StringVar(
          home, intermediate_alphabet, 0, intermediate_max_length));
        const StringVal prefix = replace_slice_symbols(sx, 0, position);
        if (prefix.empty())
          rel(home, x[2], STRT_EQ, vx[0]);
        else
          rel(home, StringVar(home, prefix), x[2], STRT_CAT, vx[0]);
        const StringVal::size_type query_size = sq.size();
        StringVal::size_type previous = position + query_size;
        position = replace_find_symbols(sx, sq, previous);
        while (position != replace_npos()) {
          StringVar tail = vx.back();
          vx.push_back(StringVar(
            home, intermediate_alphabet, 0, intermediate_max_length));
          if (position > previous) {
            StringVar z(
              home, intermediate_alphabet, 0, intermediate_max_length);
            rel(home, StringVar(home, replace_slice_symbols(
                  sx, previous, position - previous)), x[2], STRT_CAT, z);
            rel(home, tail, z, STRT_CAT, vx.back());
          }
          else
            rel(home, tail, x[2], STRT_CAT, vx.back());
          previous = position + query_size;
          position = replace_find_symbols(sx, sq, previous);
        }
        if (previous < sx.size())
          rel(home, vx.back(),
              StringVar(home, replace_slice_symbols(sx, previous)),
              STRT_CAT, x[3]);
        else
          rel(home, vx.back(), STRT_EQ, x[3]);
      }
    }
    return home.ES_SUBSUMED(*this);
  }

  // Checking if |y| = |x| + |q'| - |q| is consistent.
  forceinline bool
  Replace::check_card() const {
   int lx  = x[0].min_length(), ux  = x[0].max_length(),
       lq  = x[1].min_length(), uq  = x[1].max_length(),
       lq1 = x[2].min_length(), uq1 = x[2].max_length(),
       ly  = x[3].min_length(), uy  = x[3].max_length();
     return ly <= ux + uq1 - lq && uy >= lx + lq1 - uq;
  }

  // For replace, it returns 1 if q must occur in x[0], 0 otherwise.
  // For decomp_all, it returns the minimum number of occurrences of q in x[0].
  forceinline int
  Replace::occur(const string& q) const {
    int min_occur = 0;
    string curr;
    const DashedString& p = x[0].domain();
    for (int i = 0; i < p.length(); ++i) {
      const DSBlock& b = p.at(i);
      if (b.S.size() == 1) {
        char c = b.S.min();
        curr.append(b.l, c);
        size_t pos = curr.find(q);
        while (pos != string::npos) {
          min_occur++;
          if (!all)
            return min_occur;
          curr.erase(0, pos + q.size());
          pos = curr.find(q);
        }
        if (b.l < b.u)
          curr.assign(b.l, c);
      }
      else
        curr.clear();
    }
    return min_occur;
  }

  forceinline int
  Replace::occur(const StringVal& q) const {
    if (q.empty())
      return 0;
    int min_occur = 0;
    std::vector<StringSymbol> current;
    const DashedString& source = x[0].domain();
    for (int i = 0; i < source.length(); ++i) {
      const DSBlock& block = source.at(i);
      if (block.S.size() == 1) {
        const StringSymbol symbol = block.S.min();
        current.insert(current.end(), block.l, symbol);
        std::vector<StringSymbol>::iterator position = std::search(
          current.begin(), current.end(), q.begin(), q.end());
        while (position != current.end()) {
          ++min_occur;
          if (!all)
            return min_occur;
          current.erase(current.begin(), position + q.size());
          position = std::search(
            current.begin(), current.end(), q.begin(), q.end());
        }
        if (block.l < block.u)
          current.assign(block.l, symbol);
      }
      else
        current.clear();
    }
    return min_occur;
  }

  // If x[1] must not occur in x[0], then x[0] = x[3]. Otherwise, we use the
  // earliest/latest start/end positions of x[1] in x[0] to possibly refine
  // x[3] via equation.
  forceinline ExecStatus
  Replace::replace_q_x(Space& home, int min_occur, bool& repeat) {
    const DashedString& px  = x[0].domain();
    const DashedString& pq  = x[1].domain();
    const DashedString& pq1 = x[2].domain();
    Position pos[2];
    if (check_find(pq, px, pos)) {
      // Prefix: x[0][: es]
      NSBlocks v;
      Position es = pos[0], le = pos[1];
//       std::cerr << "ES: " << es << ", LE: " << le << "\n";
      if (es != Position({0, 0}))
        v = x[0].domain().prefix(es.idx, es.off);
      // Crush x[0][es : le], possibly adding x[2].
      int u = x[3].max_length();
      if (u > 0) {
        NSBlock b;
        b.S = x[0].may_chars();
        b.S.include(x[2].may_chars());
        b.u = u;
        v.push_back(b);
        for (int i = 0; i < min_occur; ++i) {
          for (int j = 0; j < pq1.length(); ++j)
            v.push_back(NSBlock(pq1.at(j)));
          v.push_back(b);
        }
      }
      else
        for (int i = 0; i < min_occur; ++i)
          for (int j = 0; j < pq1.length(); ++j)
            v.push_back(NSBlock(pq1.at(j)));
      // Suffix: x[0][le :]
      if (le != last_fwd(px.blocks()))
        v.extend(x[0].domain().suffix(le.idx, le.off));
      v.normalize();
//       std::cerr << "1c) Equating " << x[3] << " with " << v << " => \n";
      ModEvent me = x[3].dom(home, v);
      GECODE_ME_CHECK(me);
      repeat |= me_modified(me);
      //std::cerr << x[3] << "\n";
    }
    else {
      if (min_occur > 0)
        return ES_FAILED;
      rel(home, x[0], STRT_EQ, x[3]);
      return home.ES_SUBSUMED(*this);
    }
    return ES_OK;
  }

  // If x[2] must not occur in x[3], then find(x[1], x[0]) = 0 /\ x[0] = x[3].
  // Otherwise, we use the earliest/latest start/end positions of x[2] in x[3]
  // to possibly refine x[0] via equation.
  forceinline ExecStatus
  Replace::replace_q1_y(Space& home, int min_occur, bool& repeat) {
    const DashedString& pq  = x[1].domain();
    const DashedString& pq1 = x[2].domain();
    const DashedString& py  = x[3].domain();
    Position pos[2];
    if (check_find(pq1, py, pos)) {
      // Prefix: x[3][: es].
      NSBlocks v;
      Position es = pos[0], le = pos[1];
      // std::cerr << "ES: " << es << ", LE: " << le << "\n";
      if (es != Position({0, 0}))
        v = x[3].domain().prefix(es.idx, es.off);
      // Crush x[3][es : ls], possibly adding x[1].
      int u = x[0].max_length();
      if (u > 0) {
        NSBlock b;
        b.S = x[1].may_chars();
        b.S.include(x[3].may_chars());
        b.u = u;
        v.push_back(b);
        for (int i = 0; i < min_occur; ++i) {
          for (int j = 0; j < pq.length(); ++j)
            v.push_back(NSBlock(pq.at(j)));
          v.push_back(b);
        }
      }
      else {
        for (int i = 0; i < min_occur; ++i)
          for (int j = 0; j < pq.length(); ++j)
            v.push_back(NSBlock(pq.at(j)));
      }
      // Suffix: x[3][le :]
      if (le != last_fwd(py.blocks()))
        v.extend(x[3].domain().suffix(le.idx, le.off));
      v.normalize();
      // std::cerr << "2) Equating " << x[0] << " with " << v << " => \n";
      ModEvent me = x[0].dom(home, v);
      GECODE_ME_CHECK(me);
      repeat |= me_modified(me);
      //std::cerr << x[0] << "\n";
    }
    else {
      if (min_occur > 0)
        return ES_FAILED;
      find(home, x[1], x[0], IntVar(home, 0, 0));
      rel(home, x[0], STRT_EQ, x[3]);
      return home.ES_SUBSUMED(*this);
    }
    return ES_OK;
  }

  forceinline ExecStatus
  Replace::propagate_pass(Space& home, bool& repeat) {
//     std::cerr<<"\nReplace" << (all ? "All" : last ? "Last" : "") << "::propagate: "<< x <<"\n";
    repeat = false;
    assert(x[0].domain().is_normalized() && x[1].domain().is_normalized() &&
           x[2].domain().is_normalized() && x[3].domain().is_normalized());
    if (!all && !check_card()) {
      find(home, x[1], x[0], IntVar(home, 0, 0));
      rel(home, x[0], STRT_EQ, x[3]);
      return home.ES_SUBSUMED(*this);
    }
    int min_occur = 0;
    if (x[1].assigned()) {
      string sq;
      const bool query_is_bytes = x[1].domain().try_val_bytes(sq);
      StringVal query;
      if (!query_is_bytes)
        query = x[1].val_symbols();
      bool same_replacement = false;
      if (x[2].assigned()) {
        string replacement_bytes;
        if (query_is_bytes)
          same_replacement =
            x[2].domain().try_val_bytes(replacement_bytes) &&
            sq == replacement_bytes;
        else
          same_replacement = query == x[2].val_symbols();
      }
      if (same_replacement) {
        rel(home, x[0], STRT_EQ, x[3]);
        return home.ES_SUBSUMED(*this);
      }
      if ((query_is_bytes ? sq.empty() : query.empty()) && !all) {
        last ? rel(home, x[0], x[2], STRT_CAT, x[3])
             : rel(home, x[2], x[0], STRT_CAT, x[3]);
        return home.ES_SUBSUMED(*this);
      }
      if (x[0].assigned()) {
        if (all)
          return decomp_all(home);
        string sx;
        string replacement_bytes;
        const bool replacement_is_bytes = !x[2].assigned() ||
          x[2].domain().try_val_bytes(replacement_bytes);
        if (query_is_bytes && x[0].domain().try_val_bytes(sx) &&
            replacement_is_bytes) {
          size_t n = last ? sx.rfind(sq) : sx.find(sq);
          if (n == string::npos)
            rel(home, x[0], STRT_EQ, x[3]);
          else if (x[2].assigned()) {
            sx.replace(n, sq.size(), replacement_bytes);
            GECODE_ME_CHECK(x[3].eq(home, sx));
          }
          else {
            string pref = sx.substr(0, n);
            string suff = sx.substr(n + sq.size());
            StringVar z(home);
            rel(home, StringVar(home, pref), x[2], STRT_CAT, z);
            rel(home, z, StringVar(home, suff), STRT_CAT, x[3]);
          }
        }
        else {
          const StringVal source = x[0].val_symbols();
          if (query_is_bytes)
            query = StringVal::from_bytes(sq);
          const StringVal::size_type position = last
            ? replace_rfind_symbols(source, query)
            : replace_find_symbols(source, query);
          if (position == replace_npos())
            rel(home, x[0], STRT_EQ, x[3]);
          else if (x[2].assigned()) {
            GECODE_ME_CHECK(x[3].eq(home, replace_symbol_range(
              source, position, query.size(), x[2].val_symbols())));
          }
          else {
            const StringVal prefix = replace_slice_symbols(source, 0, position);
            const StringVal suffix = replace_slice_symbols(
              source, position + query.size());
            StringVar z(home);
            rel(home, StringVar(home, prefix), x[2], STRT_CAT, z);
            rel(home, z, StringVar(home, suffix), STRT_CAT, x[3]);
          }
        }
        return home.ES_SUBSUMED(*this);
      }
      if (query_is_bytes && x[0].may_chars().max() <= 255)
        min_occur = occur(sq);
      else {
        if (query_is_bytes)
          query = StringVal::from_bytes(sq);
        min_occur = occur(query);
      }
    }
    if (x[0].assigned() && x[3].assigned()) {
      string source_bytes, result_bytes;
      const bool equal =
        x[0].domain().try_val_bytes(source_bytes) &&
        x[3].domain().try_val_bytes(result_bytes)
          ? source_bytes == result_bytes
          : x[0].val_symbols() == x[3].val_symbols();
      if (equal) {
        find(home, x[1], x[0], IntVar(home, 0, 0));
        return home.ES_SUBSUMED(*this);
      }
      if (min_occur == 0)
        min_occur += 1;
    }
    // x[0] != x[3] => x[1] occur in x[0] /\ x[2] occur in x[3].
    const DashedString& px  = x[0].domain();
    const DashedString& pq  = x[1].domain();
    const DashedString& pq1 = x[2].domain();
    const DashedString& py  = x[3].domain();
    if (min_occur == 0 && !px.check_equate(py))
      min_occur = 1;
    if (min_occur > 0 && !all) {
      // std::<<cerr << "min_occur = "<<min_occur<<": rewriting into concat!\n";
      NSIntSet intermediate_alphabet = x[0].may_chars();
      intermediate_alphabet.include(x[1].may_chars());
      intermediate_alphabet.include(x[2].may_chars());
      intermediate_alphabet.include(x[3].may_chars());
      StringVar pref, suff;
      if (intermediate_alphabet.max() <= 255) {
        pref = StringVar(home);
        suff = StringVar(home);
      }
      else {
        pref = StringVar(
          home, intermediate_alphabet, 0, x[0].max_length());
        suff = StringVar(
          home, intermediate_alphabet, 0, x[0].max_length());
      }
      StringVarArgs lhs, rhs;
      lhs << pref << x[1] << suff;
      rhs << pref << x[2] << suff;
      gconcat(home, lhs, x[0]);
      gconcat(home, rhs, x[3]);
      find(home, x[1], last ? suff : pref, IntVar(home, 0, 0));
      return home.ES_SUBSUMED(*this);
    }
//     std::cerr << "min_occur: " << min_occur << "\n";
    ExecStatus es = replace_q_x(home, min_occur, repeat);
    if (es != ES_OK)
      return es;
    // std::cerr<<"After replace_q_x: "<< x <<"\n";
    es = replace_q1_y(home, min_occur, repeat);
    if (es != ES_OK)
      return es;
    // std::cerr<<"After replace_q1_y: "<< x <<"\n";
    if (!all && !check_card()) {
      rel(home, x[0], STRT_EQ, x[3]);
      return home.ES_SUBSUMED(*this);
    }
    if (home.failed())
      return ES_FAILED;
//     std::cerr<<"After replace: "<< x <<"\n";
    assert (px.is_normalized() && pq.is_normalized()
        && pq1.is_normalized() && py.is_normalized());
    switch (
      x[0].assigned() + x[1].assigned() + x[2].assigned() + x[3].assigned()
    ) {
      case 4:
      case 3:
        if (x[0].assigned()) {
          repeat = true;
          return ES_OK;
        }
        return ES_FIX;
      case 2:
        if (x[1].assigned() && x[0].assigned()) {
          repeat = true;
          return ES_OK;
        }
        return ES_FIX;
      default:
        return ES_FIX;
    }
  }

  forceinline ExecStatus
  Replace::propagate(Space& home, const ModEventDelta&) {
    while (true) {
      bool repeat;
      ExecStatus status = propagate_pass(home, repeat);
      if (home.failed())
        return ES_FAILED;
      if (!repeat || status != ES_OK)
        return status;
    }
  }

}}
