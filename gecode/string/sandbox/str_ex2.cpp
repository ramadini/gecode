#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;
using std::string;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex2 : public Script {

  IntVarArray int_vars;
  StringVarArray string_vars;

public:
  
  static unsigned N;

  Ex2(Ex2& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex2(*this);
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

  Ex2(const StringOptions& so): Script(so) {
    int N = Limits::MAX_STRING_LENGTH;
    StringVar x(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, N));
    IntVar l(*this, 2, 3);
    IntVarArgs iva;
    iva << l;
    int_vars = IntVarArray(*this, iva);
    StringVarArgs sva;
    sva << x;
    string_vars = StringVarArray(*this, sva);
    // Constraints.
    nq(*this, x, StringVar(*this, Block('b', 2)));
    Block b[3];
    b[0].update(*this, 'c');
    b[1].update(*this, 'a');
    b[2].update(*this, 'b');
    nq(*this, x, StringVar(*this, DashedString(*this, b, 3)));
    length(*this, x, l);
    // Branching.
    lenblock_min_lllm(*this, string_vars);
  }

  virtual void
  print(std::ostream& os) const {
    for (int i = 0; i < int_vars.size(); ++i)
      os << "int_var[" << i << "] = " << int_vars[i].val() << "\n";
    for (int i = 0; i < string_vars.size(); ++i)
      os << "string_var[" << i << "] = \"" << vec2str(string_vars[i].val()) << "\"\n";
    os << "----------\n";
    N++;
  }

};

unsigned Ex2::N = 0;

int main() {
  StringOptions opt("*** Ex2 ***");
  opt.solutions(0);
  Script::run<Ex2, DFS, StringOptions>(opt);
  assert (Ex2::N == 34);
  return 0;
}

