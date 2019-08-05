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
  BoolVarArray bool_vars;
  StringVarArray str_vars;

public:

  static string s;

  Benchmark(Benchmark& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    str_vars.update(*this, s.str_vars);
  }
  virtual Space* copy() {
    return new Benchmark(*this);
  }

  Benchmark(const StringOptions& so): Script(so) {

//  var_5 ++ "z" ++ "b" ++ var_6 IN ("z")* ++ ("" | (("b" | "a" | "c") ++ ("b" | "a" | "c"))*))
//  var_5 ++ "z" ++ "b" ++ var_6 IN ((("z" | ("a" | "c")) ++ ((("b" | ("a" | "c")))* ++ "z")))* ++ (("z" | ("a" | "c")) ++ (("b" | ("a" | "c")))*)
//  var_5 ++ "z" ++ "b" ++ var_6 IN (("z" | ("a" | "c")))* ++ ("" | ("b" ++ (("b" | ("a" | "c")))*))
//  "b" ++ var_6 IN ("a"-"u")*
//  var_5 IN ("a"-"u")*

    // Variables.
    StringVar var_5(*this, 0, so.N);
    StringVar var_6(*this, 0, so.N);
    StringVar var_abc(*this, 0, so.N);
    StringVar var_5zb(*this, 0, so.N);
    StringVar var_5zb6(*this, 0, so.N);
    StringVar expr_1(*this, 0, so.N);
    StringVar expr_2(*this, 0, so.N);
    StringVar expr_3(*this, 0, so.N);
    StringVarArgs sva;
    sva << var_5 << var_6 << var_abc << var_5zb << var_5zb6;
    str_vars = StringVarArray(*this, sva);
    IntVar n(*this, 0, so.N), m(*this, 0, so.N);
    IntVarArgs iva;
    iva << n << m;
    int_vars = IntVarArray(*this, iva);
    BoolVar b1(*this, 0, 1);
    BoolVar b2(*this, 0, 1);
    BoolVarArgs bva;
    bva << b1 << b2;
    bool_vars = BoolVarArray(*this, bva);

    // Constraints.
    rel(*this, var_5, StringVar(*this, "zb"), STRT_CAT, var_5zb);
    NSBlocks z({NSBlock(NSIntSet('z'), 0, so.N)});
    NSBlocks abc(1, NSBlock(NSIntSet('a', 'c'), 0, so.N));
    pow(*this, StringVar(*this, abc, 0, so.N), n, var_abc);
    rel(*this, StringVar(*this, z, 0, so.N), var_abc, STRT_CAT, var_5zb6);
    rel(*this, var_5zb, var_6, STRT_CAT, var_5zb6);

    NSIntSet s('a');
    s.include('c');
    s.include('z');
    NSBlocks v(1, NSBlock(s, 1, 1));
    v.push_back(NSBlock(NSIntSet('a', 'c'), 0, so.N));
    v.push_back(NSBlock(NSIntSet('z'), 1, 1));
    pow(*this, StringVar(*this, v, 2, so.N), m, expr_2);
    NSBlocks w(1, NSBlock(s, 1, 1));
    w.push_back(NSBlock(NSIntSet('a', 'c'), 0, so.N));
    rel(*this, expr_2, StringVar(*this, w, 1, so.N), STRT_CAT, var_5zb6);

    NSBlocks x(1, NSBlock(s, 0, so.N));
    NSBlocks y(1, NSBlock(NSIntSet('b'), 1, 1));
    y.push_back(NSBlock(NSIntSet('a', 'c'), 0, so.N));
    rel(*this, expr_3, STRT_EQ, StringVar(*this, ""), Reify(b1));
    rel(*this, expr_3, STRT_EQ, StringVar(*this, y, 1, so.N),  Reify(b2));
    rel(*this, b1 + b2 == 1);
    rel(*this, StringVar(*this, x, 0, so.N), expr_3, STRT_CAT, var_5zb6);

    NSBlocks u({NSBlock(NSIntSet('a', 'u'), 0, so.N)});
    rel(*this, StringVar(*this, "b"), var_5, STRT_CAT, StringVar(*this, u, 0, so.N));
    rel(*this, var_5, STRT_DOM, u, 0, so.N);

    // Branching.
    blockmin_lllm(*this, str_vars);
  }

  virtual void
  print(std::ostream& os) const {
    s = str_vars[str_vars.size() - 1].val();
    for (int i = 0; i < int_vars.size(); ++i)
      if (int_vars[i].assigned())
        os << "int_var[" << i << "] = " << int_vars[i].val() << "\n";
      else
        os << "int_var[" << i << "] = " << int_vars[i] << "\n";
    for (int i = 0; i < str_vars.size(); ++i)
      if (str_vars[i].assigned())
        os << "string_var[" << i << "] = \"" << str_vars[i].val() << "\"\n";
      else
        os << "string_var[" << i << "] = \"" << str_vars[i] << "\"\n";
    os << "----------\n";
  }

};

string Benchmark::s = "";

int main(int argc, char* argv[]) {
  StringOptions opt("*** string replace ***", atoi(argv[1]));
  opt.solutions(1);
  Script::run<Benchmark, DFS, StringOptions>(opt);
  assert (Benchmark::s == "zb");
  return 0;
}
