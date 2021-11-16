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

  static bool sat;

  Ex7(Ex7& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex7(*this);
  }

  Ex7(const StringOptions& so): Script(so) {
    // Variables.
    Block vx[4];
    vx[0].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 2));
    vx[1].update(*this, Block(*this, CharSet(*this, 'x', 'y'), 2, 3));
    vx[2].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 2));
    vx[3].update(*this, Block(*this, CharSet(*this, 'b', 'd'), 0, 4));
    Block vy[6];
    vy[0].update(*this, Block(*this, CharSet(*this, 'd', 'e'), 0, 2));
    vy[1].update(*this, Block(*this, CharSet(*this, 'c'), 1, 3));
    vy[2].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 3, 5));
    vy[3].update(*this, Block(*this, CharSet(*this, 'h', 'n'), 0, 2));
    vy[4].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 0, 1));
    vy[5].update(*this, Block(*this, CharSet(*this, 'g'), 0, 3));
    
    StringVar x1(*this, DashedString(*this, vx, 4)), 
              x2(*this, DashedString(*this, vx, 4)),
              y1(*this, DashedString(*this, vy, 6)), 
              y2(*this, DashedString(*this, vy, 6));
    StringVarArgs sva;
    sva << x1 << y1 << x2 << y2;
    string_vars = StringVarArray(*this, sva);
    IntVarArgs iva;
    IntVar n1(*this, -50, 4), n2(*this, 1, 100);
    iva << n1 << n2;
    int_vars = IntVarArray(*this, iva);
    find(*this, x1, y1, n1);
    find(*this, x2, y2, n2);
//    // Branching.
    lenblock_min_lllm(*this, string_vars);
  }

  virtual void
  print(std::ostream& os) const {
    sat = true;
    std::string x1 = vec2str(string_vars[0].val()), 
                y1 = vec2str(string_vars[1].val()),
                x2 = vec2str(string_vars[2].val()),
                y2 = vec2str(string_vars[3].val());
    int n1 = int_vars[0].val(), n2 = int_vars[1].val();
    os << "n1 = "   << n1 << "\n"
       << "x1 = \"" << x1 << "\"\n"
       << "y1 = \"" << y1 << "\"\n"
       << "n2 = "   << n2 << "\n"
       << "x2 = \"" << x2 << "\"\n"
       << "y2 = \"" << y2 << "\"\n----------\n";
    assert (n1 == 0 && n2 == 5);
    assert (x1.find(y1) == std::string::npos && y2 == "cbbb");
  }

};

bool Ex7::sat = false;

int main() {
  StringOptions opt("*** Ex7 ***");
  opt.solutions(5);
  Script::run<Ex7, DFS, StringOptions>(opt);
  assert (Ex7::sat);
  return 0;
}
