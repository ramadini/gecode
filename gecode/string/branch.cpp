#include <gecode/string/branch.hh>

namespace Gecode {

  void
  none_llll(Home home, const StringVarArgs& x) {
    if (home.failed())
      return;
    ViewArray<String::StringView> y(home, x);
    String::Branch::None_LLLL::post(home, y);
  }
  void
  none_llll(Home home, const StringVarArray& x) {
    if (home.failed())
      return;
    StringVarArgs y(x.size());
    for (int i = 0; i < x.size(); ++i)
      y[i] = x[i];
    ViewArray<String::StringView> z(home, y);
    String::Branch::None_LLLL::post(home, z);
  }
  
  void
  bdim_min_lllm(Home home, const StringVarArgs& x) {
    if (home.failed())
      return;
    ViewArray<String::StringView> y(home, x);
    String::Branch::BDim_Min_LLLM::post(home, y);
  }
  void
  bdim_min_lllm(Home home, const StringVarArray& x) {
    if (home.failed())
      return;
    StringVarArgs y(x.size());
    for (int i = 0; i < x.size(); ++i)
      y[i] = x[i];
    ViewArray<String::StringView> z(home, y);
    String::Branch::BDim_Min_LLLM::post(home, z);
  }
  
  void
  bdim_min_lslm(Home home, const StringVarArgs& x) {
    if (home.failed())
      return;
    ViewArray<String::StringView> y(home, x);
    String::Branch::BDim_Min_LSLM::post(home, y);
  }
  void
  bdim_min_lslm(Home home, const StringVarArray& x) {
    if (home.failed())
      return;
    StringVarArgs y(x.size());
    for (int i = 0; i < x.size(); ++i)
      y[i] = x[i];
    ViewArray<String::StringView> z(home, y);
    String::Branch::BDim_Min_LSLM::post(home, z);
  }
  
  void
  branch(Home home, const StringVarArgs& x,
                          StringVarBranch vars, StringValBranch vals) {
    switch (vars.select()) {
      case StringVarBranch::Select::STR_NONE:
        if (vals.select() == StringValBranch::Select::STR_LLLL)
          none_llll(home, x);
        return;
      case StringVarBranch::Select::STR_BDIM_MIN:
        if (vals.select() == StringValBranch::Select::STR_LLLM)
          bdim_min_lllm(home, x);
        else if (vals.select() == StringValBranch::Select::STR_LSLM)
          bdim_min_lslm(home, x);
        return;
      default:
        GECODE_NEVER;
    }
    std::cerr << "Undefined search heuristics. Using bdim_min_lllm.";
    bdim_min_lllm(home, x);
  };

}
