namespace Gecode {

  /*
   * Implementation
   *
   */

  forceinline
  StringVarArgs::StringVarArgs(void) {}

  forceinline
  StringVarArgs::StringVarArgs(int n)
    : VarArgArray<StringVar>(n) {}

  forceinline
  StringVarArgs::StringVarArgs(const StringVarArgs& a)
    : VarArgArray<StringVar>(a) {}

  forceinline
  StringVarArgs::StringVarArgs(const VarArray<StringVar>& a)
    : VarArgArray<StringVar>(a) {}

  forceinline
  StringVarArgs::StringVarArgs(const std::vector<StringVar>& a)
    : VarArgArray<StringVar>(a) {}

  template<class InputIterator>
  forceinline
  StringVarArgs::StringVarArgs(InputIterator first, InputIterator last)
    : VarArgArray<StringVar>(first, last) {}


  forceinline
  StringVarArray::StringVarArray(void) {}

  forceinline
  StringVarArray::StringVarArray(const StringVarArray& a)
    : VarArray<StringVar>(a) {}

  forceinline
  StringVarArray::StringVarArray(Space& home, const StringVarArgs& a)
    : VarArray<StringVar>(home, a) {}

}
