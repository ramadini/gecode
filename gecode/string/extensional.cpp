#include <gecode/string.hh>
#include <gecode/string/extensional.hh>
#include <gecode/string/extensional/parse-reg.hpp>

namespace Gecode {

  namespace {

    void
    post_native_regex(Home home, StringVar x, const String::RegEx& expression) {
      if (expression.empty()) {
        rel(home, x, STRT_EQ, StringVar(home, ""));
        return;
      }
      const String::InterEx* intersection =
        dynamic_cast<const String::InterEx*>(&expression);
      if (intersection != nullptr) {
        post_native_regex(home, x, intersection->left());
        post_native_regex(home, x, intersection->right());
        return;
      }
      extensional(home, x, expression.dfa());
    }

    void
    post_native_regex(Home home, StringVar x, const String::RegEx& expression,
                      BoolVar b, ReifyMode rm) {
      if (expression.empty()) {
        rel(home, x, STRT_EQ, StringVar(home, ""), Reify(b, rm));
        return;
      }
      const String::InterEx* intersection =
        dynamic_cast<const String::InterEx*>(&expression);
      if (intersection == nullptr) {
        extensional(home, x, expression.dfa(), b, rm);
        return;
      }

      BoolVar left(home, 0, 1);
      BoolVar right(home, 0, 1);
      BoolVar conjunction(home, 0, 1);
      post_native_regex(home, x, intersection->left(), left, RM_EQV);
      post_native_regex(home, x, intersection->right(), right, RM_EQV);
      rel(home, left, BOT_AND, right, conjunction);
      switch (rm) {
        case RM_IMP:
          rel(home, b, IRT_LQ, conjunction);
          break;
        case RM_PMI:
          rel(home, conjunction, IRT_LQ, b);
          break;
        default:
          rel(home, conjunction, IRT_EQ, b);
      }
    }

  }

  void
  extensional(Home home, StringVar x, String::RegEx* r) {
    if (Gecode::String::DashedString::_DECOMP_REGEX) {
      VarArgs v;
      r->post(home, x, v);
    }
    else
      post_native_regex(home, x, *r);
  }

  void
  extensional(Home home, StringVar x, std::string regex, VarArgs& v) {
    std::unique_ptr<String::RegEx> r = String::RegExParser(regex).parse();
    if (Gecode::String::DashedString::_DECOMP_REGEX)
      r->post(home, x, v);
    else
      post_native_regex(home, x, *r);
  }

  void
  extensional(Home home, StringVar x, std::string regex) {
    std::unique_ptr<String::RegEx> r = String::RegExParser(regex).parse();
    // std::cerr << "x = " << x << '\n';
    // std::cerr << "Expression: " << r->str() << '\n';
    // std::cerr << "DFA: " << r->dfa() << '\n';
    // std::cerr << "Reg.: " << r->reg() << '\n';
    // std::cerr << "Dec: " << r->decomp() << '\n';
    // std::cerr << "DECOMP:" << Gecode::String::DashedString::_DECOMP_REGEX << '\n';
    // std::cerr << "Intro: " << r->intro_vars() << '\n';
    if (Gecode::String::DashedString::_DECOMP_REGEX && !r->intro_vars()) {
      VarArgs v;
      r->post(home, x, v);
    }
    else
      post_native_regex(home, x, *r);
  }

  void
  extensional(
    Home home, StringVar x, const DFA& d, BoolVar b, ReifyMode rm = RM_EQV
  ) {
    switch (rm) {
      case RM_IMP:
        GECODE_ES_FAIL((String::ReReg<Int::BoolView, RM_IMP>
          ::post(home, x, d, b)));
        return;
      case RM_PMI:
        GECODE_ES_FAIL((String::ReReg<Int::BoolView, RM_PMI>
          ::post(home, x, d, b)));
        return;
      default:
        GECODE_ES_FAIL((String::ReReg<Int::BoolView, RM_EQV>
          ::post(home, x, d, b)));
    }
  }

  void
  extensional(
    Home home, StringVar x, std::string regex, BoolVar b, ReifyMode rm = RM_EQV
  ) {
    std::unique_ptr<String::RegEx> r = String::RegExParser(regex).parse();
    // std::cerr << "Expression: " << r->str() << '\n';
    // std::cerr << "DFA: " << r->dfa() << '\n';
    // std::cerr << "Reg.: " << r->reg() << '\n';
    // std::cerr << "Dec: " << r->decomp() << '\n';
    // std::cerr << "DECOMP:" << Gecode::String::DashedString::_DECOMP_REGEX << '\n';
    // std::cerr << "Intro: " << r->intro_vars() << '\n';
    post_native_regex(home, x, *r, b, rm);
  }

  void
  extensional(Home home, StringVar x, DFA d) {
    GECODE_ES_FAIL((String::Reg::post(home, x, d)));
  }

  void
  extensional(Home home, StringVar x, StringVarArgs y) {
    IntVar i(home, 1, y.size());
    element(home, y, i, x);
  }

  void
  extensional(Home home, StringVar x, const std::vector<std::string>& y) {
    IntVar i(home, 1, y.size());
    StringVarArgs a;
    for (auto& s : y)
      a << StringVar(home, s);
    element(home, a, i, x);
  }

}
