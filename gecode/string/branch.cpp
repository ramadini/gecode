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
  sizemin_llul(Home home, const StringVarArgs& x) {
    if (home.failed())
      return;
    ViewArray<String::StringView> y(home, x);
    String::Branch::SizeMin_LLUL::post(home, y);
  }

  void
  sizemin_llul(Home home, const StringVarArray& x) {
    if (home.failed())
      return;
    StringVarArgs y(x.size());
    for (int i = 0; i < x.size(); ++i)
      y[i] = x[i];
    ViewArray<String::StringView> z(home, y);
    String::Branch::SizeMin_LLUL::post(home, z);
  }

  void
  lenmax_llll(Home home, const StringVarArgs& x) {
    if (home.failed())
      return;
    ViewArray<String::StringView> y(home, x);
    String::Branch::LenMax_LLLL::post(home, y);
  }

  void
  lenmax_llll(Home home, const StringVarArray& x) {
    if (home.failed())
      return;
    StringVarArgs y(x.size());
    for (int i = 0; i < x.size(); ++i)
      y[i] = x[i];
    ViewArray<String::StringView> z(home, y);
    String::Branch::LenMax_LLLL::post(home, z);
  }

  void
  lenmin_llll(Home home, const StringVarArgs& x) {
    if (home.failed())
      return;
    ViewArray<String::StringView> y(home, x);
    String::Branch::LenMin_LLLL::post(home, y);
  }

  void
  lenmin_llll(Home home, const StringVarArray& x) {
    if (home.failed())
      return;
    StringVarArgs y(x.size());
    for (int i = 0; i < x.size(); ++i)
      y[i] = x[i];
    ViewArray<String::StringView> z(home, y);
    String::Branch::LenMin_LLLL::post(home, z);
  }

  void
  blockmin_llll(Home home, const StringVarArgs& x) {
    if (home.failed())
      return;
    ViewArray<String::StringView> y(home, x);
    String::Branch::BlockMin_LLLL::post(home, y);
  }

  void
  blockmin_llll(Home home, const StringVarArray& x) {
    if (home.failed())
      return;
    StringVarArgs y(x.size());
    for (int i = 0; i < x.size(); ++i)
      y[i] = x[i];
    ViewArray<String::StringView> z(home, y);
    String::Branch::BlockMin_LLLL::post(home, z);
  }
  
  void
  blockmin_lllm(Home home, const StringVarArgs& x) {
    if (home.failed())
      return;
    ViewArray<String::StringView> y(home, x);
    String::Branch::BlockMin_LLLM::post(home, y);
  }

  void
  blockmin_lllm(Home home, const StringVarArray& x) {
    if (home.failed())
      return;
    StringVarArgs y(x.size());
    for (int i = 0; i < x.size(); ++i)
      y[i] = x[i];
    ViewArray<String::StringView> z(home, y);
    String::Branch::BlockMin_LLLM::post(home, z);
  }
  
  void
  lenblockmin_lllm(Home home, const StringVarArgs& x) {
    if (home.failed())
      return;
    ViewArray<String::StringView> y(home, x);
    String::Branch::LenBlockMin_LLLM::post(home, y);
  }

  void
  lenblockmin_lllm(Home home, const StringVarArray& x) {
    if (home.failed())
      return;
    StringVarArgs y(x.size());
    for (int i = 0; i < x.size(); ++i)
      y[i] = x[i];
    ViewArray<String::StringView> z(home, y);
    String::Branch::LenBlockMin_LLLM::post(home, z);
  }
  
  void
  branch(Home home, const StringVarArgs& x,
         StringVarBranch vars, StringValBranch vals) {
    switch (vars.select()) {
      case StringVarBranch::Select::STRING_VAR_NONE:
        if (vals.select() == StringValBranch::Select::STRING_VAL_LLLL)
          none_llll(home, x);
        return;
      case StringVarBranch::Select::STRING_VAR_SIZEMIN:
        if (vals.select() == StringValBranch::Select::STRING_VAL_LLUL)
          sizemin_llul(home, x);
        return;        
      case StringVarBranch::Select::STRING_VAR_LENMIN:
        if (vals.select() == StringValBranch::Select::STRING_VAL_LLLL)
          lenmin_llll(home, x);
        return;
      case StringVarBranch::Select::STRING_VAR_LENMAX:
        if (vals.select() == StringValBranch::Select::STRING_VAL_LLLL)
          lenmax_llll(home, x);
        return;
      case StringVarBranch::Select::STRING_VAR_BLOCKMIN:
        if (vals.select() == StringValBranch::Select::STRING_VAL_LLLL)
          blockmin_llll(home, x);
        else if (vals.select() == StringValBranch::Select::STRING_VAL_LLLM)
          blockmin_lllm(home, x);
        return;
      case StringVarBranch::Select::STRING_VAR_LENBLOCKMIN:
        if (vals.select() == StringValBranch::Select::STRING_VAL_LLLM)
          lenblockmin_lllm(home, x);
        return;
      default:
        GECODE_NEVER;
    }
    std::cerr << "Undefined search heuristics. Using blockmin_lllm.";
    blockmin_lllm(home, x);
  };

}
