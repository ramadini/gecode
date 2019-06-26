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

  static bool sat;

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
    StringVar x(*this, 0, so.N);
    IntVar l(*this, 0, so.N);
    IntVarArgs iva;
    //iva << l;
    int_vars = IntVarArray(*this, iva);
    StringVarArgs sva;
    sva << x;
    str_vars = StringVarArray(*this, sva);
    // Constraints.
    NSBlocks v1({
      NSBlock(NSIntSet('a', 'a'), 0, so.N),
      NSBlock(NSIntSet('b', 'b'), 1, 1),
      NSBlock(NSIntSet('b', 'b'), 0, so.N)
    });
    NSBlocks v2({
      NSBlock(NSIntSet('a', 'u'), 0, so.N)
    });
    NSBlocks v3({
      NSBlock(NSIntSet('a', 'b'), 0, so.N),
      NSBlock(NSIntSet('b', 'b'), 1, 1),
      NSBlock(NSIntSet('a', 'a'), 1, 1),
      NSBlock(NSIntSet('a', 'b'), 0, so.N)
    });

    rel(*this, x, STRT_DOM, v1, 0, so.N);
    rel(*this, x, STRT_DOM, v2, 0, so.N);
    rel(*this, x, STRT_DOM, v3, 0, so.N);
    //length(*this, x, l);
    //rel(*this, l >= 0);
    //rel(*this, l * 2 == l + 2);
    // Branching.
    blockmin_llll(*this, str_vars);
  }

  virtual void
  print(std::ostream& os) const {
    sat = true;
    for (int i = 0; i < int_vars.size(); ++i)
      os << "int_var[" << i << "] = " << int_vars[i].val() << "\n";
    for (int i = 0; i < str_vars.size(); ++i)
      os << "string_var[" << i << "] = \"" << str_vars[i].val() << "\"\n";
    os << "----------\n";
  }

};

bool Benchmark::sat = false;

int main(int argc, char* argv[]) {
  StringOptions opt("*** anbn ***", atoi(argv[1]));
  opt.solutions(1);
  Script::run<Benchmark, DFS, StringOptions>(opt);
  assert (!Benchmark::sat);
  return 0;
}
