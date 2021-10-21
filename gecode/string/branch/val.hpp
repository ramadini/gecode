namespace Gecode {

  forceinline
  StringValBranch::StringValBranch(): s(STR_LLLM) {}

  forceinline
  StringValBranch::StringValBranch(Select s0): s(s0) {}

  forceinline StringValBranch::Select
  StringValBranch::select(void) const {
    return s;
  }

  forceinline StringValBranch
  STRING_VAL_LLLL(void) { return StringValBranch(StringValBranch::STR_LLLL); }
  
  forceinline StringValBranch
  STRING_VAL_LLLM(void) { return StringValBranch(StringValBranch::STR_LLLM); }
  
  forceinline StringValBranch
  STRING_VAL_LSLM(void) { return StringValBranch(StringValBranch::STR_LSLM); }

}
