#ifndef __GECODE_STRING_FIND_HH__
#define __GECODE_STRING_FIND_HH__

namespace Gecode { namespace String {

  /**
   * \brief %Propagator for string find.
   *
   */
  class Find :
    public MixTernaryPropagator<StringView, PC_STRING_DOM, 
    StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND> {
  protected:
    using MixTernaryPropagator<StringView, PC_STRING_DOM, 
      StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>
        ::x0;
    using MixTernaryPropagator<StringView, PC_STRING_DOM, 
      StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>
        ::x1;
    using MixTernaryPropagator<StringView, PC_STRING_DOM, 
      StringView, PC_STRING_DOM, Gecode::Int::IntView, Gecode::Int::PC_INT_BND>
        ::x2;
    /// Constructor for cloning \a p
    Find(Space& home, Find& p);
    /// Constructor for posting
    Find(Home home, StringView, StringView, Gecode::Int::IntView);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator for \f n = find(x, y) \f$
    static ExecStatus post(Home, StringView, StringView, Gecode::Int::IntView);
  };

}}

#include <gecode/string/int/find.hpp>

#endif
