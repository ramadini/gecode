namespace Gecode { namespace String {

  forceinline void
  StringVarImp::ext_remove(int i) {
    ext.erase(ext.begin() + i);
  }

  forceinline void
  StringVarImp::ext_list(const std::vector<string>& e) {
    ext = e;
  }

  forceinline std::vector<string>&
  StringVarImp::ext_list() {
    return ext;
  }
  forceinline const std::vector<string>&
  StringVarImp::ext_list() const {
    return ext;
  }

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
  StringVarImp::dom(Space& home, const NSBlocks& d) {
    bool b = !ds.known();
    if (!ds.equate(home, d))
      return ME_STRING_FAILED;
    if (b && ds._changed) {
      StringDelta d(true);
      return notify(home, ds.known() ? ME_STRING_VAL : ME_STRING_DOM, d);
    }
    return ME_STRING_NONE;
  }

  forceinline ModEvent
  StringVarImp::eq(Space& home, string x) {
    if (!check_sweep<DSBlock, DashedString, char, string>(ds, x))
      return ME_STRING_FAILED;
    if (ds.known())
      return ME_STRING_NONE;
    ds.update(home, x);
    StringDelta d(true);
    return notify(home, ME_STRING_VAL, d);
  }

  forceinline ModEvent
  StringVarImp::eq(Space& home, StringVarImp* x) {
    DashedString& xs = x->ds;
    bool b1 = !ds.known(), b2 = !xs.known();
    if (!ds.equate(home, xs))
      return ME_STRING_FAILED;
    ModEvent me = ME_STRING_NONE;
    if (b1 && ds._changed) {
      StringDelta d(true);
      me = notify(home, ds.known() ? ME_STRING_VAL : ME_STRING_DOM, d);
      GECODE_ME_CHECK(me);
    }
    if (b2 && xs._changed) {
      StringDelta d(true);
      ModEvent xme = x->notify(
        home, xs.known() ? ME_STRING_VAL : ME_STRING_DOM, d
      );
      GECODE_ME_CHECK(xme);
      me = me_combine(me, xme);
    }
    return me;
  }

  forceinline ModEvent
  StringVarImp::lex(Space& home, StringVarImp* x, bool lt) {
    DashedString& xs = x->ds;
    bool b1 = !ds.known(), b2 = !xs.known();
    if (!ds.lex(home, xs, lt))
      return ME_STRING_FAILED;
    ModEvent me = ME_STRING_NONE;
    if (b1 && ds._changed) {
      StringDelta d(ds._changed);
      me = notify(home, ds.known() ? ME_STRING_VAL : ME_STRING_DOM, d);
      GECODE_ME_CHECK(me);
    }
    if (b2 && xs._changed) {
      StringDelta d(xs._changed);
      ModEvent xme = x->notify(
        home, xs.known() ? ME_STRING_VAL : ME_STRING_DOM, d
      );
      GECODE_ME_CHECK(xme);
      me = me_combine(me, xme);
    }
    return me;
  }

  forceinline ModEvent
  StringVarImp::inc(Space& home, bool lt) {
    bool b = !ds.known();
    if (!ds.increasing(home, lt))
      return ME_STRING_FAILED;
    if (b && ds._changed) {
      StringDelta d(true);
      return notify(home, ds.known() ? ME_STRING_VAL : ME_STRING_DOM, d);
    }
    return ME_STRING_NONE;
  }

  forceinline ModEvent
  StringVarImp::commit(
    Space& home, Branch::Level l, Branch::Value v, Branch::Block b, unsigned a
  ) {
    ds.commit(home, l, v, b, a);
    Delta d = StringDelta(ds._changed);
    return notify(home, assigned() ? ME_STRING_VAL : ME_STRING_DOM, d);
  }

  forceinline ModEvent
  StringVarImp::lb(Space& home, int l) {
    bool b = !ds.known();
    if (ds.refine_lb(l)) {
      if (b && ds._changed) {
        StringDelta d(true);
        return notify(home, assigned() ? ME_STRING_VAL : ME_STRING_DOM, d);
      }
      else
        return ME_STRING_NONE;
    }
    else
      return ME_STRING_FAILED;
  }

  forceinline ModEvent
  StringVarImp::ub(Space& home, int u) {
    bool b = !ds.known();
    if (ds.refine_ub(home, u)) {
      if (b && ds._changed) {
        StringDelta d(true);
        return notify(home, assigned() ? ME_STRING_VAL : ME_STRING_DOM, d);
      }
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
    if (ds.concat(home, xs, ys)) {
      ModEvent me = ME_STRING_NONE;
      if (b1 && ds._changed) {
        StringDelta d(true);
        me = notify(home, ds.known() ? ME_STRING_VAL : ME_STRING_DOM, d);
        GECODE_ME_CHECK(me);
      }
      if (b2 && xs._changed) {
        StringDelta d(true);
        ModEvent xme = x->notify(
          home, xs.known() ? ME_STRING_VAL : ME_STRING_DOM, d
        );
        GECODE_ME_CHECK(xme);
        me = me_combine(me, xme);
      }
      if (b3 && ys._changed) {
        StringDelta d(true);
        ModEvent yme = y->notify(
          home, ys.known() ? ME_STRING_VAL : ME_STRING_DOM, d
        );
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
    NSBlocks xn;
    for (auto i : x) {
      DashedString& d = i->ds;
      d._changed = false;
      xs.push(&d);
      xn.extend(NSBlocks(d.blocks()));
      bv.push(!d.known());
    }
    ds._changed = false;
    if (sweep_concat(home, xn, xs, ds)) {
      ModEvent me = ME_STRING_NONE;
      if (b && ds._changed) {
        StringDelta d(true);
        me = notify(home, ds.known() ? ME_STRING_VAL : ME_STRING_DOM, d);
        GECODE_ME_CHECK(me);
      }
      for (int i = 0; i < x.size(); ++i) {
        if (bv[i] && xs[i]->_changed) {
          StringDelta d(true);
          ModEvent xme = x[i]->notify(
            home, xs[i]->known() ? ME_STRING_VAL : ME_STRING_DOM, d
          );
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
    if (ds.find(home, xs, l, u, b)) {
      ModEvent me = ME_STRING_NONE;
      if (b1 && ds._changed) {
        StringDelta d(ds._changed);
      me = notify(home, ds.known() ? ME_STRING_VAL : ME_STRING_DOM, d);
      GECODE_ME_CHECK(me);
      }
      if (b2 && xs._changed) {
        StringDelta d(xs._changed);
        ModEvent xme = x->notify(
          home, xs.known() ? ME_STRING_VAL : ME_STRING_DOM, d
        );
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
    if (ds.pow(home, xs, l, u)) {
      ModEvent me = ME_STRING_NONE;
      if (b1 && ds._changed) {
        StringDelta d(ds._changed);
        me = notify(home, ds.known() ? ME_STRING_VAL : ME_STRING_DOM, d);
        GECODE_ME_CHECK(me);
      }
      if (b2 && xs._changed) {
        StringDelta d(xs._changed);
        ModEvent xme = x->notify(
          home, xs.known() ? ME_STRING_VAL : ME_STRING_DOM, d
        );
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
    if (ds.rev(home, xs)) {
      ModEvent me = ME_STRING_NONE;
      if (b1 && ds._changed) {
        StringDelta d(ds._changed);
        me = notify(home, ds.known() ? ME_STRING_VAL : ME_STRING_DOM, d);
        GECODE_ME_CHECK(me);
      }
      if (b2 && xs._changed) {
        StringDelta d(xs._changed);
        ModEvent xme = x->notify(
          home, xs.known() ? ME_STRING_VAL : ME_STRING_DOM, d
        );
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
  StringVarImp::cancel(Space& home, Advisor& a) {
    StringVarImpBase::cancel(home, a, assigned());
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
