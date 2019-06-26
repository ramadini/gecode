#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

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

  // b => x = y; x != ""; y = "Hello world!";
  Ex1(const StringOptions& so): Script(so) {
    // Variables.
    BoolVar b(*this, 0, 1);
    int N = DashedString::_MAX_STR_LENGTH;
    int M = DashedString::_MAX_STR_ALPHA;
    NSBlocks v(1, NSBlock(NSIntSet('a', M), 0, N));
    StringVar x(*this, v, 0, N);
    StringVar y(*this);
    NSBlocks w(NSBlocks(2, NSBlock(NSIntSet('a', 'z'), 1, 2)));
    StringVar z(*this, w, 0, N);
    BoolVarArgs bva;
    StringVarArgs sva;
    bva << b;
    sva << x << y << z;
    bool_vars = BoolVarArray(*this, bva);
    string_vars = StringVarArray(*this, sva);
    // Constraints.
    rel(*this, x, STRT_NQ, StringVar(*this, ""));
    rel(*this, x, STRT_NQ, z);
    rel(*this, y, STRT_EQ, StringVar(*this, "Hello world!"));    
    rel(*this, x, STRT_EQ, y, Reify(b));
    // Branching.
    blockmin_llll(*this, string_vars);
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
        os << "string_var[" << i << "] = \"" << string_vars[i].val() << "\"\n";
      else
        os << "string_var[" << i << "] = \"" << string_vars[i] << "\"\n";
    os << "----------\n";
    if (S1 == "")
      S1 = string_vars[0].val();
    else
      S2 = string_vars[0].val();
  }

};

string Ex1::S1 = "";
string Ex1::S2 = "";

int main() {
  StringOptions opt("*** Ex1 ***");
  opt.solutions(2);
  Script::run<Ex1, DFS, StringOptions>(opt);
  assert(Ex1::S1 == "a" && Ex1::S2 == "b");
   return 0;
}
