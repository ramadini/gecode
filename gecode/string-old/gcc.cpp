#include <gecode/string.hh>
#include <gecode/string/gcc.hh>

namespace Gecode {

  // FIXME: Should cover be a shared copy?
  void
  gcc(Home home, StringVar x, const IntArgs& cover, IntVarArgs count) {
    int n = cover.size();
    assert (n == count.size());
    GECODE_POST;
    vec2 scover(n);
    for (int i = 0; i < n; ++i)
      scover[i] = std::make_pair(cover[i], i);
    std::sort(scover.begin(), scover.end());
    ViewArray<Int::IntView> a(home, count);
    GECODE_ES_FAIL((String::GCC::post(home, x, scover, a)));
  }

  void
  must_chars(Home home, StringVar x, const String::NSIntSet& s) {
    IntArgs cover;
    for (String::NSIntSet::iterator it = s.begin(); it(); ++it)
      cover << *it;
    IntVarArgs count;
    int ub = x.max_length() - cover.size() + 1;
    for (int i = 0; i < cover.size(); ++i)
      count << IntVar(home, 1, ub);
    gcc(home, x, cover, count);
  }

}
