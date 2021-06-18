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
    StringVar var_3(*this, 0, so.N);
    StringVar var_4(*this, 0, so.N);
    StringVar var_34(*this, 0, so.N);
    StringVarArgs sva;
    sva << var_3 << var_4 << var_34;
    str_vars = StringVarArray(*this, sva);
    IntVar l_3(*this, 0, so.N);
    IntVar l_4(*this, 0, so.N);
    IntVarArgs iva;
    iva << l_3 << l_4;
    int_vars = IntVarArray(*this, iva);
    BoolVar b1(*this, 0, 1);
    BoolVar b2(*this, 0, 1);
    BoolVarArgs bva;
    bva << b1 << b2;
    bool_vars = BoolVarArray(*this, bva);
    // Constraints.
    length(*this, var_3, l_3);
    length(*this, var_4, l_4);
    rel(*this, l_4 + (-1) <= 0);
    rel(*this, 0 <= l_4 + (-1));
    rel(*this, l_3 + (-1) <= 0);
    rel(*this, 0 <= l_3 + (-1));
    concat(*this, var_3, var_4, var_34);
    Block bb1[2];
    bb1[0].update(*this, 'a');
    bb1[1].update(*this, 'b');
    Block bb2[2];
    bb2[0].update(*this, 'b');
    bb2[1].update(*this, 'a');
    DashedString d1(*this, bb1, 2);
    DashedString d2(*this, bb2, 2);
    rel(*this, var_34, STRT_EQ, StringVar(*this, d1), Reify(b1));
    rel(*this, var_34, STRT_EQ, StringVar(*this, d2), Reify(b2));
    rel(*this, b1 + b2 == 1);
    // Branching.
    block_mindim_lslm(*this, str_vars);
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
  StringOptions opt("*** levenshtein ***", n);
  opt.solutions(0);
  Script::run<Benchmark, DFS, StringOptions>(opt);
  assert (Benchmark::s == "abba");
  return 0;
}
