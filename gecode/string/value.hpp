#ifndef __GECODE_STRING_VALUE_HPP__
#define __GECODE_STRING_VALUE_HPP__

#include <initializer_list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace Gecode { namespace String {

  /// Symbol stored by the string solver.
  typedef int StringSymbol;

  /// Largest Unicode code point.
  const StringSymbol UNICODE_MAX = 0x10FFFF;
  /// First UTF-16 surrogate code point (not a Unicode scalar value).
  const StringSymbol UNICODE_SURROGATE_MIN = 0xD800;
  /// Last UTF-16 surrogate code point (not a Unicode scalar value).
  const StringSymbol UNICODE_SURROGATE_MAX = 0xDFFF;

  /// Test whether \a value is a Unicode scalar value.
  inline bool
  is_unicode_scalar_value(StringSymbol value) {
    return value >= 0 && value <= UNICODE_MAX &&
      (value < UNICODE_SURROGATE_MIN || value > UNICODE_SURROGATE_MAX);
  }

  /**
   * \brief Encoding-neutral concrete string value
   *
   * A StringVal stores Unicode scalar values, not encoded bytes.  Encoding and
   * decoding are explicit boundary operations.  from_bytes() is the explicit
   * compatibility path for the legacy one-byte/one-symbol representation.
   */
  class StringVal {
  public:
    typedef std::vector<StringSymbol>::size_type size_type;
    typedef std::vector<StringSymbol>::const_iterator const_iterator;

  private:
    std::vector<StringSymbol> _symbols;

    explicit StringVal(const std::vector<StringSymbol>& symbols)
      : _symbols(symbols) {
      validate_symbols();
    }

    explicit StringVal(std::vector<StringSymbol>&& symbols)
      : _symbols(std::move(symbols)) {
      validate_symbols();
    }

    void validate_symbols(void) const {
      for (size_type i = 0; i < _symbols.size(); ++i) {
        if (!is_unicode_scalar_value(_symbols[i])) {
          std::ostringstream os;
          os << "Invalid Unicode scalar value at symbol offset " << i;
          throw std::out_of_range(os.str());
        }
      }
    }

    static std::invalid_argument utf8_error(size_type offset) {
      std::ostringstream os;
      os << "Invalid UTF-8 sequence at byte offset " << offset;
      return std::invalid_argument(os.str());
    }

    static bool continuation(unsigned char c) {
      return (c & 0xC0U) == 0x80U;
    }

  public:
    StringVal(void) : _symbols() {}

    static StringVal from_symbols(const std::vector<StringSymbol>& symbols) {
      return StringVal(symbols);
    }

    static StringVal from_symbols(std::vector<StringSymbol>&& symbols) {
      return StringVal(std::move(symbols));
    }

    static StringVal from_symbols(
      std::initializer_list<StringSymbol> symbols
    ) {
      return StringVal(std::vector<StringSymbol>(symbols));
    }

    /**
     * \brief Construct using the legacy byte-string interpretation
     *
     * Each byte becomes one solver symbol in 0..255.  No text decoding is
     * performed.
     */
    static StringVal from_bytes(const std::string& bytes) {
      std::vector<StringSymbol> symbols;
      symbols.reserve(bytes.size());
      for (std::string::const_iterator i = bytes.begin(); i != bytes.end(); ++i)
        symbols.push_back(static_cast<unsigned char>(*i));
      return StringVal(std::move(symbols));
    }

    /// Decode a strictly valid UTF-8 byte string into Unicode scalar values.
    static StringVal decode_utf8(const std::string& input) {
      std::vector<StringSymbol> symbols;
      symbols.reserve(input.size());

      size_type i = 0;
      while (i < input.size()) {
        const unsigned char b0 = static_cast<unsigned char>(input[i]);

        if (b0 <= 0x7FU) {
          symbols.push_back(static_cast<StringSymbol>(b0));
          ++i;
          continue;
        }

        if (b0 >= 0xC2U && b0 <= 0xDFU) {
          if (i + 1 >= input.size())
            throw utf8_error(i);
          const unsigned char b1 = static_cast<unsigned char>(input[i + 1]);
          if (!continuation(b1))
            throw utf8_error(i);
          symbols.push_back(static_cast<StringSymbol>(
            ((b0 & 0x1FU) << 6) | (b1 & 0x3FU)));
          i += 2;
          continue;
        }

        if (b0 >= 0xE0U && b0 <= 0xEFU) {
          if (i + 2 >= input.size())
            throw utf8_error(i);
          const unsigned char b1 = static_cast<unsigned char>(input[i + 1]);
          const unsigned char b2 = static_cast<unsigned char>(input[i + 2]);
          if (!continuation(b1) || !continuation(b2))
            throw utf8_error(i);
          if ((b0 == 0xE0U && b1 < 0xA0U) ||
              (b0 == 0xEDU && b1 > 0x9FU))
            throw utf8_error(i);
          symbols.push_back(static_cast<StringSymbol>(
            ((b0 & 0x0FU) << 12) |
            ((b1 & 0x3FU) << 6) |
            (b2 & 0x3FU)));
          i += 3;
          continue;
        }

        if (b0 >= 0xF0U && b0 <= 0xF4U) {
          if (i + 3 >= input.size())
            throw utf8_error(i);
          const unsigned char b1 = static_cast<unsigned char>(input[i + 1]);
          const unsigned char b2 = static_cast<unsigned char>(input[i + 2]);
          const unsigned char b3 = static_cast<unsigned char>(input[i + 3]);
          if (!continuation(b1) || !continuation(b2) || !continuation(b3))
            throw utf8_error(i);
          if ((b0 == 0xF0U && b1 < 0x90U) ||
              (b0 == 0xF4U && b1 > 0x8FU))
            throw utf8_error(i);
          symbols.push_back(static_cast<StringSymbol>(
            ((b0 & 0x07U) << 18) |
            ((b1 & 0x3FU) << 12) |
            ((b2 & 0x3FU) << 6) |
            (b3 & 0x3FU)));
          i += 4;
          continue;
        }

        throw utf8_error(i);
      }

      return StringVal(std::move(symbols));
    }

    /// Encode the stored Unicode scalar values as UTF-8.
    std::string to_utf8(void) const {
      std::string output;
      output.reserve(_symbols.size());

      for (size_type i = 0; i < _symbols.size(); ++i) {
        const StringSymbol c = _symbols[i];
        if (!is_unicode_scalar_value(c))
          throw std::out_of_range("StringVal contains an invalid Unicode scalar value");

        if (c <= 0x7F) {
          output.push_back(static_cast<char>(c));
        } else if (c <= 0x7FF) {
          output.push_back(static_cast<char>(0xC0 | (c >> 6)));
          output.push_back(static_cast<char>(0x80 | (c & 0x3F)));
        } else if (c <= 0xFFFF) {
          output.push_back(static_cast<char>(0xE0 | (c >> 12)));
          output.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
          output.push_back(static_cast<char>(0x80 | (c & 0x3F)));
        } else {
          output.push_back(static_cast<char>(0xF0 | (c >> 18)));
          output.push_back(static_cast<char>(0x80 | ((c >> 12) & 0x3F)));
          output.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
          output.push_back(static_cast<char>(0x80 | (c & 0x3F)));
        }
      }

      return output;
    }

    /**
     * \brief Serialize as one byte per symbol
     *
     * This is the inverse of from_bytes().  It rejects values outside 0..255
     * rather than truncating them.
     */
    std::string to_bytes(void) const {
      std::string output;
      output.reserve(_symbols.size());
      for (size_type i = 0; i < _symbols.size(); ++i) {
        const StringSymbol c = _symbols[i];
        if (c < 0 || c > 255)
          throw std::out_of_range("StringVal symbol cannot be represented as one byte");
        output.push_back(static_cast<char>(static_cast<unsigned char>(c)));
      }
      return output;
    }

    size_type size(void) const { return _symbols.size(); }
    bool empty(void) const { return _symbols.empty(); }
    StringSymbol operator[](size_type i) const { return _symbols[i]; }
    const_iterator begin(void) const { return _symbols.begin(); }
    const_iterator end(void) const { return _symbols.end(); }
    const std::vector<StringSymbol>& symbols(void) const { return _symbols; }

    bool operator==(const StringVal& that) const {
      return _symbols == that._symbols;
    }

    bool operator!=(const StringVal& that) const {
      return !(*this == that);
    }
  };

}}

#endif
