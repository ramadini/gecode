#include <gecode/string.hh>
#include <gecode/driver.hh>
#include <math.h>

using namespace Gecode;
using namespace String;

string PATTERN = "url\\(.*owamail.*\\)";
string TARGET = "src:local('Segoe UI Semilight'),local('Segoe WP Semilight'),url(//res.cdn.office.net/assets/mail/fonts/v1/fonts/segoeui-semilight.eot?#iefix) format('embedded-opentype'),url(//res.cdn.office.net/assets/mail/fonts/v1/fonts/segoeui-semilight.woff) format('woff'),url(//res.cdn.office.net/assets/mail/fonts/v1/fonts/segoeui-semilight.ttf) format('truetype');font-weight:200;font-style:normal}@font-face{font-family:FabricMDL2Icons;src:url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/o365icons-mdl2.woff') format('woff'),url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/o365icons-mdl2.ttf') format('truetype');font-weight:400;font-style:normal}@font-face{font-family:office365icons;src:url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/office365icons.eot?');src:url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/office365icons.eot?#iefix') format('embedded-opentype'),url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/office365icons.woff?') format('woff'),url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/office365icons.ttf?') format('truetype'),url('//res.cdn.office.net/owamail/20221021015.07/resources/fonts/office365icons.svg?#office365icons') format('svg');font-weight:400;font-style:normal}#preloadDiv{height:1px;margin-bottom:-1px;overflow:hidden;visibility:hidden}#loadingScreen{position:fixed;top:0;bottom:0;left:0;right:0;background-color:#fff}#loadingLogo{position:fixed;top:calc(50vh - 90px);left:calc(50vw - 90px);width:180px;height:180px}#MSLogo{position:fixed;bottom:36px;left:calc(50vw - 50px)}.dark #loadingScreen{background-color:#333}</style>";
int N = 4;
int L = -1;

class StringOptions : public Options {

public:

  StringOptions(const char* s): Options(s) {
      this->c_d(1);
  }

};

/*
% Given a target string, a pattern and an integer N, find the longest repeating
% and non-empty string matching the pattern at occurring and least N times.
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
    IntVar len(*this, 1, n);
    iva << len;
    StringVar sub(*this);
    StringVar target(*this, TARGET);
    sva << sub;
    
    IntVarArray start_idx(*this, N+1, 0, n);
    rel(*this, start_idx[0] == 0);
    for (int i = 1; i <= N; ++i) {      
      IntVar idx0(*this, 1, n);
      rel(*this, idx0 == start_idx[i-1] + 1);
      StringVar sub_i(*this);
      substr(*this, target, idx0, IntVar(*this, n, n), sub_i);
      
      IntVar idx_i(*this, 1, n);
      match(*this, sub_i, PATTERN, idx_i);
      rel(*this, start_idx[i] == idx_i + start_idx[i-1]);      
      
      IntVar idx1(*this, 1, n);
      rel(*this, idx1 == start_idx[i] + len - 1);
      substr(*this, target, start_idx[i], idx1, sub);
      iva << start_idx[i];
    }
    distinct(*this, start_idx);
    rel(*this, start_idx, IRT_LE);
    
    int_vars = IntVarArray(*this, iva);
    string_vars = StringVarArray(*this, sva);
    branch(*this, start_idx, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    blockmin_lllm(*this, sva);
  }

  virtual IntVar cost(void) const {
    return int_vars[0];
  }

  virtual void
  print(std::ostream& os) const {
    L = int_vars[0].val();
    os << "substring = " << string_vars[0].val() << " (length: " << N << ")\n";
  }

};

int main() {
  StringOptions opt("*** Longest repeating substring ***");
  opt.solutions(0);
  Script::run<MaxMatchStr, BAB, StringOptions>(opt);
  assert (L == 70);
  return 0;
}
