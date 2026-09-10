#include <gecode/string.hh>

#include <algorithm>
#include <stdexcept>


namespace Gecode { namespace String {

  StringAlphabet::StringAlphabet(const NSIntSet& symbols)
    : _symbols(symbols) {}

  StringAlphabet
  StringAlphabet::bytes(void) {
    return StringAlphabet(NSIntSet(0, 0xFF));
  }

  StringAlphabet
  StringAlphabet::ascii(void) {
    return StringAlphabet(NSIntSet(0, 0x7F));
  }

  StringAlphabet
  StringAlphabet::latin1(void) {
    return StringAlphabet(NSIntSet(0, 0xFF));
  }

  StringAlphabet
  StringAlphabet::unicode_scalars(void) {
    NSIntSet symbols(0, UNICODE_SURROGATE_MIN - 1);
    symbols.include(NSIntSet(UNICODE_SURROGATE_MAX + 1, UNICODE_MAX));
    return StringAlphabet(symbols);
  }

  StringAlphabet
  StringAlphabet::from_ranges(const std::vector<Range>& ranges) {
    if (ranges.empty())
      throw std::invalid_argument("StringAlphabet must not be empty");
    NSIntSet symbols;
    for (std::vector<Range>::size_type i = 0; i < ranges.size(); ++i) {
      const Range& range = ranges[i];
      if (range.lower > range.upper)
        throw std::invalid_argument("StringAlphabet range is reversed");
      if (!is_unicode_scalar_value(range.lower) ||
          !is_unicode_scalar_value(range.upper) ||
          (range.lower <= UNICODE_SURROGATE_MAX &&
           range.upper >= UNICODE_SURROGATE_MIN))
        throw std::out_of_range(
          "StringAlphabet range contains non-scalar code points");
      symbols.include(NSIntSet(range.lower, range.upper));
    }
    return StringAlphabet(symbols);
  }

  StringAlphabet
  StringAlphabet::from_ranges(std::initializer_list<Range> ranges) {
    return from_ranges(std::vector<Range>(ranges));
  }

  StringAlphabet
  StringAlphabet::from_codepoints(
    const std::vector<StringSymbol>& codepoints
  ) {
    if (codepoints.empty())
      throw std::invalid_argument("StringAlphabet must not be empty");
    std::vector<StringSymbol> sorted(codepoints);
    for (std::vector<StringSymbol>::const_iterator i = sorted.begin();
         i != sorted.end(); ++i)
      if (!is_unicode_scalar_value(*i))
        throw std::out_of_range(
          "StringAlphabet contains a non-scalar code point");
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
    NSIntSet symbols;
    StringSymbol lower = sorted.front();
    StringSymbol upper = lower;
    for (std::vector<StringSymbol>::size_type i = 1;
         i < sorted.size(); ++i) {
      if (sorted[i] == upper + 1) {
        upper = sorted[i];
      }
      else {
        symbols.include(NSIntSet(lower, upper));
        lower = upper = sorted[i];
      }
    }
    symbols.include(NSIntSet(lower, upper));
    return StringAlphabet(symbols);
  }

  StringAlphabet
  StringAlphabet::from_codepoints(
    std::initializer_list<StringSymbol> codepoints
  ) {
    return from_codepoints(std::vector<StringSymbol>(codepoints));
  }

  bool
  StringAlphabet::contains(StringSymbol symbol) const {
    return _symbols.contains(symbol);
  }

  int
  StringAlphabet::size(void) const {
    return _symbols.size();
  }

  int
  StringAlphabet::ranges(void) const {
    return _symbols.length();
  }

  const NSIntSet&
  StringAlphabet::symbols(void) const {
    return _symbols;
  }

}}


namespace Gecode {

  StringVar::StringVar(Space& home)
    : VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home)) {}

  StringVar::StringVar(Space& home, const string& s)
    : VarImpVar<String::StringVarImp>(new (home) String::StringVarImp(home,s)) {}

  StringVar::StringVar(Space& home, const String::StringVal& value)
    : VarImpVar<String::StringVarImp>(
        new (home) String::StringVarImp(home,value)) {}

  StringVar::StringVar(Space& home, int a, int b)
    : VarImpVar<String::StringVarImp>(
        new (home) String::StringVarImp(home,a,b)) {}

  StringVar::StringVar(Space& home, int a, int b,
                       const String::StringAlphabet& alphabet)
    : VarImpVar<String::StringVarImp>(
        new (home) String::StringVarImp(home,alphabet.symbols(),a,b)) {}

  StringVar::StringVar(Space& home, String::NSBlocks& v, int mil, int mal)
    : VarImpVar<String::StringVarImp>(
        new (home) String::StringVarImp(home,v,mil,mal)) {}

  StringVar::StringVar(Space& home, const String::NSIntSet& s, int a, int b)
    : VarImpVar<String::StringVarImp>(
        new (home) String::StringVarImp(home,s,a,b)) {}

}
