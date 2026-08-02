#ifndef __GECODE_STRING_GCC_HH__
#define __GECODE_STRING_GCC_HH__

namespace Gecode { namespace String {

  /// Shared immutable character and index data for GCC
  class GCCData : public SharedHandle::Object {
  public:
    const vec2 characters;
    NSIntSet cover;
    GCCData(const vec2& characters0);
  };

  /// Handle for shared GCC character and index data
  class GCCDataHandle : public SharedHandle {
  public:
    GCCDataHandle(GCCData* data) : SharedHandle(data) {}
    GCCDataHandle(const GCCDataHandle& data) : SharedHandle(data) {}
    const GCCData* operator ->(void) const {
      return static_cast<GCCData*>(object());
    }
  };

  class GCC :
  public MixNaryOnePropagator
  <Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> {
  private:
    GCCDataHandle data;
    ExecStatus assigned(Space&);
  protected:
    using MixNaryOnePropagator
      <Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> 
        ::x;
    using MixNaryOnePropagator
      <Gecode::Int::IntView, Gecode::Int::PC_INT_BND, StringView, PC_STRING_DOM> 
        ::y;
    /// Constructor for cloning \a p
    GCC(Space& home, GCC& p);
    /// Constructor for posting
    GCC(Home home, StringView, const vec2&, ViewArray<Gecode::Int::IntView>&);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home);
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /// Post propagator \f$ gcc(x, a, n) \f$
    static  ExecStatus post(
      Home, StringView, const vec2&, ViewArray<Gecode::Int::IntView>
    );
  };

}}

#include <gecode/string/gcc/gcc.hpp>

#endif
