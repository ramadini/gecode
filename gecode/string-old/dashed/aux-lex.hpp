namespace Gecode { namespace String {

  forceinline NSBlock
  lex_min(const DSBlock& b, float succ) {
    int m = b.S.min();
    NSBlock c;
    if (m < succ)
      c.l = c.u = b.u;
    else
      c.l = c.u = b.l;
    c.S = NSIntSet(m);
    return c;
  }

  forceinline NSBlock
  lex_max(const DSBlock& b, float succ) {
    int m = b.S.max();
    NSBlock c;
    if (m > succ)
      c.l = c.u = b.u;
    else
      c.l = c.u = b.l;
    c.S = NSIntSet(m);
    return c;
  }

  forceinline
  std::vector<float>
  min_succ(const DashedString& x) {
    float suff = -1, eps = 0.5;
    int n = x.length();
    std::vector<float> v(n);
    for (int i = n - 1; i >= 0; --i) {
      const DSBlock& b = x.at(i);
      v[i] = suff;
      int c = b.S.min();
      if (c < suff)
        suff = c + eps;
      else if (b.l > 0)
        suff = c - eps;
    }
    return v;
  }

  forceinline
  std::vector<float>
  max_succ(const DashedString& x) {
    float suff = -1, eps = 0.5;
    int n = x.length();
    std::vector<float> v(n);
    for (int i = n - 1; i >= 0; --i) {
      const DSBlock& b = x.at(i);
      v[i] = suff;
      int c = b.S.max();
      //std::cerr << b << ' ' << m << ' ' << curr << '\n';
      if (c < suff)
        suff = c - eps;
      else if (b.l > 0)
        suff = c + eps;
      // std::cerr << v[i] << '\n';
    }
    return v;
  }

  forceinline bool
  lex_step_xy(
    const DSBlock& xj, float succ, NSBlocks& y, Position& pos, bool& sub
  ) {
    int cs = xj.S.min(), cap;
    if (succ > cs)
      cap = xj.u;
    else
      cap = xj.l;
    while (cap > 0) {
      int i = pos.idx, o = pos.off;
      if (i >= (int) y.size())
        return false;
      NSBlock& yi = y.at(i);
      int cy = yi.S.max(), card_y = yi.u - o;
      if (cs > cy)
        return false;
      if (cs < cy) {
        sub = true;
        if (yi.l > 0 && cs > yi.S.min()) {
          pos = {-1, 0};
          succ = cs;
        }
        return true;
      }
      if (card_y <= cap) {
        cap -= card_y;
        pos = Position{i + 1, 0};
      }
      else {
        pos.off += cap;
        return true;
      }
    }
    return true;
  }

  forceinline bool
  lex_step_yx(
    const DSBlock& yj, float& succ, const NSBlocks& x, Position& pos, bool& sub
  ) {
    int cs = yj.S.max(), cap;
    if (succ > cs)
      cap = yj.l;
    else
      cap = yj.u;
    while (cap > 0) {
      int i = pos.idx, o = pos.off;
      if (i >= (int) x.size()) {
        sub = true;
        return true;
      }
      const NSBlock& xi = x.at(i);
      int cx = xi.S.min(), card_x = xi.l - o;
      if (cs < cx)
        return false;
      if (cs > cx) {
        sub = true;
        if (xi.l > 0 && cx > yj.S.min()) {
          pos = {-1, 0};
          succ = cx;
        }
        return true;
      }
      if (card_x <= cap) {
        cap -= card_x;
        pos = Position{i + 1, 0};
      }
      else {
        pos.off += cap;
        return true;
      }
    }
    return true;
  }

}}
