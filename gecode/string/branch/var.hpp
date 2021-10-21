namespace Gecode {

  forceinline
  StringVarBranch::StringVarBranch(void)
  : VarBranch<StringVar>(), s(STR_LENBLOCK_MIN) {}

  forceinline
  StringVarBranch::StringVarBranch(Select s0)
  : VarBranch<StringVar>(), s(s0) {}

  forceinline StringVarBranch::Select
  StringVarBranch::select(void) const {
    return s;
  }

  forceinline StringVarBranch
  STRING_VAR_NONE(void) { 
    return StringVarBranch(StringVarBranch::STR_NONE);
  }

  forceinline StringVarBranch
  STRING_VAR_BLOCK_MINDIM(void) {
    return StringVarBranch(StringVarBranch::STR_BLOCKDIM_MIN);
  }
  
  forceinline StringVarBranch
  STRING_VAR_LENBLOCK_MIN(void) { 
    return StringVarBranch(StringVarBranch::STR_LENBLOCK_MIN);
  }
  

}
