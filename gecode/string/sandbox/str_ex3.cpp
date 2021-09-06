#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;
using std::string;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex3 : public IntMaximizeScript {

  IntVarArray int_vars;
  StringVarArray str_vars;

public:
  
  static string S;
  static const int M = 100;

  Ex3(Ex3& s): IntMaximizeScript(s) {
    int_vars.update(*this, s.int_vars);
    str_vars.update(*this, s.str_vars);
  }
  virtual Space* copy() {
    return new Ex3(*this);
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

  Ex3(const StringOptions& so): IntMaximizeScript(so){
    // Variables.
    int N = Limits::MAX_STRING_LENGTH;    
    StringVar x(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, N));
    IntVar l(*this, 0, M);
    IntVarArgs iva;
    iva << l;
    int_vars = IntVarArray(*this, iva);
    StringVarArgs sva;
    sva << x;
    str_vars = StringVarArray(*this, sva);
    // Constraints.
    nq(*this, x, StringVar(*this, Block('a', M)));
    length(*this, x, l);
    // Branching.
    none_llll(*this, str_vars);

  }
  
  virtual IntVar cost(void) const {
    return int_vars[0];
  }

  virtual void
  print(std::ostream& os) const {
    for (int i = 0; i < int_vars.size(); ++i)
      os << "int_var[" << i << "] = " << int_vars[i].val() << "\n";
    for (int i = 0; i < str_vars.size(); ++i)
      os << "string_var[" << i << "] = \"" << vec2str(str_vars[i].val()) << "\"\n";
    os << "----------\n";
    S = vec2str(str_vars[0].val());
  }

};

string Ex3::S = "";

int main() {
  StringOptions opt("*** Ex3 ***");
  opt.solutions(0);
  IntMaximizeScript::run<Ex3, BAB, StringOptions>(opt);
  assert (Ex3::S == string(Ex3::M-1, 'a') + "b");
  std::cout<<"==========\n";
  return 0;
}

