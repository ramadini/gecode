#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;
using std::string;

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
  static std::string
  vec2str(const std::vector<int>& v) {
    std::string w;
    for (int i : v)
      w += int2str(i);
    return w;
  }
  static std::vector<int>
  str2vec(const std::string& w) {
    std::vector<int> v(w.size());
    for (int i = 0; i < (int) w.size(); ++i)
      v[i] = w[i];
    return v;
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
    Block b[2];
    b[0].update(*this, 'z');
    b[1].update(*this, 'b');
    concat(*this, var_5, StringVar(*this, DashedString(*this, b, 2)), var_5zb);
    pow(*this, StringVar(*this, Block(*this, CharSet(*this,'a','c'), 0, so.N)), 
         n, var_abc);
    concat(*this, StringVar(*this, Block(*this, CharSet(*this, 'z'), 0, so.N)), 
                  var_abc, var_5zb6);
    concat(*this, var_5zb, var_6, var_5zb6);

    Block d[3];
    d[0].update(*this, Block(*this, 
                       CharSet(*this, IntSet({'a','c','z'})), 1, 1));
    d[1].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, so.N));
    d[2].update(*this, 'z');
    pow(*this, StringVar(*this, DashedString(*this, d, 3)), m, expr_2);
    concat(*this, expr_2, StringVar(*this,DashedString(*this, d, 2)), var_5zb6);

    DashedString dx(*this, 
      Block(*this, CharSet(*this, IntSet({'a','c','z'})), 1, 1));
    Block dy[2];
    dy[0].update(*this, 'b');
    dy[1].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, so.N));
    rel(*this, expr_3, STRT_EQ, StringVar(*this, Block()), Reify(b1));
    rel(*this, expr_3, STRT_EQ, StringVar(*this, DashedString(*this, dy, 2)), 
                                                           Reify(b2));
    rel(*this, b1 + b2 == 1);
    concat(*this, StringVar(*this, dx), expr_3, var_5zb6);

    Block u(*this, CharSet(*this, 'a', 'u'), 0, so.N);
    concat(*this, StringVar(*this, Block('b')), var_5, StringVar(*this, u));
    dom(*this, var_5, u);
    // Branching.
    lenblock_min_lllm(*this, str_vars);
  }
  
  virtual void
  print(std::ostream& os) const {
    s += vec2str(str_vars[str_vars.size() - 1].val());
    for (int i = 0; i < int_vars.size(); ++i)
      if (int_vars[i].assigned())
        os << "int_var[" << i << "] = " << int_vars[i].val() << "\n";
      else
        os << "int_var[" << i << "] = " << int_vars[i] << "\n";
    for (int i = 0; i < str_vars.size(); ++i)
      if (str_vars[i].assigned())
        os << "string_var[" << i << "] = \"" << vec2str(str_vars[i].val()) << "\"\n";
      else
        os << "string_var[" << i << "] = \"" << str_vars[i] << "\"\n";
    os << "----------\n";
  }

};
string Benchmark::s = "";

int main(int argc, char* argv[]) {
  int n = argc == 1 ? 5000 : atoi(argv[1]);
  StringOptions opt("*** string replace ***", n);
  opt.solutions(1);
  Script::run<Benchmark, DFS, StringOptions>(opt);
  assert (Benchmark::s == "zb");
  return 0;
}
