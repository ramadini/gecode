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

  forceinline
  StringVarImp::DomainState::DomainState(StringVarImp& x0)
    : x(&x0), min_length(x0.min_length()), max_length(x0.max_length()),
      unassigned(!x0.assigned()) {
    x->ds.changed(false);
  }

  forceinline bool
  StringVarImp::DomainState::modified(void) const {
    return unassigned && x->ds.changed();
  }

  forceinline ModEvent
  StringVarImp::notify(
    Space& home, ModEvent me, const DomainState& state
  ) {
    if (!state.modified())
      return me;
    ModEvent next = state.x->notify(
      home, state.min_length, state.max_length);
    GECODE_ME_CHECK(next);
    return me_combine(me, next);
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
    DomainState state(*this);
    if (!ds.equate(home, d))
      return ME_STRING_FAILED;
    return notify(home, ME_STRING_NONE, state);
  }

  forceinline ModEvent
  StringVarImp::refine(Space& home, const NSBlocks& d) {
    DomainState state(*this);
    d.empty() ? ds.set_null(home) : ds.update(home, d);
    ds.changed(true);
    return notify(home, ME_STRING_NONE, state);
  }

  forceinline StringVarImp::DomainState
  StringVarImp::begin_refinement(void) {
    return DomainState(*this);
  }

  forceinline ModEvent
  StringVarImp::commit_refinement(
    Space& home, const DomainState& state
  ) {
    ds.changed(true);
    return notify(home, ME_STRING_NONE, state);
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
    DomainState state(*this);
    DomainState xstate(*x);
    if (!ds.equate(home, xs))
      return ME_STRING_FAILED;
    ModEvent me = notify(home, ME_STRING_NONE, state);
    GECODE_ME_CHECK(me);
    return notify(home, me, xstate);
  }

  forceinline ModEvent
  StringVarImp::lex(Space& home, StringVarImp* x, bool lt) {
    DashedString& xs = x->ds;
    DomainState state(*this);
    DomainState xstate(*x);
    if (!ds.lex(home, xs, lt))
      return ME_STRING_FAILED;
    ModEvent me = notify(home, ME_STRING_NONE, state);
    GECODE_ME_CHECK(me);
    return notify(home, me, xstate);
  }

  forceinline ModEvent
  StringVarImp::inc(Space& home, bool lt) {
    DomainState state(*this);
    if (!ds.increasing(home, lt))
      return ME_STRING_FAILED;
    return notify(home, ME_STRING_NONE, state);
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
    DomainState state(*this);
    if (!ds.refine_lb(l))
      return ME_STRING_FAILED;
    return notify(home, ME_STRING_NONE, state);
  }

  forceinline ModEvent
  StringVarImp::ub(Space& home, int u) {
    DomainState state(*this);
    if (!ds.refine_ub(home, u))
      return ME_STRING_FAILED;
    return notify(home, ME_STRING_NONE, state);
  }

  forceinline ModEvent
  StringVarImp::concat(Space& home, StringVarImp* x, StringVarImp* y) {
    DashedString& xs = x->ds;
    DashedString& ys = y->ds;
    DomainState state(*this);
    DomainState xstate(*x);
    DomainState ystate(*y);
    if (!ds.concat(home, xs, ys))
      return ME_STRING_FAILED;
    ModEvent me = notify(home, ME_STRING_NONE, state);
    GECODE_ME_CHECK(me);
    me = notify(home, me, xstate);
    GECODE_ME_CHECK(me);
    return notify(home, me, ystate);
  }

  forceinline ModEvent
  StringVarImp::gconcat(Space& home, vec<StringVarImp*> x) {
    vec<DashedString*> xs;
    DomainState state(*this);
    std::vector<DomainState> states;
    states.reserve(x.size());
    for (auto& i : x) {
      // Operands assigned to "" contribute no block: including their null
      // block would break the sweep normalization invariant (null blocks
      // must not occur inside a dashed view).
      if (i->ds.max_length() == 0)
        continue;
      states.push_back(DomainState(*i));
      xs.push(&i->ds);
    }
    Region region;
    GConcatView view(region, xs);
    if (!sweep_concat(home, view, xs, ds))
      return ME_STRING_FAILED;
    ModEvent me = notify(home, ME_STRING_NONE, state);
    GECODE_ME_CHECK(me);
    for (const DomainState& xstate : states) {
      me = notify(home, me, xstate);
      GECODE_ME_CHECK(me);
    }
    return me;
  }

  forceinline ModEvent
  StringVarImp::find(Space& home, StringVarImp* x, int& l, int& u, bool b) {
    DashedString& xs = x->ds;
    DomainState state(*this);
    DomainState xstate(*x);
    if (!ds.find(home, xs, l, u, b))
      return ME_STRING_FAILED;
    ModEvent me = notify(home, ME_STRING_NONE, state);
    GECODE_ME_CHECK(me);
    return notify(home, me, xstate);
  }

  forceinline ModEvent
  StringVarImp::pow(Space& home, StringVarImp* x, int& l, int& u) {
    DashedString& xs = x->ds;
    DomainState state(*this);
    DomainState xstate(*x);
    if (!ds.pow(home, xs, l, u))
      return ME_STRING_FAILED;
    ModEvent me = notify(home, ME_STRING_NONE, state);
    GECODE_ME_CHECK(me);
    return notify(home, me, xstate);
  }

  forceinline ModEvent
  StringVarImp::rev(Space& home, StringVarImp* x) {
    DashedString& xs = x->ds;
    DomainState state(*this);
    DomainState xstate(*x);
    if (!ds.rev(home, xs))
      return ME_STRING_FAILED;
    ModEvent me = notify(home, ME_STRING_NONE, state);
    GECODE_ME_CHECK(me);
    return notify(home, me, xstate);
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
