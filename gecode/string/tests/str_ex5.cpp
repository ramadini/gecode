#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex5 : public IntMinimizeScript {

  StringVarArray string_vars;
  IntVarArray int_vars;

public:

  static string S;

  Ex5(Ex5& s): IntMinimizeScript(s) {
    int_vars.update(*this, s.int_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex5(*this);
  }

  Ex5(const StringOptions& so): IntMinimizeScript(so) {
    // Variables.
    int N = DashedString::_MAX_STR_LENGTH;
    NSBlocks v(NSBlocks(1, NSBlock(NSIntSet('a', 'z'), 0, N)));
    StringVar x(*this, v, 0, N);
    StringVarArgs sva;
    sva << x;
    string_vars = StringVarArray(*this, sva);
    IntVar n(*this, 0, N);
    IntVarArgs iva;
    iva << n;
    int_vars = IntVarArray(*this, iva);
    length(*this, x, n);
    rel(*this, n % 2 == 1);
    rel(*this, x, STRT_REV, x);
    NSIntSet a('a'), b('b'), c('c');
    NSIntSet na('b', 'z');
    NSIntSet nb('c', 'z'); nb.add('a');
    NSIntSet nc('a', 'z'); nc.remove('c');
    NSBlocks va({
      NSBlock(na, 0, N), NSBlock(a, 1, 1), NSBlock(na, 0, N), NSBlock(a, 1, 1),
      NSBlock(na, 0, N)
    });
    NSBlocks vb({
      NSBlock(nb, 0, N), NSBlock(b, 1, 1), NSBlock(nb, 0, N), NSBlock(b, 1, 1),
      NSBlock(nb, 0, N)
    });
    NSBlocks vc({
      NSBlock(nc, 0, N), NSBlock(c, 1, 1), NSBlock(nc, 0, N), NSBlock(c, 1, 1),
      NSBlock(nc, 0, N)
    });
    rel(*this, x, STRT_DOM, va, 0, N);
    rel(*this, x, STRT_DOM, vb, 0, N);
    rel(*this, x, STRT_DOM, vc, 0, N);
    std::vector<string> d({"aabbccd", "abcdabc", "dcbaabc", "abcdcba"});
    extensional(*this, x, d);
    // Branching.
    blockmin_lllm(*this, string_vars);
  }

  virtual IntVar cost(void) const {
    return int_vars[0];
  }

  virtual void
  print(std::ostream& os) const {
    S = string_vars[0].val();
    os << "x = \"" << S << "\"\n";
  }

};

string Ex5::S = "";

int main() {
  StringOptions opt("*** Ex5 ***");
  opt.solutions(0);
  Script::run<Ex5, BAB, StringOptions>(opt);
  assert (Ex5::S == "abcdcba");
  return 0;
}
