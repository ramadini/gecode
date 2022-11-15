#ifndef __GECODE_STRING_EXT_HH__
#define __GECODE_STRING_EXT_HH__

namespace Gecode { namespace String {

  // DFA data structure for non-reified regular.
  struct stringDFA {
    typedef std::vector<std::vector<std::pair<int, int>>> delta_t;
    int ua;
    int ur;
    int n_states;
    int final_fst;
    int final_lst;
    delta_t delta;
    stringDFA(const DFA&);
    void negate(const NSIntSet&);
    bool final(int) const;
    int search(int, int) const;
    bool accepted(string) const;
    bool univ_accepted(const NSIntSet& Q) const;
    bool univ_rejected(const NSIntSet& Q) const;
    NSIntSet alphabet() const;
    NSIntSet neighbours(int) const;
    NSIntSet neighbours(int, const DSIntSet&) const;
    void compute_univ(const NSIntSet& alphabet);
    protected:
      int nstate(int) const;
  };

  // Complete DFA for reified regular.
  struct stringCDFA : public stringDFA {
    stringCDFA(const DFA&, const NSIntSet&);
    void negate();
  };


  /**
   * \brief %Propagator for DFA.
   *
   */
  class Reg : public UnaryPropagator<StringView, PC_STRING_DOM> {
  private:
    stringDFA* dfa;
  protected:
    using UnaryPropagator<StringView, PC_STRING_DOM>::x0;
    /// Constructor for cloning \a p
    Reg(Space& home, Reg& p);
    /// Constructor for posting
    Reg(Home home, StringView, stringDFA* p);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator
    static ExecStatus post(Home home, StringView x, const DFA& dfa);
    static ExecStatus post(Home home, StringView x, stringDFA* pdfa);
    static NSBlocks dom(stringDFA*);
    static std::vector<NSIntSet> reach_fwd(
      stringDFA*, const NSIntSet&, const DSBlock&,
      bool reif = false, bool bwd = false
    );
    static NSBlocks reach_bwd(stringDFA*, 
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
    ReReg(Home home, StringView x, stringCDFA* d, CtrlView b);
  private:
    stringCDFA* dfa;
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
