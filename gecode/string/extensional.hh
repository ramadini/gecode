#ifndef __GECODE_STRING_EXT_HH__
#define __GECODE_STRING_EXT_HH__

namespace Gecode { namespace String {

  // Abstract class for trimDFA and compDFA.  
  class stringDFA : public SharedHandle::Object {
  protected:
    int final_fst;
    int final_lst;
  public:
    int n_states;
    stringDFA(const DFA& d): final_fst(d.final_fst()), 
                             final_lst(d.final_lst()-1),
                             n_states(d.n_states()) {};
    stringDFA(const stringDFA& d)
      : final_fst(d.final_fst), final_lst(d.final_lst),
        n_states(d.n_states) {}
    bool accepting(int q) const {
      return final_fst <= q && q <= final_lst; 
    }
    NSIntSet accepting_states(void) const {
      return NSIntSet(final_fst, final_lst);
    }
    bool accepted(const string& s, string::size_type begin = 0) const {
      assert(begin <= s.size());
      int q = 0;
      for (string::size_type i = begin; i < s.size(); ++i) {
        q = search(q, s[i]);
        if (q == -1)
          return false;
      }
      return accepting(q);
    };
    virtual NSIntSet alphabet() const = 0;
    virtual int search(int, int) const = 0;
    virtual NSIntSet neighbours(int) const = 0;
    virtual NSIntSet neighbours(int, const DSIntSet&) const = 0;    
  };

  // trimmed-DFA data structure for non-reified regular.
  class trimDFA : public stringDFA {
  public:
    typedef std::vector<std::vector<std::pair<int, int>>> delta_t;
    delta_t delta;
    trimDFA(const DFA&);
    NSIntSet alphabet() const;
    int min_word_length() const;
    int search(int, int) const;
    NSIntSet neighbours(int) const;
    void include_neighbours(NSIntSet&, int, const DSIntSet&) const;
    NSIntSet neighbours(int, const DSIntSet&) const;
    bool include_all_neighbours(NSIntSet&, int, const DSIntSet&) const;
    NSIntSet neighbot(int, const DSIntSet&) const;
    /// Return reachable states, or no states if any represented word is rejected
    NSIntSet reach_all(const DSBlock&, const NSIntSet&) const;
  };

  // complete-DFA data structure for reified regular.
  class compDFA : public stringDFA {
    int nstate(int) const;
  public:
    int q_bot;
    typedef std::vector<std::vector<std::pair<NSIntSet, int>>> Delta_t;
    Delta_t delta;
    compDFA(const DFA&, const NSIntSet&);
    compDFA(const trimDFA&, const NSIntSet&);
    void negate();
    NSIntSet alphabet() const;
    int search(int, int) const;
    NSIntSet neighbours(int) const;
    void include_neighbours(NSIntSet&, int, const DSIntSet&) const;
    NSIntSet neighbours(int, const DSIntSet&) const;
  };
  
  struct matchNFA : public compDFA {
    typedef std::pair<int, int> ReverseTransition;
    typedef std::vector<ReverseTransition> ReverseTransitions;
    typedef std::vector<ReverseTransitions> ReverseDelta;

    class Predecessors {
    private:
      const matchNFA& nfa;
      const DSIntSet& characters;
      ReverseTransitions::const_iterator current;
      ReverseTransitions::const_iterator finish;
      void skip_disjoint(void);
    public:
      Predecessors(const matchNFA&, int, const DSIntSet&);
      bool operator ()(void) const;
      void operator ++(void);
      int operator *(void) const;
    };

    // 0 = initial state q_0, 1 = accepting state q_F, bot = q_⊥ state.
    // delta[i][j] =  k means δ(i,j) = {k}
    // delta[i][j] = -k means δ(i,j) = {k,q_bot}
    // Reverse entries store (source state, edge index in delta[source]).
    ReverseDelta reverse;
    matchNFA(const trimDFA&, const NSIntSet&);
    void include_neighbours(NSIntSet&, int, const DSIntSet&) const;
    NSIntSet neighbours(int, const DSIntSet&) const;
  };

  template<class Automaton>
  class AutomatonHandle : public SharedHandle {
  public:
    AutomatonHandle(void) {}
    AutomatonHandle(Automaton* automaton) : SharedHandle(automaton) {}
    AutomatonHandle(const AutomatonHandle& handle) : SharedHandle(handle) {}
    AutomatonHandle& operator =(const AutomatonHandle& handle) {
      SharedHandle::operator =(handle);
      return *this;
    }
    AutomatonHandle& operator =(Automaton* automaton) {
      object(automaton);
      return *this;
    }
    Automaton* operator ->(void) const {
      return static_cast<Automaton*>(object());
    }
    Automaton* get(void) const {
      return static_cast<Automaton*>(object());
    }
    Automaton& operator *(void) const {
      return *static_cast<Automaton*>(object());
    }
  };

  typedef AutomatonHandle<trimDFA> TrimDFAHandle;
  typedef AutomatonHandle<compDFA> CompDFAHandle;
  typedef AutomatonHandle<matchNFA> MatchNFAHandle;


  /**
   * \brief %Propagator for DFA.
   *
   */
  class Reg : public UnaryPropagator<StringView, PC_STRING_DOM> {
  private:
    TrimDFAHandle dfa;
  protected:
    using UnaryPropagator<StringView, PC_STRING_DOM>::x0;
    /// Constructor for cloning \a p
    Reg(Space& home, Reg& p);
    /// Constructor for posting
    Reg(Home home, StringView, trimDFA* p);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static ExecStatus post(Home home, StringView x, const DFA& dfa);
    static ExecStatus post(Home home, StringView x, trimDFA* pdfa);
    static NSBlocks dom(trimDFA*);
    static std::vector<NSIntSet> reach_fwd(trimDFA*, const NSIntSet&,
      const DSBlock&, bool rev = false
    );
    static NSBlocks reach_bwd(trimDFA*, const std::vector<NSIntSet>&,
      NSIntSet&, const DSBlock&, bool&, bool rev = false
    );
    static std::vector<NSIntSet> reach_fwd(compDFA*, const NSIntSet&,
                                                     const DSBlock&);
    static NSBlocks reach_bwd(compDFA*, const std::vector<NSIntSet>&, NSIntSet&,
                                                      const DSBlock&, bool&);
    template <typename DFA_t>
    static ExecStatus propagate_blocks(Space&, NSBlocks&, DFA_t*);
  };

  /**
   * \brief %Propagator for reified DFA.
   *
   */
  template<class CtrlView, ReifyMode rm>
  class ReReg : public Propagator {
  protected:
    StringView x0;
    CtrlView b;
    /// Constructor for cloning \a p
    ReReg(Space& home, ReReg&);
    /// Constructor for posting
    ReReg(Home home, StringView x, compDFA* d, CtrlView b);
  private:
    CompDFAHandle dfa;
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Cost function (defined as PC_TERNARY_LO)
    virtual PropCost cost(const Space& home, const ModEventDelta& med) const;
    /// Schedule function
    virtual void reschedule(Space& home);
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator for \f$ (x=y) \Leftrightarrow b\f$
    static ExecStatus post(Home home, StringView x, const DFA& d, CtrlView b);
    static ExecStatus post(Home home, StringView x, compDFA* d, CtrlView b);
  };

}}

#include <gecode/string/extensional/reg.hpp>
#include <gecode/string/extensional/re-reg.hpp>

#endif
