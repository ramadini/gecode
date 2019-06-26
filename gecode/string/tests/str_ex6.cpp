#include <gecode/string.hh>
#include <gecode/driver.hh>
#include <math.h>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex6 : public Script {

  StringVarArray string_vars;

public:

  static int M;
  static double N;

  Ex6(Ex6& s): Script(s) {
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex6(*this);
  }

  Ex6(const StringOptions& so): Script(so) {
    // Variables.
    StringVarArgs sva;
    NSBlocks w(1, NSBlock(NSIntSet('a', 'a' + M - 1), N, N));
    double n = pow(M, N);
    for (int i = 0; i < n; ++i)
      sva << StringVar(*this, w, N, N);
    string_vars = StringVarArray(*this, sva);
    // Constraints.
    for (int i = 0; i < n - 1; ++i)
      rel(*this, sva[i], STRT_LEXLT, sva[i + 1]);
    // Branching.
    sizemin_llul(*this, string_vars);
  }

  virtual void
  print(std::ostream& os) const {
  string prev = "";
    for (int i = 0; i < pow(M, N); ++i) {
      string curr = string_vars[i].val();
      os << "x [" << i << "] = \"" << curr << "\"\n";
      assert (prev < curr);
      prev = curr;
    }
    std ::cerr << "----------\n==========\n";
  }

};

int Ex6::M = 3;
double Ex6::N = 5;

int main() {
  StringOptions opt("*** Ex6 ***");
  opt.solutions(1);
  Script::run<Ex6, DFS, StringOptions>(opt);
  return 0;
}
