#include <gecode/string.hh>
#include <gecode/string/gcc.hh>

namespace Gecode {

  void
  gcc(Home home, StringVar x, const IntArgs& cover, IntVarArgs count) {
    int n = cover.size();
    if (n != count.size())
      throw Int::ArgumentSizeMismatch("String::gcc");
    GECODE_POST;
    vec2 scover(n);
    for (int i = 0; i < n; ++i)
      scover[i] = std::make_pair(cover[i], i);
    std::sort(scover.begin(), scover.end());
    vec2 distinct_cover;
    IntVarArgs distinct_counts;
    for (int i = 0; i < n; ++i) {
      int c = scover[i].first;
      IntVar occurrences = count[scover[i].second];
      if (!distinct_cover.empty() && distinct_cover.back().first == c) {
        rel(home, distinct_counts[distinct_counts.size() - 1], IRT_EQ,
            occurrences);
      } else {
        distinct_cover.push_back(std::make_pair(c, distinct_counts.size()));
        distinct_counts << occurrences;
      }
    }
    if (home.failed())
      return;
    ViewArray<Int::IntView> a(home, distinct_counts);
    GECODE_ES_FAIL((String::GCC::post(home, x, distinct_cover, a)));
  }

  void
  must_chars(Home home, StringVar x, const String::NSIntSet& s) {
    GECODE_POST;
    IntArgs cover;
    for (String::NSIntSet::iterator it = s.begin(); it(); ++it)
      cover << *it;
    if (cover.size() == 0)
      return;
    if (cover.size() > x.max_length()) {
      home.fail();
      return;
    }
    IntVarArgs count;
    int ub = x.max_length() - cover.size() + 1;
    for (int i = 0; i < cover.size(); ++i)
      count << IntVar(home, 1, ub);
    gcc(home, x, cover, count);
  }

}

// STATISTICS: string-post
