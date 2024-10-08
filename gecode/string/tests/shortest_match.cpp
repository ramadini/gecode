#include <gecode/string.hh>
#include <gecode/driver.hh>
#include <math.h>

using namespace Gecode;
using namespace String;

std::vector<string> PATTERNS({
  "(acc|rg|bc)", 
  "ea(c|d)*", 
  "dqdrd*", 
  "qqhx(ad|ew)*"
});

int N = -1;

class StringOptions : public Options {

public:

  bool newMatch;

  StringOptions(const char* s): Options(s), newMatch(false) {
      this->c_d(1);
  }

};

/*
* Given patterns r_1, ..., r_n, find the shortest superstring matching all of 
them, i.e., a minimal-length string w s.t. match(w, r_i) > 0 for i = 1, ..., n
*/
class MinMatchStr : public IntMinimizeScript {

  StringVarArray string_vars;
  IntVarArray int_vars;

public:


  MinMatchStr(MinMatchStr& s): IntMinimizeScript(s) {
    string_vars.update(*this, s.string_vars);
    int_vars.update(*this, s.int_vars);
  }
  virtual Space* copy() {
    return new MinMatchStr(*this);
  }

  MinMatchStr(const StringOptions& so): IntMinimizeScript(so) {
    // Variables.
    IntVarArgs iva;
    StringVarArgs sva;
    StringVar w(*this);
    sva << w;
    int n = DashedString::_MAX_STR_LENGTH;
    IntVar l(*this, 0, n);
    iva << l;
    length(*this, w, l);
    bool use_regular = false;
    std::cerr << "Patterns:\n";
    for (auto& ri : PATTERNS) {
      std::cerr << "\t" << ri << '\n';
      if (use_regular)
        extensional(*this, w, ".*(" + ri + ").*");
      else {
        IntVar i(*this, 1, n);
        iva << i;
        if (so.newMatch)
          match_new(*this, w, ri, i);
        else
          match(*this, w, ri, i);
      }
      //FIXME w ="dqdrgqqhxea"
//      rel(*this, l, IRT_EQ, 11);
//      rel(*this, StringVar(*this, "dqdrgqqhxea"), StringVar(*this), STRT_CAT, w);
    }
    
    int_vars = IntVarArray(*this, iva);
    string_vars = StringVarArray(*this, sva);
    IntVarArgs branch_vars = use_regular ? int_vars : int_vars.slice(1);
    branch_vars << l;
    branch(*this, branch_vars, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    blockmin_lllm(*this, sva);
  }

  virtual IntVar cost(void) const {
    return int_vars[0];
  }

  virtual void
  print(std::ostream& os) const {
    N = string_vars[0].min_length();
    os << "substring = " << string_vars[0] << " (length: " << N << ")\n";
  }

};

int main() {
  StringOptions opt("*** Shortest matching super-string ***");
  opt.solutions(0);
  Script::run<MinMatchStr, BAB, StringOptions>(opt);
  assert (N == 11);
  opt.newMatch = true;
  opt.solutions(0);
  Script::run<MinMatchStr, BAB, StringOptions>(opt);
  assert (N == 11);
  // FIXME: substring = Var. 0x5aa7f2fab970: "dqdrgqqhxea" [11..11] (length: 11)
  return 0;
}
