/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 37 "gecode/flatzinc/parser.yxx"

#define YYPARSE_PARAM parm
#define YYLEX_PARAM static_cast<ParserState*>(parm)->yyscanner
#include <gecode/flatzinc.hh>
#include <gecode/flatzinc/parser.hh>
#include <iostream>
#include <fstream>

#ifdef HAVE_MMAP
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#endif

using namespace std;

int yyparse(void*);
int yylex(YYSTYPE*, void* scanner);
int yylex_init (void** scanner);
int yylex_destroy (void* scanner);
int yyget_lineno (void* scanner);
void yyset_extra (void* user_defined ,void* yyscanner );

extern int yydebug;

using namespace Gecode;
using namespace Gecode::FlatZinc;
using namespace Gecode::String;
//TODO: #include <gecode/string/ext/parse-reg.hpp>
//vector<pair<AST::StringVar*, RegEx*>> REGEX;

void yyerror(void* parm, const char *str) {
  ParserState* pp = static_cast<ParserState*>(parm);
  pp->err << "Error: " << str
          << " in line no. " << yyget_lineno(pp->yyscanner)
          << std::endl;
  pp->hadError = true;
}

void yyassert(ParserState* pp, bool cond, const char* str)
{
  if (!cond) {
    pp->err << "Error: " << str
            << " in line no. " << yyget_lineno(pp->yyscanner)
            << std::endl;
    pp->hadError = true;
  }
}

/*
 * The symbol tables
 *
 */

AST::Node* getArrayElement(ParserState* pp, string id, int offset,
                           bool annotation) {
  if (offset > 0) {
    SymbolEntry e;
    if (pp->symbols.get(id,e)) {
      switch (e.t) {
      case ST_INTVARARRAY:
        if (offset > pp->arrays[e.i])
          goto error;
        {
          std::string n;
          if (annotation) {
            std::ostringstream oss;
            oss << id << "[" << offset << "]";
            n = oss.str();
          }
          return new AST::IntVar(pp->arrays[e.i+offset],n);
        }
      case ST_BOOLVARARRAY:
        if (offset > pp->arrays[e.i])
          goto error;
        {
          std::string n;
          if (annotation) {
            std::ostringstream oss;
            oss << id << "[" << offset << "]";
            n = oss.str();
          }
          return new AST::BoolVar(pp->arrays[e.i+offset],n);
        }
      case ST_SETVARARRAY:
        if (offset > pp->arrays[e.i])
          goto error;
        {
          std::string n;
          if (annotation) {
            std::ostringstream oss;
            oss << id << "[" << offset << "]";
            n = oss.str();
          }
          return new AST::SetVar(pp->arrays[e.i+offset],n);
        }
      case ST_FLOATVARARRAY:
        if (offset > pp->arrays[e.i])
          goto error;
        {
          std::string n;
          if (annotation) {
            std::ostringstream oss;
            oss << id << "[" << offset << "]";
            n = oss.str();
          }
          return new AST::FloatVar(pp->arrays[e.i+offset],n);
        }
      case ST_STRINGVARARRAY:
        if (offset > pp->arrays[e.i])
          goto error;
        {
          std::string n;
          if (annotation) {
            std::ostringstream oss;
            oss << id << "[" << offset << "]";
            n = oss.str();
          }          
          return new AST::StringVar(pp->arrays[e.i+offset],n);
        }
      case ST_INTVALARRAY:
        if (offset > pp->arrays[e.i])
          goto error;
        return new AST::IntLit(pp->arrays[e.i+offset]);
      case ST_SETVALARRAY:
        if (offset > pp->arrays[e.i])
          goto error;
        return new AST::SetLit(pp->setvals[pp->arrays[e.i+1]+offset-1]);
      case ST_FLOATVALARRAY:
        if (offset > pp->arrays[e.i])
          goto error;
        return new AST::FloatLit(pp->floatvals[pp->arrays[e.i+1]+offset-1]);
      case ST_STRINGVALARRAY:
        if (offset > pp->arrays[e.i])
          goto error;
        return new AST::StringDom(pp->stringvals[pp->arrays[e.i+1]+offset-1]);
      default:
        break;
      }
    }
  }
error:
  pp->err << "Error: array access to " << id << " invalid"
          << " in line no. "
          << yyget_lineno(pp->yyscanner) << std::endl;
  pp->hadError = true;
  return new AST::IntVar(0); // keep things consistent
}
AST::Node* getVarRefArg(ParserState* pp, string id, bool annotation = false) {
  SymbolEntry e;
  string n;
  if (annotation)
    n = id;
  if (pp->symbols.get(id, e)) {
    switch (e.t) {
    case ST_INTVAR: return new AST::IntVar(e.i,n);
    case ST_BOOLVAR: return new AST::BoolVar(e.i,n);
    case ST_SETVAR: return new AST::SetVar(e.i,n);
    case ST_FLOATVAR: return new AST::FloatVar(e.i,n);
    case ST_STRINGVAR: return new AST::StringVar(e.i,n);
    default: break;
    }
  }

  if (annotation)
    return new AST::Atom(id);
  pp->err << "Error: undefined variable " << id
          << " in line no. "
          << yyget_lineno(pp->yyscanner) << std::endl;
  pp->hadError = true;
  return new AST::IntVar(0); // keep things consistent
}

void addDomainConstraint(ParserState* pp, std::string id, AST::Node* var,
                         Option<AST::SetLit* >& dom) {
  if (!dom())
    return;
  AST::Array* args = new AST::Array(2);
  args->a[0] = var;
  args->a[1] = dom.some();
  pp->domainConstraints.push_back(new ConExpr(id, args, NULL));
}

void addDomainConstraint(ParserState* pp, AST::Node* var,
                         Option<std::pair<double,double>* > dom) {
  if (!dom())
    return;
  {
    AST::Array* args = new AST::Array(2);
    args->a[0] = new AST::FloatLit(dom.some()->first);
    args->a[1] = var;
    pp->domainConstraints.push_back(new ConExpr("float_le", args, NULL));
  }
  {
    AST::Array* args = new AST::Array(2);
    AST::FloatVar* fv = static_cast<AST::FloatVar*>(var);
    args->a[0] = new AST::FloatVar(fv->i,fv->n);
    args->a[1] = new AST::FloatLit(dom.some()->second);
    pp->domainConstraints.push_back(new ConExpr("float_le", args, NULL));
  }
  delete dom.some();
}

void addDomainConstraint(ParserState* pp, AST::Node* var,
                         Option<AST::StringDom*> dom) {
  if (!dom())
    return;
  if (dom.some()->u != -1) {
    AST::Array* args = new AST::Array(2);
    args->a[0] = var;
    args->a[1] = dom.some();
    pp->domainConstraints.push_back(new ConExpr("str_chars", args, NULL));
  }
  else if (dom.some()->c == NULL) {
    AST::Array* args = new AST::Array(2);
    args->a[0] = var;
    args->a[1] = new AST::String(dom.some()->v);
    pp->domainConstraints.push_back(new ConExpr("str_eq", args, NULL));
  }
}

int getBaseIntVar(ParserState* pp, int i) {
  int base = i;
  IntVarSpec* ivs = static_cast<IntVarSpec*>(pp->intvars[base].second);
  while (ivs->alias) {
    base = ivs->i;
    ivs = static_cast<IntVarSpec*>(pp->intvars[base].second);
  }
  return base;
}
int getBaseBoolVar(ParserState* pp, int i) {
  int base = i;
  BoolVarSpec* ivs = static_cast<BoolVarSpec*>(pp->boolvars[base].second);
  while (ivs->alias) {
    base = ivs->i;
    ivs = static_cast<BoolVarSpec*>(pp->boolvars[base].second);
  }
  return base;
}
int getBaseFloatVar(ParserState* pp, int i) {
  int base = i;
  FloatVarSpec* ivs = static_cast<FloatVarSpec*>(pp->floatvars[base].second);
  while (ivs->alias) {
    base = ivs->i;
    ivs = static_cast<FloatVarSpec*>(pp->floatvars[base].second);
  }
  return base;
}
int getBaseSetVar(ParserState* pp, int i) {
  int base = i;
  SetVarSpec* ivs = static_cast<SetVarSpec*>(pp->setvars[base].second);
  while (ivs->alias) {
    base = ivs->i;
    ivs = static_cast<SetVarSpec*>(pp->setvars[base].second);
  }
  return base;
}
int getBaseStringVar(ParserState* pp, int i) {
  int base = i;
  StringVarSpec* ivs = static_cast<StringVarSpec*>(pp->stringvars[base].second);
  while (ivs->alias) {
    base = ivs->i;
    ivs = static_cast<StringVarSpec*>(pp->stringvars[base].second);
  }
  return base;
}

/*
 * Initialize the root gecode space
 *
 */

void initfg(ParserState* pp) {
  if (!pp->hadError) {
    //TODO: Unfolding regular expressions.
//    for (auto x : REGEX)
//      x.second->add(x.first, pp);
//    pp->fg->init(pp->intvars.size(),
//                 pp->boolvars.size(),
//                 pp->setvars.size(),
//                 pp->floatvars.size(),
//                 pp->stringvars.size());
  }
  for (unsigned int i=0; i<pp->intvars.size(); i++) {
    if (!pp->hadError) {
      try {
        pp->fg->newIntVar(static_cast<IntVarSpec*>(pp->intvars[i].second));
      } catch (Gecode::FlatZinc::Error& e) {
        yyerror(pp, e.toString().c_str());
      }
    }
    delete pp->intvars[i].second;
    pp->intvars[i].second = NULL;
  }
  for (unsigned int i=0; i<pp->boolvars.size(); i++) {
    if (!pp->hadError) {
      try {
        pp->fg->newBoolVar(
          static_cast<BoolVarSpec*>(pp->boolvars[i].second));
      } catch (Gecode::FlatZinc::Error& e) {
        yyerror(pp, e.toString().c_str());
      }
    }
    delete pp->boolvars[i].second;
    pp->boolvars[i].second = NULL;
  }
  for (unsigned int i=0; i<pp->setvars.size(); i++) {
    if (!pp->hadError) {
      try {
        pp->fg->newSetVar(static_cast<SetVarSpec*>(pp->setvars[i].second));
      } catch (Gecode::FlatZinc::Error& e) {
        yyerror(pp, e.toString().c_str());
      }
    }
    delete pp->setvars[i].second;
    pp->setvars[i].second = NULL;
  }
  for (unsigned int i=0; i<pp->floatvars.size(); i++) {
    if (!pp->hadError) {
      try {
        pp->fg->newFloatVar(
          static_cast<FloatVarSpec*>(pp->floatvars[i].second));
      } catch (Gecode::FlatZinc::Error& e) {
        yyerror(pp, e.toString().c_str());
      }
    }
    delete pp->floatvars[i].second;
    pp->floatvars[i].second = NULL;
  }
  for (unsigned int i=0; i<pp->stringvars.size(); i++) {
    if (!pp->hadError) {
      try {
        pp->fg->newStringVar(
          static_cast<StringVarSpec*>(pp->stringvars[i].second));
      } catch (Gecode::FlatZinc::Error& e) {
        yyerror(pp, e.toString().c_str());
      }
    }      
    delete pp->stringvars[i].second;
    pp->stringvars[i].second = NULL;
  }
  if (!pp->hadError) {
    pp->fg->postConstraints(pp->domainConstraints);
    pp->fg->postConstraints(pp->constraints);
  }
}

void fillPrinter(ParserState& pp, Gecode::FlatZinc::Printer& p) {
  p.init(pp.getOutput());
  for (unsigned int i=0; i<pp.intvars.size(); i++) {
    if (!pp.hadError) {
      p.addIntVarName(pp.intvars[i].first);
    }
  }
  for (unsigned int i=0; i<pp.boolvars.size(); i++) {
    if (!pp.hadError) {
      p.addBoolVarName(pp.boolvars[i].first);
    }
  }
#ifdef GECODE_HAS_FLOAT_VARS
  for (unsigned int i=0; i<pp.floatvars.size(); i++) {
    if (!pp.hadError) {
      p.addFloatVarName(pp.floatvars[i].first);
    }
  }
#endif
#ifdef GECODE_HAS_SET_VARS
  for (unsigned int i=0; i<pp.setvars.size(); i++) {
    if (!pp.hadError) {
      p.addSetVarName(pp.setvars[i].first);
    }
  }
#endif
#ifdef GECODE_HAS_STRING_VARS
  for (unsigned int i=0; i<pp.stringvars.size(); i++) {
    if (!pp.hadError) {
      p.addStringVarName(pp.stringvars[i].first);
    }
  }
#endif 
}

AST::Node* arrayOutput(AST::Call* ann) {
  AST::Array* a = NULL;

  if (ann->args->isArray()) {
    a = ann->args->getArray();
  } else {
    a = new AST::Array(ann->args);
  }

  std::ostringstream oss;

  oss << "array" << a->a.size() << "d(";
  for (unsigned int i=0; i<a->a.size(); i++) {
    AST::SetLit* s = a->a[i]->getSet();
    if (s->empty())
      oss << "{}, ";
    else if (s->interval)
      oss << s->min << ".." << s->max << ", ";
    else {
      oss << "{";
      for (unsigned int j=0; j<s->s.size(); j++) {
        oss << s->s[j];
        if (j<s->s.size()-1)
          oss << ",";
      }
      oss << "}, ";
    }
  }

  if (!ann->args->isArray()) {
    a->a[0] = NULL;
    delete a;
  }
  return new AST::String(oss.str());
}

/*
 * The main program
 *
 */

namespace Gecode { namespace FlatZinc {

  FlatZincSpace* parse(const std::string& filename, Printer& p, 
                       const FlatZincOptions&, std::ostream& err,
                       FlatZincSpace* fzs, Rnd& rnd) {
#ifdef HAVE_MMAP
    int fd;
    char* data;
    struct stat sbuf;
    fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
      err << "Cannot open file " << filename << endl;
      return NULL;
    }
    if (stat(filename.c_str(), &sbuf) == -1) {
      err << "Cannot stat file " << filename << endl;
      return NULL;
    }
    data = (char*)mmap((caddr_t)0, sbuf.st_size, PROT_READ, MAP_SHARED, fd,0);
    if (data == (caddr_t)(-1)) {
      err << "Cannot mmap file " << filename << endl;
      return NULL;
    }

    if (fzs == NULL) {
      fzs = new FlatZincSpace(rnd);
    }
    ParserState pp(data, sbuf.st_size, err, fzs);
#else
    std::ifstream file;
    file.open(filename.c_str());
    if (!file.is_open()) {
      err << "Cannot open file " << filename << endl;
      return NULL;
    }
    std::string s = string(istreambuf_iterator<char>(file),
                           istreambuf_iterator<char>());
    if (fzs == NULL) {
      fzs = new FlatZincSpace(rnd);
    }
    ParserState pp(s, err, fzs);
#endif
    yylex_init(&pp.yyscanner);
    yyset_extra(&pp, pp.yyscanner);
    
    // yydebug = 1;
    yyparse(&pp);
    fillPrinter(pp, p);

    if (pp.yyscanner)
      yylex_destroy(pp.yyscanner);
    return pp.hadError ? NULL : pp.fg;
  }

  FlatZincSpace* parse(std::istream& is, Printer& p, const FlatZincOptions&, 
                       std::ostream& err, FlatZincSpace* fzs, Rnd& rnd) {
    std::string s = string(istreambuf_iterator<char>(is),
                           istreambuf_iterator<char>());

    if (fzs == NULL) {
      fzs = new FlatZincSpace(rnd);
    }
    ParserState pp(s, err, fzs);
    yylex_init(&pp.yyscanner);
    yyset_extra(&pp, pp.yyscanner);

    // yydebug = 1;
    yyparse(&pp);
    fillPrinter(pp, p);

    if (pp.yyscanner)
      yylex_destroy(pp.yyscanner);
    return pp.hadError ? NULL : pp.fg;
  }

}}


#line 576 "gecode/flatzinc/parser.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_GECODE_FLATZINC_PARSER_TAB_HPP_INCLUDED
# define YY_YY_GECODE_FLATZINC_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FZ_INT_LIT = 258,
    FZ_BOOL_LIT = 259,
    FZ_FLOAT_LIT = 260,
    FZ_ID = 261,
    FZ_U_ID = 262,
    FZ_STRING_LIT = 263,
    FZ_VAR = 264,
    FZ_PAR = 265,
    FZ_ANNOTATION = 266,
    FZ_ANY = 267,
    FZ_ARRAY = 268,
    FZ_BOOL = 269,
    FZ_CASE = 270,
    FZ_COLONCOLON = 271,
    FZ_CONSTRAINT = 272,
    FZ_DEFAULT = 273,
    FZ_DOTDOT = 274,
    FZ_ELSE = 275,
    FZ_ELSEIF = 276,
    FZ_ENDIF = 277,
    FZ_ENUM = 278,
    FZ_FLOAT = 279,
    FZ_FUNCTION = 280,
    FZ_IF = 281,
    FZ_INCLUDE = 282,
    FZ_INT = 283,
    FZ_LET = 284,
    FZ_MAXIMIZE = 285,
    FZ_MINIMIZE = 286,
    FZ_OF = 287,
    FZ_SATISFY = 288,
    FZ_OUTPUT = 289,
    FZ_PREDICATE = 290,
    FZ_RECORD = 291,
    FZ_SET = 292,
    FZ_SHOW = 293,
    FZ_SHOWCOND = 294,
    FZ_SOLVE = 295,
    FZ_STRING = 296,
    FZ_TEST = 297,
    FZ_THEN = 298,
    FZ_TUPLE = 299,
    FZ_TYPE = 300,
    FZ_VARIANT_RECORD = 301,
    FZ_WHERE = 302
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 543 "gecode/flatzinc/parser.yxx"
 int iValue; char* sValue; bool bValue; double dValue;
         std::vector<int>* setValue;
         Gecode::FlatZinc::AST::SetLit* setLit;
         std::vector<double>* floatSetValue;
         std::vector<std::string>* stringSetValue;
         std::vector<Gecode::FlatZinc::AST::SetLit>* setValueList;
         Gecode::FlatZinc::Option<Gecode::FlatZinc::AST::SetLit* > oSet;
         Gecode::FlatZinc::Option<std::pair<double,double>* > oPFloat;
         Gecode::FlatZinc::AST::CharSetLit* charsetLit;
         Gecode::FlatZinc::Option<Gecode::FlatZinc::AST::StringDom*> oPString;
         Gecode::FlatZinc::VarSpec* varSpec;
         Gecode::FlatZinc::Option<Gecode::FlatZinc::AST::Node*> oArg;
         std::vector<Gecode::FlatZinc::VarSpec*>* varSpecVec;
         Gecode::FlatZinc::Option<std::vector<Gecode::FlatZinc::VarSpec*>* > oVarSpecVec;
         Gecode::FlatZinc::AST::Node* arg;
         Gecode::FlatZinc::AST::Array* argVec;
       

#line 695 "gecode/flatzinc/parser.tab.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (void *parm);

#endif /* !YY_YY_GECODE_FLATZINC_PARSER_TAB_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   433

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  77
/* YYNRULES -- Number of rules.  */
#define YYNRULES  188
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  405

#define YYUNDEFTOK  2
#define YYMAXUTOK   302


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      49,    50,     2,     2,    51,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    52,    48,
       2,    55,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    53,     2,    54,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    56,     2,    57,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   655,   655,   657,   659,   662,   663,   665,   667,   670,
     671,   673,   675,   678,   679,   686,   689,   691,   694,   695,
     698,   702,   703,   704,   705,   708,   710,   712,   713,   714,
     717,   718,   721,   722,   728,   728,   731,   763,   795,   834,
     867,   909,   918,   928,   937,   949,   959,  1029,  1095,  1166,
    1234,  1305,  1326,  1346,  1366,  1386,  1408,  1412,  1427,  1451,
    1452,  1456,  1458,  1461,  1461,  1463,  1467,  1469,  1484,  1507,
    1508,  1512,  1514,  1518,  1522,  1524,  1539,  1562,  1563,  1567,
    1569,  1572,  1575,  1577,  1592,  1615,  1616,  1620,  1622,  1625,
    1629,  1631,  1649,  1672,  1673,  1677,  1679,  1683,  1688,  1689,
    1694,  1695,  1700,  1701,  1706,  1707,  1712,  1713,  1718,  1867,
    1881,  1906,  1908,  1910,  1916,  1918,  1931,  1933,  1938,  1940,
    1942,  1944,  1952,  1954,  1961,  1962,  1966,  1968,  1973,  1974,
    1978,  1980,  1985,  1986,  1990,  1992,  1997,  1998,  2002,  2004,
    2008,  2016,  2018,  2022,  2024,  2029,  2030,  2034,  2036,  2041,
    2042,  2046,  2048,  2050,  2052,  2054,  2056,  2058,  2174,  2189,
    2190,  2194,  2196,  2204,  2238,  2245,  2252,  2278,  2279,  2287,
    2288,  2292,  2294,  2298,  2302,  2306,  2308,  2312,  2314,  2316,
    2319,  2319,  2322,  2324,  2326,  2328,  2330,  2332,  2461
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FZ_INT_LIT", "FZ_BOOL_LIT",
  "FZ_FLOAT_LIT", "FZ_ID", "FZ_U_ID", "FZ_STRING_LIT", "FZ_VAR", "FZ_PAR",
  "FZ_ANNOTATION", "FZ_ANY", "FZ_ARRAY", "FZ_BOOL", "FZ_CASE",
  "FZ_COLONCOLON", "FZ_CONSTRAINT", "FZ_DEFAULT", "FZ_DOTDOT", "FZ_ELSE",
  "FZ_ELSEIF", "FZ_ENDIF", "FZ_ENUM", "FZ_FLOAT", "FZ_FUNCTION", "FZ_IF",
  "FZ_INCLUDE", "FZ_INT", "FZ_LET", "FZ_MAXIMIZE", "FZ_MINIMIZE", "FZ_OF",
  "FZ_SATISFY", "FZ_OUTPUT", "FZ_PREDICATE", "FZ_RECORD", "FZ_SET",
  "FZ_SHOW", "FZ_SHOWCOND", "FZ_SOLVE", "FZ_STRING", "FZ_TEST", "FZ_THEN",
  "FZ_TUPLE", "FZ_TYPE", "FZ_VARIANT_RECORD", "FZ_WHERE", "';'", "'('",
  "')'", "','", "':'", "'['", "']'", "'='", "'{'", "'}'", "$accept",
  "model", "preddecl_items", "preddecl_items_head", "vardecl_items",
  "vardecl_items_head", "constraint_items", "constraint_items_head",
  "preddecl_item", "pred_arg_list", "pred_arg_list_head", "pred_arg",
  "pred_arg_type", "pred_arg_simple_type", "pred_array_init",
  "pred_array_init_arg", "var_par_id", "vardecl_item", "int_init",
  "int_init_list", "int_init_list_head", "list_tail",
  "int_var_array_literal", "float_init", "float_init_list",
  "float_init_list_head", "float_var_array_literal", "bool_init",
  "bool_init_list", "bool_init_list_head", "bool_var_array_literal",
  "set_init", "set_init_list", "set_init_list_head",
  "set_var_array_literal", "string_init", "string_init_list",
  "string_init_list_head", "string_var_array_literal",
  "vardecl_int_var_array_init", "vardecl_bool_var_array_init",
  "vardecl_float_var_array_init", "vardecl_set_var_array_init",
  "vardecl_string_var_array_init", "constraint_item", "solve_item",
  "int_ti_expr_tail", "bool_ti_expr_tail", "float_ti_expr_tail",
  "string_ti_expr_tail", "set_literal", "int_list", "int_list_head",
  "bool_list", "bool_list_head", "float_list", "float_list_head",
  "set_literal_list", "set_literal_list_head", "charset_literal",
  "flat_expr_list", "flat_expr", "string_list", "string_list_head",
  "non_array_expr_opt", "non_array_expr", "non_array_expr_list",
  "non_array_expr_list_head", "solve_expr", "minmax", "annotations",
  "annotations_head", "annotation", "annotation_list", "annotation_expr",
  "annotation_list_tail", "ann_non_array_expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,    59,    40,
      41,    44,    58,    91,    93,    61,   123,   125
};
# endif

#define YYPACT_NINF (-141)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -14,    25,    23,   328,   -14,   -23,    -1,  -141,    99,     2,
       6,    34,    37,    66,    40,    79,   328,    52,    67,  -141,
      19,    98,   106,  -141,  -141,  -141,   112,   -19,    50,    93,
     101,   117,   123,   151,   102,   102,   102,   130,   102,   170,
     137,    79,   135,   138,  -141,  -141,   209,   134,  -141,  -141,
     156,  -141,   187,   141,   144,  -141,   140,  -141,  -141,   191,
     193,    11,   188,   197,  -141,  -141,   146,   150,   154,   102,
     102,   102,   102,   189,  -141,  -141,   195,   195,   195,   155,
     195,   160,   195,   167,   168,  -141,  -141,  -141,    17,    11,
    -141,    19,  -141,   208,  -141,  -141,   172,   211,  -141,   181,
    -141,   229,  -141,   230,   178,   195,   195,   195,   195,   233,
     125,   183,   223,   185,   186,   102,   190,   157,    47,  -141,
    -141,   228,  -141,   -42,  -141,  -141,  -141,  -141,   102,   240,
     219,  -141,  -141,  -141,   199,   199,   199,   199,   203,   239,
    -141,  -141,   210,  -141,    58,   187,   213,  -141,  -141,  -141,
    -141,   280,   125,   280,   280,   195,   280,   239,  -141,  -141,
     211,   280,   215,  -141,  -141,    84,  -141,  -141,  -141,  -141,
    -141,   100,   258,    17,   231,   195,  -141,   188,   280,  -141,
    -141,  -141,  -141,   232,   259,   125,  -141,  -141,   225,   217,
     293,  -141,  -141,  -141,  -141,   224,  -141,  -141,   216,   227,
     280,   195,   157,  -141,  -141,   236,  -141,  -141,  -141,   143,
     199,  -141,  -141,    60,  -141,    91,   125,   226,  -141,   238,
     280,  -141,   280,  -141,   248,  -141,  -141,   278,   209,  -141,
    -141,   165,   241,   242,   251,   272,   256,  -141,   125,  -141,
    -141,  -141,  -141,  -141,  -141,   262,  -141,   285,   266,   269,
     270,   273,   102,   102,   102,   282,   102,  -141,    11,   102,
     102,   102,   102,   195,   195,   195,   274,   195,   277,   195,
     195,   195,   195,   235,   279,   283,   102,   284,   102,   288,
     289,   290,   291,   271,   287,   294,   195,   295,   195,   297,
    -141,   298,  -141,   300,  -141,   301,  -141,   329,   327,   187,
     302,   347,   303,    76,  -141,    64,  -141,    35,  -141,   166,
    -141,   305,   154,  -141,   306,   311,   309,   313,  -141,   310,
     316,   315,  -141,  -141,   317,  -141,   318,   320,  -141,   321,
    -141,   319,   324,  -141,   323,  -141,   325,   326,  -141,   330,
    -141,   331,   333,  -141,  -141,  -141,   373,  -141,  -141,    16,
    -141,   372,  -141,    43,  -141,   378,  -141,    76,  -141,   379,
    -141,    64,  -141,   383,  -141,    35,  -141,   384,  -141,   166,
    -141,  -141,   239,  -141,   334,   338,  -141,   337,  -141,   339,
     340,  -141,   341,  -141,   342,  -141,   343,  -141,   344,  -141,
    -141,    16,  -141,   389,  -141,    43,  -141,  -141,  -141,  -141,
    -141,  -141,   345,  -141,  -141
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     7,     4,     0,     0,     1,     0,     0,
       0,     0,     0,     0,     0,    11,     8,     0,     0,     5,
      16,     0,     0,   114,   116,   111,     0,   118,   124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    12,     0,     0,     9,     6,     0,     0,    27,    28,
       0,    29,   124,     0,    63,    18,     0,    24,    25,     0,
       0,     0,     0,     0,   126,   130,     0,    63,    63,     0,
       0,     0,     0,     0,    34,    35,   169,   169,   169,     0,
     169,     0,   169,     0,     0,    13,    10,    23,     0,     0,
      15,    64,    17,     0,   113,   117,     0,     0,   120,     0,
     112,    64,   125,    64,     0,   169,   169,   169,   169,     0,
       0,     0,   170,     0,     0,     0,     0,     0,     0,     2,
      14,     0,    32,     0,    30,    26,    19,    20,     0,     0,
     119,   127,   131,   115,   149,   149,   149,   149,     0,   183,
     182,   184,    34,   185,     0,   124,   187,   186,   171,   174,
     177,     0,     0,     0,     0,   169,     0,   152,   151,   153,
     154,   159,   157,   155,   156,     0,   141,   143,   168,   167,
     109,     0,     0,     0,     0,   169,   140,     0,     0,    36,
      37,    38,    40,     0,     0,     0,   178,   175,   180,     0,
       0,    43,   172,    42,    41,     0,    45,   161,     0,    63,
       0,   169,     0,   164,   165,   163,   110,    33,    31,     0,
     149,   121,   150,     0,   123,     0,   181,     0,   122,     0,
       0,   144,    64,   160,     0,   108,   142,     0,     0,    21,
      39,     0,     0,     0,     0,     0,     0,   173,     0,   176,
     179,   188,    44,   162,   158,     0,    22,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   166,     0,     0,
       0,     0,     0,   169,   169,   169,     0,   169,     0,   169,
     169,   169,   169,     0,     0,     0,     0,     0,     0,    98,
     100,   102,   106,     0,     0,     0,   169,     0,   169,     0,
      46,     0,    47,     0,    48,     0,    50,   128,   132,   124,
       0,   145,   104,    59,    99,    77,   101,    69,   103,    93,
     107,     0,    63,   134,     0,    63,     0,     0,   147,     0,
      63,     0,    49,    56,    57,    61,     0,    63,    74,    75,
      79,     0,    63,    66,    67,    71,     0,    63,    90,    91,
      95,     0,    63,    52,   129,    53,    64,   133,    51,   136,
      55,    64,   146,    85,   105,     0,    65,    64,    60,     0,
      81,    64,    78,     0,    73,    64,    70,     0,    97,    64,
      94,   135,     0,   138,     0,    63,   148,    83,    87,     0,
      63,    82,     0,    62,     0,    80,     0,    72,     0,    96,
      54,    64,   137,     0,    89,    64,    86,    58,    76,    68,
      92,   139,     0,    88,    84
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -141,  -141,  -141,  -141,  -141,  -141,  -141,  -141,   390,  -141,
    -141,   312,  -141,   -43,  -141,   234,   -34,   385,    45,  -141,
    -141,   -60,  -141,    39,  -141,  -141,  -141,    44,  -141,  -141,
    -141,     5,  -141,  -141,  -141,    41,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,   365,  -141,    -2,   177,   180,   182,
    -100,  -140,  -141,  -141,   115,  -141,  -141,  -141,  -141,   -51,
    -141,   207,  -141,  -141,  -119,  -127,  -141,  -141,  -141,  -141,
      42,  -141,   -95,   237,  -141,  -141,   243
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,    15,    16,    40,    41,     5,    53,
      54,    55,    56,    57,   123,   124,   162,    17,   325,   326,
     327,    92,   304,   335,   336,   337,   308,   330,   331,   332,
     306,   378,   379,   380,   354,   340,   341,   342,   310,   290,
     292,   294,   322,   296,    42,    83,    58,    30,    31,    32,
     163,    66,    67,   311,    68,   314,   315,   374,   375,   164,
     165,   166,   319,   320,   179,   167,   198,   199,   206,   171,
     111,   112,   187,   188,   149,   217,   150
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,    77,    78,    87,    80,   189,    29,   102,   104,   173,
     147,    98,   174,    62,    21,   148,   180,   181,   182,   372,
     121,     1,    21,     7,   191,    19,   193,   194,    46,   196,
      63,     6,    47,    48,   197,   105,   106,   107,   108,    25,
     333,    74,    75,    49,   147,   122,   372,    25,    20,    74,
      75,   212,   147,    64,    65,    33,    50,   192,    34,    96,
      51,   139,   140,   141,   142,    75,   143,    52,   328,   231,
      74,    75,   145,   224,   232,    52,   146,   168,   169,   323,
     170,   155,    74,    75,   233,   147,    35,   125,   234,    36,
     147,   230,    38,   242,   175,   243,    39,   235,    37,   145,
      44,   236,    21,   203,    22,   204,    74,    75,    74,    75,
     146,   144,   186,    23,   145,    45,   147,    59,   146,   113,
     114,   239,   116,    24,   118,    60,   211,    25,   139,   140,
     141,   142,    75,   143,   201,   202,    26,   205,   147,   223,
      27,   237,   238,   239,    61,    69,    21,   134,   135,   136,
     137,   146,   228,    70,    73,    28,   146,    48,    79,   316,
     157,   158,   159,    74,    75,   160,   229,    49,    21,    71,
      22,    25,    74,    75,   338,    72,    81,    82,   144,    23,
      50,   145,   146,    85,    51,   246,    86,    88,    89,    24,
      64,    90,    93,    25,    94,    91,    97,   195,    95,    52,
      99,   101,   247,   100,   146,   103,    27,   115,   109,   117,
     161,   110,    21,   145,   127,   119,   120,   210,   263,   264,
     265,    28,   267,    48,   128,   269,   270,   271,   272,   248,
     129,   130,   131,    49,   132,   133,   138,    25,   151,   152,
     153,   154,   286,   225,   288,   156,    50,   172,   176,   373,
      51,   177,   344,   381,   178,   347,   268,   183,   184,   185,
     352,   207,   214,   209,   213,    52,   190,   358,   200,   324,
     221,   329,   362,   334,   218,   339,   216,   366,   222,   220,
     240,   245,   370,   157,   158,   159,    74,    75,   160,   227,
     283,   401,   241,   252,   253,   381,   139,   140,   141,    74,
      75,   143,   244,   254,   255,   273,   274,   275,   256,   277,
     266,   279,   280,   281,   282,   392,   257,   258,   259,   377,
     396,   260,   261,   324,   297,   262,   276,   329,   300,   278,
     302,   334,   313,    65,   284,   339,   145,     8,   285,   287,
     298,     9,    10,   289,   291,   293,   295,   299,   301,   145,
     303,   305,    11,   307,   309,   318,    12,   317,   321,   343,
     345,   377,   346,   348,   350,    13,   349,   351,   353,    14,
     355,   357,   356,   360,   359,   361,   363,   365,   371,   364,
     376,   382,   384,   367,   369,   368,   386,   388,   390,   391,
     393,   395,   402,   394,    18,   397,   398,   399,   400,   404,
     403,    43,   383,   126,   387,   385,    84,   208,   249,   226,
     389,   250,   312,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   215,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   219
};

static const yytype_int16 yycheck[] =
{
      34,    35,    36,    46,    38,   145,     8,    67,    68,    51,
     110,    62,    54,    32,     3,   110,   135,   136,   137,     3,
       3,    35,     3,     0,   151,    48,   153,   154,     9,   156,
      49,     6,    13,    14,   161,    69,    70,    71,    72,    28,
       5,     6,     7,    24,   144,    28,     3,    28,    49,     6,
       7,   178,   152,     3,     4,    53,    37,   152,    52,    61,
      41,     3,     4,     5,     6,     7,     8,    56,     4,     9,
       6,     7,    56,   200,    14,    56,   110,    30,    31,     3,
      33,   115,     6,     7,    24,   185,    52,    89,    28,    52,
     190,   210,    52,   220,   128,   222,    17,    37,    32,    56,
      48,    41,     3,     3,     5,     5,     6,     7,     6,     7,
     144,    53,    54,    14,    56,    48,   216,    19,   152,    77,
      78,   216,    80,    24,    82,    19,   177,    28,     3,     4,
       5,     6,     7,     8,    50,    51,    37,   171,   238,   199,
      41,    50,    51,   238,    32,    52,     3,   105,   106,   107,
     108,   185,     9,    52,     3,    56,   190,    14,    28,   299,
       3,     4,     5,     6,     7,     8,   209,    24,     3,    52,
       5,    28,     6,     7,     8,    52,     6,    40,    53,    14,
      37,    56,   216,    48,    41,   228,    48,    53,    32,    24,
       3,    50,    52,    28,     3,    51,     8,   155,     5,    56,
       3,    51,    37,    57,   238,    51,    41,    52,    19,    49,
      53,    16,     3,    56,     6,    48,    48,   175,   252,   253,
     254,    56,   256,    14,    52,   259,   260,   261,   262,   231,
      19,    50,     3,    24,     4,    57,     3,    28,    55,    16,
      55,    55,   276,   201,   278,    55,    37,    19,     8,   349,
      41,    32,   312,   353,    55,   315,   258,    54,    19,    49,
     320,     3,     3,    32,    32,    56,    53,   327,    53,   303,
      54,   305,   332,   307,    57,   309,    51,   337,    51,    55,
      54,     3,   342,     3,     4,     5,     6,     7,     8,    53,
      55,   391,    54,    52,    52,   395,     3,     4,     5,     6,
       7,     8,    54,    52,    32,   263,   264,   265,    52,   267,
      28,   269,   270,   271,   272,   375,    54,    32,    52,   353,
     380,    52,    52,   357,    53,    52,    52,   361,   286,    52,
     288,   365,     5,     4,    55,   369,    56,     9,    55,    55,
      53,    13,    14,    55,    55,    55,    55,    53,    53,    56,
      53,    53,    24,    53,    53,     8,    28,    55,    55,    54,
      54,   395,    51,    54,    54,    37,    53,    51,    53,    41,
      53,    51,    54,    54,    53,    51,    53,    51,     5,    54,
       8,     3,     3,    53,    51,    54,     3,     3,    54,    51,
      53,    51,     3,    54,     4,    54,    54,    54,    54,    54,
     395,    16,   357,    91,   365,   361,    41,   173,   231,   202,
     369,   231,   297,   231,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    35,    59,    60,    61,    66,     6,     0,     9,    13,
      14,    24,    28,    37,    41,    62,    63,    75,    66,    48,
      49,     3,     5,    14,    24,    28,    37,    41,    56,   104,
     105,   106,   107,    53,    52,    52,    52,    32,    52,    17,
      64,    65,   102,    75,    48,    48,     9,    13,    14,    24,
      37,    41,    56,    67,    68,    69,    70,    71,   104,    19,
      19,    32,    32,    49,     3,     4,   109,   110,   112,    52,
      52,    52,    52,     3,     6,     7,    74,    74,    74,    28,
      74,     6,    40,   103,   102,    48,    48,    71,    53,    32,
      50,    51,    79,    52,     3,     5,   104,     8,   117,     3,
      57,    51,    79,    51,    79,    74,    74,    74,    74,    19,
      16,   128,   129,   128,   128,    52,   128,    49,   128,    48,
      48,     3,    28,    72,    73,   104,    69,     6,    52,    19,
      50,     3,     4,    57,   128,   128,   128,   128,     3,     3,
       4,     5,     6,     8,    53,    56,    74,   108,   130,   132,
     134,    55,    16,    55,    55,    74,    55,     3,     4,     5,
       8,    53,    74,   108,   117,   118,   119,   123,    30,    31,
      33,   127,    19,    51,    54,    74,     8,    32,    55,   122,
     122,   122,   122,    54,    19,    49,    54,   130,   131,   109,
      53,   123,   130,   123,   123,   128,   123,   123,   124,   125,
      53,    50,    51,     3,     5,    74,   126,     3,    73,    32,
     128,   117,   123,    32,     3,   131,    51,   133,    57,   134,
      55,    54,    51,    79,   123,   128,   119,    53,     9,    71,
     122,     9,    14,    24,    28,    37,    41,    50,    51,   130,
      54,    54,   123,   123,    54,     3,    71,    37,   104,   105,
     106,   107,    52,    52,    52,    32,    52,    54,    32,    52,
      52,    52,    52,    74,    74,    74,    28,    74,   104,    74,
      74,    74,    74,   128,   128,   128,    52,   128,    52,   128,
     128,   128,   128,    55,    55,    55,    74,    55,    74,    55,
      97,    55,    98,    55,    99,    55,   101,    53,    53,    53,
     128,    53,   128,    53,    80,    53,    88,    53,    84,    53,
      96,   111,   112,     5,   113,   114,   109,    55,     8,   120,
     121,    55,   100,     3,    74,    76,    77,    78,     4,    74,
      85,    86,    87,     5,    74,    81,    82,    83,     8,    74,
      93,    94,    95,    54,    79,    54,    51,    79,    54,    53,
      54,    51,    79,    53,    92,    53,    54,    51,    79,    53,
      54,    51,    79,    53,    54,    51,    79,    53,    54,    51,
      79,     5,     3,   108,   115,   116,     8,    74,    89,    90,
      91,   108,     3,    76,     3,    85,     3,    81,     3,    93,
      54,    51,    79,    53,    54,    51,    79,    54,    54,    54,
      54,   108,     3,    89,    54
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    58,    59,    60,    60,    61,    61,    62,    62,    63,
      63,    64,    64,    65,    65,    66,    67,    67,    68,    68,
      69,    70,    70,    70,    70,    71,    71,    71,    71,    71,
      72,    72,    73,    73,    74,    74,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    76,    76,    76,    77,
      77,    78,    78,    79,    79,    80,    81,    81,    81,    82,
      82,    83,    83,    84,    85,    85,    85,    86,    86,    87,
      87,    88,    89,    89,    89,    90,    90,    91,    91,    92,
      93,    93,    93,    94,    94,    95,    95,    96,    97,    97,
      98,    98,    99,    99,   100,   100,   101,   101,   102,   103,
     103,   104,   104,   104,   105,   105,   106,   106,   107,   107,
     107,   107,   108,   108,   109,   109,   110,   110,   111,   111,
     112,   112,   113,   113,   114,   114,   115,   115,   116,   116,
     117,   118,   118,   119,   119,   120,   120,   121,   121,   122,
     122,   123,   123,   123,   123,   123,   123,   123,   123,   124,
     124,   125,   125,   126,   126,   126,   126,   127,   127,   128,
     128,   129,   129,   130,   130,   131,   131,   132,   132,   132,
     133,   133,   134,   134,   134,   134,   134,   134,   134
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     0,     1,     2,     3,     0,     1,     2,
       3,     0,     1,     2,     3,     5,     0,     2,     1,     3,
       3,     6,     7,     2,     1,     1,     3,     1,     1,     1,
       1,     3,     1,     3,     1,     1,     6,     6,     6,     8,
       6,     6,     6,     6,     8,     6,    13,    13,    13,    15,
      13,    15,    15,    15,    17,    15,     1,     1,     4,     0,
       2,     1,     3,     0,     1,     3,     1,     1,     4,     0,
       2,     1,     3,     3,     1,     1,     4,     0,     2,     1,
       3,     3,     1,     1,     4,     0,     2,     1,     3,     3,
       1,     1,     4,     0,     2,     1,     3,     3,     0,     2,
       0,     2,     0,     2,     0,     2,     0,     2,     6,     3,
       4,     1,     3,     3,     1,     4,     1,     3,     1,     4,
       3,     6,     3,     3,     0,     2,     1,     3,     0,     2,
       1,     3,     0,     2,     1,     3,     0,     2,     1,     3,
       3,     1,     3,     1,     3,     0,     2,     1,     3,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     4,     0,
       2,     1,     3,     1,     1,     1,     4,     1,     1,     0,
       1,     2,     3,     4,     1,     1,     3,     1,     2,     4,
       0,     1,     1,     1,     1,     1,     1,     1,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (parm, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, parm); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void *parm)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (parm);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void *parm)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, parm);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule, void *parm)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              , parm);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, parm); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, void *parm)
{
  YYUSE (yyvaluep);
  YYUSE (parm);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *parm)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, YYLEX_PARAM);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 15:
#line 687 "gecode/flatzinc/parser.yxx"
      { free((yyvsp[-3].sValue)); }
#line 2178 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 20:
#line 699 "gecode/flatzinc/parser.yxx"
      { free((yyvsp[0].sValue)); }
#line 2184 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 25:
#line 709 "gecode/flatzinc/parser.yxx"
      { if ((yyvsp[0].oSet)()) delete (yyvsp[0].oSet).some(); }
#line 2190 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 26:
#line 711 "gecode/flatzinc/parser.yxx"
      { if ((yyvsp[0].oSet)()) delete (yyvsp[0].oSet).some(); }
#line 2196 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 36:
#line 732 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        bool print = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasAtom("output_var");
        bool funcDep = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasAtom("is_defined_var");
        yyassert(pp,
          pp->symbols.put((yyvsp[-2].sValue), se_iv(pp->intvars.size())),
          "Duplicate symbol");
        if (print) {
          pp->output(std::string((yyvsp[-2].sValue)), new AST::IntVar(pp->intvars.size()));
        }
        if ((yyvsp[0].oArg)()) {
          AST::Node* arg = (yyvsp[0].oArg).some();
          if (arg->isInt()) {
            pp->intvars.push_back(varspec((yyvsp[-2].sValue),
              new IntVarSpec(arg->getInt(),!print,funcDep)));
          } else if (arg->isIntVar()) {
            pp->intvars.push_back(varspec((yyvsp[-2].sValue),
              new IntVarSpec(Alias(arg->getIntVar()),!print,funcDep)));
          } else {
            yyassert(pp, false, "Invalid var int initializer");
          }
          if (!pp->hadError)
            addDomainConstraint(pp, "int_in",
                                new AST::IntVar(pp->intvars.size()-1), (yyvsp[-4].oSet));
          delete arg;
        } else {
          pp->intvars.push_back(varspec((yyvsp[-2].sValue),
            new IntVarSpec((yyvsp[-4].oSet),!print,funcDep)));
        }
        delete (yyvsp[-1].argVec); free((yyvsp[-2].sValue));
      }
#line 2232 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 37:
#line 764 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        bool print = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasAtom("output_var");
        bool funcDep = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasAtom("is_defined_var");
        yyassert(pp,
          pp->symbols.put((yyvsp[-2].sValue), se_bv(pp->boolvars.size())),
          "Duplicate symbol");
        if (print) {
          pp->output(std::string((yyvsp[-2].sValue)), new AST::BoolVar(pp->boolvars.size()));
        }
        if ((yyvsp[0].oArg)()) {
          AST::Node* arg = (yyvsp[0].oArg).some();
          if (arg->isBool()) {
            pp->boolvars.push_back(varspec((yyvsp[-2].sValue),
              new BoolVarSpec(arg->getBool(),!print,funcDep)));
          } else if (arg->isBoolVar()) {
            pp->boolvars.push_back(varspec((yyvsp[-2].sValue),
              new BoolVarSpec(Alias(arg->getBoolVar()),!print,funcDep)));
          } else {
            yyassert(pp, false, "Invalid var bool initializer");
          }
          if (!pp->hadError)
            addDomainConstraint(pp, "int_in",
                                new AST::BoolVar(pp->boolvars.size()-1), (yyvsp[-4].oSet));
          delete arg;
        } else {
          pp->boolvars.push_back(varspec((yyvsp[-2].sValue),
            new BoolVarSpec((yyvsp[-4].oSet),!print,funcDep)));
        }
        delete (yyvsp[-1].argVec); free((yyvsp[-2].sValue));
      }
#line 2268 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 38:
#line 796 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        bool print = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasAtom("output_var");
        bool funcDep = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasAtom("is_defined_var");
        yyassert(pp,
          pp->symbols.put((yyvsp[-2].sValue), se_fv(pp->floatvars.size())),
          "Duplicate symbol");
        if (print) {
          pp->output(std::string((yyvsp[-2].sValue)),
                     new AST::FloatVar(pp->floatvars.size()));
        }
        if ((yyvsp[0].oArg)()) {
          AST::Node* arg = (yyvsp[0].oArg).some();
          if (arg->isFloat()) {
            pp->floatvars.push_back(varspec((yyvsp[-2].sValue),
              new FloatVarSpec(arg->getFloat(),!print,funcDep)));
          } else if (arg->isFloatVar()) {
            pp->floatvars.push_back(varspec((yyvsp[-2].sValue),
              new FloatVarSpec(
                Alias(arg->getFloatVar()),!print,funcDep)));
          } else {
            yyassert(pp, false, "Invalid var float initializer");
          }
          if (!pp->hadError && (yyvsp[-4].oPFloat)()) {
            AST::FloatVar* fv = new AST::FloatVar(pp->floatvars.size()-1);
            addDomainConstraint(pp, fv, (yyvsp[-4].oPFloat));
          }
          delete arg;
        } else {
          Option<std::pair<double,double> > dom =
            (yyvsp[-4].oPFloat)() ? Option<std::pair<double,double> >::some(*(yyvsp[-4].oPFloat).some())
                 : Option<std::pair<double,double> >::none();
          if ((yyvsp[-4].oPFloat)()) delete (yyvsp[-4].oPFloat).some();
          pp->floatvars.push_back(varspec((yyvsp[-2].sValue),
            new FloatVarSpec(dom,!print,funcDep)));
        }
        delete (yyvsp[-1].argVec); free((yyvsp[-2].sValue));
      }
#line 2311 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 39:
#line 835 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        bool print = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasAtom("output_var");
        bool funcDep = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasAtom("is_defined_var");
        yyassert(pp,
          pp->symbols.put((yyvsp[-2].sValue), se_sv(pp->setvars.size())),
          "Duplicate symbol");
        if (print) {
          pp->output(std::string((yyvsp[-2].sValue)), new AST::SetVar(pp->setvars.size()));
        }
        if ((yyvsp[0].oArg)()) {
          AST::Node* arg = (yyvsp[0].oArg).some();
          if (arg->isSet()) {
            pp->setvars.push_back(varspec((yyvsp[-2].sValue),
              new SetVarSpec(arg->getSet(),!print,funcDep)));
          } else if (arg->isSetVar()) {
            pp->setvars.push_back(varspec((yyvsp[-2].sValue),
              new SetVarSpec(Alias(arg->getSetVar()),!print,funcDep)));
            delete arg;
          } else {
            yyassert(pp, false, "Invalid var set initializer");
            delete arg;
          }
          if (!pp->hadError)
            addDomainConstraint(pp, "set_subset",
                                new AST::SetVar(pp->setvars.size()-1), (yyvsp[-4].oSet));
        } else {
          pp->setvars.push_back(varspec((yyvsp[-2].sValue),
            new SetVarSpec((yyvsp[-4].oSet),!print,funcDep)));
        }
        delete (yyvsp[-1].argVec); free((yyvsp[-2].sValue));
      }
#line 2348 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 40:
#line 868 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        bool print = (yyvsp[-1].argVec)->hasAtom("output_var");
        bool introduced = (yyvsp[-1].argVec)->hasAtom("var_is_introduced");
        bool funcDep = (yyvsp[-1].argVec)->hasAtom("is_defined_var");
        yyassert(pp,
          pp->symbols.put((yyvsp[-2].sValue), se_tv(pp->stringvars.size())),
          "Duplicate symbol");
        if (print) {
          pp->output(std::string((yyvsp[-2].sValue)),
                     new AST::StringVar(pp->stringvars.size()));
        } else {
          introduced = true;
        }
        if ((yyvsp[0].oArg)()) {
          AST::Node* arg = (yyvsp[0].oArg).some();
          if (arg->isString()) {
            pp->stringvars.push_back(varspec((yyvsp[-2].sValue),
              new StringVarSpec(arg->getString(),introduced,funcDep)));
          } else if (arg->isStringVar()) {
            pp->stringvars.push_back(varspec((yyvsp[-2].sValue),
              new StringVarSpec(
                Alias(arg->getStringVar()),introduced,funcDep)));
          } else if (arg->isStringDom()) {
            pp->stringvars.push_back(varspec((yyvsp[-2].sValue),
              new StringVarSpec(arg->getStringDom()->v,introduced,funcDep)));
          }
          else {
            yyassert(pp, false, "Invalid var string initializer");
          }
          if (!pp->hadError && (yyvsp[-4].oPString)()) {
            AST::StringVar* tv = new AST::StringVar(pp->stringvars.size()-1);
            addDomainConstraint(pp, tv, (yyvsp[-4].oPString));
          }
          delete arg;
        } else {
          pp->stringvars.push_back(varspec((yyvsp[-2].sValue),
            new StringVarSpec((yyvsp[-4].oPString),introduced,funcDep)));
        }
        delete (yyvsp[-1].argVec); free((yyvsp[-2].sValue));
      }
#line 2394 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 41:
#line 910 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        yyassert(pp, (yyvsp[0].arg)->isInt(), "Invalid int initializer");
        yyassert(pp,
          pp->symbols.put((yyvsp[-3].sValue), se_i((yyvsp[0].arg)->getInt())),
          "Duplicate symbol");
        delete (yyvsp[-2].argVec); free((yyvsp[-3].sValue));
      }
#line 2407 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 42:
#line 919 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        yyassert(pp, (yyvsp[0].arg)->isFloat(), "Invalid float initializer");
        pp->floatvals.push_back((yyvsp[0].arg)->getFloat());
        yyassert(pp,
          pp->symbols.put((yyvsp[-3].sValue), se_f(pp->floatvals.size()-1)),
          "Duplicate symbol");
        delete (yyvsp[-2].argVec); free((yyvsp[-3].sValue));
      }
#line 2421 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 43:
#line 929 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        yyassert(pp, (yyvsp[0].arg)->isBool(), "Invalid bool initializer");
        yyassert(pp,
          pp->symbols.put((yyvsp[-3].sValue), se_b((yyvsp[0].arg)->getBool())),
          "Duplicate symbol");
        delete (yyvsp[-2].argVec); free((yyvsp[-3].sValue));
      }
#line 2434 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 44:
#line 938 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        yyassert(pp, (yyvsp[0].arg)->isSet(), "Invalid set initializer");
        AST::SetLit* set = (yyvsp[0].arg)->getSet();
        pp->setvals.push_back(*set);
        yyassert(pp,
          pp->symbols.put((yyvsp[-3].sValue), se_s(pp->setvals.size()-1)),
          "Duplicate symbol");
        delete set;
        delete (yyvsp[-2].argVec); free((yyvsp[-3].sValue));
      }
#line 2450 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 45:
#line 950 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        yyassert(pp, (yyvsp[0].arg)->isStringDom(), "Invalid string initializer");
        pp->stringvals.push_back((yyvsp[0].arg)->getString());
        yyassert(pp,
          pp->symbols.put((yyvsp[-3].sValue), se_t(pp->stringvals.size()-1)),
          "Duplicate symbol");
        delete (yyvsp[-2].argVec); free((yyvsp[-3].sValue));
      }
#line 2464 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 46:
#line 961 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        yyassert(pp, (yyvsp[-10].iValue)==1, "Arrays must start at 1");
        if (!pp->hadError) {
          bool print = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasCall("output_array");
          vector<int> vars((yyvsp[-8].iValue));
          if (!pp->hadError) {
            if ((yyvsp[0].oVarSpecVec)()) {
              vector<VarSpec*>* vsv = (yyvsp[0].oVarSpecVec).some();
              yyassert(pp, vsv->size() == static_cast<unsigned int>((yyvsp[-8].iValue)),
                       "Initializer size does not match array dimension");
              if (!pp->hadError) {
                for (int i=0; i<(yyvsp[-8].iValue); i++) {
                  IntVarSpec* ivsv = static_cast<IntVarSpec*>((*vsv)[i]);
                  if (ivsv->alias) {
                    if (print)
                      static_cast<IntVarSpec*>(pp->intvars[ivsv->i].second)->introduced = false;
                    vars[i] = ivsv->i;
                  } else {
                    if (print)
                      ivsv->introduced = false;
                    vars[i] = pp->intvars.size();
                    pp->intvars.push_back(varspec((yyvsp[-2].sValue), ivsv));
                  }
                  if (!pp->hadError && (yyvsp[-4].oSet)()) {
                    Option<AST::SetLit*> opt =
                      Option<AST::SetLit*>::some(new AST::SetLit(*(yyvsp[-4].oSet).some()));
                    addDomainConstraint(pp, "int_in",
                                        new AST::IntVar(vars[i]),
                                        opt);
                  }
                }
              }
              delete vsv;
            } else {
              if ((yyvsp[-8].iValue)>0) {
                for (int i=0; i<(yyvsp[-8].iValue); i++) {
                  Option<AST::SetLit*> dom =
                    (yyvsp[-4].oSet)() ? Option<AST::SetLit*>::some(new AST::SetLit((yyvsp[-4].oSet).some()))
                         : Option<AST::SetLit*>::none();
                  IntVarSpec* ispec = new IntVarSpec(dom,!print,false);
                  vars[i] = pp->intvars.size();
                  pp->intvars.push_back(varspec((yyvsp[-2].sValue), ispec));
                }
              }
              if ((yyvsp[-4].oSet)()) delete (yyvsp[-4].oSet).some();
            }
          }
          if (print) {
            AST::Array* a = new AST::Array();
            a->a.push_back(arrayOutput((yyvsp[-1].argVec)->getCall("output_array")));
            AST::Array* output = new AST::Array();
            for (int i=0; i<(yyvsp[-8].iValue); i++)
              output->a.push_back(new AST::IntVar(vars[i]));
            a->a.push_back(output);
            a->a.push_back(new AST::String(")"));
            pp->output(std::string((yyvsp[-2].sValue)), a);
          }
          int iva = pp->arrays.size();
          pp->arrays.push_back(vars.size());
          for (unsigned int i=0; i<vars.size(); i++)
            pp->arrays.push_back(vars[i]);
          yyassert(pp,
            pp->symbols.put((yyvsp[-2].sValue), se_iva(iva)),
            "Duplicate symbol");
        }
        delete (yyvsp[-1].argVec); free((yyvsp[-2].sValue));
      }
#line 2537 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 47:
#line 1031 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        bool print = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasCall("output_array");
        yyassert(pp, (yyvsp[-10].iValue)==1, "Arrays must start at 1");
        if (!pp->hadError) {
          vector<int> vars((yyvsp[-8].iValue));
          if ((yyvsp[0].oVarSpecVec)()) {
            vector<VarSpec*>* vsv = (yyvsp[0].oVarSpecVec).some();
            yyassert(pp, vsv->size() == static_cast<unsigned int>((yyvsp[-8].iValue)),
                     "Initializer size does not match array dimension");
            if (!pp->hadError) {
              for (int i=0; i<(yyvsp[-8].iValue); i++) {
                BoolVarSpec* bvsv = static_cast<BoolVarSpec*>((*vsv)[i]);
                if (bvsv->alias) {
                  if (print)
                    static_cast<BoolVarSpec*>(pp->boolvars[bvsv->i].second)->introduced = false;
                  vars[i] = bvsv->i;
                } else {
                  if (print)
                    bvsv->introduced = false;
                  vars[i] = pp->boolvars.size();
                  pp->boolvars.push_back(varspec((yyvsp[-2].sValue), (*vsv)[i]));
                }
                if (!pp->hadError && (yyvsp[-4].oSet)()) {
                  Option<AST::SetLit*> opt =
                    Option<AST::SetLit*>::some(new AST::SetLit(*(yyvsp[-4].oSet).some()));
                  addDomainConstraint(pp, "int_in",
                                      new AST::BoolVar(vars[i]),
                                      opt);
                }
              }
            }
            delete vsv;
          } else {
            for (int i=0; i<(yyvsp[-8].iValue); i++) {
              Option<AST::SetLit*> dom =
                (yyvsp[-4].oSet)() ? Option<AST::SetLit*>::some(new AST::SetLit((yyvsp[-4].oSet).some()))
                     : Option<AST::SetLit*>::none();
              vars[i] = pp->boolvars.size();
              pp->boolvars.push_back(varspec((yyvsp[-2].sValue),
                                       new BoolVarSpec(dom,!print,false)));
            }
            if ((yyvsp[-4].oSet)()) delete (yyvsp[-4].oSet).some();
          }
          if (print) {
            AST::Array* a = new AST::Array();
            a->a.push_back(arrayOutput((yyvsp[-1].argVec)->getCall("output_array")));
            AST::Array* output = new AST::Array();
            for (int i=0; i<(yyvsp[-8].iValue); i++)
              output->a.push_back(new AST::BoolVar(vars[i]));
            a->a.push_back(output);
            a->a.push_back(new AST::String(")"));
            pp->output(std::string((yyvsp[-2].sValue)), a);
          }
          int bva = pp->arrays.size();
          pp->arrays.push_back(vars.size());
          for (unsigned int i=0; i<vars.size(); i++)
            pp->arrays.push_back(vars[i]);
          yyassert(pp,
            pp->symbols.put((yyvsp[-2].sValue), se_bva(bva)),
            "Duplicate symbol");
        }
        delete (yyvsp[-1].argVec); free((yyvsp[-2].sValue));
      }
#line 2606 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 48:
#line 1098 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        yyassert(pp, (yyvsp[-10].iValue)==1, "Arrays must start at 1");
        if (!pp->hadError) {
          bool print = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasCall("output_array");
          vector<int> vars((yyvsp[-8].iValue));
          if (!pp->hadError) {
            if ((yyvsp[0].oVarSpecVec)()) {
              vector<VarSpec*>* vsv = (yyvsp[0].oVarSpecVec).some();
              yyassert(pp, vsv->size() == static_cast<unsigned int>((yyvsp[-8].iValue)),
                       "Initializer size does not match array dimension");
              if (!pp->hadError) {
                for (int i=0; i<(yyvsp[-8].iValue); i++) {
                  FloatVarSpec* ivsv = static_cast<FloatVarSpec*>((*vsv)[i]);
                  if (ivsv->alias) {
                    if (print)
                      static_cast<FloatVarSpec*>(pp->floatvars[ivsv->i].second)->introduced = false;
                    vars[i] = ivsv->i;
                  } else {
                    if (print)
                      ivsv->introduced = false;
                    vars[i] = pp->floatvars.size();
                    pp->floatvars.push_back(varspec((yyvsp[-2].sValue), ivsv));
                  }
                  if (!pp->hadError && (yyvsp[-4].oPFloat)()) {
                    Option<std::pair<double,double>*> opt =
                      Option<std::pair<double,double>*>::some(
                        new std::pair<double,double>(*(yyvsp[-4].oPFloat).some()));
                    addDomainConstraint(pp, new AST::FloatVar(vars[i]),
                                        opt);
                  }
                }
              }
              delete vsv;
            } else {
              if ((yyvsp[-8].iValue)>0) {
                Option<std::pair<double,double> > dom =
                  (yyvsp[-4].oPFloat)() ? Option<std::pair<double,double> >::some(*(yyvsp[-4].oPFloat).some())
                       : Option<std::pair<double,double> >::none();
                for (int i=0; i<(yyvsp[-8].iValue); i++) {
                  FloatVarSpec* ispec = new FloatVarSpec(dom,!print,false);
                  vars[i] = pp->floatvars.size();
                  pp->floatvars.push_back(varspec((yyvsp[-2].sValue), ispec));
                }
              }
            }
          }
          if (print) {
            AST::Array* a = new AST::Array();
            a->a.push_back(arrayOutput((yyvsp[-1].argVec)->getCall("output_array")));
            AST::Array* output = new AST::Array();
            for (int i=0; i<(yyvsp[-8].iValue); i++)
              output->a.push_back(new AST::FloatVar(vars[i]));
            a->a.push_back(output);
            a->a.push_back(new AST::String(")"));
            pp->output(std::string((yyvsp[-2].sValue)), a);
          }
          int fva = pp->arrays.size();
          pp->arrays.push_back(vars.size());
          for (unsigned int i=0; i<vars.size(); i++)
            pp->arrays.push_back(vars[i]);
          yyassert(pp,
            pp->symbols.put((yyvsp[-2].sValue), se_fva(fva)),
            "Duplicate symbol");
        }
        if ((yyvsp[-4].oPFloat)()) delete (yyvsp[-4].oPFloat).some();
        delete (yyvsp[-1].argVec); free((yyvsp[-2].sValue));
      }
#line 2679 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 49:
#line 1168 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        bool print = (yyvsp[-1].argVec) != NULL && (yyvsp[-1].argVec)->hasCall("output_array");
        yyassert(pp, (yyvsp[-12].iValue)==1, "Arrays must start at 1");
        if (!pp->hadError) {
          vector<int> vars((yyvsp[-10].iValue));
          if ((yyvsp[0].oVarSpecVec)()) {
            vector<VarSpec*>* vsv = (yyvsp[0].oVarSpecVec).some();
            yyassert(pp, vsv->size() == static_cast<unsigned int>((yyvsp[-10].iValue)),
                     "Initializer size does not match array dimension");
            if (!pp->hadError) {
              for (int i=0; i<(yyvsp[-10].iValue); i++) {
                SetVarSpec* svsv = static_cast<SetVarSpec*>((*vsv)[i]);
                if (svsv->alias) {
                  if (print)
                    static_cast<SetVarSpec*>(pp->setvars[svsv->i].second)->introduced = false;
                  vars[i] = svsv->i;
                } else {
                  if (print)
                    svsv->introduced = false;
                  vars[i] = pp->setvars.size();
                  pp->setvars.push_back(varspec((yyvsp[-2].sValue), (*vsv)[i]));
                }
                if (!pp->hadError && (yyvsp[-4].oSet)()) {
                  Option<AST::SetLit*> opt =
                    Option<AST::SetLit*>::some(new AST::SetLit(*(yyvsp[-4].oSet).some()));
                  addDomainConstraint(pp, "set_subset",
                                      new AST::SetVar(vars[i]),
                                      opt);
                }
              }
            }
            delete vsv;
          } else {
            if ((yyvsp[-10].iValue)>0) {
              for (int i=0; i<(yyvsp[-10].iValue); i++) {
                Option<AST::SetLit*> dom =
                  (yyvsp[-4].oSet)() ? Option<AST::SetLit*>::some(new AST::SetLit((yyvsp[-4].oSet).some()))
                        : Option<AST::SetLit*>::none();
                SetVarSpec* ispec = new SetVarSpec(dom,!print,false);
                vars[i] = pp->setvars.size();
                pp->setvars.push_back(varspec((yyvsp[-2].sValue), ispec));
              }
              if ((yyvsp[-4].oSet)()) delete (yyvsp[-4].oSet).some();
            }
          }
          if (print) {
            AST::Array* a = new AST::Array();
            a->a.push_back(arrayOutput((yyvsp[-1].argVec)->getCall("output_array")));
            AST::Array* output = new AST::Array();
            for (int i=0; i<(yyvsp[-10].iValue); i++)
              output->a.push_back(new AST::SetVar(vars[i]));
            a->a.push_back(output);
            a->a.push_back(new AST::String(")"));
            pp->output(std::string((yyvsp[-2].sValue)), a);
          }
          int sva = pp->arrays.size();
          pp->arrays.push_back(vars.size());
          for (unsigned int i=0; i<vars.size(); i++)
            pp->arrays.push_back(vars[i]);
          yyassert(pp,
            pp->symbols.put((yyvsp[-2].sValue), se_sva(sva)),
            "Duplicate symbol");
        }
        delete (yyvsp[-1].argVec); free((yyvsp[-2].sValue));
      }
#line 2750 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 50:
#line 1237 "gecode/flatzinc/parser.yxx"
       {
         ParserState* pp = static_cast<ParserState*>(parm);
         yyassert(pp, (yyvsp[-10].iValue)==1, "Arrays must start at 1");
         if (!pp->hadError) {
           bool print = (yyvsp[-1].argVec)->hasCall("output_array");
           vector<int> vars((yyvsp[-8].iValue));
           if (!pp->hadError) {
             if ((yyvsp[0].oVarSpecVec)()) {
               vector<VarSpec*>* vsv = (yyvsp[0].oVarSpecVec).some();
               yyassert(pp, vsv->size() == static_cast<unsigned int>((yyvsp[-8].iValue)),
                        "Initializer size does not match array dimension");
               if (!pp->hadError) {
                 for (int i=0; i<(yyvsp[-8].iValue); i++) {
                   StringVarSpec* tvsv = static_cast<StringVarSpec*>((*vsv)[i]);
                   if (tvsv->alias) {
                     if (print)
                       static_cast<StringVarSpec*>(pp->stringvars[tvsv->i].second)->introduced = false;
                     vars[i] = tvsv->i;
                   } else {
                     if (print)
                       tvsv->introduced = false;
                     vars[i] = pp->stringvars.size();
                     pp->stringvars.push_back(varspec((yyvsp[-2].sValue), tvsv));
                   }
                   if (!pp->hadError && (yyvsp[-4].oPString)()) {
                     Option<AST::StringDom*> opt =
                       Option<AST::StringDom*>::some(
                         new AST::StringDom(*(yyvsp[-4].oPString).some()));
                     addDomainConstraint(pp, new AST::StringVar(vars[i]),
                                         opt);
                   }
                 }
               }
               delete vsv;
             } else {
               if ((yyvsp[-8].iValue)>0) {
                 Option<AST::StringDom*> dom =
                   (yyvsp[-4].oPString)() ? Option<AST::StringDom*>::some(new AST::StringDom(*(yyvsp[-4].oPString).some()))
                        : Option<AST::StringDom*>::none();
                 for (int i=0; i<(yyvsp[-8].iValue); i++) {
                   StringVarSpec* ispec = new StringVarSpec(dom,!print,false);
                   vars[i] = pp->stringvars.size();
                   pp->stringvars.push_back(varspec((yyvsp[-2].sValue), ispec));
                 }
               }
             }
           }
           if (print) {
             AST::Array* a = new AST::Array();
             a->a.push_back(arrayOutput((yyvsp[-1].argVec)->getCall("output_array")));
             AST::Array* output = new AST::Array();
             for (int i=0; i<(yyvsp[-8].iValue); i++)
               output->a.push_back(new AST::StringVar(vars[i]));
             a->a.push_back(output);
             a->a.push_back(new AST::String(")"));
             pp->output(std::string((yyvsp[-2].sValue)), a);
           }
           int tva = pp->arrays.size();
           pp->arrays.push_back(vars.size());
           for (unsigned int i=0; i<vars.size(); i++)
             pp->arrays.push_back(vars[i]);
           yyassert(pp,
             pp->symbols.put((yyvsp[-2].sValue), se_tva(tva)),
             "Duplicate symbol");
         }
         if ((yyvsp[-4].oPString)()) delete (yyvsp[-4].oPString).some();
         delete (yyvsp[-1].argVec); free((yyvsp[-2].sValue));
       }
#line 2823 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 51:
#line 1307 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        yyassert(pp, (yyvsp[-12].iValue)==1, "Arrays must start at 1");
        yyassert(pp, (yyvsp[-1].setValue)->size() == static_cast<unsigned int>((yyvsp[-10].iValue)),
                 "Initializer size does not match array dimension");

        if (!pp->hadError) {
          int ia = pp->arrays.size();
          pp->arrays.push_back((yyvsp[-1].setValue)->size());
          for (unsigned int i=0; i<(yyvsp[-1].setValue)->size(); i++)
            pp->arrays.push_back((*(yyvsp[-1].setValue))[i]);
          yyassert(pp,
            pp->symbols.put((yyvsp[-5].sValue), se_ia(ia)),
            "Duplicate symbol");
        }
        delete (yyvsp[-1].setValue);
        free((yyvsp[-5].sValue));
        delete (yyvsp[-4].argVec);
      }
#line 2847 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 52:
#line 1328 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        yyassert(pp, (yyvsp[-12].iValue)==1, "Arrays must start at 1");
        yyassert(pp, (yyvsp[-1].setValue)->size() == static_cast<unsigned int>((yyvsp[-10].iValue)),
                 "Initializer size does not match array dimension");
        if (!pp->hadError) {
          int ia = pp->arrays.size();
          pp->arrays.push_back((yyvsp[-1].setValue)->size());
          for (unsigned int i=0; i<(yyvsp[-1].setValue)->size(); i++)
            pp->arrays.push_back((*(yyvsp[-1].setValue))[i]);
          yyassert(pp,
            pp->symbols.put((yyvsp[-5].sValue), se_ba(ia)),
            "Duplicate symbol");
        }
        delete (yyvsp[-1].setValue);
        free((yyvsp[-5].sValue));
        delete (yyvsp[-4].argVec);
      }
#line 2870 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 53:
#line 1348 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        yyassert(pp, (yyvsp[-12].iValue)==1, "Arrays must start at 1");
        yyassert(pp, (yyvsp[-1].floatSetValue)->size() == static_cast<unsigned int>((yyvsp[-10].iValue)),
                 "Initializer size does not match array dimension");
        if (!pp->hadError) {
          int fa = pp->arrays.size();
          pp->arrays.push_back((yyvsp[-1].floatSetValue)->size());
          pp->arrays.push_back(pp->floatvals.size());
          for (unsigned int i=0; i<(yyvsp[-1].floatSetValue)->size(); i++)
            pp->floatvals.push_back((*(yyvsp[-1].floatSetValue))[i]);
          yyassert(pp,
            pp->symbols.put((yyvsp[-5].sValue), se_fa(fa)),
            "Duplicate symbol");
        }
        delete (yyvsp[-1].floatSetValue);
        delete (yyvsp[-4].argVec); free((yyvsp[-5].sValue));
      }
#line 2893 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 54:
#line 1368 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        yyassert(pp, (yyvsp[-14].iValue)==1, "Arrays must start at 1");
        yyassert(pp, (yyvsp[-1].setValueList)->size() == static_cast<unsigned int>((yyvsp[-12].iValue)),
                 "Initializer size does not match array dimension");
        if (!pp->hadError) {
          int sa = pp->arrays.size();
          pp->arrays.push_back((yyvsp[-1].setValueList)->size());
          pp->arrays.push_back(pp->setvals.size());
          for (unsigned int i=0; i<(yyvsp[-1].setValueList)->size(); i++)
            pp->setvals.push_back((*(yyvsp[-1].setValueList))[i]);
          yyassert(pp,
            pp->symbols.put((yyvsp[-5].sValue), se_sa(sa)),
            "Duplicate symbol");
        }
        delete (yyvsp[-1].setValueList);
        delete (yyvsp[-4].argVec); free((yyvsp[-5].sValue));
      }
#line 2916 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 55:
#line 1388 "gecode/flatzinc/parser.yxx"
       {
         ParserState* pp = static_cast<ParserState*>(parm);
         yyassert(pp, (yyvsp[-12].iValue)==1, "Arrays must start at 1");
         yyassert(pp, (yyvsp[-1].stringSetValue)->size() == static_cast<unsigned int>((yyvsp[-10].iValue)),
                  "Initializer size does not match array dimension");
         if (!pp->hadError) {
           int ta = pp->arrays.size();
           pp->arrays.push_back((yyvsp[-1].stringSetValue)->size());
           pp->arrays.push_back(pp->stringvals.size());
           for (unsigned int i=0; i<(yyvsp[-1].stringSetValue)->size(); i++)
             pp->stringvals.push_back(str2vec((*(yyvsp[-1].stringSetValue))[i]));
           yyassert(pp,
             pp->symbols.put((yyvsp[-5].sValue), se_ta(ta)),
             "Duplicate symbol");
         }
         delete (yyvsp[-1].stringSetValue);
         delete (yyvsp[-4].argVec); free((yyvsp[-5].sValue));
       }
#line 2939 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 56:
#line 1409 "gecode/flatzinc/parser.yxx"
      {
        (yyval.varSpec) = new IntVarSpec((yyvsp[0].iValue),false,false);
      }
#line 2947 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 57:
#line 1413 "gecode/flatzinc/parser.yxx"
      {
        SymbolEntry e;
        ParserState* pp = static_cast<ParserState*>(parm);
        if (pp->symbols.get((yyvsp[0].sValue), e) && e.t == ST_INTVAR)
          (yyval.varSpec) = new IntVarSpec(Alias(e.i),false,false);
        else {
          pp->err << "Error: undefined identifier for type int " << (yyvsp[0].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
          (yyval.varSpec) = new IntVarSpec(0,false,false); // keep things consistent
        }
        free((yyvsp[0].sValue));
      }
#line 2966 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 58:
#line 1428 "gecode/flatzinc/parser.yxx"
      {
        vector<int> v;
        SymbolEntry e;
        ParserState* pp = static_cast<ParserState*>(parm);
        if (pp->symbols.get((yyvsp[-3].sValue), e) && e.t == ST_INTVARARRAY) {
          yyassert(pp,(yyvsp[-1].iValue) > 0 && (yyvsp[-1].iValue) <= pp->arrays[e.i],
                   "array access out of bounds");
          if (!pp->hadError)
            (yyval.varSpec) = new IntVarSpec(Alias(pp->arrays[e.i+(yyvsp[-1].iValue)]),false,false);
          else
            (yyval.varSpec) = new IntVarSpec(0,false,false); // keep things consistent
        } else {
          pp->err << "Error: undefined array identifier for type int " << (yyvsp[-3].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
          (yyval.varSpec) = new IntVarSpec(0,false,false); // keep things consistent
        }
        free((yyvsp[-3].sValue));
      }
#line 2991 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 59:
#line 1451 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = new vector<VarSpec*>(0); }
#line 2997 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 60:
#line 1453 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = (yyvsp[-1].varSpecVec); }
#line 3003 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 61:
#line 1457 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = new vector<VarSpec*>(1); (*(yyval.varSpecVec))[0] = (yyvsp[0].varSpec); }
#line 3009 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 62:
#line 1459 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = (yyvsp[-2].varSpecVec); (yyval.varSpecVec)->push_back((yyvsp[0].varSpec)); }
#line 3015 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 65:
#line 1464 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = (yyvsp[-1].varSpecVec); }
#line 3021 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 66:
#line 1468 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpec) = new FloatVarSpec((yyvsp[0].dValue),false,false); }
#line 3027 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 67:
#line 1470 "gecode/flatzinc/parser.yxx"
      {
        SymbolEntry e;
        ParserState* pp = static_cast<ParserState*>(parm);
        if (pp->symbols.get((yyvsp[0].sValue), e) && e.t == ST_FLOATVAR)
          (yyval.varSpec) = new FloatVarSpec(Alias(e.i),false,false);
        else {
          pp->err << "Error: undefined identifier for type float " << (yyvsp[0].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
          (yyval.varSpec) = new FloatVarSpec(0.0,false,false);
        }
        free((yyvsp[0].sValue));
      }
#line 3046 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 68:
#line 1485 "gecode/flatzinc/parser.yxx"
      {
        SymbolEntry e;
        ParserState* pp = static_cast<ParserState*>(parm);
        if (pp->symbols.get((yyvsp[-3].sValue), e) && e.t == ST_FLOATVARARRAY) {
          yyassert(pp,(yyvsp[-1].iValue) > 0 && (yyvsp[-1].iValue) <= pp->arrays[e.i],
                   "array access out of bounds");
          if (!pp->hadError)
            (yyval.varSpec) = new FloatVarSpec(Alias(pp->arrays[e.i+(yyvsp[-1].iValue)]),false,false);
          else
            (yyval.varSpec) = new FloatVarSpec(0.0,false,false);
        } else {
          pp->err << "Error: undefined array identifier for type float " << (yyvsp[-3].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
          (yyval.varSpec) = new FloatVarSpec(0.0,false,false);
        }
        free((yyvsp[-3].sValue));
      }
#line 3070 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 69:
#line 1507 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = new vector<VarSpec*>(0); }
#line 3076 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 70:
#line 1509 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = (yyvsp[-1].varSpecVec); }
#line 3082 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 71:
#line 1513 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = new vector<VarSpec*>(1); (*(yyval.varSpecVec))[0] = (yyvsp[0].varSpec); }
#line 3088 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 72:
#line 1515 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = (yyvsp[-2].varSpecVec); (yyval.varSpecVec)->push_back((yyvsp[0].varSpec)); }
#line 3094 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 73:
#line 1519 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = (yyvsp[-1].varSpecVec); }
#line 3100 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 74:
#line 1523 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpec) = new BoolVarSpec((yyvsp[0].iValue),false,false); }
#line 3106 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 75:
#line 1525 "gecode/flatzinc/parser.yxx"
      {
        SymbolEntry e;
        ParserState* pp = static_cast<ParserState*>(parm);
        if (pp->symbols.get((yyvsp[0].sValue), e) && e.t == ST_BOOLVAR)
          (yyval.varSpec) = new BoolVarSpec(Alias(e.i),false,false);
        else {
          pp->err << "Error: undefined identifier for type bool " << (yyvsp[0].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
          (yyval.varSpec) = new BoolVarSpec(false,false,false);
        }
        free((yyvsp[0].sValue));
      }
#line 3125 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 76:
#line 1540 "gecode/flatzinc/parser.yxx"
      {
        SymbolEntry e;
        ParserState* pp = static_cast<ParserState*>(parm);
        if (pp->symbols.get((yyvsp[-3].sValue), e) && e.t == ST_BOOLVARARRAY) {
          yyassert(pp,(yyvsp[-1].iValue) > 0 && (yyvsp[-1].iValue) <= pp->arrays[e.i],
                   "array access out of bounds");
          if (!pp->hadError)
            (yyval.varSpec) = new BoolVarSpec(Alias(pp->arrays[e.i+(yyvsp[-1].iValue)]),false,false);
          else
            (yyval.varSpec) = new BoolVarSpec(false,false,false);
        } else {
          pp->err << "Error: undefined array identifier for type bool " << (yyvsp[-3].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
          (yyval.varSpec) = new BoolVarSpec(false,false,false);
        }
        free((yyvsp[-3].sValue));
      }
#line 3149 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 77:
#line 1562 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = new vector<VarSpec*>(0); }
#line 3155 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 78:
#line 1564 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = (yyvsp[-1].varSpecVec); }
#line 3161 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 79:
#line 1568 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = new vector<VarSpec*>(1); (*(yyval.varSpecVec))[0] = (yyvsp[0].varSpec); }
#line 3167 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 80:
#line 1570 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = (yyvsp[-2].varSpecVec); (yyval.varSpecVec)->push_back((yyvsp[0].varSpec)); }
#line 3173 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 81:
#line 1572 "gecode/flatzinc/parser.yxx"
                                                { (yyval.varSpecVec) = (yyvsp[-1].varSpecVec); }
#line 3179 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 82:
#line 1576 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpec) = new SetVarSpec((yyvsp[0].setLit),false,false); }
#line 3185 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 83:
#line 1578 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        SymbolEntry e;
        if (pp->symbols.get((yyvsp[0].sValue), e) && e.t == ST_SETVAR)
          (yyval.varSpec) = new SetVarSpec(Alias(e.i),false,false);
        else {
          pp->err << "Error: undefined identifier for type set " << (yyvsp[0].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
          (yyval.varSpec) = new SetVarSpec(Alias(0),false,false);
        }
        free((yyvsp[0].sValue));
      }
#line 3204 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 84:
#line 1593 "gecode/flatzinc/parser.yxx"
      {
        SymbolEntry e;
        ParserState* pp = static_cast<ParserState*>(parm);
        if (pp->symbols.get((yyvsp[-3].sValue), e) && e.t == ST_SETVARARRAY) {
          yyassert(pp,(yyvsp[-1].iValue) > 0 && (yyvsp[-1].iValue) <= pp->arrays[e.i],
                   "array access out of bounds");
          if (!pp->hadError)
            (yyval.varSpec) = new SetVarSpec(Alias(pp->arrays[e.i+(yyvsp[-1].iValue)]),false,false);
          else
            (yyval.varSpec) = new SetVarSpec(Alias(0),false,false);
        } else {
          pp->err << "Error: undefined array identifier for type set " << (yyvsp[-3].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
          (yyval.varSpec) = new SetVarSpec(Alias(0),false,false);
        }
        free((yyvsp[-3].sValue));
      }
#line 3228 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 85:
#line 1615 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = new vector<VarSpec*>(0); }
#line 3234 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 86:
#line 1617 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = (yyvsp[-1].varSpecVec); }
#line 3240 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 87:
#line 1621 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = new vector<VarSpec*>(1); (*(yyval.varSpecVec))[0] = (yyvsp[0].varSpec); }
#line 3246 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 88:
#line 1623 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = (yyvsp[-2].varSpecVec); (yyval.varSpecVec)->push_back((yyvsp[0].varSpec)); }
#line 3252 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 89:
#line 1626 "gecode/flatzinc/parser.yxx"
      { (yyval.varSpecVec) = (yyvsp[-1].varSpecVec); }
#line 3258 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 90:
#line 1630 "gecode/flatzinc/parser.yxx"
     { (yyval.varSpec) = new StringVarSpec(str2vec((yyvsp[0].sValue)),false,false); }
#line 3264 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 91:
#line 1632 "gecode/flatzinc/parser.yxx"
     {
       SymbolEntry e;
       ParserState* pp = static_cast<ParserState*>(parm);
       bool b = pp->symbols.get((yyvsp[0].sValue), e);
       if (pp->symbols.get((yyvsp[0].sValue), e) && e.t == ST_STRINGVAR)
         (yyval.varSpec) = new StringVarSpec(Alias(e.i),false,false);
       else if (b && e.t == ST_STRING)
         (yyval.varSpec) = new StringVarSpec(pp->stringvals[e.i], false, false);
       else {
         pp->err << "Error: undefined identifier for type string " << (yyvsp[0].sValue)
                 << " in line no. "
                 << yyget_lineno(pp->yyscanner) << std::endl;
         pp->hadError = true;
         (yyval.varSpec) = new StringVarSpec(0,false,false);
       }
       free((yyvsp[0].sValue));
     }
#line 3286 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 92:
#line 1650 "gecode/flatzinc/parser.yxx"
     {
       SymbolEntry e;
       ParserState* pp = static_cast<ParserState*>(parm);
       if (pp->symbols.get((yyvsp[-3].sValue), e) && e.t == ST_STRINGVARARRAY) {
         yyassert(pp,(yyvsp[-1].iValue) > 0 && (yyvsp[-1].iValue) <= pp->arrays[e.i],
                  "array access out of bounds");
         if (!pp->hadError)
           (yyval.varSpec) = new StringVarSpec(Alias(pp->arrays[e.i+(yyvsp[-1].iValue)]),false,false);
         else
           (yyval.varSpec) = new StringVarSpec(0,false,false);
       } else {
         pp->err << "Error: undefined array identifier for type string " << (yyvsp[-3].sValue)
                 << " in line no. "
                 << yyget_lineno(pp->yyscanner) << std::endl;
         pp->hadError = true;
         (yyval.varSpec) = new StringVarSpec(0,false,false);
       }
       free((yyvsp[-3].sValue));
     }
#line 3310 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 93:
#line 1672 "gecode/flatzinc/parser.yxx"
     { (yyval.varSpecVec) = new vector<VarSpec*>(0); }
#line 3316 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 94:
#line 1674 "gecode/flatzinc/parser.yxx"
     { (yyval.varSpecVec) = (yyvsp[-1].varSpecVec); }
#line 3322 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 95:
#line 1678 "gecode/flatzinc/parser.yxx"
     { (yyval.varSpecVec) = new vector<VarSpec*>(1); (*(yyval.varSpecVec))[0] = (yyvsp[0].varSpec); }
#line 3328 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 96:
#line 1680 "gecode/flatzinc/parser.yxx"
     { (yyval.varSpecVec) = (yyvsp[-2].varSpecVec); (yyval.varSpecVec)->push_back((yyvsp[0].varSpec)); }
#line 3334 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 97:
#line 1684 "gecode/flatzinc/parser.yxx"
     { (yyval.varSpecVec) = (yyvsp[-1].varSpecVec); }
#line 3340 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 98:
#line 1688 "gecode/flatzinc/parser.yxx"
      { (yyval.oVarSpecVec) = Option<vector<VarSpec*>* >::none(); }
#line 3346 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 99:
#line 1690 "gecode/flatzinc/parser.yxx"
      { (yyval.oVarSpecVec) = Option<vector<VarSpec*>* >::some((yyvsp[0].varSpecVec)); }
#line 3352 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 100:
#line 1694 "gecode/flatzinc/parser.yxx"
      { (yyval.oVarSpecVec) = Option<vector<VarSpec*>* >::none(); }
#line 3358 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 101:
#line 1696 "gecode/flatzinc/parser.yxx"
      { (yyval.oVarSpecVec) = Option<vector<VarSpec*>* >::some((yyvsp[0].varSpecVec)); }
#line 3364 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 102:
#line 1700 "gecode/flatzinc/parser.yxx"
      { (yyval.oVarSpecVec) = Option<vector<VarSpec*>* >::none(); }
#line 3370 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 103:
#line 1702 "gecode/flatzinc/parser.yxx"
      { (yyval.oVarSpecVec) = Option<vector<VarSpec*>* >::some((yyvsp[0].varSpecVec)); }
#line 3376 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 104:
#line 1706 "gecode/flatzinc/parser.yxx"
      { (yyval.oVarSpecVec) = Option<vector<VarSpec*>* >::none(); }
#line 3382 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 105:
#line 1708 "gecode/flatzinc/parser.yxx"
      { (yyval.oVarSpecVec) = Option<vector<VarSpec*>* >::some((yyvsp[0].varSpecVec)); }
#line 3388 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 106:
#line 1712 "gecode/flatzinc/parser.yxx"
      { (yyval.oVarSpecVec) = Option<vector<VarSpec*>* >::none(); }
#line 3394 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 107:
#line 1714 "gecode/flatzinc/parser.yxx"
      { (yyval.oVarSpecVec) = Option<vector<VarSpec*>* >::some((yyvsp[0].varSpecVec)); }
#line 3400 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 108:
#line 1719 "gecode/flatzinc/parser.yxx"
      {
        ParserState *pp = static_cast<ParserState*>(parm);
        if (!pp->hadError) {
          std::string cid((yyvsp[-4].sValue));
          if (cid=="int_eq" && (yyvsp[-2].argVec)->a[0]->isIntVar() && (yyvsp[-2].argVec)->a[1]->isIntVar()) {
            int base0 = getBaseIntVar(pp,(yyvsp[-2].argVec)->a[0]->getIntVar());
            int base1 = getBaseIntVar(pp,(yyvsp[-2].argVec)->a[1]->getIntVar());
            if (base0 > base1) {
              std::swap(base0, base1);
            }
            if (base0==base1) {
              // do nothing, already aliased
            } else {
              IntVarSpec* ivs1 = static_cast<IntVarSpec*>(pp->intvars[base1].second);
              AST::SetLit* sl = NULL;
              if (ivs1->assigned) {
                sl = new AST::SetLit(ivs1->i,ivs1->i);
              } else if (ivs1->domain()) {
                sl = new AST::SetLit(ivs1->domain.some()->getSet());
              }
              if (sl) {
                Option<AST::SetLit*> newdom = Option<AST::SetLit*>::some(sl);
                addDomainConstraint(pp, "int_in",
                                    new AST::IntVar(base0), newdom);
                ivs1->domain = Option<AST::SetLit*>::none();
              }
              ivs1->alias = true;
              ivs1->i = base0;
            }
          } else if (cid=="bool_eq" && (yyvsp[-2].argVec)->a[0]->isBoolVar() && (yyvsp[-2].argVec)->a[1]->isBoolVar()) {
            int base0 = getBaseBoolVar(pp,(yyvsp[-2].argVec)->a[0]->getBoolVar());
            int base1 = getBaseBoolVar(pp,(yyvsp[-2].argVec)->a[1]->getBoolVar());
            if (base0 > base1) {
              std::swap(base0, base1);
            }
            if (base0==base1) {
              // do nothing, already aliased
            } else {
              BoolVarSpec* ivs1 = static_cast<BoolVarSpec*>(pp->boolvars[base1].second);
              AST::SetLit* sl = NULL;
              if (ivs1->assigned) {
                sl = new AST::SetLit(ivs1->i,ivs1->i);
              } else if (ivs1->domain()) {
                sl = new AST::SetLit(ivs1->domain.some()->getSet());
              }
              if (sl) {
                Option<AST::SetLit*> newdom = Option<AST::SetLit*>::some(sl);
                addDomainConstraint(pp, "int_in",
                                    new AST::BoolVar(base0), newdom);
                ivs1->domain = Option<AST::SetLit*>::none();
              }
              ivs1->alias = true;
              ivs1->i = base0;
            }
          } else if (cid=="float_eq" && (yyvsp[-2].argVec)->a[0]->isFloatVar() && (yyvsp[-2].argVec)->a[1]->isFloatVar()) {
            int base0 = getBaseFloatVar(pp,(yyvsp[-2].argVec)->a[0]->getFloatVar());
            int base1 = getBaseFloatVar(pp,(yyvsp[-2].argVec)->a[1]->getFloatVar());
            if (base0 > base1) {
              std::swap(base0, base1);
            }
            if (base0==base1) {
              // do nothing, already aliased
            } else {
              FloatVarSpec* ivs1 = static_cast<FloatVarSpec*>(pp->floatvars[base1].second);
              ivs1->alias = true;
              ivs1->i = base0;
              if (ivs1->domain()) {
                std::pair<double,double>* dom = new std::pair<double,double>(ivs1->domain.some());
                addDomainConstraint(pp, new AST::FloatVar(base0), Option<std::pair<double,double>* >::some(dom));
                ivs1->domain = Option<std::pair<double,double> >::none();
              }
            }
          } else if (cid=="set_eq" && (yyvsp[-2].argVec)->a[0]->isSetVar() && (yyvsp[-2].argVec)->a[1]->isSetVar()) {
            int base0 = getBaseSetVar(pp,(yyvsp[-2].argVec)->a[0]->getSetVar());
            int base1 = getBaseSetVar(pp,(yyvsp[-2].argVec)->a[1]->getSetVar());
            if (base0 > base1) {
              std::swap(base0, base1);
            }
            if (base0==base1) {
              // do nothing, already aliased
            } else {
              SetVarSpec* ivs1 = static_cast<SetVarSpec*>(pp->setvars[base1].second);
              ivs1->alias = true;
              ivs1->i = base0;
              if (ivs1->upperBound()) {
                AST::SetLit* sl = new AST::SetLit(ivs1->upperBound.some()->getSet());
                Option<AST::SetLit*> newdom = Option<AST::SetLit*>::some(sl);
                if (ivs1->assigned) {
                  addDomainConstraint(pp, "set_eq",
                                      new AST::SetVar(base0), newdom);
                } else {
                  addDomainConstraint(pp, "set_subset",
                                      new AST::SetVar(base0), newdom);
                }
                ivs1->upperBound = Option<AST::SetLit*>::none();
              }
            }
          } 
          else if (cid=="string_eq" && (yyvsp[-2].argVec)->a[0]->isStringVar() && (yyvsp[-2].argVec)->a[1]->isStringVar()) {
            int base0 = getBaseStringVar(pp,(yyvsp[-2].argVec)->a[0]->getStringVar());
            int base1 = getBaseStringVar(pp,(yyvsp[-2].argVec)->a[1]->getStringVar());
            if (base0 > base1) {
              std::swap(base0, base1);
            }
            if (base0==base1) {
              // do nothing, already aliased
            } else {
              StringVarSpec* ivs1 = static_cast<StringVarSpec*>(pp->stringvars[base1].second);
              ivs1->alias = true;
              ivs1->i = base0;
              if (ivs1->domain()) {
                AST::StringDom* sl = new AST::StringDom(ivs1->domain.some()->getStringDom());
                Option<AST::StringDom*> newdom = Option<AST::StringDom*>::some(sl);
                addDomainConstraint(pp, new AST::StringVar(base0), newdom);
                }
                ivs1->domain = Option<AST::StringDom*>::none();
              }
            }          
          else if ( (cid=="int_le" || cid=="int_lt" || cid=="int_ge" || cid=="int_gt"  ||
                       cid=="int_eq" || cid=="int_ne") &&
                      ((yyvsp[-2].argVec)->a[0]->isInt() || (yyvsp[-2].argVec)->a[1]->isInt()) ) {
            pp->domainConstraints.push_back(new ConExpr((yyvsp[-4].sValue), (yyvsp[-2].argVec), (yyvsp[0].argVec)));
          } else if ( cid=="set_in" && ((yyvsp[-2].argVec)->a[0]->isSet() || (yyvsp[-2].argVec)->a[1]->isSet()) ) {
            pp->domainConstraints.push_back(new ConExpr((yyvsp[-4].sValue), (yyvsp[-2].argVec), (yyvsp[0].argVec)));
          } 
//TODO:          else if (cid=="str_reg" && DashedString::_DECOMP_REGEX && $4->a[0]->isStringVar()) {
//            // FIXME: RegEx will be computed twice!
//            RegEx* r = RegExParser($4->a[1]->getString()).parse();
//            //std::cerr << "Expression: " << r->str() << '\n';
//            //std::cerr << "DFA: " << r->dfa() << '\n';
//            //std::cerr << "Reg.: " << r->reg() << '\n';
//            //std::cerr << "Dec: " << r->decomp() << '\n';
//            //std::cerr << "_DEC:" << Gecode::String::DashedString::_DECOMP_REGEX << '\n';
//            //std::cerr << r->intro_vars() << "\n";
//            if (r->intro_vars()) {
//              AST::StringVar* x = dynamic_cast<AST::StringVar*>($4->a[0]);
//              REGEX.push_back(pair<AST::StringVar*, RegEx*>(x, r));
//            }
//            else
//              pp->constraints.push_back(new ConExpr($2, $4, $6));
//          }
          else {
            pp->constraints.push_back(new ConExpr((yyvsp[-4].sValue), (yyvsp[-2].argVec), (yyvsp[0].argVec)));
          }
        }
        free((yyvsp[-4].sValue));
      }
#line 3552 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 109:
#line 1868 "gecode/flatzinc/parser.yxx"
      {
        ParserState *pp = static_cast<ParserState*>(parm);
        initfg(pp);
        if (!pp->hadError) {
          try {
            pp->fg->solve((yyvsp[-1].argVec));
          } catch (Gecode::FlatZinc::Error& e) {
            yyerror(pp, e.toString().c_str());
          }
        } else {
          delete (yyvsp[-1].argVec);
        }
      }
#line 3570 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 110:
#line 1882 "gecode/flatzinc/parser.yxx"
      {
        ParserState *pp = static_cast<ParserState*>(parm);
        initfg(pp);
        if (!pp->hadError) {
          try {
            int v = (yyvsp[0].iValue) < 0 ? (-(yyvsp[0].iValue)-1) : (yyvsp[0].iValue);
            bool vi = (yyvsp[0].iValue) >= 0;
            if ((yyvsp[-1].bValue))
              pp->fg->minimize(v,vi,(yyvsp[-2].argVec));
            else
              pp->fg->maximize(v,vi,(yyvsp[-2].argVec));
          } catch (Gecode::FlatZinc::Error& e) {
            yyerror(pp, e.toString().c_str());
          }
        } else {
          delete (yyvsp[-2].argVec);
        }
      }
#line 3593 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 111:
#line 1907 "gecode/flatzinc/parser.yxx"
      { (yyval.oSet) = Option<AST::SetLit* >::none(); }
#line 3599 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 112:
#line 1909 "gecode/flatzinc/parser.yxx"
      { (yyval.oSet) = Option<AST::SetLit* >::some(new AST::SetLit(*(yyvsp[-1].setValue))); }
#line 3605 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 113:
#line 1911 "gecode/flatzinc/parser.yxx"
      {
        (yyval.oSet) = Option<AST::SetLit* >::some(new AST::SetLit((yyvsp[-2].iValue), (yyvsp[0].iValue)));
      }
#line 3613 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 114:
#line 1917 "gecode/flatzinc/parser.yxx"
      { (yyval.oSet) = Option<AST::SetLit* >::none(); }
#line 3619 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 115:
#line 1919 "gecode/flatzinc/parser.yxx"
      { bool haveTrue = false;
        bool haveFalse = false;
        for (int i=(yyvsp[-2].setValue)->size(); i--;) {
          haveTrue |= ((*(yyvsp[-2].setValue))[i] == 1);
          haveFalse |= ((*(yyvsp[-2].setValue))[i] == 0);
        }
        delete (yyvsp[-2].setValue);
        (yyval.oSet) = Option<AST::SetLit* >::some(
          new AST::SetLit(!haveFalse,haveTrue));
      }
#line 3634 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 116:
#line 1932 "gecode/flatzinc/parser.yxx"
      { (yyval.oPFloat) = Option<std::pair<double,double>* >::none(); }
#line 3640 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 117:
#line 1934 "gecode/flatzinc/parser.yxx"
      { std::pair<double,double>* dom = new std::pair<double,double>((yyvsp[-2].dValue),(yyvsp[0].dValue));
        (yyval.oPFloat) = Option<std::pair<double,double>* >::some(dom); }
#line 3647 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 118:
#line 1939 "gecode/flatzinc/parser.yxx"
      { (yyval.oPString) = Option<AST::StringDom*>::none(); }
#line 3653 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 119:
#line 1941 "gecode/flatzinc/parser.yxx"
      { (yyval.oPString) = Option<AST::StringDom*>::some(new AST::StringDom((yyvsp[-1].iValue))); }
#line 3659 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 120:
#line 1943 "gecode/flatzinc/parser.yxx"
      { (yyval.oPString) = Option<AST::StringDom*>::some(new AST::StringDom((yyvsp[0].charsetLit))); }
#line 3665 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 121:
#line 1945 "gecode/flatzinc/parser.yxx"
      { (yyval.oPString) = Option<AST::StringDom*>::some(new AST::StringDom((yyvsp[-3].iValue), (yyvsp[0].charsetLit))); }
#line 3671 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 122:
#line 1953 "gecode/flatzinc/parser.yxx"
      { (yyval.setLit) = new AST::SetLit(*(yyvsp[-1].setValue)); }
#line 3677 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 123:
#line 1955 "gecode/flatzinc/parser.yxx"
      { (yyval.setLit) = new AST::SetLit((yyvsp[-2].iValue), (yyvsp[0].iValue)); }
#line 3683 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 124:
#line 1961 "gecode/flatzinc/parser.yxx"
      { (yyval.setValue) = new vector<int>(0); }
#line 3689 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 125:
#line 1963 "gecode/flatzinc/parser.yxx"
      { (yyval.setValue) = (yyvsp[-1].setValue); }
#line 3695 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 126:
#line 1967 "gecode/flatzinc/parser.yxx"
      { (yyval.setValue) = new vector<int>(1); (*(yyval.setValue))[0] = (yyvsp[0].iValue); }
#line 3701 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 127:
#line 1969 "gecode/flatzinc/parser.yxx"
      { (yyval.setValue) = (yyvsp[-2].setValue); (yyval.setValue)->push_back((yyvsp[0].iValue)); }
#line 3707 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 128:
#line 1973 "gecode/flatzinc/parser.yxx"
      { (yyval.setValue) = new vector<int>(0); }
#line 3713 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 129:
#line 1975 "gecode/flatzinc/parser.yxx"
      { (yyval.setValue) = (yyvsp[-1].setValue); }
#line 3719 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 130:
#line 1979 "gecode/flatzinc/parser.yxx"
      { (yyval.setValue) = new vector<int>(1); (*(yyval.setValue))[0] = (yyvsp[0].iValue); }
#line 3725 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 131:
#line 1981 "gecode/flatzinc/parser.yxx"
      { (yyval.setValue) = (yyvsp[-2].setValue); (yyval.setValue)->push_back((yyvsp[0].iValue)); }
#line 3731 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 132:
#line 1985 "gecode/flatzinc/parser.yxx"
      { (yyval.floatSetValue) = new vector<double>(0); }
#line 3737 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 133:
#line 1987 "gecode/flatzinc/parser.yxx"
      { (yyval.floatSetValue) = (yyvsp[-1].floatSetValue); }
#line 3743 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 134:
#line 1991 "gecode/flatzinc/parser.yxx"
      { (yyval.floatSetValue) = new vector<double>(1); (*(yyval.floatSetValue))[0] = (yyvsp[0].dValue); }
#line 3749 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 135:
#line 1993 "gecode/flatzinc/parser.yxx"
      { (yyval.floatSetValue) = (yyvsp[-2].floatSetValue); (yyval.floatSetValue)->push_back((yyvsp[0].dValue)); }
#line 3755 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 136:
#line 1997 "gecode/flatzinc/parser.yxx"
      { (yyval.setValueList) = new vector<AST::SetLit>(0); }
#line 3761 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 137:
#line 1999 "gecode/flatzinc/parser.yxx"
      { (yyval.setValueList) = (yyvsp[-1].setValueList); }
#line 3767 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 138:
#line 2003 "gecode/flatzinc/parser.yxx"
      { (yyval.setValueList) = new vector<AST::SetLit>(1); (*(yyval.setValueList))[0] = *(yyvsp[0].setLit); delete (yyvsp[0].setLit); }
#line 3773 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 139:
#line 2005 "gecode/flatzinc/parser.yxx"
      { (yyval.setValueList) = (yyvsp[-2].setValueList); (yyval.setValueList)->push_back(*(yyvsp[0].setLit)); delete (yyvsp[0].setLit); }
#line 3779 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 140:
#line 2009 "gecode/flatzinc/parser.yxx"
      { (yyval.charsetLit) = new AST::CharSetLit(IntSet((yyvsp[-2].sValue)[0], (yyvsp[0].sValue)[0])); }
#line 3785 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 141:
#line 2017 "gecode/flatzinc/parser.yxx"
      { (yyval.argVec) = new AST::Array((yyvsp[0].arg)); }
#line 3791 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 142:
#line 2019 "gecode/flatzinc/parser.yxx"
      { (yyval.argVec) = (yyvsp[-2].argVec); (yyval.argVec)->append((yyvsp[0].arg)); }
#line 3797 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 143:
#line 2023 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = (yyvsp[0].arg); }
#line 3803 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 144:
#line 2025 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = (yyvsp[-1].argVec); }
#line 3809 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 145:
#line 2029 "gecode/flatzinc/parser.yxx"
      { (yyval.stringSetValue) = new vector<std::string>(0); }
#line 3815 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 146:
#line 2031 "gecode/flatzinc/parser.yxx"
      { (yyval.stringSetValue) = (yyvsp[-1].stringSetValue); }
#line 3821 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 147:
#line 2035 "gecode/flatzinc/parser.yxx"
      { (yyval.stringSetValue) = new vector<std::string>(1); (*(yyval.stringSetValue))[0] = (yyvsp[0].sValue); }
#line 3827 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 148:
#line 2037 "gecode/flatzinc/parser.yxx"
      { (yyval.stringSetValue) = (yyvsp[-2].stringSetValue); (yyval.stringSetValue)->push_back((yyvsp[0].sValue)); }
#line 3833 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 149:
#line 2041 "gecode/flatzinc/parser.yxx"
      { (yyval.oArg) = Option<AST::Node*>::none(); }
#line 3839 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 150:
#line 2043 "gecode/flatzinc/parser.yxx"
      { (yyval.oArg) = Option<AST::Node*>::some((yyvsp[0].arg)); }
#line 3845 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 151:
#line 2047 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = new AST::BoolLit((yyvsp[0].iValue)); }
#line 3851 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 152:
#line 2049 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = new AST::IntLit((yyvsp[0].iValue)); }
#line 3857 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 153:
#line 2051 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = new AST::FloatLit((yyvsp[0].dValue)); }
#line 3863 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 154:
#line 2053 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = new AST::StringDom(str2vec((yyvsp[0].sValue))); }
#line 3869 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 155:
#line 2055 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = (yyvsp[0].setLit); }
#line 3875 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 156:
#line 2057 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = (yyvsp[0].charsetLit); }
#line 3881 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 157:
#line 2059 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        SymbolEntry e;
        if (pp->symbols.get((yyvsp[0].sValue), e)) {
          switch (e.t) {
          case ST_INTVARARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::IntVar(pp->arrays[e.i+i+1]);
              (yyval.arg) = v;
            }
            break;
          case ST_BOOLVARARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::BoolVar(pp->arrays[e.i+i+1]);
              (yyval.arg) = v;
            }
            break;
          case ST_FLOATVARARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::FloatVar(pp->arrays[e.i+i+1]);
              (yyval.arg) = v;
            }
            break;
          case ST_SETVARARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::SetVar(pp->arrays[e.i+i+1]);
              (yyval.arg) = v;
            }
            break;
          case ST_STRINGVARARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;)\
                v->a[i] = new AST::StringVar(pp->arrays[e.i+i+1]);
              (yyval.arg) = v;
            }
            break;
          case ST_INTVALARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::IntLit(pp->arrays[e.i+i+1]);
              (yyval.arg) = v;
            }
            break;
          case ST_BOOLVALARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::BoolLit(pp->arrays[e.i+i+1]);
              (yyval.arg) = v;
            }
            break;
          case ST_SETVALARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              int idx = pp->arrays[e.i+1];
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::SetLit(pp->setvals[idx+i]);
              (yyval.arg) = v;
            }
            break;
          case ST_FLOATVALARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              int idx = pp->arrays[e.i+1];
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::FloatLit(pp->floatvals[idx+i]);
              (yyval.arg) = v;
            }
            break;
          case ST_STRINGVALARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              int idx = pp->arrays[e.i+1];
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::StringDom(pp->stringvals[idx+i]);
              (yyval.arg) = v;
            }
            break;
          case ST_INT:
            (yyval.arg) = new AST::IntLit(e.i);
            break;
          case ST_BOOL:
            (yyval.arg) = new AST::BoolLit(e.i);
            break;
          case ST_FLOAT:
            (yyval.arg) = new AST::FloatLit(pp->floatvals[e.i]);
            break;
          case ST_SET:
            (yyval.arg) = new AST::SetLit(pp->setvals[e.i]);
            break;
          case ST_STRING:
            (yyval.arg) = new AST::StringDom(pp->stringvals[e.i]);
            break;
          default:
            (yyval.arg) = getVarRefArg(pp,(yyvsp[0].sValue));
          }
        } else {
          pp->err << "Error: undefined identifier " << (yyvsp[0].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
          (yyval.arg) = NULL;
        }
        free((yyvsp[0].sValue));
      }
#line 4001 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 158:
#line 2175 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        int i = -1;
        yyassert(pp, (yyvsp[-1].arg)->isInt(i), "Non-integer array index");
        if (!pp->hadError)
          (yyval.arg) = getArrayElement(static_cast<ParserState*>(parm),(yyvsp[-3].sValue),i,false);
        else
          (yyval.arg) = new AST::IntLit(0); // keep things consistent
        delete (yyvsp[-1].arg);
        free((yyvsp[-3].sValue));
      }
#line 4017 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 159:
#line 2189 "gecode/flatzinc/parser.yxx"
      { (yyval.argVec) = new AST::Array(0); }
#line 4023 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 160:
#line 2191 "gecode/flatzinc/parser.yxx"
      { (yyval.argVec) = (yyvsp[-1].argVec); }
#line 4029 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 161:
#line 2195 "gecode/flatzinc/parser.yxx"
      { (yyval.argVec) = new AST::Array((yyvsp[0].arg)); }
#line 4035 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 162:
#line 2197 "gecode/flatzinc/parser.yxx"
      { (yyval.argVec) = (yyvsp[-2].argVec); (yyval.argVec)->append((yyvsp[0].arg)); }
#line 4041 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 163:
#line 2205 "gecode/flatzinc/parser.yxx"
      {
        ParserState *pp = static_cast<ParserState*>(parm);
        SymbolEntry e;
        bool haveSym = pp->symbols.get((yyvsp[0].sValue),e);
        if (haveSym) {
          switch (e.t) {
          case ST_INTVAR:
            (yyval.iValue) = e.i;
            break;
          case ST_FLOATVAR:
            (yyval.iValue) = -e.i-1;
            break;
          case ST_INT:
          case ST_FLOAT:
            pp->intvars.push_back(varspec("OBJ_CONST_INTRODUCED",
              new IntVarSpec(0,true,false)));
            (yyval.iValue) = pp->intvars.size()-1;
            break;
          default:
            pp->err << "Error: unknown int or float variable " << (yyvsp[0].sValue)
                    << " in line no. "
                    << yyget_lineno(pp->yyscanner) << std::endl;
            pp->hadError = true;
            break;
          }
        } else {
          pp->err << "Error: unknown int or float variable " << (yyvsp[0].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
        }
        free((yyvsp[0].sValue));
      }
#line 4079 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 164:
#line 2239 "gecode/flatzinc/parser.yxx"
      {
        ParserState *pp = static_cast<ParserState*>(parm);
        pp->intvars.push_back(varspec("OBJ_CONST_INTRODUCED",
          new IntVarSpec(0,true,false)));
        (yyval.iValue) = pp->intvars.size()-1;
      }
#line 4090 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 165:
#line 2246 "gecode/flatzinc/parser.yxx"
      {
        ParserState *pp = static_cast<ParserState*>(parm);
        pp->intvars.push_back(varspec("OBJ_CONST_INTRODUCED",
          new IntVarSpec(0,true,false)));
        (yyval.iValue) = pp->intvars.size()-1;
      }
#line 4101 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 166:
#line 2253 "gecode/flatzinc/parser.yxx"
      {
        SymbolEntry e;
        ParserState *pp = static_cast<ParserState*>(parm);
        if ( (!pp->symbols.get((yyvsp[-3].sValue), e)) ||
             (e.t != ST_INTVARARRAY && e.t != ST_FLOATVARARRAY)) {
          pp->err << "Error: unknown int or float variable array " << (yyvsp[-3].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
        }
        if ((yyvsp[-1].iValue) == 0 || (yyvsp[-1].iValue) > pp->arrays[e.i]) {
          pp->err << "Error: array index out of bounds for array " << (yyvsp[-3].sValue)
                  << " in line no. "
                  << yyget_lineno(pp->yyscanner) << std::endl;
          pp->hadError = true;
        } else {
          if (e.t == ST_INTVARARRAY)
            (yyval.iValue) = pp->arrays[e.i+(yyvsp[-1].iValue)];
          else
            (yyval.iValue) = -pp->arrays[e.i+(yyvsp[-1].iValue)]-1;
        }
        free((yyvsp[-3].sValue));
      }
#line 4129 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 169:
#line 2287 "gecode/flatzinc/parser.yxx"
      { (yyval.argVec) = NULL; }
#line 4135 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 170:
#line 2289 "gecode/flatzinc/parser.yxx"
      { (yyval.argVec) = (yyvsp[0].argVec); }
#line 4141 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 171:
#line 2293 "gecode/flatzinc/parser.yxx"
      { (yyval.argVec) = new AST::Array((yyvsp[0].arg)); }
#line 4147 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 172:
#line 2295 "gecode/flatzinc/parser.yxx"
      { (yyval.argVec) = (yyvsp[-2].argVec); (yyval.argVec)->append((yyvsp[0].arg)); }
#line 4153 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 173:
#line 2299 "gecode/flatzinc/parser.yxx"
      {
        (yyval.arg) = new AST::Call((yyvsp[-3].sValue), AST::extractSingleton((yyvsp[-1].arg))); free((yyvsp[-3].sValue));
      }
#line 4161 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 174:
#line 2303 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = (yyvsp[0].arg); }
#line 4167 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 175:
#line 2307 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = new AST::Array((yyvsp[0].arg)); }
#line 4173 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 176:
#line 2309 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = (yyvsp[-2].arg); (yyval.arg)->append((yyvsp[0].arg)); }
#line 4179 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 177:
#line 2313 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = (yyvsp[0].arg); }
#line 4185 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 178:
#line 2315 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = new AST::Array(); }
#line 4191 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 179:
#line 2317 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = (yyvsp[-2].arg); }
#line 4197 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 182:
#line 2323 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = new AST::BoolLit((yyvsp[0].iValue)); }
#line 4203 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 183:
#line 2325 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = new AST::IntLit((yyvsp[0].iValue)); }
#line 4209 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 184:
#line 2327 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = new AST::FloatLit((yyvsp[0].dValue)); }
#line 4215 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 185:
#line 2329 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = new AST::StringDom(str2vec((yyvsp[0].sValue))); }
#line 4221 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 186:
#line 2331 "gecode/flatzinc/parser.yxx"
      { (yyval.arg) = (yyvsp[0].setLit); }
#line 4227 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 187:
#line 2333 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        SymbolEntry e;
        bool gotSymbol = false;
        if (pp->symbols.get((yyvsp[0].sValue), e)) {
          gotSymbol = true;
          switch (e.t) {
          case ST_INTVARARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;) {
                std::ostringstream oss;
                oss << (yyvsp[0].sValue) << "["<<(i+1)<<"]";
                v->a[i] = new AST::IntVar(pp->arrays[e.i+i+1], oss.str());
              }
              (yyval.arg) = v;
            }
            break;
          case ST_BOOLVARARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;) {
                std::ostringstream oss;
                oss << (yyvsp[0].sValue) << "["<<(i+1)<<"]";
                v->a[i] = new AST::BoolVar(pp->arrays[e.i+i+1], oss.str());
              }
              (yyval.arg) = v;
            }
            break;
          case ST_FLOATVARARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;) {
                std::ostringstream oss;
                oss << (yyvsp[0].sValue) << "["<<(i+1)<<"]";
                v->a[i] = new AST::FloatVar(pp->arrays[e.i+i+1], oss.str());
              }
              (yyval.arg) = v;
            }
            break;
          case ST_SETVARARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;) {
                std::ostringstream oss;
                oss << (yyvsp[0].sValue) << "["<<(i+1)<<"]";
                v->a[i] = new AST::SetVar(pp->arrays[e.i+i+1], oss.str());
              }
              (yyval.arg) = v;
            }
            break;
          case ST_STRINGVARARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;) {
                std::ostringstream oss;
                oss << (yyvsp[0].sValue) << "["<<(i+1)<<"]";
                v->a[i] = new AST::StringVar(pp->arrays[e.i+i+1], oss.str());
              }
              (yyval.arg) = v;
            }
            break;          
          case ST_INTVALARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::IntLit(pp->arrays[e.i+i+1]);
              (yyval.arg) = v;
            }
            break;
          case ST_BOOLVALARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::BoolLit(pp->arrays[e.i+i+1]);
              (yyval.arg) = v;
            }
            break;
          case ST_SETVALARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              int idx = pp->arrays[e.i+1];
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::SetLit(pp->setvals[idx+i]);
              (yyval.arg) = v;
            }
            break;
          case ST_FLOATVALARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              int idx = pp->arrays[e.i+1];
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::FloatLit(pp->floatvals[idx+i]);
              (yyval.arg) = v;
            }
            break;
          case ST_STRINGVALARRAY:
            {
              AST::Array *v = new AST::Array(pp->arrays[e.i]);
              int idx = pp->arrays[e.i+1];
              for (int i=pp->arrays[e.i]; i--;)
                v->a[i] = new AST::StringDom(pp->floatvals[idx+i]);
              (yyval.arg) = v;
            }
            break;
          case ST_INT:
            (yyval.arg) = new AST::IntLit(e.i);
            break;
          case ST_BOOL:
            (yyval.arg) = new AST::BoolLit(e.i);
            break;
          case ST_FLOAT:
            (yyval.arg) = new AST::FloatLit(pp->floatvals[e.i]);
            break;
          case ST_SET:
            (yyval.arg) = new AST::SetLit(pp->setvals[e.i]);
            break;
          case ST_STRING:
            (yyval.arg) = new AST::StringDom(pp->stringvals[e.i]);
            break;
          default:
            gotSymbol = false;
          }
        }
        if (!gotSymbol)
          (yyval.arg) = getVarRefArg(pp,(yyvsp[0].sValue),true);
        free((yyvsp[0].sValue));
      }
#line 4360 "gecode/flatzinc/parser.tab.cpp"
    break;

  case 188:
#line 2462 "gecode/flatzinc/parser.yxx"
      {
        ParserState* pp = static_cast<ParserState*>(parm);
        int i = -1;
        yyassert(pp, (yyvsp[-1].arg)->isInt(i), "Non-integer array index");
        if (!pp->hadError)
          (yyval.arg) = getArrayElement(static_cast<ParserState*>(parm),(yyvsp[-3].sValue),i,true);
        else
          (yyval.arg) = new AST::IntLit(0); // keep things consistent
        free((yyvsp[-3].sValue));
      }
#line 4375 "gecode/flatzinc/parser.tab.cpp"
    break;


#line 4379 "gecode/flatzinc/parser.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (parm, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (parm, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, parm);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, parm);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (parm, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, parm);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, parm);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
