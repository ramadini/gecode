#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex2 : public Script {

  IntVarArray int_vars;
  StringVarArray str_vars;

public:

  static unsigned N;

  Ex2(Ex2& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    str_vars.update(*this, s.str_vars);
  }
  virtual Space* copy() {
    return new Ex2(*this);
  }

  Ex2(const StringOptions& so): Script(so) {
    // Variables.
    int N = DashedString::_MAX_STR_LENGTH;
    NSBlocks v(1, NSBlock(NSIntSet('a', 'c'), 0, N));
    StringVar x(*this, v, 0, N);
    IntVar l(*this, 2, 3);
    IntVarArgs iva;
    iva << l;
    int_vars = IntVarArray(*this, iva);
    StringVarArgs sva;
    sva << x;
    str_vars = StringVarArray(*this, sva);
    // Constraints.
    rel(*this, x, STRT_NQ, StringVar(*this, "aa"));
    rel(*this, x, STRT_NQ, StringVar(*this, "aaa"));
    length(*this, x, l);
    // Branching.
    blockmin_llll(*this, str_vars);
  }

  virtual void
  print(std::ostream& os) const {
    for (int i = 0; i < int_vars.size(); ++i)
      os << "int_var[" << i << "] = " << int_vars[i].val() << "\n";
    for (int i = 0; i < str_vars.size(); ++i)
      os << "string_var[" << i << "] = \"" << str_vars[i].val() << "\"\n";
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
