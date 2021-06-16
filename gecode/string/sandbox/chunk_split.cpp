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

  static std::string s;

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

//       var_5 ++ var_6 ++ var_7 ++ "z" ++ var_5 IN ("z" | (("a" | "b") | "c"))*
//       var_5 IN ("a"-"u")*
//       var_5 ++ var_6 ++ var_7 IN ("a"-"u")*
//       (|var_6| + -8) <= 0 AND 0 <= (|var_6| + -8)
//         AND (((|var_7| + |var_5|) + -1 * |var_2|) + 8) <= 0
//         AND 0 <= (((|var_7| + |var_5|) + -1 * |var_2|) + 8)
//         AND ((|var_3| + -1 * |var_5|) + -8) <= 0
//         AND 0 <= ((|var_3| + -1 * |var_5|) + -8)
//         AND 0 <= ((-1 * |var_5| + |var_2|) + -8) AND 0 <= |var_5|

    // Variables.
    StringVar var_3(*this, 0, so.N);
    StringVar var_5(*this, 0, so.N);
    StringVar var_6(*this, 0, so.N);
    StringVar var_7(*this, 0, so.N);
    StringVar var_56(*this, 0, so.N);
    StringVar var_567(*this, 0, so.N);
    StringVar var_567z(*this, 0, so.N);
    StringVar var_567z5(*this, 0, so.N);
    
    StringVarArgs sva;
    sva << var_3 << var_5 << var_6 << var_7
        << var_56 << var_567 << var_567z << var_567z5;
    str_vars = StringVarArray(*this, sva);

    IntVar l_2(*this, 0, so.N);
    IntVar l_3(*this, 0, so.N);
    IntVar l_5(*this, 0, so.N);
    IntVar l_6(*this, 0, so.N);
    IntVar l_7(*this, 0, so.N);
    IntVarArgs iva;
    iva << l_2 << l_3 << l_5 << l_6 << l_7;
    int_vars = IntVarArray(*this, iva);

    // Constraints.
    length(*this, var_3, l_3);
    length(*this, var_5, l_5);
    length(*this, var_6, l_6);
    length(*this, var_7, l_7);
    rel(*this, l_6 + (-8) <= 0);
    rel(*this, 0 <= l_6 + (-8));
    rel(*this, ((l_7 + l_5) + (-1 * l_2)) + (8) <= 0);
    rel(*this, 0 <= ((l_7 + l_5) + (-1 * l_2)) + (8));
    rel(*this, (l_3 + (-1 * l_5)) + (-8) <= 0);
    rel(*this, 0 <= (l_3 + (-1 * l_5)) + (-8));
    rel(*this, 0 <= ((-1 * l_5) + l_2) + (-8));
    rel(*this, 0 <= l_5);
    
//TODO:    rel(*this, STRT_CAT, var_5, var_6, var_56);
//    concat(*this, var_56, var_7, var_567);
//    rel(*this, STRT_CAT, var_567, StringVar(*this, Block('z')), var_567z);
//    concat(*this, var_567z, var_5, var_567z5);

    IntSet s({'a', 'c', 'z'});
    eq(*this, var_567z5, StringVar(*this, s, 0, so.N));
    eq(*this, var_5, StringVar(*this, IntSet('a', 'u'), 0, so.N));
    eq(*this, var_567, StringVar(*this, IntSet('a', 'u'), 0, so.N));
    lblock_mindim_lllm(*this, str_vars);
  }
  


  virtual void
  print(std::ostream& os) const {
    s = vec2str(str_vars[str_vars.size() - 1].val());
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

std::string Benchmark::s = "";

int main(int argc, char* argv[]) {
  int n = argc == 1 ? 5000 : atoi(argv[1]);
  StringOptions opt("*** chunk split ***", n);
  opt.solutions(1);
  Script::run<Benchmark, DFS, StringOptions>(opt);
  assert (Benchmark::s == "aaaaaaaaz");
  return 0;
}
