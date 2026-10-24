/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "test/string.hh"

#include <memory>

namespace Test { namespace String { namespace Var {

  using namespace Gecode;

  struct Counts {
    int changes = 0;
    int failures = 0;
    int propagations = 0;
    int disposals = 0;
    int advisor_disposals = 0;
    ModEvent event = ME_GEN_NONE;
    bool changed = false;
  };

  /// Keep observations local to each space, including its clones
  class ObserverSpace : public Space {
  public:
    StringVar x;
    IntVar i;
    Counts counts[3];

    ObserverSpace(int l=1, int u=3)
      : x(*this, Gecode::String::NSIntSet('a', 'b'), l, u), i(*this, 0, 1) {}

    ObserverSpace(ObserverSpace& s) : Space(s) {
      x.update(*this, s.x);
      i.update(*this, s.i);
      for (int j = 0; j < 3; ++j)
        counts[j] = s.counts[j];
    }

    virtual Space* copy(void) {
      return new ObserverSpace(*this);
    }
  };

  /// Observe String events and compare failure notification with Int
  template<class View>
  class Observer : public Propagator {
  private:
    View x;
    Council<Advisor> c;
    int slot;
    PropCond pc;
    bool failures;
    bool reject;
    bool stopped;

    Counts& counts(Space& home) const {
      return static_cast<ObserverSpace&>(home).counts[slot];
    }

    Observer(Space& home, Observer& p)
      : Propagator(home, p), slot(p.slot), pc(p.pc), failures(p.failures),
        reject(p.reject), stopped(p.stopped) {
      x.update(home, p.x);
      c.update(home, p.c);
    }

  public:
    Observer(Home home, View x0, int slot0, PropCond pc0,
             bool failures0=true, bool reject0=false)
      : Propagator(home), x(x0), c(home), slot(slot0), pc(pc0),
        failures(failures0), reject(reject0), stopped(false) {
      if (pc != PC_GEN_NONE)
        x.subscribe(home, *this, pc, false);
      x.subscribe(home, *new (home) Advisor(home, *this, c), failures);
    }

    virtual Actor* copy(Space& home) {
      return new (home) Observer(home, *this);
    }

    virtual PropCost cost(const Space&, const ModEventDelta&) const {
      return PropCost::unary(PropCost::LO);
    }

    virtual void reschedule(Space& home) {
      if (pc != PC_GEN_NONE)
        x.reschedule(home, *this, pc);
    }

    virtual ExecStatus advise(Space& home, Advisor& a, const Delta& d) {
      Counts& n = counts(home);
      ++n.changes;
      n.event = View::modevent(d);
      n.changed = x.any(d);
      if (reject)
        return ES_FAILED;
      if (x.assigned()) {
        ++n.advisor_disposals;
        return home.ES_FIX_DISPOSE(c, a);
      }
      return ES_FIX;
    }

    virtual void advise(Space& home, Advisor&) {
      ++counts(home).failures;
    }

    virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
      ++counts(home).propagations;
      return stopped ? home.ES_SUBSUMED(*this) : ES_FIX;
    }

    void stop(Space& home) {
      stopped = true;
      View::schedule(home, *this, ME_GEN_ASSIGNED);
    }

    virtual size_t dispose(Space& home) {
      if (pc != PC_GEN_NONE)
        x.cancel(home, *this, pc);
      for (Advisors<Advisor> a(c); a(); ++a) {
        x.cancel(home, a.advisor(), failures);
        ++counts(home).advisor_disposals;
      }
      c.dispose(home);
      ++counts(home).disposals;
      (void) Propagator::dispose(home);
      return sizeof(*this);
    }
  };

  // Keep each direct operation's forced-inline implementation in its own frame.
  ModEvent reject_scalar(ObserverSpace& home) {
    return home.x.varimp()->eq(home,
      Gecode::String::StringVal::from_symbols({'c'}));
  }

  ModEvent reject_bytes(ObserverSpace& home) {
    return home.x.varimp()->eq(home, std::string("c"));
  }

  ModEvent reject_domain(ObserverSpace& home) {
    return home.x.varimp()->dom(home, Gecode::String::NSBlocks("c"));
  }

  ModEvent raise_lower_bound(ObserverSpace& home, int l) {
    return home.x.varimp()->lb(home, l);
  }

  ModEvent reject_upper(ObserverSpace& home) {
    return home.x.varimp()->ub(home, 0);
  }

  ModEvent reject_equality(ObserverSpace& home) {
    StringVar y(home, "c");
    return home.x.varimp()->eq(home, y.varimp());
  }

  ModEvent reject_concat(ObserverSpace& home) {
    StringVar y(home, "c"), z(home, "d");
    return home.x.varimp()->concat(home, y.varimp(), z.varimp());
  }

  ModEvent commit_symbol(ObserverSpace& home, int symbol) {
    return home.x.varimp()->commit(home, Gecode::String::Branch::BASE,
      Gecode::String::Branch::MIN, Gecode::String::Branch::FIRST, symbol, 0);
  }

  ModEvent assign(ObserverSpace& home, const std::string& value) {
    return home.x.varimp()->eq(home, value);
  }

  ModEvent narrow_alphabet(ObserverSpace& home) {
    Gecode::String::NSBlocks blocks;
    blocks.emplace_back(Gecode::String::NSIntSet('a'), 2, 3);
    return home.x.varimp()->dom(home, blocks);
  }

  ModEvent reject_lower(ObserverSpace& home) {
    return raise_lower_bound(home, 4);
  }

  ModEvent reject_commit(ObserverSpace& home) {
    return commit_symbol(home, 'c');
  }

  /// Rejected tells notify only failure subscribers, exactly once
  class FailureAdvisors : public Base {
  public:
    FailureAdvisors(void) : Base("String::Var::FailureAdvisors") {}

    virtual bool run(void) {
      typedef ModEvent (*Tell)(ObserverSpace&);
      const Tell tells[] = {
        reject_scalar, reject_bytes, reject_domain, reject_lower, reject_upper,
        reject_equality, reject_concat, reject_commit
      };
      for (bool failures : {false, true}) {
        for (Tell tell : tells) {
          ObserverSpace home;
          (void) new (home) Observer<Gecode::String::StringView>(
            home, home.x, 0, Gecode::String::PC_STRING_NONE, failures);
          if (home.status() == SS_FAILED || !me_failed(tell(home)) ||
              home.counts[0].changes != 0 ||
              home.counts[0].failures != (failures ? 1 : 0))
            return false;
        }
      }
      ObserverSpace native;
      (void) new (native) Observer<Int::IntView>(
        native, native.i, 0, Int::PC_INT_NONE);
      Int::IntView i(native.i);
      if (!me_failed(i.eq(native, 2)) || native.counts[0].failures != 1 ||
          native.counts[0].changes != 0)
        return false;
      ObserverSpace rejected;
      (void) new (rejected) Observer<Gecode::String::StringView>(
        rejected, rejected.x, 0, Gecode::String::PC_STRING_NONE, true, true);
      return me_failed(raise_lower_bound(rejected, 2)) &&
        rejected.counts[0].changes == 1 && rejected.counts[0].failures == 0;
    }
  };

  /// Keep value, length and domain scheduling distinct and retain typed deltas
  class Events : public Base {
  private:
    static bool observed(const ObserverSpace& home, int changes,
                         int val, int len, int dom, ModEvent event) {
      const int propagations[] = {val, len, dom};
      for (int j = 0; j < 3; ++j) {
        const Counts& n = home.counts[j];
        if (n.changes != changes || n.failures != 0 ||
            n.propagations != propagations[j] || n.event != event ||
            n.changed != (changes != 0))
          return false;
      }
      return true;
    }

  public:
    Events(void) : Base("String::Var::Events") {}

    virtual bool run(void) {
      using namespace Gecode::String;
      ObserverSpace home;
      const PropCond conditions[] = {
        PC_STRING_VAL, PC_STRING_LEN, PC_STRING_DOM
      };
      for (int j = 0; j < 3; ++j)
        (void) new (home) Observer<StringView>(home, home.x, j, conditions[j]);
      if (home.status() == SS_FAILED ||
          raise_lower_bound(home, 1) != ME_STRING_NONE ||
          !observed(home, 0, 0, 0, 0, ME_STRING_NONE))
        return false;
      if (raise_lower_bound(home, 2) != ME_STRING_LEN ||
          home.status() == SS_FAILED ||
          !observed(home, 1, 0, 1, 1, ME_STRING_LEN))
        return false;
      if (narrow_alphabet(home) != ME_STRING_DOM ||
          home.status() == SS_FAILED ||
          !observed(home, 2, 0, 1, 2, ME_STRING_DOM))
        return false;
      if (assign(home, "aa") != ME_STRING_VAL || home.status() == SS_FAILED ||
          !observed(home, 3, 1, 2, 3, ME_STRING_VAL))
        return false;
      if (assign(home, "aa") != ME_STRING_NONE ||
          !observed(home, 3, 1, 2, 3, ME_STRING_VAL))
        return false;
      ObserverSpace branched(2, 2);
      (void) new (branched) Observer<StringView>(
        branched, branched.x, 0, PC_STRING_DOM);
      if (commit_symbol(branched, 'a') != ME_STRING_DOM ||
          branched.status() == SS_FAILED)
        return false;
      const Counts& n = branched.counts[0];
      const DashedString& d = branched.x.domain();
      return n.changes == 1 && n.failures == 0 && n.propagations == 1 &&
        n.changed && n.event == ME_STRING_DOM && !branched.x.assigned() &&
        d.min_length() == 2 && d.max_length() == 2 && d.length() == 2 &&
        d.at(0).l == 1 && d.at(0).u == 1 && d.at(0).S.size() == 1 &&
        d.at(0).S.in('a') && d.at(1).S.in('a') && d.at(1).S.in('b');
    }
  };

  /// Copy subscriptions normally, then cancel them on assignment or disposal
  class Lifecycle : public Base {
  public:
    Lifecycle(void) : Base("String::Var::Lifecycle") {}

    virtual bool run(void) {
      using namespace Gecode::String;
      ObserverSpace original;
      Observer<StringView>* assigned = new (original) Observer<StringView>(
        original, original.x, 0, PC_STRING_DOM);
      if (original.status() == SS_FAILED)
        return false;
      std::unique_ptr<ObserverSpace> cloned(
        static_cast<ObserverSpace*>(original.clone()));
      if (raise_lower_bound(*cloned, 2) != ME_STRING_LEN ||
          cloned->status() == SS_FAILED || cloned->counts[0].changes != 1 ||
          cloned->counts[0].propagations != 1 ||
          original.counts[0].changes != 0 || original.x.min_length() != 1 ||
          !me_failed(reject_scalar(*cloned)) ||
          cloned->counts[0].failures != 1 ||
          original.counts[0].failures != 0)
        return false;
      if (assign(original, "a") != ME_STRING_VAL ||
          original.status() == SS_FAILED || original.counts[0].changes != 1 ||
          original.counts[0].propagations != 1 ||
          original.counts[0].advisor_disposals != 1 ||
          !me_failed(reject_scalar(original)) ||
          original.counts[0].failures != 0)
        return false;
      assigned->stop(original);
      if (original.status() == SS_FAILED || original.counts[0].disposals != 1 ||
          original.counts[0].advisor_disposals != 1 ||
          original.counts[0].propagations != 2)
        return false;
      ObserverSpace disposed;
      Observer<StringView>* observer = new (disposed) Observer<StringView>(
        disposed, disposed.x, 0, PC_STRING_DOM);
      observer->stop(disposed);
      if (disposed.status() == SS_FAILED ||
          disposed.counts[0].disposals != 1 ||
          disposed.counts[0].advisor_disposals != 1 ||
          disposed.counts[0].propagations != 1 ||
          raise_lower_bound(disposed, 2) != ME_STRING_LEN ||
          disposed.status() == SS_FAILED)
        return false;
      return me_failed(reject_scalar(disposed)) &&
        disposed.counts[0].changes == 0 &&
        disposed.counts[0].failures == 0 &&
        disposed.counts[0].propagations == 1;
    }
  };

  FailureAdvisors failure_advisors;
  Events events;
  Lifecycle lifecycle;

}}}

// STATISTICS: test-string
