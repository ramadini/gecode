#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

  public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

string S;

class Ex9 : public IntMaximizeScript {

  StringVarArray string_vars;
  IntVarArray    int_vars;

public:

  Ex9(Ex9& s): IntMaximizeScript(s) {
    int_vars.update(*this, s.int_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex9(*this);
  }

  Ex9(const StringOptions& so): IntMaximizeScript(so) {
    // Variables.
    StringVar s(*this);
    IntVar n(*this, -10, 10);
    str2nat(*this, s, n);
    length(*this, s, n);
    StringVarArgs sva;
    sva << s;
    string_vars = StringVarArray(*this, sva);
    IntVarArgs iva;
    iva << n;
    int_vars = IntVarArray(*this, iva);
    blockmin_llll(*this, string_vars);
  }

  virtual void
  print(std::ostream& os) const {
    os << "S = " << string_vars[0] << '\n';
    os << "|S| = "  << int_vars[0] << '\n';
    S = string_vars[0].val();
  }

  virtual IntVar cost(void) const {
    return int_vars[0];
  }

};

int main() {
  StringOptions opt("*** Ex9 ***");
  opt.solutions(0);
  IntMaximizeScript::run<Ex9, BAB, StringOptions>(opt);
  assert (S == string(8, '0') + "10");
  return 0;
}
