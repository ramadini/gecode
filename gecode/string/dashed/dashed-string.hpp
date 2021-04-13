#include <gecode/set.hh>

/*** Declarations ***/

namespace Gecode { namespace String {

  /**
   * \brief Set of integers contained in [0, String::Limits::MAX_ALPHABET_SIZE) 
   * abstracting a set of characters, used as base of dashed strings' blocks.
   *
   * \ingroup TaskModelStringBranch
   */
  class GECODE_STRING_EXPORT CharSet : public Gecode::Set::LUBndSet {
    
  
  public:
    
    /// Prints set according to Limits::CHAR_ENCODING
    friend std::ostream& operator<<(std::ostream& os, const CharSet& set);
    
  };

}}

namespace Gecode { namespace String {
  
  class GECODE_STRING_EXPORT Block {
  
  };

}}

namespace Gecode { namespace String {
  
  class GECODE_STRING_EXPORT DashedString : 
  public Gecode::Support::DynamicArray<Block, Space> {
  
  };

}}

/*** Definitions ***/

namespace Gecode { namespace String {

  /// Char encoding. FIXME: This better be an option. 
  const CharEncoding CHAR_ENCODING = UNSPEC;

  /** 
   * Convert \a x to its string representation according to the character
   * encoding. By default, CHAR_ENCODING=UNSPEC, i.e.:  //FIXME: Change this?
   * int2str(x) = char(x), if \f$32 \leq x \leq 126\f$ (printable character);
   * otherwise, int2str(x) = "<" + std::to_string(x) + ">" 
   */
  std::string int2str(int x);

  forceinline std::string 
  int2str_unspec(int x) {
    return x > 31 && x < 127 ? std::string(1, char(x)) 
                             : "<" + std::to_string(x) + ">";
  }

  forceinline std::string 
  int2str(int x) {
    switch (CHAR_ENCODING) {
      case UNSPEC:
      // TODO:
      case ASCII:
      case EASCII:
      case UTF_8:
      case UTF_16:
      case UTF_32:
      default:
        return int2str_unspec(x);
    }
    GECODE_NEVER;
  }

  forceinline std::ostream&
  operator<<(std::ostream& os, const CharSet& set) {
    Gecode::Set::BndSetRanges i(set);
    os << "{";
    while (i()) {
      std::string lb = int2str(i.min()), ub = int2str(i.max());
      os << lb;
      if (lb < ub)
        os << ".." << ub;
      ++i;
      if (i())
        os << ",";
    }
    return os << "}";
  }


}}
