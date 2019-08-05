#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

  public:

    StringOptions(const char* s): Options(s) { }

};

class Ex10 : public Script {

  StringVarArray string_vars;

public:

  static bool unsat;

  Ex10(Ex10& s): Script(s) {
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex10(*this);
  }

  Ex10(const StringOptions& so): Script(so) {
    // Variables.
    StringVar x(*this, NSIntSet('A', 'B'), 0, 10000);
    StringVar y(*this, NSIntSet('A', 'B'), 0, 10000);
    StringVarArgs e1, e2;
    e1 << x << StringVar(*this, "A") << x;
    e2 << x << StringVar(*this, "B") << x;
    gconcat(*this, e1, y);
    gconcat(*this, e2, y);
    StringVarArgs sva;
    sva << x << y;
    string_vars = StringVarArray(*this, sva);
    blockmin_lllm(*this, string_vars);

  }

  virtual void
  print(std::ostream& os) const {
    unsat = false;
  }

};

bool Ex10::unsat = true;

int main() {
  StringOptions opt("*** Ex10 ***");
  opt.solutions(1);
  Script::run<Ex10, DFS, StringOptions>(opt);
  assert (Ex10::unsat);
  return 0;
}
