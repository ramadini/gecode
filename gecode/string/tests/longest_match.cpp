#include <gecode/string.hh>
#include <gecode/driver.hh>
#include <math.h>

using namespace Gecode;
using namespace String;

string PATTERN = "url\\(.*owamail.*\\)";
string TARGET = "src:local('Segoe UI Semilight'),local('Segoe WP Semilight'),url(//res.cdn.office.net/assets/mail/fonts/v1/fonts/segoeui-semilight.eot?#iefix) format('embedded-opentype'),url(//res.cdn.office.net/assets/mail/fonts/v1/fonts/segoeui-semilight.woff) format('woff'),url(//res.cdn.office.net/assets/mail/fonts/v1/fonts/segoeui-semilight.ttf) format('truetype');font-weight:200;font-style:normal}@font-face{font-family:FabricMDL2Icons;src:url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/o365icons-mdl2.woff') format('woff'),url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/o365icons-mdl2.ttf') format('truetype');font-weight:400;font-style:normal}@font-face{font-family:office365icons;src:url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/office365icons.eot?');src:url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/office365icons.eot?#iefix') format('embedded-opentype'),url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/office365icons.woff?') format('woff'),url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/office365icons.ttf?') format('truetype'),url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/office365icons.svg?#office365icons') format('svg');font-weight:400;font-style:normal}#preloadDiv{height:1px;margin-bottom:-1px;overflow:hidden;visibility:hidden}#loadingScreen{position:fixed;top:0;bottom:0;left:0;right:0;background-color:#fff}#loadingLogo{position:fixed;top:calc(50vh - 90px);left:calc(50vw - 90px);width:180px;height:180px}#MSLogo{position:fixed;bottom:36px;left:calc(50vw - 50px)}.dark #loadingScreen{background-color:#333}</style>";
int N = -1;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) {
      this->c_d(1);
  }

};

/*
Given a target string and a pattern, find the longest repeating substring 
matching the pattern in the target. We require the substring to be non-empty and
non-overlapping:
  max(k − i) s.t.
  i = match(w, ρ)
  j = match(w[i + 1..|w|], ρ) + i
  0 < i ≤ k < j ≤ |w|
  w[i..k] = w[j..j + k − i]
*/
class MaxMatchStr : public IntMaximizeScript {

  StringVarArray string_vars;
  IntVarArray int_vars;

public:


  MaxMatchStr(MaxMatchStr& s): IntMaximizeScript(s) {
    string_vars.update(*this, s.string_vars);
    int_vars.update(*this, s.int_vars);
  }
  virtual Space* copy() {
    return new MaxMatchStr(*this);
  }

  MaxMatchStr(const StringOptions& so): IntMaximizeScript(so) {
    IntVarArgs iva;
    StringVarArgs sva;
    int n = TARGET.size();
    IntVar i(*this, 1, n);    
    IntVar j(*this, 2, n);
    IntVar k(*this, 1, n);    
    IntVar i1(*this, 2, n+1);
    IntVar j1(*this, 2, n+1);
    IntVar k1(*this, 1, n);
    IntVar obj(*this, 1-n , n-1);
    iva << obj << i << j << k << i1 << j1 << k1;
    StringVar s1(*this);
    StringVar s2(*this);
    StringVar target(*this, TARGET);
    sva << s1 << s2;
    
    rel(*this, k < j);
    rel(*this, i <= k);
    match(*this, target, PATTERN, i);
    
rel(*this, i == 433);    
    
    rel(*this, i1 == i + 1);
    substr(*this, target, i1, IntVar(*this, n, n), s1);
    match(*this, s1, PATTERN, k1);
    rel(*this, j == k1 + i);
    
    substr(*this, target, i, k, s2);
    rel(*this, j1 == j + k - i);
    substr(*this, target, j, j1, s2);
    
    rel(*this, obj == k - i);
    
    int_vars = IntVarArray(*this, iva);
    string_vars = StringVarArray(*this, sva);
    blockmin_lllm(*this, sva);
  }

  virtual IntVar cost(void) const {
    return int_vars[0];
  }

  virtual void
  print(std::ostream& os) const {
    N = string_vars[1].min_length();
    os << "i = "   << int_vars[1] 
       << ", j = " << int_vars[2]
       << ", k = " << int_vars[3] << "\n";
    os << "substring = " << string_vars[1] << " (length: " << N << ")\n";
    os << "----------\n";
  }

};

int main() {
  StringOptions opt("*** Longest repeating substring ***");
  opt.solutions(0);
  Script::run<MaxMatchStr, BAB, StringOptions>(opt);
  assert (N == 80);
  return 0;
}
