#include <gecode/string.hh>
#include <gecode/driver.hh>
#include <set>
using namespace Gecode;
using namespace String;
using std::string;
using std::set;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex4 : public Script {

  IntVarArray int_vars;
  StringVarArray string_vars;

public:
  
  static std::set<string> solset;

  Ex4(Ex4& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex4(*this);
  }
  
  static std::vector<int>
  str2vec(const string& w) {
    int n = w.size();
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i)
      v[i] = w[i];
    return v;
  }
  static string
  vec2str(const std::vector<int>& v) {
    int n = v.size();
    string w;
    for (int i = 0; i < n; ++i)
      w += v[i];
    return w;
  }

  Ex4(const StringOptions& so): Script(so) {
    StringVar x(*this, Block(*this, CharSet(*this, '0', '1'), 0, 3));
    StringVar y(*this);
    StringVarArgs sva;
    sva << x << y;
    IntVar j(*this, 2, 3);
    IntVarArgs iva;
    iva << j;
    int_vars = IntVarArray(*this, iva);
    string_vars = StringVarArray(*this, sva);
    // Constraints.
    substr(*this, x, IntVar(*this, 2, 2), j, y);
    rel(*this, STRT_REV, y, y);
    // Branching.
    lenblock_min_lllm(*this, string_vars);
    branch(*this, int_vars, INT_VAR_NONE(), INT_VAL_MIN());
  }

  virtual void
  print(std::ostream& os) const {
    os   << "j = "   << int_vars[0] .val() << " --- "
       << "x = \"" << vec2str(string_vars[0].val()) << "\" --- "
       << "y = \"" << vec2str(string_vars[1].val()) << "\"\n----------\n";
    solset.insert(vec2str(string_vars[1].val()));
  }

};
std::set<string> Ex4::solset = std::set<string>();

int main() {
  StringOptions opt("*** Ex4 ***");
  opt.solutions(0);
  Script::run<Ex4, DFS, StringOptions>(opt);
  assert (Ex4::solset.size() == 5);
  return 0;
}
