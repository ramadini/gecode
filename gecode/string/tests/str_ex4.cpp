#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex4 : public Script {

  StringVarArray string_vars;
  IntVarArray int_vars;

public:

  static std::set<string> solset;

  Ex4(Ex4& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex4(*this);
  }

  Ex4(const StringOptions& so): Script(so) {
    // Variables.
    NSBlocks v(NSBlocks(1, NSBlock(NSIntSet('0', '1'), 0, 3)));
    StringVar x(*this, v, 0, 3);
    StringVar y(*this);
    StringVarArgs sva;
    sva << x << y;
    string_vars = StringVarArray(*this, sva);
    IntVar j(*this, 2, 3);
    IntVarArgs iva;
    iva << j;
    int_vars = IntVarArray(*this, iva);

    substr(*this, x, IntVar(*this, 2, 2), j, y);
    rel(*this, y, STRT_REV, y);
    // Branching.
    blockmin_llll(*this, string_vars);
    branch(*this, int_vars, INT_VAR_NONE(), INT_VAL_MIN());
  }

  virtual void
  print(std::ostream& os) const {
    os   << "j = "   << int_vars[0]   .val() << " --- "
       << "x = \"" << string_vars[0].val() << "\" --- "
       << "y = \"" << string_vars[1].val() << "\"\n----------\n";
    solset.insert(string_vars[1].val());
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
