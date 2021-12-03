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

  static bool sat;

  Ex8(Ex8& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex8(*this);
  }

  Ex8(const StringOptions& so): Script(so) {
    // Variables.
    Block v[3]; int i = 0;
    v[i++].update(*this, Block(*this, CharSet(*this,'c', 'z'), 1, 5));
    v[i++].update(*this, Block(*this, CharSet(*this,'a'), 0, 3));
    v[i++].update(*this, Block(*this, CharSet(*this,'x', 'y'), 1, 2));
    Block v1[3]; i = 0;
    v1[i++].update(*this, Block(*this));
    v1[i++].update(*this, Block(*this, CharSet(*this,'b'), 1, MAX_STRING_LENGTH));
    v1[i++].update(*this, Block(*this));
    Block v2[2]; i = 0;
    v2[i++].update(*this, Block(*this, CharSet(*this,'a'), 5, 8));
    v2[i++].update(*this, Block(*this, CharSet(*this,'a', 'b'), 1, 1));
    StringVar x(*this, str2vec("aa"));
    StringVar x1(*this, str2vec("bb"));
    StringVar y(*this, DashedString(*this, v, 3));
    StringVar y1(*this, DashedString(*this, v1, 3));
    StringVar z(*this, DashedString(*this, v, 3));
    StringVar z1(*this, DashedString(*this, v, 3));
    StringVar s(*this, str2vec("xyxxyxxyx"));
    StringVar t(*this, DashedString(*this, v2, 2));
    StringVar t1(*this);    

    StringVarArgs sva;
    sva << x << x1 << y << y1 << z << z1 << s << t << t1;
    string_vars = StringVarArray(*this, sva);

    IntVar l(*this, 2, 3);
    IntVarArgs iva;
    iva << l;

    length(*this, z1, l);
    replace(*this, y, x, x1, y1);
    replace(*this, z, x, x1, z1);
    replace_all(*this, t, x, t1, s);
    dimdeg_lenblock_min_lllm(*this, string_vars);
    
    std::cout << "x: " << x << '\n';
    std::cout << "x1: " << x1 << '\n';
    std::cout << "y: " << y << '\n';
    std::cout << "y1: " << y1 << '\n';
    std::cout << "z: " << z << '\n';
    std::cout << "z1: " << z1 << '\n';
    std::cout << "s: " << s << '\n';
    std::cout << "t: " << t << '\n';
    std::cout << "t1: " << t1 << '\n';
  }

  virtual void
  print(std::ostream& os) const {
    std::string x = vec2str(string_vars[0].val()),
               x1 = vec2str(string_vars[1].val()),
               y  = vec2str(string_vars[2].val()),
               y1 = vec2str(string_vars[3].val()),
               z  = vec2str(string_vars[4].val()),
               z1 = vec2str(string_vars[5].val()),
               s  = vec2str(string_vars[6].val()),
               t  = vec2str(string_vars[7].val()),
               t1 = vec2str(string_vars[8].val());
    os << "x  = \"" <<  x << "\"\n"
       << "x1 = \"" << x1 << "\"\n"
       << "y  = \"" <<  y << "\"\n"
       << "y1 = \"" << y1 << "\"\n"
       << "z  = \"" <<  z << "\"\n"       
       << "z1 = \"" << z1 << "\"\n"
       << "s = \""  <<  s << "\"\n"
       << "t  = \"" <<  t << "\"\n"
       << "t1 = \"" << t1 << "\"\n"       
       << "----------\n";
    assert (y.find(x) && y1.find(x1) && z.find(x) && z1.find(x1) && 
            z == z1 && t1 == "xyx");
    sat = true;
  }

};

bool Ex8::sat = false;

int main() {
  StringOptions opt("*** Ex8 ***");
  opt.solutions(3);
  Script::run<Ex8, DFS, StringOptions>(opt);
  assert (Ex8::sat);
  return 0;
}
