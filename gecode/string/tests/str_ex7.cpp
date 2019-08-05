#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex7 : public Script {

  StringVarArray string_vars;
  IntVarArray       int_vars;

public:

  Ex7(Ex7& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex7(*this);
  }

  Ex7(const StringOptions& so): Script(so) {
    // Variables.
    NSBlocks vx({
      NSBlock(NSIntSet('d', 'e'), 0, 2),
      NSBlock(NSIntSet('c'), 1, 3),
      NSBlock(NSIntSet('a', 'b'), 3, 5),
      NSBlock(NSIntSet('h', 'n'), 0, 2),
      NSBlock(NSIntSet('a', 'b'), 0, 1),
      NSBlock(NSIntSet('g'), 0, 3),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'b'), 1, 2),
      NSBlock(NSIntSet('x', 'y'), 2, 3),
      NSBlock(NSIntSet('a', 'b'), 1, 2),
      NSBlock(NSIntSet('b', 'd'), 0, 4),
    });
    StringVar x1(*this, vx, 0, 100), x2(*this, vx, 0, 100),
              y1(*this, vy, 0, 100), y2(*this, vy, 0, 100);
    StringVarArgs sva;
    sva << x1 << y1 << x2 << y2;
    string_vars = StringVarArray(*this, sva);
    IntVarArgs iva;
    IntVar n1(*this, -50, 4), n2(*this, 1, 100);
    iva << n1 << n2;
    int_vars = IntVarArray(*this, iva);
    find(*this, x1, y1, n1);
    find(*this, x2, y2, n2);
    // Branching.
    blockmin_lllm(*this, string_vars);
  }

  virtual void
  print(std::ostream& os) const {
    string x1 = string_vars[0].val(), y1 = string_vars[1].val(),
           x2 = string_vars[2].val(), y2 = string_vars[3].val();
    int n1 = int_vars[0].val(), n2 = int_vars[1].val();
    os << "n1 = "   << n1 << "\n"
       << "x1 = \"" << x1 << "\"\n"
       << "y1 = \"" << y1 << "\"\n"
       << "n2 = "   << n2 << "\n"
       << "x2 = \"" << x2 << "\"\n"
       << "y2 = \"" << y2 << "\"\n----------\n";
    assert (n1 == 0 && n2 == 5);
    assert (y1.find(x1) == string::npos && x2 == "cbbb");
  }

};

int main() {
  StringOptions opt("*** Ex7 ***");
  opt.solutions(5);
  Script::run<Ex7, DFS, StringOptions>(opt);
  return 0;
}
