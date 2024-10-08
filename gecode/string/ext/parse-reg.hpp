#ifndef __GECODE_STRING_PARSE_REG__
#define __GECODE_STRING_PARSE_REG__

/*
 * Parsing if an expression is generated by the grammar:
 *
 * <regex>  ::= (<term> '|' <regex>) | (<term> '&' <regex>) | <term>
 *
 * <term>   ::= { <factor> }
 *
 * <factor> ::= <base> { '*' }
 *
 * <base>   ::= '.' | '\' <char> | <char> | '[' <char> '-' <char> ']'
 *            | '[' <char> '-' <char> ',' int ']' |
 *            | '[' <char> '-' <char> ',' int, ',' int ']' |
 *            | '(' <regex> ')'
 *
 *
 * (from http://matt.might.net/articles/parsing-regex-with-recursive-descent/).
 *
 * The \ symbol is used to escape characters '&' '|' '*' '[' ']' '(' ')' '\' '"'
 *
*/

#include <gecode/minimodel.hh>
#include <gecode/flatzinc/parser.hh>

using namespace Gecode::FlatZinc;

namespace Gecode { namespace String {

  class RegEx {
  public:
    virtual bool decomp()     const { return false; }
    virtual bool empty()      const { return false; }
    virtual bool has_union()  const { return false; }
    virtual bool has_concat() const { return false; }
    virtual bool has_star()   const { return false; }
    virtual bool has_empty()  const { return false; }
    virtual bool intro_vars() const { return false; }
    //FIXME: post and add only work in a positive context.
    virtual void post(Home, StringVar, VarArgs&)    const = 0;
    virtual void add(AST::StringVar*, ParserState*) const = 0;
    virtual string str() const = 0;
    virtual REG reg()    const = 0;
    DFA dfa() const {
      try {
        return DFA(this->reg());
      }
      catch (const MiniModel::TooFewArguments&) {
        DFA::Transition t[1] = { DFA::Transition(-1, -1, -1) };
        int f[1] = {-1};
        return DFA(0, t, f);
      };
    }
    virtual string get_string()  const = 0;
    virtual NSIntSet get_chars() const = 0;
    virtual ~RegEx() {};
  };

  class EmptyEx: public RegEx {
  public:
    string str() const { return "eps"; }
    bool empty() const { return true;  }
    bool has_empty() const { return true; }
    bool decomp() const { return true;  }
    string get_string() const { return ""; }
    NSIntSet get_chars() const { return NSIntSet(); }
    void
    post(Home home, StringVar x, VarArgs&) const {
      GECODE_POST;
      if (x.min_length() > 0)
        home.fail();
      else
        rel(home, x, STRT_EQ, StringVar(home, ""));
    }
    void
    add(AST::StringVar* x, ParserState* pp) const {
      AST::Array* ce = new AST::Array(2);
      ce->a[0] = new AST::StringVar(x->i);
      ce->a[1] = new AST::StringDom("");
      pp->constraints.push_back(new ConExpr("str_eq", ce, NULL));
    }
    REG reg() const { return REG(); }
    ~EmptyEx() {}
  };

  class ConcatEx: public RegEx {
  private:
    RegEx* lhs;
    RegEx* rhs;
  public:
    ConcatEx(RegEx* x, RegEx* y): lhs(x), rhs(y) {}
    string str() const { return "(" + lhs->str() + " ++ " + rhs->str() + ")"; }
    bool decomp() const { return lhs->decomp() && rhs->decomp(); }
    bool empty() const { return lhs->empty() && rhs->empty(); }
    bool has_empty() const { return lhs->has_empty() && rhs->has_empty(); }
    bool has_star() const { return lhs->has_star() || rhs->has_star(); }
    bool has_union() const {
      if (lhs->empty())
        return rhs->has_union();
      else if (rhs->empty())
        return lhs->has_union();
      else
      return lhs->has_union() || rhs->has_union();
    }
    bool has_concat() const {
      if (lhs->empty())
        return rhs->has_concat();
      else if (rhs->empty())
        return lhs->has_concat();
      else
        return true;
    }
    bool intro_vars() const { return !empty(); }
    string get_string() const { return lhs->get_string() + rhs->get_string(); }
    NSIntSet
    get_chars() const {
      NSIntSet s(lhs->get_chars());
      s.include(rhs->get_chars());
      return s;
    }
    void
    post(Home home, StringVar x, VarArgs& va) const {
      GECODE_POST;
      // std::cerr << "ConcatEx::post " << x << "\n";
      if (lhs->empty()) {
        if (rhs->empty()) {
          if (x.min_length() > 0)
            home.fail();
          else
            rel(home, x, STRT_EQ, StringVar(home, ""));
          return;
        }
        rhs->post(home, x, va);
      }
      else {
        if (rhs->empty())
          lhs->post(home, x, va);
        else {
          StringVar x1(home), x2(home);
          // std::cerr << "Posting " << x << " = " << x1 << " + " << x2 << "\n";
          lhs->post(home, x1, va);
          rhs->post(home, x2, va);
          rel(home, x1, x2, STRT_CAT, x);
          va.sva << x1 << x2;
        }
      }
    }
    void
    add(AST::StringVar* x, ParserState* pp) const {
      // std::cerr << "ConcatEx::add Var" << x->i << " = " << lhs->str()
      //   << " ++ " <<  rhs->str() << "\n";
      if (lhs->empty()) {
        if (rhs->empty()) {
          AST::Array* ce = new AST::Array(2);
          ce->a[0] = new AST::StringVar(x->i);
          ce->a[1] = new AST::StringDom("");
          pp->constraints.push_back(new ConExpr("str_eq", ce, NULL));
          return;
        }
        rhs->add(x, pp);
      }
      else {
        if (rhs->empty())
          lhs->add(x, pp);
        else {
          AST::StringVar* x1 = new AST::StringVar(pp->stringvars.size());
          pp->stringvars.push_back(varspec("", new StringVarSpec(
            Option<AST::StringDom*>::some(new AST::StringDom()), false, false
          )));
          AST::StringVar* x2 = new AST::StringVar(pp->stringvars.size());
          pp->stringvars.push_back(varspec("", new StringVarSpec(
            Option<AST::StringDom*>::some(new AST::StringDom()), false, false
          )));
          lhs->add(x1, pp);
          rhs->add(x2, pp);
          AST::Array* ce = new AST::Array(3);
          ce->a[0] = x1;
          ce->a[1] = x2;
          ce->a[2] = new AST::StringVar(x->i);
          pp->constraints.push_back(new ConExpr("str_concat", ce, NULL));
        }
      }
    }
    REG reg() const {
      if (lhs->empty()) {
        if (rhs->empty())
          return REG();
        else
          return rhs->reg();
      }
      else {
        if (rhs->empty())
        return lhs->reg();
      else
        return lhs->reg() + rhs->reg();
      }
    }
    ~ConcatEx() { delete lhs; delete rhs; }
  };

  class InterEx: public RegEx {
    private:
      RegEx* lhs;
      RegEx* rhs;
    public:
      InterEx(RegEx* x, RegEx* y): lhs(x), rhs(y) {}
      string str() const { return "(" + lhs->str() + " & " + rhs->str() + ")"; }
      bool decomp() const { return lhs->decomp() && rhs->decomp(); }
      bool empty() const { return lhs->empty() && rhs->empty(); }
      bool has_empty() const { return lhs->has_empty() && rhs->has_empty(); }
      bool has_star() const { return lhs->has_star() || rhs->has_star(); }
      bool has_union() const { return lhs->has_union() || rhs->has_union(); }
      bool has_concat() const { return lhs->has_concat() || rhs->has_concat(); }
      bool intro_vars() const { return lhs->intro_vars() || rhs->intro_vars(); }
      string get_string() const {
      	try {
      	  string x = lhs->get_string(), y = rhs->get_string();
      	  if (x == y)
      	    return x;
            std::cerr << "Can't get a string from empty regex!\n";
            throw 0;
      	}
      	catch (const std::runtime_error& e) {
      	  throw std::runtime_error("Can't get a string from <E & E'> regex!");
      	}
      }
      NSIntSet
      get_chars() const {
        NSIntSet s(lhs->get_chars());
        s.intersect(rhs->get_chars());
        return s;
      }
      void
      post(Home home, StringVar x, VarArgs& va) const {
        GECODE_POST;
        if (lhs->empty() && rhs->empty()) {
            if (x.min_length() > 0)
              home.fail();
            else
              rel(home, x, STRT_EQ, StringVar(home, ""));
            return;
        }
        lhs->post(home, x, va);
        rhs->post(home, x, va);
      }
      void
      add(AST::StringVar* x, ParserState* pp) const {
        if (lhs->empty() && rhs->empty()) {
          AST::Array* ce = new AST::Array(2);
          ce->a[0] = new AST::StringVar(x->i);
          ce->a[1] = new AST::StringDom("");
          pp->constraints.push_back(new ConExpr("str_eq", ce, NULL));
          return;
        }
        lhs->add(x, pp);
        rhs->add(x, pp);
      }
      REG reg() const {
        try {
          if (lhs->get_string() == rhs->get_string())
            return lhs->reg();
        }
        catch (int) {
          // FIXME: Workaround for empty regular expression.
          return REG(IntArgs::create(0, 1));
        }
        catch (...) {
          std::cerr << "REG intersection not implemented!\n";
          throw 1;
        }
        GECODE_NEVER;
        return REG();
      }
      ~InterEx() { delete lhs; delete rhs; }
  };

  class UnionEx: public RegEx {
  private:
    RegEx* lhs;
    RegEx* rhs;
  public:
    UnionEx(RegEx* x, RegEx* y): lhs(x), rhs(y) {}
    string str() const { return "(" + lhs->str() + " | " + rhs->str() + ")"; }
    bool decomp()  const { return lhs->decomp() && rhs->decomp(); }
    bool empty() const { return lhs->empty() && rhs->empty(); }
    bool has_empty() const { return lhs->has_empty() || rhs->has_empty(); }
    bool has_star() const { return lhs->has_star() || rhs->has_star(); }
    bool has_union()  const { return !empty(); }
    bool intro_vars() const { return !empty(); }
    bool has_concat()  const {
      if (lhs->empty())
        return rhs->has_concat();
      else if (rhs->empty())
        return lhs->has_concat();
      else
        return lhs->has_concat() || rhs->has_concat();
    }
    string
    get_string() const {
      if (lhs->empty())
        return rhs->get_string();
      if (rhs->empty())
        return lhs->get_string();
      throw std::runtime_error("Can't get a string from <E | E'> regex.\n");
    }
    NSIntSet
    get_chars() const {
      NSIntSet s(lhs->get_chars());
      s.include(rhs->get_chars());
      return s;
    }
    void
    post(Home home, StringVar x, VarArgs& va) const {
      GECODE_POST;
      if (this->empty()) {
    	if (x.min_length() > 0)
    	  home.fail();
    	else
          rel(home, x, STRT_EQ, StringVar(home, ""));
        return;
      }
      int lx = x.min_length(), ux = x.max_length();
      NSIntSet Sx = x.may_chars();
      StringVar x1(home, Sx, lx, ux), x2(home, Sx, lx, ux);
      StringVarArgs a;
      a << x1 << x2;
      IntVar i(home, 1, 2);
      element(home, a, i, x);
      lhs->post(home, x1, va);
      rhs->post(home, x2, va);
      va.iva << i;
      va.sva << x1 << x2;
    }
    void
    add(AST::StringVar* x, ParserState* pp) const {
      if (this->empty()) {
        AST::Array* ce = new AST::Array(2);
        ce->a[0] = new AST::StringVar(x->i);
        ce->a[1] = new AST::StringDom("");
        pp->constraints.push_back(new ConExpr("str_eq", ce, NULL));
        return;
      }
      AST::StringVar* x1 = new AST::StringVar(pp->stringvars.size());
      pp->stringvars.push_back(varspec("", new StringVarSpec(
        Option<AST::StringDom*>::some(new AST::StringDom()), false, false
      )));
      AST::StringVar* x2 = new AST::StringVar(pp->stringvars.size());
      pp->stringvars.push_back(varspec("", new StringVarSpec(
        Option<AST::StringDom*>::some(new AST::StringDom()), false, false
      )));
      lhs->add(x1, pp);
      rhs->add(x2, pp);
      AST::Array* a = new AST::Array(2);
      a->a[0] = x1;
      a->a[1] = x2;
      AST::IntVar* i = new AST::IntVar(pp->intvars.size());
      pp->intvars.push_back(varspec("", new IntVarSpec(
        Option<AST::SetLit*>::some(new AST::SetLit(1, 2)), false, false
      )));
      AST::Array* ce = new AST::Array(3);
      ce->a[0] = i;
      ce->a[1] = a;
      ce->a[2] = new AST::StringVar(x->i);
      pp->constraints.push_back(
        new ConExpr("array_var_string_element", ce, NULL)
      );
    }
    REG reg() const { return lhs->reg() | rhs->reg(); }
    ~UnionEx() { delete lhs; delete rhs; }
  };

  class StarEx: public RegEx {
  private:
    RegEx* base;
  public:
    StarEx(RegEx* b): base(b) {};
    string str() const { return "(" + base->str() + ")^*"; }
    bool decomp() const {
      return !base->has_star() && !(has_union() && has_concat());
    }
    bool empty() const { return base->empty(); }
    bool has_empty() const { return true; }
    bool has_star() const { return !empty(); }
    bool has_union() const { return base->has_union(); }
    bool has_concat() const { return base->has_concat(); }
    bool intro_vars() const {
      return this->decomp() && DashedString::_DECOMP_REGEX && base->has_concat();
    }
    string get_string() const {
      if (base->empty())
        return "";
      throw std::runtime_error("Can't get a string from <E*> regex.\n");
    }
    NSIntSet get_chars() const {
      return base->get_chars();
    }
    void
    post(Home home, StringVar x, VarArgs& va) const {
      GECODE_POST;
      if (this->decomp() && DashedString::_DECOMP_REGEX) {
        int l = DashedString::_MAX_STR_LENGTH;
        if (base->has_concat()) {
          string s = base->get_string();
          IntVar n(home, 0, l);
          pow(home, StringVar(home, s), n, x);
          va.iva << n;
        }
        else {
          DashedString* p = ((StringView) x).pdomain();
          const NSIntSet s(base->get_chars());
          bool norm = false;
          for (int i = 0; i < p->length(); ++i) {
            NSIntSet si(p->at(i).S);
            if (s.disjoint(si)) {
              if (x.min_length() > 0)
                home.fail();
              else
                rel(home, x, STRT_EQ, StringVar(home, ""));
              return;
            }
            int n = si.size();
            si.intersect(s);
            if (si.size() < n) {
              p->at(i).S.update(home, si);
              norm = true;
            }
            p->at(i).u = x.max_length();
          }
          if (norm)
            p->normalize(home);
        }
        return;
      }
      extensional(home, x, dfa());
    }
    void
    add(AST::StringVar* x, ParserState* pp) const {
      if (decomp() && DashedString::_DECOMP_REGEX) {
        if (base->has_concat()) {
          string s = base->get_string();
          AST::IntVar* n = new AST::IntVar(pp->intvars.size());
          pp->intvars.push_back(varspec("",
            new IntVarSpec(Option<AST::SetLit*>::some(new AST::SetLit(
              0, Gecode::String::DashedString::_MAX_STR_LENGTH)), false, false
          )));
          AST::StringVar* z = new AST::StringVar(pp->stringvars.size());
          pp->stringvars.push_back(varspec("", new StringVarSpec(
            Option<AST::StringDom*>::some(new AST::StringDom(s)), false, false
          )));
          AST::Array* ce = new AST::Array(3);
          ce->a[0] = z;
          ce->a[1] = n;
          ce->a[2] = new AST::StringVar(x->i);
          pp->constraints.push_back(new ConExpr("str_pow", ce, NULL));
        }
        else {
          AST::Array* ce = new AST::Array(2);
          ce->a[0] = new AST::StringVar(x->i);
          ce->a[1] = new AST::StringDom(base->get_chars());
          pp->constraints.push_back(new ConExpr("str_chars", ce, NULL));
        }
          return;
      }
      AST::Array* ce = new AST::Array(2);
      ce->a[0] = new AST::StringVar(x->i);
      ce->a[1] = new AST::DFA(dfa());
      pp->constraints.push_back(new ConExpr("str_dfa", ce, NULL));
    }
    REG reg() const { return *(base->reg()); }
    ~StarEx() { delete base; }
  };

  class CharEx: public RegEx {
  private:
    int i;
  public:
    CharEx(int x): i(x) {};
    CharEx(char c): i(char2int(c)) {};
    string str() const { return "'" + string(1, int2char(i)) + "'"; }
    bool decomp() const { return true; }
    string get_string() const { return string(1, int2char(i)); }
    NSIntSet get_chars() const { return NSIntSet(i); }
    void
    post(Home home, StringVar x, VarArgs&) const {
      GECODE_POST;
      if (x.may_chars().contains(i))
        rel(home, x, STRT_EQ, StringVar(home, string(1, int2char(i))));
      else
        home.fail();
    }
    void
    add(AST::StringVar* x, ParserState* pp) const {
      AST::Array* ce = new AST::Array(2);
      ce->a[0] = new AST::StringVar(x->i);
      ce->a[1] = new AST::StringDom(get_string());
      pp->constraints.push_back(new ConExpr("str_eq", ce, NULL));
    }
    REG reg() const { return REG(i); }
  };

  class RangeEx: public RegEx {
  private:
    int minS, maxS, l, u;
    const int _MAX_SIZE = 500;
  public:
    RangeEx(char a, char b): minS(char2int(a)), maxS(char2int(b)), l(1), u(1) {};
    RangeEx(char a, char b, int i): minS(char2int(a)), maxS(char2int(b)), l(i),
      u(DashedString::_MAX_STR_LENGTH) {};
    RangeEx(char a, char b, int i, int j): minS(char2int(a)), maxS(char2int(b)),
      l(i), u(j) {};
    string
    str() const {
      return "[" + string(1, int2char(minS)) + "-" + string(1, int2char(maxS)) +
             ", " + std::to_string(l) + ", " + std::to_string(u) + "]";
    }
    bool decomp() const { return true; }
    bool has_union() const { return maxS > minS; }
    string
    get_string() const {
      if (minS == maxS && l == u)
        return string(l, int2char(minS));
      throw std::runtime_error("Can't get a string from " + str() + "\n");
    }
    NSIntSet get_chars() const { return NSIntSet(minS, maxS); }
    void
    post(Home home, StringVar x, VarArgs&) const {
      GECODE_POST;
      if (minS > maxS || l > u)
        home.fail();
      else {
        NSBlocks d({NSBlock(NSIntSet(minS, maxS), l, u)});
        rel(home, x, STRT_DOM, d, l, u);
      }
    }
    void
    add(AST::StringVar* x, ParserState* pp) const {
      if (minS > maxS || l > u)
        pp->fg->fail();
      else {
        AST::Array* ce1 = new AST::Array(2);
        ce1->a[0] = new AST::StringVar(x->i);
        if (l == u)
          ce1->a[1] = new AST::IntLit(l);
        else
          ce1->a[1] = new AST::SetLit(l, u);
        pp->constraints.push_back(new ConExpr("str_len", ce1, NULL));
        AST::Array* ce2 = new AST::Array(2);
        ce2->a[0] = new AST::StringVar(x->i);
        ce2->a[1] = new AST::StringDom(NSIntSet(minS, maxS));
        pp->constraints.push_back(new ConExpr("str_chars", ce2, NULL));
      }
    }
    REG reg() const {
      if (u * (maxS - minS + 1) > _MAX_SIZE) {
        std::cerr << "Warning: The resulting DFA size is too big! "
                  << "Ignored max cardinality " << u << "\n";
        return REG(IntArgs::create(maxS - minS + 1, minS))(l);
      }
      return REG(IntArgs::create(maxS - minS + 1, minS))(l, u);
    }
  };

  class RegExParser {

  private:

    int _open;
    int _currI;
    int _lastI;
    const string& _input;


    forceinline char top() const { return _input[_currI]; }

    forceinline bool more() const { return _currI < _lastI; }

    forceinline char
    next(bool escaped = false) {
      char c = top();
      consume(c, escaped);
      return c;
    }

    forceinline void
    consume(char c, bool escaped = false) {
      // std::cerr << "consume <" << c << "> (top = <" << top() << ">)\n";
      if (top() == c) {
        _currI++;
        if (c == '(' && !escaped)
          ++_open;
        else if (c == ')' && !escaped)
          --_open;
      }
      else
        throw std::runtime_error("Ill-formed expression\n");
    }

    RegEx*
    term() {
      // std::cerr << "*** term ***\n";
      RegEx* curr = new EmptyEx();
      char c = top();
      if (c == '|' || c == '&')
        throw std::runtime_error("Ill-formed expression\n");
      while (more() && c != ')' && c != '|' && c != '&') {
        RegEx* next = factor();
        curr = new ConcatEx(curr, next);
        c = top();
      }
      return curr;
    }

    RegEx*
    regex() {
      // std::cerr << "*** regex ***\n";
      RegEx* curr = term();
      char c = top();
      if (more() && (c == '|' || c == '&')) {
        consume(c);
        RegEx* next = regex();
        if (c == '|')
          return new UnionEx(curr, next);
        else
          return new InterEx(curr, next);
      }
      else
        return curr;
    }

    RegEx*
    factor() {
      // std::cerr << "*** factor ***\n";
      RegEx* b = base();
      while (more() && top() == '*') {
        consume('*');
        b = new StarEx(b);
      }
      return b;
    }

    RegEx*
    base() {
      // std::cerr << "*** base ***" << top() << "\n";
      switch (top()) {
        case '(': {
          consume('(');
          RegEx* r = regex();
          consume(')');
          return r;
        }
        case '[': {
          consume('[');
          char l = next();
          if (l == '\\') { 
        	  l = next();
        	  if (l == '0') {
        	    --_lastI;
        	    l = '\0';
        	  }
        	}
          consume('-');
          char u = next();
          if (u == '\\') {
        	  u = next();
        	  if (u == '0') {
        	    --_lastI;
        	    u = '\0';
        	  }
        	}
          char c = next();
          // std::cerr << c << ' ' << l << ' ' << u << '\n';
          if (c == ']')
            return new RangeEx(l, u);
          else if (c == ',') {
          	string s = "";
          	int x = -1;
            while (true) {
              char i = next();
              if (i == ',') {
                x = stoi(s);
                s = "";
              }
              else if (i == ']') {
                if (x == -1)
                  return new RangeEx(l, u, stoi(s));
                else
                  return new RangeEx(l, u, x, stoi(s));
              }
              else
                s += i;
            }
          }
          else
            throw std::runtime_error("Ill-formed expression\n");
        }
        case '.': {
          consume('.');
          // FIXME: CharExacters of extended ASCII alphabet.
          return new RangeEx(0, 255);
        }
        case '\\': {
          consume(top());
          switch (top()) {
            case '(':
            case ')':
            case '[':
            case ']':
            case '|':
            case '&':
            case '*':
            case '.':
            case '\\':
            case '\'':
              return new CharEx(next(true));
            case '0':
              consume('0');
              return new CharEx('\0');
            default:
              throw std::runtime_error("Unescaped char " + char2int(top()));
          }
          break;
        }
        case ')':
        case ']':
          throw std::runtime_error("Ill-formed expression");
      }
      return new CharEx(next());
    }

  public:

    RegExParser(const string& in)
    : _open(0), _currI(0), _lastI(in.size()), _input(in) {}

    RegEx* parse() {
      RegEx* r = regex();
      if (_open != 0)
        throw std::runtime_error("Unbalanced brackets");
      else if (more())
        throw std::runtime_error("Ill-formed expression");
//       std::cerr << r->str() << "\n";
      return r;
    }

  };

}}

#endif
