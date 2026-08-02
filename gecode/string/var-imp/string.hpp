namespace Gecode { namespace String {

  forceinline bool
  StringVarImp::assigned() const {
    return ds.known();
  }

  forceinline string
  StringVarImp::val() const {
    return ds.val();
  }

  forceinline const DashedString&
  StringVarImp::domain() const {
    return ds;
  }

  forceinline int
  StringVarImp::min_length() const {
    return ds.min_length();
  }

  forceinline int
  StringVarImp::max_length() const {
    return ds.max_length();
  }

  forceinline ModEvent
  StringVarImp::notify(Space& home, int old_min_length, int old_max_length,
                       Delta& d) {
    ModEvent me = assigned() ? ME_STRING_VAL :
      ((old_min_length != min_length()) || (old_max_length != max_length()) ?
       ME_STRING_LEN : ME_STRING_DOM);
    return StringVarImpBase::notify(home, me, d);
  }

  forceinline ModEvent
  StringVarImp::notify(Space& home, int old_min_length, int old_max_length) {
    StringDelta d(true);
    return notify(home, old_min_length, old_max_length, d);
  }

  forceinline ModEvent
  StringVarImp::dom(Space& home, const NSBlocks& d) {
    bool b = !ds.known();
    int old_min_length = min_length(), old_max_length = max_length();
    if (!ds.equate(home, d))
      return ME_STRING_FAILED;
    if (b && ds._changed)
      return notify(home, old_min_length, old_max_length);
    return ME_STRING_NONE;
  }

  forceinline ModEvent
  StringVarImp::eq(Space& home, string x) {
    if (!check_sweep<DSBlock, DashedString, char, string>(ds, x))
      return ME_STRING_FAILED;
    if (ds.known())
      return ME_STRING_NONE;
    int old_min_length = min_length(), old_max_length = max_length();
    ds.update(home, x);
    return notify(home, old_min_length, old_max_length);
  }

  forceinline ModEvent
  StringVarImp::eq(Space& home, StringVarImp* x) {
    DashedString& xs = x->ds;
    bool b1 = !ds.known(), b2 = !xs.known();
    int old_min_length = min_length(), old_max_length = max_length();
    int old_x_min_length = x->min_length(),
        old_x_max_length = x->max_length();
    if (!ds.equate(home, xs))
      return ME_STRING_FAILED;
    ModEvent me = ME_STRING_NONE;
    if (b1 && ds._changed) {
      me = notify(home, old_min_length, old_max_length);
      GECODE_ME_CHECK(me);
    }
    if (b2 && xs._changed) {
      ModEvent xme = x->notify(home, old_x_min_length, old_x_max_length);
      GECODE_ME_CHECK(xme);
      me = me_combine(me, xme);
    }
    return me;
  }

  forceinline ModEvent
  StringVarImp::lex(Space& home, StringVarImp* x, bool lt) {
    DashedString& xs = x->ds;
    bool b1 = !ds.known(), b2 = !xs.known();
    int old_min_length = min_length(), old_max_length = max_length();
    int old_x_min_length = x->min_length(),
        old_x_max_length = x->max_length();
    if (!ds.lex(home, xs, lt))
      return ME_STRING_FAILED;
    ModEvent me = ME_STRING_NONE;
    if (b1 && ds._changed) {
      me = notify(home, old_min_length, old_max_length);
      GECODE_ME_CHECK(me);
    }
    if (b2 && xs._changed) {
      ModEvent xme = x->notify(home, old_x_min_length, old_x_max_length);
      GECODE_ME_CHECK(xme);
      me = me_combine(me, xme);
    }
    return me;
  }

  forceinline ModEvent
  StringVarImp::inc(Space& home, bool lt) {
    bool b = !ds.known();
    int old_min_length = min_length(), old_max_length = max_length();
    if (!ds.increasing(home, lt))
      return ME_STRING_FAILED;
    if (b && ds._changed)
      return notify(home, old_min_length, old_max_length);
    return ME_STRING_NONE;
  }

  forceinline ModEvent
  StringVarImp::commit(
    Space& home, Branch::Level l, Branch::Value v, Branch::Block b, unsigned a
  ) {
    int old_min_length = min_length(), old_max_length = max_length();
    ds.commit(home, l, v, b, a);
    Delta d = StringDelta(ds._changed);
    return notify(home, old_min_length, old_max_length, d);
  }

  forceinline ModEvent
  StringVarImp::lb(Space& home, int l) {
    bool b = !ds.known();
    int old_min_length = min_length(), old_max_length = max_length();
    if (ds.refine_lb(l)) {
      if (b && ds._changed)
        return notify(home, old_min_length, old_max_length);
      else
        return ME_STRING_NONE;
    }
    else
      return ME_STRING_FAILED;
  }

  forceinline ModEvent
  StringVarImp::ub(Space& home, int u) {
    bool b = !ds.known();
    int old_min_length = min_length(), old_max_length = max_length();
    if (ds.refine_ub(home, u)) {
      if (b && ds._changed)
        return notify(home, old_min_length, old_max_length);
      else
        return ME_STRING_NONE;
    }
    else
      return ME_STRING_FAILED;
  }

  forceinline ModEvent
  StringVarImp::concat(Space& home, StringVarImp* x, StringVarImp* y) {
    DashedString& xs = x->ds;
    DashedString& ys = y->ds;
    bool b1 = !ds.known(), b2 = !xs.known(), b3 = !ys.known();
    int old_min_length = min_length(), old_max_length = max_length();
    int old_x_min_length = x->min_length(),
        old_x_max_length = x->max_length();
    int old_y_min_length = y->min_length(),
        old_y_max_length = y->max_length();
    if (ds.concat(home, xs, ys)) {
      ModEvent me = ME_STRING_NONE;
      if (b1 && ds._changed) {
        me = notify(home, old_min_length, old_max_length);
        GECODE_ME_CHECK(me);
      }
      if (b2 && xs._changed) {
        ModEvent xme = x->notify(home, old_x_min_length, old_x_max_length);
        GECODE_ME_CHECK(xme);
        me = me_combine(me, xme);
      }
      if (b3 && ys._changed) {
        ModEvent yme = y->notify(home, old_y_min_length, old_y_max_length);
        GECODE_ME_CHECK(yme);
        me = me_combine(me, yme);
      }
      return me;
    }
    else
      return ME_STRING_FAILED;
  }

  forceinline ModEvent
  StringVarImp::gconcat(Space& home, vec<StringVarImp*> x) {
    vec<DashedString*> xs;
    bool b = !ds.known();
    vec<bool> bv;
    vec<int> old_min_lengths, old_max_lengths;
    int old_min_length = min_length(), old_max_length = max_length();
    for (auto& i : x) {
      DashedString& d = i->ds;
      d._changed = false;
      xs.push(&d);
      bv.push(!d.known());
      old_min_lengths.push(i->min_length());
      old_max_lengths.push(i->max_length());
    }
    ds._changed = false;
    Region region;
    GConcatView view(region, xs);
    if (sweep_concat(home, view, xs, ds)) {
      ModEvent me = ME_STRING_NONE;
      if (b && ds._changed) {
        me = notify(home, old_min_length, old_max_length);
        GECODE_ME_CHECK(me);
      }
      for (int i = 0; i < x.size(); ++i) {
        if (bv[i] && xs[i]->_changed) {
          ModEvent xme = x[i]->notify(
            home, old_min_lengths[i], old_max_lengths[i]);
          GECODE_ME_CHECK(xme);
          me = me_combine(me, xme);
        }
      }
      return me;
    }
    else
      return ME_STRING_FAILED;
  }

  forceinline ModEvent
  StringVarImp::find(Space& home, StringVarImp* x, int& l, int& u, bool b) {
    DashedString& xs = x->ds;
    bool b1 = !ds.known(), b2 = !xs.known();
    int old_min_length = min_length(), old_max_length = max_length();
    int old_x_min_length = x->min_length(),
        old_x_max_length = x->max_length();
    if (ds.find(home, xs, l, u, b)) {
      ModEvent me = ME_STRING_NONE;
      if (b1 && ds._changed) {
        me = notify(home, old_min_length, old_max_length);
        GECODE_ME_CHECK(me);
      }
      if (b2 && xs._changed) {
        ModEvent xme = x->notify(home, old_x_min_length, old_x_max_length);
        GECODE_ME_CHECK(xme);
        me = me_combine(me, xme);
      }
      return me;
    }
    else
      return ME_STRING_FAILED;
  }

  forceinline ModEvent
  StringVarImp::pow(Space& home, StringVarImp* x, int& l, int& u) {
    DashedString& xs = x->ds;
    bool b1 = !ds.known(), b2 = !xs.known();
    int old_min_length = min_length(), old_max_length = max_length();
    int old_x_min_length = x->min_length(),
        old_x_max_length = x->max_length();
    if (ds.pow(home, xs, l, u)) {
      ModEvent me = ME_STRING_NONE;
      if (b1 && ds._changed) {
        me = notify(home, old_min_length, old_max_length);
        GECODE_ME_CHECK(me);
      }
      if (b2 && xs._changed) {
        ModEvent xme = x->notify(home, old_x_min_length, old_x_max_length);
        GECODE_ME_CHECK(xme);
        me = me_combine(me, xme);
      }
      return me;
    }
    else
      return ME_STRING_FAILED;
  }

  forceinline ModEvent
  StringVarImp::rev(Space& home, StringVarImp* x) {
    DashedString& xs = x->ds;
    bool b1 = !ds.known(), b2 = !xs.known();
    int old_min_length = min_length(), old_max_length = max_length();
    int old_x_min_length = x->min_length(),
        old_x_max_length = x->max_length();
    if (ds.rev(home, xs)) {
      ModEvent me = ME_STRING_NONE;
      if (b1 && ds._changed) {
        me = notify(home, old_min_length, old_max_length);
        GECODE_ME_CHECK(me);
      }
      if (b2 && xs._changed) {
        ModEvent xme = x->notify(home, old_x_min_length, old_x_max_length);
        GECODE_ME_CHECK(xme);
        me = me_combine(me, xme);
      }
      return me;
    }
    else
      return ME_STRING_FAILED;
  }

  forceinline void
  StringVarImp::subscribe(
    Space& home, Propagator& p, PropCond pc, bool schedule = true) {
    StringVarImpBase::subscribe(home, p, pc, assigned(), schedule);
  }

  forceinline void
  StringVarImp::cancel(Space& home, Propagator& p, PropCond pc) {
    StringVarImpBase::cancel(home, p, pc);
  }

  forceinline void
  StringVarImp::subscribe(Space& home, Advisor& a, bool fail) {
    StringVarImpBase::subscribe(home, a, assigned(), fail);
  }

  forceinline void
  StringVarImp::cancel(Space& home, Advisor& a, bool fail) {
    StringVarImpBase::cancel(home, a, fail);
  }

  forceinline void
  StringVarImp::reschedule(Space& home, Propagator& p, PropCond pc) {
    StringVarImpBase::reschedule(home,p,pc,assigned());
  }

  forceinline StringVarImp*
  StringVarImp::copy(Space& home) {
    return copied() ? static_cast<StringVarImp*>(forward())
                    : new (home) StringVarImp(home, *this);
  }

}}
