#include <gecode/string.hh>
#include <gecode/string/substr.hh>

#include <algorithm>

namespace Gecode {

  // Clip both boundaries into x, keeping the end at or after the start.
  // Empty intervals then leave x = prefix ++ suffix without overlap.
  void
  substr(Home home, StringVar x, IntVar i, IntVar j, StringVar y) {
    GECODE_POST;
    String::StringView source(x), result(y);
    Gecode::Int::IntView from(i), to(j);
    if (from == to) {
      GECODE_ES_FAIL(String::CharAt::post(home, source, from, result));
      return;
    }
    if (source.assigned() && String::Substring::supports_domains(from, to)) {
      GECODE_ES_FAIL(String::Substring::post
        (home, source, from, to, result));
      return;
    }
    const int lower = x.min_length(), upper = x.max_length();
    const int first_lower = std::max(1, i.min());
    const int first_upper = std::max(1, i.max());
    IntVar lx(home, lower, upper),
      first(home, first_lower, first_upper),
      before_first(home, first_lower - 1, first_upper - 1),
      nonnegative_last(home, std::max(0, j.min()), std::max(0, j.max())),
      clipped_last(home, 0, upper),
      end(home, 0, upper),
      prefix_length(home, 0, upper),
      result_length(home, 0, upper),
      suffix_length(home, 0, upper);

    length(home, x, lx);
    max(home, IntVar(home, 1, 1), i, first);
    IntArgs offset_coefficients;
    offset_coefficients << 1 << -1;
    IntVarArgs offset_bounds;
    offset_bounds << first << before_first;
    linear(home, offset_coefficients, offset_bounds, IRT_EQ, 1);
    min(home, lx, before_first, prefix_length);
    max(home, IntVar(home, 0, 0), j, nonnegative_last);
    min(home, lx, nonnegative_last, clipped_last);
    max(home, prefix_length, clipped_last, end);

    IntArgs coefficients;
    coefficients << 1 << 1 << -1;
    IntVarArgs result_bounds;
    result_bounds << prefix_length << result_length << end;
    linear(home, coefficients, result_bounds, IRT_EQ, 0);
    IntVarArgs suffix_bounds;
    suffix_bounds << end << suffix_length << lx;
    linear(home, coefficients, suffix_bounds, IRT_EQ, 0);
    length(home, y, result_length);

    StringVar prefix(home, x.may_chars(), 0, upper),
      prefix_result(home, x.may_chars(), 0, upper),
      suffix(home, x.may_chars(), 0, upper);
    length(home, prefix, prefix_length);
    length(home, suffix, suffix_length);
    rel(home, prefix, y, STRT_CAT, prefix_result);
    rel(home, prefix_result, suffix, STRT_CAT, x);
  }

  // x contains y <==> find(y, x) = n /\ n > 0.
  void
  contains(Home home, StringVar x, StringVar y) {
    GECODE_POST;
    int max_index = x.max_length() - y.min_length() + 1;
    if (max_index < 1) {
      home.fail();
      return;
    }
    IntVar n(home, 1, max_index);
    find(home, y, x, n);
  }

  // b = (x contains y) <==> find(y, x) = n /\ b = [n > 0]
  void
  contains(Home home, StringVar x, StringVar y, BoolVar b) {
    GECODE_POST;
    IntVar n(home, 0, std::max(0, x.max_length() - y.min_length() + 1));
    find(home, y, x, n);
    rel(home, n, IRT_GR, 0, b);
  }

  // x startswith y <==> x = y ++ z.
  void
  startswith(Home home, StringVar x, StringVar y) {
    GECODE_POST;
    StringVar z(home, x.may_chars(), 0,
                std::max(0, x.max_length() - y.min_length()));
    rel(home, y, z, STRT_CAT, x);
  }

  // x endswith y <==> x = z ++ y.
  void
  endswith(Home home, StringVar x, StringVar y) {
    GECODE_POST;
    StringVar z(home, x.may_chars(), 0,
                std::max(0, x.max_length() - y.min_length()));
    rel(home, z, y, STRT_CAT, x);
  }

  // b = (x startswith y) <==> b = [find(y, x) = 1].
  void
  startswith(Home home, StringVar x, StringVar y, BoolVar b) {
    IntVar i(home, 0, std::max(1, x.max_length()));
    find(home, y, x, i);
    rel(home, i, IRT_EQ, 1, Reify(b));
  }

  // b = (x endswith y) <==> b = [find(rev(y), rev(x)) = 1].
  void
  endswith(Home home, StringVar x, StringVar y, BoolVar b) {
    IntVar i(home, 0, std::max(1, x.max_length()));
    StringVar x1(home, x.may_chars(), x.min_length(), x.max_length()),
      y1(home, y.may_chars(), y.min_length(), y.max_length());
    rel(home, x, STRT_REV, x1);
    rel(home, y, STRT_REV, y1);
    find(home, y1, x1, i);
    rel(home, i, IRT_EQ, 1, Reify(b));
  }


}

// STATISTICS: string-post
