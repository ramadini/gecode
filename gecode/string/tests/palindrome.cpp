#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) {
      //this->c_d(1);
  }

};

class Palindrome : public IntMinimizeScript {

  StringVarArray string_vars;
  IntVarArray int_vars;

public:

  static int N;
  static string S;

  Palindrome(Palindrome& s): IntMinimizeScript(s) {
    string_vars.update(*this, s.string_vars);
    int_vars.update(*this, s.int_vars);
  }
  virtual Space* copy() {
    return new Palindrome(*this);
  }

  Palindrome(const StringOptions& so): IntMinimizeScript(so) {
    // Variables.
    int M = DashedString::_MAX_STR_LENGTH;
    StringVar x(*this, 0, M);
    StringVarArgs sva;
    sva << x;
    string_vars = StringVarArray(*this, sva);
    IntVar l(*this, 0, M), m(*this, 0, M), n(*this, 0, M);
    IntVarArgs iva;
    iva << l << m << n;
    int_vars = IntVarArray(*this, iva);
    length(*this, x, l);
    IntArgs A;
    A << 'a' << 'b' << 'c';
    IntVarArgs N;
    N << n << n << n;
    // Constraints.
    NSBlocks d(1, NSBlock(NSIntSet('a', 'z'), 0, M));
    rel(*this, x, STRT_DOM, d, 0, M);
    rel(*this, n > 0);
    rel(*this, l % 2 == 1);
    rel(*this, x, STRT_REV, x);
    gcc(*this, x, A, N);

    // Symmetry breaking.
    bool symm = true;
    if (symm) {
      StringVar y(*this);
      sva << y;
      length(*this, y, m);
      rel(*this, m == l/2 - 1);
      rel(*this, y, STRT_INCLT);
      substr(*this, x, IntVar(*this, 0, 0), m, y);
    }
    // Branching.
    sizemin_llul(*this, string_vars);
  }

  virtual IntVar cost(void) const {
    return int_vars[0];
  }

  virtual void
  print(std::ostream& os) const {
  S = string_vars[0].val();
  N = int_vars[2].val();
    os << "x = \"" << S << "\" --- |x| = " << int_vars[0].val()
       << " --- n = " << N << '\n' ;
  }

};

int    Palindrome::N = 0;
string Palindrome::S = "";

int main() {
  StringOptions opt("*** Palindrome ***");
  opt.solutions(0);
  Script::run<Palindrome, BAB, StringOptions>(opt);
  assert (Palindrome::S == "abcdcba" && Palindrome::N == 2);
  return 0;
}
