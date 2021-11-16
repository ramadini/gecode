#include <gecode/string.hh>
#include <gecode/driver.hh>
#include <math.h>

using namespace Gecode;
using namespace String;

std::vector<std::string> W({
  "]c4]?e]qCu|B,LSV!W(e:",
  "W3ee:?@NbT",
  ":?@NbT;^AZR3IuW3ee:)DpBr%&C]=x=BqcG8[Pe.Uj`",
  "e.Uj`ju#",
  "bT]?e]qCu|B,LSV!W("
});
int N = 0;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) {
      this->c_d(1);
  }

};

/*
* Given a set of fixed strings {w_1, ..., w_k} determine its Shortest Common 
* Superstring (SCS), i.e., the shortest w s.t. find(w,w_i) > 0 for i=1,...,k
*/
class SCS : public IntMinimizeScript {

  StringVarArray string_vars;
  IntVarArray int_vars;

public:

  SCS(SCS& s): IntMinimizeScript(s) {
    string_vars.update(*this, s.string_vars);
    int_vars.update(*this, s.int_vars);
  }
  virtual Space* copy() {
    return new SCS(*this);
  }

  SCS(const StringOptions& so): IntMinimizeScript(so) {
    for (auto wi : W)
      N += wi.size();
    // Variables.
    IntVarArgs iva;
    StringVarArgs sva;
    StringVar w(*this);
    sva << w;
    IntVar l(*this, 0, N);
    iva << l;
    length(*this, w, l);
    for (auto wi : W) {
      IntVar i(*this, 1, N-wi.size()+1);
      iva << i;
      find(*this, w, StringVar(*this, str2vec(wi)), i);
    }
    int_vars = IntVarArray(*this, iva);
    string_vars = StringVarArray(*this, sva);
    IntVarArgs branch_vars = int_vars.slice(1);
    branch_vars << l;
    branch(*this, branch_vars, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    lenblock_min_lllm(*this, sva);
  }

  virtual IntVar cost(void) const {
    return int_vars[0];
  }

  virtual void
  print(std::ostream& os) const {
    int i = 0;
    N = string_vars[0].min_length();
    std::cerr << "Superstring: " << vec2str(string_vars[0].val()) 
                                 << " (length " << N << ")\n";
    for (auto wi : W)
      std::cerr << "\t\"" << wi << "\": starts at index " 
                                << int_vars[++i] << "\n";
    std::cerr << "----------\n";
  }

};

int main() {
  StringOptions opt("*** Shortest Common Superstring ***");
  opt.solutions(0);
  Script::run<SCS, BAB, StringOptions>(opt);
  assert (N == 89);
  std::cerr << "\n";
  return 0;
}
