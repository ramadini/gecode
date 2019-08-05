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

}
