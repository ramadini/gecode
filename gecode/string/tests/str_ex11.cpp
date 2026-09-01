#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

  public:

    StringOptions(const char* s): Options(s) { this->c_d(1); }

};

int N = 0;

class Ex11 : public Script {

  IntVarArray int_vars;
  StringVarArray string_vars;

public:

  Ex11(Ex11& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex11(*this);
  }

  Ex11(const StringOptions& so): Script(so) {
    // Variables.
    StringVar x(*this);
    StringVar y(*this, 0, 5);
    StringVar z(*this, NSIntSet('a', 'c'), 0, 3);
    VarArgs va;
    extensional(*this, x, "(\'a\"|\'b\\*)(c\'|d\')", va);
    extensional(*this, y, "a0*b", va);
    extensional(*this, z, "a..*", va);

    StringVarArgs sva;
    sva << x << y << z << va.sva;
    IntVarArgs iva(va.iva);
    string_vars = StringVarArray(*this, sva);
    int_vars = IntVarArray(*this, iva);
    branch(*this, int_vars, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    sizemin_llul(*this, string_vars);
  }

  virtual void
  print(std::ostream& os) const {
    os << "x = " << string_vars[0] << '\n';
    os << "y = " << string_vars[1] << '\n';
    os << "z = " << string_vars[2] << '\n';
  os << "----------\n";
  ++N;
  }

};

int main() {
  StringOptions opt("*** Ex11 ***");
  opt.solutions(0);
  Script::run<Ex11, DFS, StringOptions>(opt);
  assert (N == 4 * 4 * 12);
  return 0;
}
