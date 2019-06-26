#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

  public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex8 : public Script {

  StringVarArray string_vars;
  IntVarArray       int_vars;

public:

  Ex8(Ex8& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex8(*this);
  }

  Ex8(const StringOptions& so): Script(so) {
    // Variables.
  NSBlocks v({
    NSBlock(NSIntSet('c', 'z'), 1, 5),
    NSBlock(NSIntSet('a'), 0, 3),
      NSBlock(NSIntSet('x', 'y'), 1, 2),
  });
  NSBlocks v1({
    NSBlock::top(),
    NSBlock(NSIntSet('b'), 1, DashedString::_MAX_STR_LENGTH),
    NSBlock::top()
  });
  StringVar x(*this, "aa");
  StringVar x1(*this, "bb");
  StringVar y(*this, v, 0, 100);
  StringVar y1(*this, v1, 0, 100);
  StringVar z(*this, v, 0, 100);
    StringVar z1(*this, v, 0, 100);

  StringVarArgs sva;
  sva << x << x1 << y << y1 << z << z1;
  string_vars = StringVarArray(*this, sva);

  IntVar l(*this, 2, 3);
  IntVarArgs iva;
  iva << l;

  length(*this, z1, l);
  VarArgs va;
  replace(*this, x, x1, y, y1, va);
  replace(*this, x, x1, z, z1, va);
  sizemin_llul(*this, string_vars);
  }

  virtual void
  print(std::ostream& os) const {
    string x  = string_vars[0].val(),
           x1 = string_vars[1].val(),
       y  = string_vars[2].val(),
       y1 = string_vars[3].val(),
       z  = string_vars[4].val(),
       z1 = string_vars[5].val();
    os << "x  = \"" << x  << "\"\n"
       << "x1 = \"" << x1 << "\"\n"
     << "y  = \"" << y  << "\"\n"
     << "y1 = \"" << y1 << "\"\n"
       << "z  = \"" << z  << "\"\n"
       << "z1 = \"" << z1 << "\"\n"
     << "----------\n";
    assert (y.find(x) != string::npos && y1.find(x1) != string::npos &&
      z.find(x) == string::npos && z1.find(x1) == string::npos &&
      z == z1);
  }

};

int main() {
  StringOptions opt("*** Ex8 ***");
  opt.solutions(3);
  Script::run<Ex8, DFS, StringOptions>(opt);
  return 0;
}
