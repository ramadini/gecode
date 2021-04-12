namespace Gecode { namespace String {

    forceinline void
    Limits::check_length(int n, const char* l) {
      if (n < 0 || n > MAX_STRING_LENGTH)
        throw OutOfLimits(l);
    }

    forceinline void
    Limits::check_alphabet(int a, const char* l) {
      if (a < 0 || a >= MAX_ALPHABET_SIZE)
        throw OutOfLimits(l);
    }
    
    forceinline void
    Limits::check_alphabet(const CharSet& S, const char* l) {
      if (S.min() < 0 || S.max() >= MAX_ALPHABET_SIZE)
        throw OutOfLimits(l);
    }

}}

// STATISTICS: string-var
