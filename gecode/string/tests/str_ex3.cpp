#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex3 : public IntMaximizeScript {

  IntVarArray int_vars;
  StringVarArray str_vars;

public:

  static string S;

  Ex3(Ex3& s): IntMaximizeScript(s) {
    int_vars.update(*this, s.int_vars);
    str_vars.update(*this, s.str_vars);
  }
  virtual Space* copy() {
    return new Ex3(*this);
  }

  Ex3(const StringOptions& so): IntMaximizeScript(so) {
    // Variables.
    int N = DashedString::_MAX_STR_LENGTH;
    NSBlocks v(1, NSBlock(NSIntSet('a', 'c'), 0, N));
    StringVar x(*this, v, 0, N);
    IntVar l(*this, 0, 1000);
    IntVarArgs iva;
    iva << l;
    int_vars = IntVarArray(*this, iva);
    StringVarArgs sva;
    sva << x;
    str_vars = StringVarArray(*this, sva);
    // Constraints.
    NSBlocks w(1, NSBlock(NSIntSet('a'), 1000, 1000));
    rel(*this, x, STRT_NQ, StringVar(*this, w, 1000, 1000));
    length(*this, x, l);
    // Branching.
    lenmax_llll(*this, str_vars);
  }

  virtual void
  print(std::ostream& os) const {
    for (int i = 0; i < int_vars.size(); ++i)
      os << "int_var[" << i << "] = " << int_vars[i].val() << "\n";
    for (int i = 0; i < str_vars.size(); ++i)
      os << "string_var[" << i << "] = \"" << str_vars[i].val() << "\"\n";
    os << "----------\n";
    S = str_vars[0].val();
  }

  virtual IntVar cost(void) const {
    return int_vars[0];
  }

};

string Ex3::S = "";

int main() {
  StringOptions opt("*** Ex3 ***");
  opt.solutions(0);
  Script::run<Ex3, BAB, StringOptions>(opt);
  assert (Ex3::S != string(1000, 'a'));
  std::cerr<<"==========\n";
   return 0;
}
