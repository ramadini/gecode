#include <gecode/string.hh>
#include <gecode/string/extensional.hh>
#include <gecode/string/ext/parse-reg.hpp>

namespace Gecode {

  void
  extensional(Home home, StringVar x, String::RegEx* r) {
    if (Gecode::String::DashedString::_DECOMP_REGEX) {
      VarArgs v;
      r->post(home, x, v);
    }
    else
      extensional(home, x, r->dfa());
  }

  void
  extensional(Home home, StringVar x, std::string regex, VarArgs& v) {
    String::RegEx* r = String::RegExParser(regex).parse();
    if (Gecode::String::DashedString::_DECOMP_REGEX)
      r->post(home, x, v);
    else
      extensional(home, x, r->dfa());
  }

  void
  extensional(Home home, StringVar x, std::string regex) {
    String::RegEx* r = String::RegExParser(regex).parse();
//     std::cerr << "x = " << x << '\n';
//     std::cerr << "Expression: " << r->str() << '\n';
//     std::cerr << "DFA: " << r->dfa() << '\n';
//     std::cerr << "Reg.: " << r->reg() << '\n';
//     std::cerr << "Dec: " << r->decomp() << '\n';
//     std::cerr << "DECOMP:" << Gecode::String::DashedString::_DECOMP_REGEX << '\n';
//     std::cerr << "Intro: " << r->intro_vars() << '\n';
    if (Gecode::String::DashedString::_DECOMP_REGEX && !r->intro_vars()) {
      VarArgs v;
      r->post(home, x, v);
    }
    else
      extensional(home, x, r->dfa());
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
    String::RegEx* r = String::RegExParser(regex).parse();
    // std::cerr << "Expression: " << r->str() << '\n';
    // std::cerr << "DFA: " << r->dfa() << '\n';
    // std::cerr << "Reg.: " << r->reg() << '\n';
    // std::cerr << "Dec: " << r->decomp() << '\n';
    // std::cerr << "DECOMP:" << Gecode::String::DashedString::_DECOMP_REGEX << '\n';
    // std::cerr << "Intro: " << r->intro_vars() << '\n';
    try {
      extensional(home, x, r->dfa(), b, rm);
    }
    catch (int) {
      std::cerr << "Ignoring regex!\n";
    }
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
    for (auto s : y)
      a << StringVar(home, s);
    element(home, a, i, x);
  }

}
