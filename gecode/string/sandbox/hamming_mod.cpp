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
  static std::string
  vec2str(const std::vector<int>& v) {
    std::string w;
    for (int i : v)
      w += int2str(i);
    return w;
  }

  Benchmark(const StringOptions& so): Script(so) {

//      var_5 ++ "z" ++ var_6 IN (("a" ++ (("z" | "b") ++ (("b")* ++ ("z" | "a")))))*
//      var_5 ++ "z" ++ var_6 IN ((("z" | "b") ++ ("a" ++ ("z" | "b" | "a"))))* ++ "a"
//      var_5 ++ "z" ++ var_6 IN ((("z" | "b") ++ ("a" ++ ("z" | "a"))))* ++ "a"
//      var_6 IN ((("a"-"u"))*)
//      var_5 IN ((("a"-"u"))*)
//      ((((|var_1| + -1) <= 0) AND (0 <= (|var_1| + -1))) AND
//      ((((|var_3| + (-1 * |var_6|)) + -1) <= 0) AND
//      (0 <= ((|var_3| + (-1 * |var_6|)) + -1)))) AND (0 <= |var_6|)
//TODO:
//    // Variables.
//    StringVar var_1(*this, 0, so.N);
//    StringVar var_3(*this, 0, so.N);
//    StringVar var_5(*this, 0, so.N);
//    StringVar var_6(*this, 0, so.N);
//    StringVar var_5z(*this, 0, so.N);
//    StringVar var_5z6(*this, 0, so.N);
//    StringVar expr_1(*this, 0, so.N);
//    StringVar expr_2(*this, 0, so.N);
//    StringVar expr_3(*this, 0, so.N);
//    StringVarArgs sva;
//    sva << var_1 << var_3 << var_5 << var_6 << expr_1 << expr_2 << expr_3;
//    str_vars = StringVarArray(*this, sva);
//    IntVar l_1(*this, 0, so.N);
//    IntVar l_3(*this, 0, so.N);
//    IntVar l_6(*this, 0, so.N);
//    IntVar n(*this, 0, so.N);
//    IntVar m(*this, 0, so.N);
//    IntVar l(*this, 0, so.N);
//    IntVarArgs iva;
//    iva << l_1 << l_3 << l_6 << n << m << l;
//    int_vars = IntVarArray(*this, iva);

//    // Constraints.
//    length(*this, var_1, l_1);
//    length(*this, var_3, l_3);
//    length(*this, var_6, l_6);
//    rel(*this, l_1 + (-1) <= 0);
//    rel(*this, 0 <= l_1 + (-1));
//    rel(*this, 0 <= l_6);
//    rel(*this, (l_3 + (-1) * l_6) + 1 <= 0);
//    rel(*this, (l_3 + (-1) * l_6) + 1 >= 0);

//    NSBlocks v({NSBlock(NSIntSet('a', 'u'), 0, so.N)});
//    rel(*this, var_5, STRT_DOM, v, 0, so.N);
//    rel(*this, var_6, STRT_DOM, v, 0, so.N);
//    rel(*this, var_5, StringVar(*this, "z"), STRT_CAT, var_5z);
//    rel(*this, var_5z, var_6, STRT_CAT, var_5z6);

//    // var_5 ++ "z" ++ var_6 IN (("a" ++ (("z" | "b") ++ (("b")* ++ ("z" | "a")))))*
//    NSIntSet zb('z');
//    zb.include('b');
//    NSIntSet za('z');
//    za.include('a');
//    NSBlocks v1({
//      NSBlock(NSIntSet('a', 'a'), 1, 1),
//      NSBlock(zb, 1, 1),
//      NSBlock(NSIntSet('b', 'b'), 0, so.N),
//      NSBlock(za, 1, 1)
//    });
//    pow(*this, StringVar(*this, v1, 3, so.N), n, expr_1);
//    rel(*this, var_5z6, STRT_EQ, expr_1);

//    // var_5 ++ "z" ++ var_6 IN ((("z" | "b") ++ ("a" ++ ("z" | "b" | "a"))))* ++ "a"
//    NSIntSet zba(zb);
//    zba.include('a');
//    NSBlocks v2({
//      NSBlock(zb, 1, 1),
//      NSBlock(NSIntSet('a', 'a'), 1, 1),
//      NSBlock(zba, 1, 1)
//    });
//    pow(*this, StringVar(*this, v2, 3, 3), m, expr_2);
//    rel(*this, expr_2, StringVar(*this, "a"), STRT_CAT, var_5z6);

//    // var_5 ++ "z" ++ var_6 IN ((("z" | "b") ++ ("a" ++ ("z" | "a"))))* ++ "a"
//    NSBlocks v3({
//      NSBlock(zb, 1, 1),
//      NSBlock(NSIntSet('a', 'a'), 1, 1),
//      NSBlock(za, 1, 1)
//    });
//    pow(*this, StringVar(*this, v3, 3, 3), l, expr_3);
//    rel(*this, expr_3, StringVar(*this, "a"), STRT_CAT, var_5z6);

    // Branching.
    lblock_mindim_lllm(*this, str_vars);
  }
  


  virtual void
  print(std::ostream& os) const {
    assert (0);
  }

};

int main(int argc, char* argv[]) {
  int n = argc == 1 ? 5000 : atoi(argv[1]);
  StringOptions opt("*** hamming mod. ***", n);
  opt.solutions(1);
  Script::run<Benchmark, DFS, StringOptions>(opt);
  return 0;
}
