#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;
using std::string;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex1 : public Script {

  BoolVarArray bool_vars;
  StringVarArray string_vars;

public:

  static string S1;
  static string S2;

  Ex1(Ex1& s): Script(s) {
    bool_vars.update(*this, s.bool_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex1(*this);
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

  // x != "" /\ x != z /\ z :: {a..z}^(2,4) /\ y = "Hello world!" /\ b <=> x = y
  Ex1(const StringOptions& so): Script(so) {
    // Variables.
    BoolVar b(*this, 0, 1);
    int N = Limits::MAX_STRING_LENGTH;
    int M = Limits::MAX_ALPHABET_SIZE;
    DashedString v(*this, Block(*this, CharSet(*this, 'a', M-1), 0, N));
    StringVar x(*this, v);
    StringVar y(*this);
    DashedString w(*this, Block(*this, CharSet(*this, 'a', 'z'), 2, 4));
    StringVar z(*this, w);
    BoolVarArgs bva;
    StringVarArgs sva;
    bva << b;
    sva << x << y << z;
    bool_vars = BoolVarArray(*this, bva);
    string_vars = StringVarArray(*this, sva);
    // Constraints.
    nq(*this, x, std::vector<int>());
    rel(*this, x, STRT_NQ, z);
    eq(*this, y, str2vec("Hello world!"));    
    rel(*this, x, STRT_EQ, y, Reify(b));
    // Branching.
    lenblock_min_lllm(*this, string_vars);
  }

  virtual void
  print(std::ostream& os) const {
    for (int i = 0; i < bool_vars.size(); ++i)
      if (bool_vars[i].assigned())
        os << "bool_var[" << i << "] = " << bool_vars[i].val() << "\n";
      else
        os << "bool_var[" << i << "] = " << bool_vars[i] << "\n";
    for (int i = 0; i < string_vars.size(); ++i)
      if (string_vars[i].assigned())
        os << "string_var[" << i << "] = \"" << vec2str(string_vars[i].val()) << "\"\n";
      else
        os << "string_var[" << i << "] = \"" << string_vars[i] << "\"\n";
    os << "----------\n";
    if (S1 == "")
      S1 = vec2str(string_vars[0].val());
    else
      S2 = vec2str(string_vars[0].val());
  }

};

string Ex1::S1 = "";
string Ex1::S2 = "";

int main() {
  StringOptions opt("*** Ex1 ***");
  opt.solutions(2);
  Script::run<Ex1, DFS, StringOptions>(opt);
  assert(Ex1::S1 == "d" && Ex1::S2 == "e");
  return 0;
}
