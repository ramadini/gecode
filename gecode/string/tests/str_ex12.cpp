#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;

int n_sol = 0;

class StringOptions : public Options {

  public:

  StringOptions(const char* s): Options(s) { this->c_d(1); }

};

class Ex12 : public Script {

  StringVarArray string_vars;
  IntVarArray       int_vars;

public:

  Ex12(Ex12& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    string_vars.update(*this, s.string_vars);
  }
  virtual Space* copy() {
    return new Ex12(*this);
  }

  Ex12(const StringOptions& so): Script(so) {
    // Variables.
    StringVar x(*this);
    StringVar x1(*this);
    StringVar y(*this, "Hello world!!!");
    StringVar y1(*this);
    StringVar z(*this);
    StringVar z1(*this);

    StringVarArgs sva;
    sva << x << x1 << y << y1 << z << z1;
    string_vars = StringVarArray(*this, sva);

    IntVar lx(*this, 0, 2);
    IntVar ly1(*this, 0, y.val().size() - 3);
    IntVarArgs iva;
    iva << lx << ly1;
    int_vars = IntVarArray(*this, iva);

    replace_all(*this, x, x1, y, y1);
    replace_last(*this, StringVar(*this, "o"),  StringVar(*this, "O"), y1, z);
    replace_last(
      *this, z1, StringVar(*this, "*"), z, StringVar(*this, "H*o wOrd!!!")
    );
    length(*this, x, lx);
    length(*this, y1, ly1);
    
    blockmin_lllm(*this, string_vars);
    branch(*this, int_vars, INT_VAR_SIZE_MIN(), INT_VAL_SPLIT_MIN());
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
       << "|x| = " << int_vars[0].val() << "\n"
       << "|y1| = " << int_vars[1].val() << "\n"
       << "----------\n";
    assert (y.find(x) != string::npos && x1 == "" && z1 == "e");
    n_sol++;
  }

};

int main() {
  StringOptions opt("*** Ex12 ***");
  opt.solutions(0);
  Script::run<Ex12, DFS, StringOptions>(opt);
  assert (n_sol == 1);
  return 0;
}
