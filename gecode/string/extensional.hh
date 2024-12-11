#ifndef __GECODE_STRING_EXT_HH__
#define __GECODE_STRING_EXT_HH__

namespace Gecode { namespace String {

  struct matchNFA {
    // 0 = initial state q_0, 1 = accepting state q_F, bot = q_⊥ state.    
    typedef std::vector<std::vector<std::pair<int, int>>> delta_t;
    int bot;    
    int n_states;
    // delta_t[i][j] = k >= 0 means δ(i,j) = {k}.
    // delta_t[i][j] = k <  0 means δ(i,j) = {-k,bot}.
    delta_t delta;
    NSIntSet neighbours(int, const DSIntSet&) const;
    bool accepting(int) const;
    NSIntSet accepting_states(void) const;
  };

  // Abstract class for trimDFA and compDFA.  
  class stringDFA {
  protected:
    int final_fst;
    int final_lst;
  public:
    int n_states;
    stringDFA(const DFA& d): final_fst(d.final_fst()), 
                             final_lst(d.final_lst()-1),
                             n_states(d.n_states()) {};
    bool accepting(int q) const {
      return final_fst <= q && q <= final_lst; 
    }
    NSIntSet accepting_states(void) const {
      return NSIntSet(final_fst, final_lst);
    }
    bool accepted(const string& s) const {
      int q = 0;
      for (auto& c : s) {
        q = search(q, c);
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
    int search(int, int) const;
    NSIntSet neighbours(int) const;
    NSIntSet neighbours(int, const DSIntSet&) const;
    //TODO: Better a matchNFA constructor.
    matchNFA toMatchNFA(const NSIntSet&); 
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
    NSIntSet neighbours(int, const DSIntSet&) const;
  };


  /**
   * \brief %Propagator for DFA.
   *
   */
  class Reg : public UnaryPropagator<StringView, PC_STRING_DOM> {
  private:
    trimDFA* dfa;
  protected:
    using UnaryPropagator<StringView, PC_STRING_DOM>::x0;
    /// Constructor for cloning \a p
    Reg(Space& home, Reg& p);
    /// Constructor for posting
    Reg(Home home, StringView, trimDFA* p);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static ExecStatus post(Home home, StringView x, const DFA& dfa);
    static ExecStatus post(Home home, StringView x, trimDFA* pdfa);
    static NSBlocks dom(trimDFA*);
    static std::vector<NSIntSet> reach_fwd(trimDFA*, const NSIntSet&,
      const DSBlock&, bool bwd = false
    );
    static NSBlocks reach_bwd(trimDFA*, const std::vector<NSIntSet>&,
      NSIntSet&, const DSBlock&, bool&, bool rev = false
    );
    static std::vector<NSIntSet> reach_fwd(compDFA*, const NSIntSet&,
                                                     const DSBlock&);
    static NSBlocks reach_bwd(compDFA*, const std::vector<NSIntSet>&, NSIntSet&,
                                                      const DSBlock&, bool&);
    ~Reg();
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
    compDFA* dfa;
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
    ~ReReg();
  };

}}

#include <gecode/string/ext/reg.hpp>
#include <gecode/string/ext/re-reg.hpp>

#endif
