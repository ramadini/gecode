#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

public:

  int N;

  StringOptions(const char* s, int n): Options(s), N(n) {
    this->c_d(1);
  }

};

class Benchmark : public Script {

  IntVarArray int_vars;
  StringVarArray str_vars;

public:

  Benchmark(Benchmark& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    str_vars.update(*this, s.str_vars);
  }
  virtual Space* copy() {
    return new Benchmark(*this);
  }

  Benchmark(const StringOptions& so): Script(so) {

//    %   var_0 IN "a"* ++ "b" ++ "b"*
//    %   var_0 IN ("a"-"u")*
//    %   var_0 IN ("a" | "b")* ++ "b" ++ "a" ++ ("a" | "b")*
//    %   0 <= |var_0| AND NOT NOT (EXISTS v in Int)(v * 2 = (|var_0| + 2))

    // Variables.
    StringVar x(*this, Block(*this, CharSet(*this), 0, so.N));
    IntVar l(*this, 0, so.N);
    IntVarArgs iva;
    //iva << l;
    int_vars = IntVarArray(*this, iva);
    StringVarArgs sva;
    sva << x;
    str_vars = StringVarArray(*this, sva);
    // Constraints.
    Block v1[3];
    v1[0].update(*this, Block(*this, CharSet(*this, 'a'), 0, so.N));
    v1[1].update(*this, 'b');
    v1[2].update(*this, Block(*this, CharSet(*this, 'b'), 0, so.N));
    DashedString d1(*this, v1, 3);
    eq(*this, x, StringVar(*this, d1));
    
    DashedString d2(*this, Block(*this, CharSet(*this, 'a', 'u'), 0, so.N));
    eq(*this, x, StringVar(*this, d2));
    
    Block v2[4];
    v2[0].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 0, so.N));
    v2[1].update(*this, 'b');
    v2[2].update(*this, 'a');
    v2[3].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 0, so.N));
    DashedString d3(*this, v2, 3);
    eq(*this, x, StringVar(*this, d3));

    length(*this, x, l);
    rel(*this, l >= 0);
    rel(*this, l * 2 == l + 2);
    // Branching.
    lenblock_min_lllm(*this, str_vars);
  }

  virtual void
  print(std::ostream& os) const {
    assert (0);
  }

};

int main(int argc, char* argv[]) {
  int n = argc == 1 ? 5000 : atoi(argv[1]);
  StringOptions opt("*** anbn ***", n);
  opt.solutions(1);
  Script::run<Benchmark, DFS, StringOptions>(opt);
  return 0;
}
