namespace Gecode {

  forceinline
  StringVarBranch::StringVarBranch(void)
  : VarBranch<StringVar>(), s(STRING_VAR_NONE) {}

  forceinline
  StringVarBranch::StringVarBranch(Select s0)
  : VarBranch<StringVar>(), s(s0) {}

  forceinline StringVarBranch::Select
  StringVarBranch::select(void) const {
    return s;
  }

  forceinline StringVarBranch
  STRING_VAR_NONE(void) {
    return StringVarBranch(StringVarBranch::STRING_VAR_NONE);
  }

  forceinline StringVarBranch
  STRING_VAR_SIZEMIN(void) {
    return StringVarBranch(StringVarBranch::STRING_VAR_SIZEMIN);
  }

  forceinline StringVarBranch
  STRING_VAR_BLOCKMIN(void) {
    return StringVarBranch(StringVarBranch::STRING_VAR_BLOCKMIN);
  }

  forceinline StringVarBranch
  STRING_VAR_LENMIN(void) {
    return StringVarBranch(StringVarBranch::STRING_VAR_LENMIN);
  }

  forceinline StringVarBranch
  STRING_VAR_LENMAX(void) {
    return StringVarBranch(StringVarBranch::STRING_VAR_LENMAX);
  }

}
