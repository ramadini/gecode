namespace Gecode { namespace String { namespace Int {

  template <class ViewX, class ViewY>
  forceinline int
  find_fixed(ViewX x, ViewY y) {
    assert (x.assigned() && y.assigned() && y.max_length() <= x.min_length());
    int j = 0, k = 0, nx = x.size(), ny = y.size();
    for (int i = 0; i < nx && ny > 0; ) {
      if (baseMin(x[i]) == baseMin(y[j])) {
        int lx = lbound(x[i]), ly = ubound(y[j]);
        if (lx == ly || (lx > ly && (j == 0 || j == ny-1))) {
          ++i;
          ++j;
          ny -= ly;
          continue;
        }
      }
      ++i;
      j = 0;
      k = i;
      ny = y.size();
    }
    return ny == 0 ? k : 0;
  }

}}}
