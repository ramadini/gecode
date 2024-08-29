#include <gecode/string.hh>
#include <gecode/driver.hh>
#include <math.h>

using namespace Gecode;
using namespace String;

std::vector<string> W({
  "]c4]?e]qCu|B,LSV!W(e:",
  "W3ee:?@NbT",
  ":?@NbT;^AZR3IuW3ee:)DpBr%&C]=x=BqcG8[Pe.Uj`",
  "e.Uj`ju#",
  "bT]?e]qCu|B,LSV!W("
});

int N = -1;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) {
      this->c_d(1);
  }

};

/*
* Given a set of fixed strings W = {w_1, ..., w_k} determine the minimum length
* string w such that substr(w_1, w) /\ ... /\ substr(w_k, w).
*/
class MinSuperStr : public IntMinimizeScript {

  StringVarArray string_vars;
  IntVarArray int_vars;

public:


  MinSuperStr(MinSuperStr& s): IntMinimizeScript(s) {
    string_vars.update(*this, s.string_vars);
    int_vars.update(*this, s.int_vars);
  }
  virtual Space* copy() {
    return new MinSuperStr(*this);
  }

  MinSuperStr(const StringOptions& so): IntMinimizeScript(so) {
    // Variables.
    IntVarArgs iva;
    StringVarArgs sva;
    int m = 0, n = 100;
    NSIntSet s = NSIntSet::top();
    StringVar w(*this, s, m, n);
    sva << w;
    IntVar l(*this, 0, n);
    iva << l;
    length(*this, w, l);
    bool cont = false;
    for (auto& wi : W) {
      std::cerr << wi << '\n';
      if (cont)
        contains(*this, w, StringVar(*this, wi));
      else {
        IntVar i(*this, 1, n - wi.size() + 1);
        iva << i;
        find(*this, StringVar(*this, wi), w, i);
      }
    }
    int_vars = IntVarArray(*this, iva);
    string_vars = StringVarArray(*this, sva);
    if (!cont) {
      IntVarArgs branch_vars = int_vars.slice(1);
      branch_vars << l;
      branch(*this, branch_vars, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    }
    lenblockmin_lllm(*this, sva);
    // sizemin_llul(*this, sva); 
  }

  virtual IntVar cost(void) const {
    return int_vars[0];
  }

  virtual void
  print(std::ostream& os) const {
  os << "w = " << string_vars[0] << "\n";
  N = string_vars[0].min_length();
  }

};

int main() {
  StringOptions opt("*** Minimum super-string ***");
  opt.solutions(0);
  Script::run<MinSuperStr, BAB, StringOptions>(opt);
  assert (N == 89);
  return 0;
}
