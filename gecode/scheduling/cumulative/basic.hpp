/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2010
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
 *
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
 *
 */

namespace Gecode { namespace Scheduling { namespace Cumulative {

  /// Event for task
  class Event {
  public:
    /// Event type for task with order in which they are processed
    enum Type {
      LRT = 0, ///< Latest required time of task
      LCT = 1, ///< Latest completion time of task
      EST = 2, ///< Earliest start time of task
      ERT = 3, ///< Earliest required time of task
      END = 4  ///< End marker
    };
    Type e; ///< Type of event
    int t;  ///< Time of event
    int i;  ///< Number of task
    /// Initialize event
    void init(Type e, int t, int i);
    /// Order among events
    bool operator <(const Event& e) const;
  };

  forceinline void
  Event::init(Event::Type e0, int t0, int i0) {
    e=e0; t=t0; i=i0;
  }

  forceinline bool
  Event::operator <(const Event& e) const {
    if (this->t == e.t)
      return this->e < e.e;
    return this->t < e.t;
  }

  // Basic propagation
  template<class Task>
  ExecStatus
  basic(Space& home, int c, TaskArray<Task>& t) {
    Region r(home);

    Event* e = r.alloc<Event>(4*t.size()+1);

    // Initialize events
    {
      int n=0;
      for (int i=t.size(); i--; ) {
        e[n++].init(Event::EST,t[i].est(),i);
        e[n++].init(Event::LCT,t[i].lct(),i);
        // Check whether task has required part
        if (t[i].lst() < t[i].ect()) {
          e[n++].init(Event::ERT,t[i].lst(),i);
          e[n++].init(Event::LRT,t[i].ect(),i);
        }
      }
      
      // Check whether no task has a required part
      if (n == 2*t.size())
        return ES_OK;
      
      // Write end marker
      e[n++].init(Event::END,Int::Limits::infinity,-1);
      
      // Sort events
      Support::quicksort(e, n);
    }

    // Set of current but not required tasks
    Support::BitSet<Region> tasks(r,t.size());

    // Process events, use c as the capacity that is still free
    while (true) {
      // Current time
      int time = e->t;
      // Process sorted events with same timestamp
      for ( ; e->t == time; e++)
        switch (e->e) {
        case Event::LRT:
          // Process events for completion of required part
          tasks.set(e->i); c += t[e->i].c();
          break;
        case Event::LCT:
          // Process events for completion of task
          tasks.clear(e->i);
          break;
        case Event::EST:
          // Process events for start of task
          tasks.set(e->i);
          break;
        case Event::ERT:
          // Process events for start of required part
          tasks.clear(e->i); c -= t[e->i].c();        
          if (c < 0)
            return ES_FAILED;
          break;
        case Event::END: default:
          GECODE_NEVER;
        }
      
      if (e->e == Event::END)
        break;
      
      for (Iter::Values::BitSet<Support::BitSet<Region> > j(tasks); j(); ++j) 
        // Task j cannot run from time to next time - 1
        if (t[j.val()].c() > c)
          GECODE_ME_CHECK(t[j.val()].norun(home, time, e->t - 1));

    }

    return ES_OK;
  }

}}}

// STATISTICS: scheduling-prop
