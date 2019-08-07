namespace Gecode {

  forceinline
  StringValBranch::StringValBranch(): s(STRING_VAL_LLLM) {}

  forceinline
  StringValBranch::StringValBranch(Select s0): s(s0) {}

  forceinline StringValBranch::Select
  StringValBranch::select(void) const {
    return s;
  }

  forceinline StringValBranch
  STRING_VAL_LLLL(void) {
    return StringValBranch(StringValBranch::STRING_VAL_LLLL);
  }

  forceinline StringValBranch
  STRING_VAL_LLUL(void) {
    return StringValBranch(StringValBranch::STRING_VAL_LLUL);
  }
  
  forceinline StringValBranch
  STRING_VAL_LLLM(void) {
    return StringValBranch(StringValBranch::STRING_VAL_LLLM);
  }

}
