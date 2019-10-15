#include <gecode/string.hh>
#include <gecode/driver.hh>
#include <math.h>

using namespace Gecode;
using namespace String;

int S = 0;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) {
      this->c_d(1);
  }

};

/*
* Find M^K distinct strings W_1, W_2, ... W_{M^K} of length N having alphabet
* {0, ..., M - 1} and such that W_{i+1} is derived from W_i by only changing one
* digit W_i[j], with 1 <= j <= N, and W_i[j] never occurs in W[1] ... W[j-1].
* If M <= N = K, the resulting sequence is a M-ary Grey code having width N.
*/
class Gray : public Script {

  StringVarArray string_vars;
  IntVarArray int_vars;

public:

  static int K;
  static int M;
  static int N;
  static float MAX;

  Gray(Gray& s): Script(s) {
    string_vars.update(*this, s.string_vars);
    int_vars.update(*this, s.int_vars);
  }
  virtual Space* copy() {
    return new Gray(*this);
  }

  Gray(const StringOptions& so): Script(so) {
    // Variables.
    assert (M > 0 && N > 0 && 1 < K && K <= N);
    NSIntSet s('0', '0' + M - 1);
    StringVarArgs sva;
    StringVarArgs aux;
    IntVarArgs iva;
    sva << StringVar(*this, s, N, N);
    VarArgs va;
    for (int i = 1; i < MAX; ++i) {
      sva << StringVar(*this, s, N, N);
      StringVar a1(*this, s, 1, 1);
      StringVar a2(*this, s, 1, 1);
      aux << a1 << a2;
      IntVar m(*this, 1, N);
      find(*this, a1, sva[i - 1], m);
      replace(*this, sva[i - 1], a1, a2, sva[i]);
      for (auto v : va.sva)
        aux << v;
      rel(*this, a1, STRT_NQ, a2);
    }
    for (int i = 0; i < aux.size(); ++i)
      sva << aux[i];
    for (int i = 0; i < MAX - 1; ++i)
      for (int j = i + 1; j < MAX; ++j)
        rel(*this, sva[i], STRT_NQ, sva[j]);
    string_vars = StringVarArray(*this, sva);
    int_vars = IntVarArray(*this, iva);
    // Branching.
    blockmin_lllm(*this, string_vars);
  }

  virtual void
  print(std::ostream& os) const {
    for (int i = 0; i < MAX; ++i)
      os << "x[" << i << "] = " << string_vars[i].val() << "\n";
    os << "----------\n";
    S++;
  }

};

int Gray::K = 3;
int Gray::M = 2;
int Gray::N = 3;
float Gray::MAX = pow(M, (float) K);

int main() {
  StringOptions opt("*** G(K, M, N) ***");
  opt.solutions(0);
  Script::run<Gray, DFS, StringOptions>(opt);
  assert (S == 2);
  return 0;
}
