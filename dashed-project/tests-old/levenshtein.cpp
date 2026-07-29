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
    // Variables.
    NSBlocks v = NSBlocks(1, NSBlock(NSIntSet('a', 'b'), 0, so.N));
    StringVar var_3(*this, 0, so.N);
    StringVar var_4(*this, 0, so.N);
    StringVar var_34(*this, v, 0, so.N);
    StringVarArgs sva;
    sva << var_3 << var_4 << var_34;
    str_vars = StringVarArray(*this, sva);
    IntVar l_3(*this, 0, so.N);
    IntVar l_4(*this, 0, so.N);
    IntVar i(*this, 1, 2);
    IntVarArgs iva;
    iva << l_3 << l_4 << i;
    int_vars = IntVarArray(*this, iva);
    // Constraints.
    length(*this, var_3, l_3);
    length(*this, var_4, l_4);
    rel(*this, l_4 + (-1) <= 0);
    rel(*this, 0 <= l_4 + (-1));
    rel(*this, l_3 + (-1) <= 0);
    rel(*this, 0 <= l_3 + (-1));
    rel(*this, var_3, var_4, STRT_CAT, var_34);
    StringVarArgs var_3or4;
    var_3or4 << StringVar(*this, "ab") << StringVar(*this, "ba");
    element(*this, var_3or4, i, var_34);
    // Branching.
    sizemin_llul(*this, str_vars);
  }

  virtual void
  print(std::ostream& os) const {
    s += str_vars[str_vars.size() - 1].val();
    for (int i = 0; i < int_vars.size(); ++i)
      os << "int_var[" << i << "] = " << int_vars[i].val() << "\n";
    for (int i = 0; i < str_vars.size(); ++i)
      os << "string_var[" << i << "] = \"" << str_vars[i].val() << "\"\n";
    os << "----------\n";
  }

};

string Benchmark::s = "";

int
main(int argc, char* argv[]) {
  StringOptions opt("*** levenshtein ***", atoi(argv[1]));
  opt.solutions(0);
  Script::run<Benchmark, DFS, StringOptions>(opt);
  assert (Benchmark::s == "abba");
  return 0;
}
