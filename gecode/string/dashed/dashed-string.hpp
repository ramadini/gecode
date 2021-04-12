#include <gecode/set.hh>

namespace Gecode { namespace String {

  class GECODE_STRING_EXPORT CharSet : public Gecode::Set::LUBndSet {
  
  };
  
  class GECODE_STRING_EXPORT Block {
  
  };
  
  class GECODE_STRING_EXPORT DashedString : 
  public Gecode::Support::DynamicArray<Block, Space> {
  
  };

}}
