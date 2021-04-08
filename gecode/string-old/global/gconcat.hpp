namespace Gecode { namespace String {

  forceinline
  GConcat::GConcat(Home home, ViewArray<StringView>& x, StringView y)
  : NaryOnePropagator<StringView, PC_STRING_DOM>(home, x, y) {}

  forceinline
  GConcat::GConcat(Space& home, GConcat& p)
  : NaryOnePropagator<StringView, PC_STRING_DOM>(home, p) {}

  forceinline ExecStatus
  GConcat::post(Home home, ViewArray<StringView>& x, StringView y) {
    // Remove aliases.
    for (int i = 0; i < x.size(); ++i)
      for (int j = i + 1; j < x.size(); ++j)
        if (x[i].varimp() == x[j].varimp()) {
          StringVar z(
            home, x[j].may_chars(), x[j].min_length(), x[j].max_length()
          );
          x[j] = z;
          rel(home, x[i], STRT_EQ, z);
        }
    (void) new (home) GConcat(home, x, y);
    return ES_OK;
  }
  
  forceinline Actor*
  GConcat::copy(Space& home) {
    return new (home) GConcat(home, *this);
  }

  forceinline bool
  GConcat::refine_card(Space& home) {
    // std::cerr << "GConcat<StringView>::refine_card " << y << "\n";
    long lx = 0, ux = 0;
    for (auto xi : x) {
      lx += xi.min_length();
      ux += xi.max_length();
    }
    if (lx > ux || lx > y.max_length() || ux < y.min_length())
      return false;
    bool again;
    do {
      // std::cerr << "x: " << x << '\n' << "y: " << y << '\n';
      // std::cerr << "lx: " << lx << " ux: " << ux
      // << " minl(y): "<<y.min_length()<<" maxl(y): "<<y.max_length() << '\n';
      again = false;
      if (lx > y.min_length()) {
        y.lb(home, lx);
        again = true;
      }
      if (ux < y.max_length()) {
        y.ub(home, ux);
        again = true;
      }
      int i = -1, ly = y.min_length(), uy = y.max_length();
      for (auto xi : x) {
        i++;
        long lxi = ly - ux + xi.max_length();
        long uxi = max(0, uy - lx + xi.min_length());
        if (lxi > uxi)
          return false;
        if (lxi > xi.min_length()) {
          xi.lb(home, lxi);
          again = true;
        }
        if (uxi < xi.max_length()) {
          xi.ub(home, uxi);
          again = true;
        }
      }
      if (again) {
        lx = 0, ux = 0;
        for (auto xi : x) {
          lx += xi.min_length();
          ux += xi.max_length();
        }
      }
      if (lx > ux || lx > y.max_length() || ux < y.min_length())
        return false;
    }
    while (again);
    // std::cerr << "GConcat<StringView>::refined!\n";
    return true;
  }

  forceinline ExecStatus
  GConcat::propagate(Space& home, const ModEventDelta& m) {
    // std::cerr<<"\nGConcat::propagate " <<y<< " = gconcat(" <<x<< ")\n";
    if (x.assigned()) {
      string val;
      for (auto s : x)
        val += s.val();
      if (y.assigned())
        return val == y.val() ? home.ES_SUBSUMED(*this) : ES_FAILED;
      else
        rel(home, y, STRT_EQ, StringVar(home, val));
      // std::cerr<<"propagated "<<y<<" = "<<val<<"\n";
      return home.ES_SUBSUMED(*this);
    }
    if (!refine_card(home))
      return ES_FAILED;
    ModEvent me = y.gconcat(home, x);
    GECODE_ME_CHECK(me);
    if (!refine_card(home))
      return ES_FAILED;
    if (me != ME_STRING_NONE) {
      StringDelta d(true);
      if (y.pdomain()->changed() && !y.assigned()) {
        me = y.varimp()->notify(
          home, y.assigned() ? ME_STRING_VAL : ME_STRING_DOM, d
        );
        GECODE_ME_CHECK(me);
        assert (y.pdomain()->is_normalized());
      }
      int i = -1;
      for (auto xi : x) {
        i++;
        if (xi.pdomain()->changed() && !xi.assigned()) {
          ModEvent xme = xi.varimp()->notify(
            home, xi.assigned() ? ME_STRING_VAL : ME_STRING_DOM, d
          );
          GECODE_ME_CHECK(xme);
          me = xi.varimp()->me_combine(me, xme);
        }
        assert (xi.pdomain()->is_normalized());
      }
    }
    // std::cerr<<"propagated "<<y<<" = gconcat("<<x<<")\n";
    return x.assigned() ? propagate(home, m) : ES_FIX;
  }

}}
