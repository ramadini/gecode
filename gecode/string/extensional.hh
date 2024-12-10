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
  };

  // trimmed-DFA data structure for non-reified regular.
  class trimDFA {
  protected:
    int final_fst;
    int final_lst;
  public:
    typedef std::vector<std::vector<std::pair<int, int>>> delta_t;
    int n_states;    
    delta_t delta;
    trimDFA(const DFA&);
    void negate(const NSIntSet&);
    bool accepting(int) const;
    NSIntSet accepting_states(void) const;
    int search(int, int) const;
    bool accepted(const string& s) const;
    NSIntSet alphabet() const;
    NSIntSet neighbours(int) const;
    NSIntSet neighbours(int, const DSIntSet&) const;
    matchNFA toMatchNFA(const NSIntSet&);
  protected:
    int nstate(int) const;
  };

  // complete-DFA data structure for reified regular.
  struct compDFA : public trimDFA {
    compDFA(const DFA&, const NSIntSet&);
    void negate(); //FIXME: Specialize.
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
    static std::vector<NSIntSet> reach_fwd(
      trimDFA*, const NSIntSet&, const DSBlock&,
      bool reif = false, bool bwd = false
    );
    static NSBlocks reach_bwd(trimDFA*, 
      const std::vector<NSIntSet>&, NSIntSet&, const DSBlock&, bool&,
      bool rev = false
    );
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
    ~ReReg();
  };

}}

#include <gecode/string/ext/reg.hpp>
#include <gecode/string/ext/re-reg.hpp>

#endif
