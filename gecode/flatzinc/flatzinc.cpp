/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2007
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <gecode/flatzinc.hh>
#include <gecode/flatzinc/registry.hh>
#include <gecode/flatzinc/plugin.hh>

#include <gecode/search.hh>

#include <vector>
#include <string>
using namespace std;

namespace Gecode { namespace FlatZinc {

  IntSet vs2is(IntVarSpec* vs) {
    if (vs->assigned) {
      return IntSet(vs->i,vs->i);
    }
    if (vs->domain()) {
      AST::SetLit* sl = vs->domain.some();
      if (sl->interval) {
        return IntSet(sl->min, sl->max);
      } else {
        int* newdom = heap.alloc<int>(static_cast<unsigned long int>(sl->s.size()));
        for (int i=sl->s.size(); i--;)
          newdom[i] = sl->s[i];
        IntSet ret(newdom, sl->s.size());
        heap.free(newdom, static_cast<unsigned long int>(sl->s.size()));
        return ret;
      }
    }
    return IntSet(Int::Limits::min, Int::Limits::max);
  }

  int vs2bsl(BoolVarSpec* bs) {
    if (bs->assigned) {
      return bs->i;
    }
    if (bs->domain()) {
      AST::SetLit* sl = bs->domain.some();
      assert(sl->interval);
      return std::min(1, std::max(0, sl->min));
    }
    return 0;
  }

  int vs2bsh(BoolVarSpec* bs) {
    if (bs->assigned) {
      return bs->i;
    }
    if (bs->domain()) {
      AST::SetLit* sl = bs->domain.some();
      assert(sl->interval);
      return std::max(0, std::min(1, sl->max));
    }
    return 1;
  }

  TieBreakVarBranch<IntVarBranch> ann2ivarsel(AST::Node* ann) {
    if (AST::Atom* s = dynamic_cast<AST::Atom*>(ann)) {
      if (s->id == "input_order")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_NONE);
      if (s->id == "first_fail")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_SIZE_MIN);
      if (s->id == "anti_first_fail")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_SIZE_MAX);
      if (s->id == "smallest")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_MIN_MIN);
      if (s->id == "largest")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_MAX_MAX);
      if (s->id == "occurrence")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_DEGREE_MAX);
      if (s->id == "max_regret")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_REGRET_MIN_MAX);
      if (s->id == "most_constrained")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_SIZE_MIN,
          INT_VAR_DEGREE_MAX);
      if (s->id == "random")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_RND);
      if (s->id == "afc_min")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_AFC_MIN);
      if (s->id == "afc_max")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_AFC_MAX);
      if (s->id == "size_afc_min")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_SIZE_AFC_MIN);
      if (s->id == "size_afc_max")
        return TieBreakVarBranch<IntVarBranch>(INT_VAR_SIZE_AFC_MAX);
    }
    std::cerr << "Warning, ignored search annotation: ";
    ann->print(std::cerr);
    std::cerr << std::endl;
    return TieBreakVarBranch<IntVarBranch>(INT_VAR_NONE);
  }

  IntValBranch ann2ivalsel(AST::Node* ann) {
    if (AST::Atom* s = dynamic_cast<AST::Atom*>(ann)) {
      if (s->id == "indomain_min")
        return INT_VAL_MIN;
      if (s->id == "indomain_max")
        return INT_VAL_MAX;
      if (s->id == "indomain_median")
        return INT_VAL_MED;
      if (s->id == "indomain_split")
        return INT_VAL_SPLIT_MIN;
      if (s->id == "indomain_reverse_split")
        return INT_VAL_SPLIT_MAX;
      if (s->id == "indomain_random")
        return INT_VAL_RND;
      if (s->id == "indomain")
        return INT_VALUES_MIN;
    }
    std::cerr << "Warning, ignored search annotation: ";
    ann->print(std::cerr);
    std::cerr << std::endl;
    return INT_VAL_MIN;
  }

  IntAssign ann2asnivalsel(AST::Node* ann) {
    if (AST::Atom* s = dynamic_cast<AST::Atom*>(ann)) {
      if (s->id == "indomain_min")
        return INT_ASSIGN_MIN;
      if (s->id == "indomain_max")
        return INT_ASSIGN_MAX;
      if (s->id == "indomain_median")
        return INT_ASSIGN_MED;
      if (s->id == "indomain_random")
        return INT_ASSIGN_RND;
    }
    std::cerr << "Warning, ignored search annotation: ";
    ann->print(std::cerr);
    std::cerr << std::endl;
    return INT_ASSIGN_MIN;
  }

#ifdef GECODE_HAS_SET_VARS
  SetVarBranch ann2svarsel(AST::Node* ann) {
    if (AST::Atom* s = dynamic_cast<AST::Atom*>(ann)) {
      if (s->id == "input_order")
        return SET_VAR_NONE;
      if (s->id == "first_fail")
        return SET_VAR_SIZE_MIN;
      if (s->id == "anti_first_fail")
        return SET_VAR_SIZE_MAX;
      if (s->id == "smallest")
        return SET_VAR_MIN_MIN;
      if (s->id == "largest")
        return SET_VAR_MIN_MAX;
    }
    std::cerr << "Warning, ignored search annotation: ";
    ann->print(std::cerr);
    std::cerr << std::endl;
    return SET_VAR_NONE;
  }

  SetValBranch ann2svalsel(AST::Node* ann) {
    if (AST::Atom* s = dynamic_cast<AST::Atom*>(ann)) {
      if (s->id == "indomain_min")
        return SET_VAL_MIN_INC;
      if (s->id == "indomain_max")
        return SET_VAL_MAX_INC;
      if (s->id == "outdomain_min")
        return SET_VAL_MIN_EXC;
      if (s->id == "outdomain_max")
        return SET_VAL_MAX_EXC;
    }
    std::cerr << "Warning, ignored search annotation: ";
    ann->print(std::cerr);
    std::cerr << std::endl;
    return SET_VAL_MIN_INC;
  }
#endif

  FlatZincSpace::FlatZincSpace(bool share, FlatZincSpace& f)
    : Space(share, f), _solveAnnotations(NULL) {
      _optVar = f._optVar;
      _method = f._method;
      iv.update(*this, share, f.iv);
      intVarCount = f.intVarCount;
      bv.update(*this, share, f.bv);
      boolVarCount = f.boolVarCount;
#ifdef GECODE_HAS_SET_VARS
      sv.update(*this, share, f.sv);
      setVarCount = f.setVarCount;
#endif
    }
  
  FlatZincSpace::FlatZincSpace(void)
  : intVarCount(-1), boolVarCount(-1), setVarCount(-1), _optVar(-1),
    _solveAnnotations(NULL) {}

  void
  FlatZincSpace::init(int intVars, int boolVars,
#ifdef GECODE_HAS_SET_VARS
                                 int setVars) {
#else
                                 int) {
#endif
    intVarCount = 0;
    iv = IntVarArray(*this, intVars);
    iv_introduced = std::vector<bool>(intVars);
    boolVarCount = 0;
    bv = BoolVarArray(*this, boolVars);
    bv_introduced = std::vector<bool>(boolVars);
#ifdef GECODE_HAS_SET_VARS
    setVarCount = 0;
    sv = SetVarArray(*this, setVars);
    sv_introduced = std::vector<bool>(setVars);
#endif
  }

  void
  FlatZincSpace::newIntVar(IntVarSpec* vs) {
    if (vs->alias) {
      iv[intVarCount++] = iv[vs->i];
    } else {
      iv[intVarCount++] = IntVar(*this, vs2is(vs));
    }
    iv_introduced[intVarCount-1] = vs->introduced;
  }

  void
  FlatZincSpace::newBoolVar(BoolVarSpec* vs) {
    if (vs->alias) {
      bv[boolVarCount++] = bv[vs->i];
    } else {
      bv[boolVarCount++] = BoolVar(*this, vs2bsl(vs), vs2bsh(vs));
    }
    bv_introduced[boolVarCount-1] = vs->introduced;
  }

#ifdef GECODE_HAS_SET_VARS
  void
  FlatZincSpace::newSetVar(SetVarSpec* vs) {
    if (vs->alias) {
      sv[setVarCount++] = sv[vs->i];
    } else if (vs->assigned) {
      assert(vs->upperBound());
      AST::SetLit* vsv = vs->upperBound.some();
      if (vsv->interval) {
        IntSet d(vsv->min, vsv->max);
        sv[setVarCount++] = SetVar(*this, d, d);
      } else {
        int* is = heap.alloc<int>(static_cast<unsigned long int>(vsv->s.size()));
        for (int i=vsv->s.size(); i--; )
          is[i] = vsv->s[i];
        IntSet d(is, vsv->s.size());
        heap.free(is,static_cast<unsigned long int>(vsv->s.size()));
        sv[setVarCount++] = SetVar(*this, d, d);
      }
    } else if (vs->upperBound()) {
      AST::SetLit* vsv = vs->upperBound.some();
      if (vsv->interval) {
        IntSet d(vsv->min, vsv->max);
        sv[setVarCount++] = SetVar(*this, IntSet::empty, d);
      } else {
        int* is = heap.alloc<int>(static_cast<unsigned long int>(vsv->s.size()));
        for (int i=vsv->s.size(); i--; )
          is[i] = vsv->s[i];
        IntSet d(is, vsv->s.size());
        heap.free(is,static_cast<unsigned long int>(vsv->s.size()));
        sv[setVarCount++] = SetVar(*this, IntSet::empty, d);
      }
    } else {
      sv[setVarCount++] = SetVar(*this, IntSet::empty,
                                 IntSet(Set::Limits::min, 
                                        Set::Limits::max));
    }
    sv_introduced[setVarCount-1] = vs->introduced;
  }
#else
  void
  FlatZincSpace::newSetVar(SetVarSpec*) {
    throw FlatZinc::Error("Gecode", "set variables not supported");
  }
#endif

  void
  FlatZincSpace::postConstraint(const ConExpr& ce, AST::Node* ann) {
    try {
      registry().post(*this, ce, ann);
    } catch (Gecode::Exception& e) {
      throw FlatZinc::Error("Gecode", e.what());
    } catch (AST::TypeError& e) {
      throw FlatZinc::Error("Type error", e.what());
    }
  }

  void flattenAnnotations(AST::Array* ann, std::vector<AST::Node*>& out) {
      for (unsigned int i=0; i<ann->a.size(); i++) {
        if (ann->a[i]->isCall("seq_search")) {
          AST::Call* c = ann->a[i]->getCall();
          if (c->args->isArray())
            flattenAnnotations(c->args->getArray(), out);
          else
            out.push_back(c->args);
        } else {
          out.push_back(ann->a[i]);
        }
      }
  }

  void
  FlatZincSpace::createBranchers(bool ignoreUnknown, std::ostream& err) {
    bool hadSearchAnnotation = false;
    if (_solveAnnotations) {
      std::vector<AST::Node*> flatAnn;
      flattenAnnotations(_solveAnnotations, flatAnn);

      for (unsigned int i=0; i<flatAnn.size(); i++) {
        if (flatAnn[i]->isCall("gecode_search"))
          continue;
        try {
          AST::Call *call = flatAnn[i]->getCall("int_search");
          AST::Array *args = call->getArgs(4);
          AST::Array *vars = args->a[0]->getArray();
          int k=vars->a.size();
          for (int i=vars->a.size(); i--;)
            if (vars->a[i]->isInt())
              k--;
          IntVarArgs va(k);
          k=0;
          for (unsigned int i=0; i<vars->a.size(); i++) {
            if (vars->a[i]->isInt())
              continue;
            va[k++] = iv[vars->a[i]->getIntVar()];
          }
          branch(*this, va, ann2ivarsel(args->a[1]), ann2ivalsel(args->a[2]));
          hadSearchAnnotation = true;
        } catch (AST::TypeError& e) {
        try {
          AST::Call *call = flatAnn[i]->getCall("int_assign");
          AST::Array *args = call->getArgs(2);
          AST::Array *vars = args->a[0]->getArray();
          int k=vars->a.size();
          for (int i=vars->a.size(); i--;)
            if (vars->a[i]->isInt())
              k--;
          IntVarArgs va(k);
          k=0;
          for (unsigned int i=0; i<vars->a.size(); i++) {
            if (vars->a[i]->isInt())
              continue;
            va[k++] = iv[vars->a[i]->getIntVar()];
          }
          assign(*this, va, ann2asnivalsel(args->a[1]));
          hadSearchAnnotation = true;
        } catch (AST::TypeError& e) {
          (void) e;
          try {
            AST::Call *call = flatAnn[i]->getCall("bool_search");
            AST::Array *args = call->getArgs(4);
            AST::Array *vars = args->a[0]->getArray();
            int k=vars->a.size();
            for (int i=vars->a.size(); i--;)
              if (vars->a[i]->isBool())
                k--;
            BoolVarArgs va(k);
            k=0;
            for (unsigned int i=0; i<vars->a.size(); i++) {
              if (vars->a[i]->isBool())
                continue;
              va[k++] = bv[vars->a[i]->getBoolVar()];
            }
            branch(*this, va, ann2ivarsel(args->a[1]), 
                   ann2ivalsel(args->a[2]));        
            hadSearchAnnotation = true;
          } catch (AST::TypeError& e) {
            (void) e;
#ifdef GECODE_HAS_SET_VARS
            try {
              AST::Call *call = flatAnn[i]->getCall("set_search");
              AST::Array *args = call->getArgs(4);
              AST::Array *vars = args->a[0]->getArray();
              int k=vars->a.size();
              for (int i=vars->a.size(); i--;)
                if (vars->a[i]->isSet())
                  k--;
              SetVarArgs va(k);
              k=0;
              for (unsigned int i=0; i<vars->a.size(); i++) {
                if (vars->a[i]->isSet())
                  continue;
                va[k++] = sv[vars->a[i]->getSetVar()];
              }
              branch(*this, va, ann2svarsel(args->a[1]), 
                               ann2svalsel(args->a[2]));        
              hadSearchAnnotation = true;
            } catch (AST::TypeError& e) {
              (void) e;
              if (!ignoreUnknown) {
                err << "Warning, ignored search annotation: ";
                flatAnn[i]->print(err);
                err << std::endl;
              }
            }
#else
            if (!ignoreUnknown) {
              err << "Warning, ignored search annotation: ";
              flatAnn[i]->print(err);
              err << std::endl;
            }
#endif
          }
        }
        }
      }
    }
    if (!hadSearchAnnotation) {
      int countUp = 0;
      int countDown = iv.size()-1;
      IntVarArgs iva(iv.size());
      for (int i=0; i<iv.size(); i++)
        if (iv_introduced[i])
          iva[countDown--] = iv[i];
        else
          iva[countUp++] = iv[i];
      countUp = 0;
      countDown = bv.size()-1;
      BoolVarArgs bva(bv.size());
      for (int i=0; i<bv.size(); i++)
        if (bv_introduced[i])
          bva[countDown--] = bv[i];
        else
          bva[countUp++] = bv[i];
      branch(*this, iva, INT_VAR_NONE, INT_VAL_MIN);
      branch(*this, bva, INT_VAR_NONE, INT_VAL_MIN);
#ifdef GECODE_HAS_SET_VARS
      countUp = 0;
      countDown = sv.size()-1;
      SetVarArgs sva(sv.size());
      for (int i=0; i<sv.size(); i++)
        if (sv_introduced[i])
          sva[countDown--] = sv[i];
        else
          sva[countUp++] = sv[i];
      branch(*this, sva, SET_VAR_NONE, SET_VAL_MIN_INC);
#endif
    }
  }

  void
  FlatZincSpace::parseSearchOptions(void) {
    if (_solveAnnotations) {
      for (unsigned int i=0; i<_solveAnnotations->a.size(); i++) {
        if (_solveAnnotations->a[i]->isCall("gecode_search")) {
          AST::Call* c = _solveAnnotations->a[i]->getCall();
          branchWithPlugin(c->args);
        }
      }
    }
  }

  void
  FlatZincSpace::solve(AST::Array* ann) {
    _method = SAT;
    _solveAnnotations = ann;
    parseSearchOptions();
  }

  void
  FlatZincSpace::minimize(int var, AST::Array* ann) {
    _method = MIN;
    _optVar = var;
    _solveAnnotations = ann;
    parseSearchOptions();
    // Branch on optimization variable to ensure that it is given a value.
    AST::Array* args = new AST::Array(4);
    args->a[0] = new AST::Array(new AST::IntVar(_optVar));
    args->a[1] = new AST::Atom("input_order");
    args->a[2] = new AST::Atom("indomain_min");
    args->a[3] = new AST::Atom("complete");
    AST::Call* c = new AST::Call("int_search", args);
    if (!ann)
      ann = new AST::Array(c);
    else
      ann->a.push_back(c);
  }

  void
  FlatZincSpace::maximize(int var, AST::Array* ann) {
    _method = MAX;
    _optVar = var;
    _solveAnnotations = ann;
    parseSearchOptions();
    // Branch on optimization variable to ensure that it is given a value.
    AST::Array* args = new AST::Array(4);
    args->a[0] = new AST::Array(new AST::IntVar(_optVar));
    args->a[1] = new AST::Atom("input_order");
    args->a[2] = new AST::Atom("indomain_min");
    args->a[3] = new AST::Atom("complete");
    AST::Call* c = new AST::Call("int_search", args);
    if (!ann)
      ann = new AST::Array(c);
    else
      ann->a.push_back(c);
  }

  FlatZincSpace::~FlatZincSpace(void) {
    delete _solveAnnotations;
  }

#ifdef GECODE_HAS_GIST

  /**
   * \brief Traits class for search engines
   */
  template<class Engine>
  class GistEngine {
  };

  /// Specialization for DFS
  template<typename S>
  class GistEngine<DFS<S> > {
  public:
    static void explore(S* root, const FlatZincOptions& opt,
                        Gist::Inspector* i) {
      Gecode::Gist::Options o;
      o.c_d = opt.c_d(); o.a_d = opt.a_d();
      o.inspect.click(i);
      (void) Gecode::Gist::dfs(root, o);
    }
  };

  /// Specialization for LDS
  template<typename S>
  class GistEngine<LDS<S> > {
  public:
    static void explore(S* root, const FlatZincOptions& opt,
                        Gist::Inspector* i) {
      Gecode::Gist::Options o;
      o.c_d = opt.c_d(); o.a_d = opt.a_d();
      o.inspect.click(i);
      (void) Gecode::Gist::dfs(root, o);
    }
  };

  /// Specialization for BAB
  template<typename S>
  class GistEngine<BAB<S> > {
  public:
    static void explore(S* root, const FlatZincOptions& opt,
                        Gist::Inspector* i) {
      Gecode::Gist::Options o;
      o.c_d = opt.c_d(); o.a_d = opt.a_d();
      o.inspect.click(i);
      (void) Gecode::Gist::bab(root, o);
    }
  };

  /// Specialization for Restart
  template<typename S>
  class GistEngine<Restart<S> > {
  public:
    static void explore(S* root, const FlatZincOptions& opt,
                        Gist::Inspector* i) {
      Gecode::Gist::Options o;
      o.c_d = opt.c_d(); o.a_d = opt.a_d();
      o.inspect.click(i);
      (void) Gecode::Gist::bab(root, o);
    }
  };

  /// \brief An inspector for printing simple text output
  template<class S>
  class FZPrintingInspector : public Gecode::Gist::TextInspector {
  private:
    const Printer& p;
  public:
    /// Constructor
    FZPrintingInspector(const Printer& p0);
    /// Use the print method of the template class S to print a space
    virtual void inspect(const Space& node);
  };

  template<class S>
  FZPrintingInspector<S>::FZPrintingInspector(const Printer& p0)
  : TextInspector("Gecode/FlatZinc"), p(p0) {}

  template<class S>
  void
  FZPrintingInspector<S>::inspect(const Space& node) {
    init();
    dynamic_cast<const S&>(node).print(getStream(), p);
    getStream() << std::endl;
  }

#endif

  template<template<class> class Engine>
  void
  FlatZincSpace::runEngine(std::ostream& out, const Printer& p,
                            const FlatZincOptions& opt, Support::Timer& t_total) {
#ifdef GECODE_HAS_GIST
    if (opt.mode() == SM_GIST) {
      FZPrintingInspector<FlatZincSpace> pi(p);
      (void) GistEngine<Engine<FlatZincSpace> >::explore(this,opt,&pi);
      return;
    }
#endif
    StatusStatistics sstat;
    unsigned int n_p = 0;
    Support::Timer t_solve;
    t_solve.start();
    if (status(sstat) != SS_FAILED) {
      n_p = propagators();
    }
    Search::Options o;
    o.stop = Driver::Cutoff::create(opt.node(), opt.fail(), opt.time());
    o.c_d = opt.c_d();
    o.a_d = opt.a_d();
    o.threads = opt.threads();
    Engine<FlatZincSpace> se(this,o);
    int noOfSolutions = _method == SAT ? opt.solutions() : 0;
    int findSol = noOfSolutions;
    FlatZincSpace* sol = NULL;
    while (FlatZincSpace* next_sol = se.next()) {
      delete sol;
      sol = next_sol;
      if (opt.print()==0) {
        sol->print(out, p);
        out << "----------" << std::endl;
      }
      if (--findSol==0)
        goto stopped;
    }
    if (sol && opt.print()!=0) {
      sol->print(out, p);
      out << "----------" << std::endl;
    }
    delete sol;
    if (!se.stopped())
      out << "==========" << endl;
    stopped:
    if (opt.mode() == SM_STAT) {
      Gecode::Search::Statistics stat = se.statistics();
      out << endl
           << "%%  runtime:       ";
      Driver::stop(t_total,out);
      out << endl
           << "%%  solvetime:     ";
      Driver::stop(t_solve,out);
      out << endl
           << "%%  solutions:     " 
           << std::abs(noOfSolutions - findSol) << endl
           << "%%  variables:     " 
           << (intVarCount + boolVarCount + setVarCount) << endl
           << "%%  propagators:   " << n_p << endl
           << "%%  propagations:  " << sstat.propagate+stat.propagate << endl
           << "%%  nodes:         " << stat.node << endl
           << "%%  failures:      " << stat.fail << endl
           << "%%  peak depth:    " << stat.depth << endl
           << "%%  peak memory:   "
           << static_cast<int>((stat.memory+1023) / 1024) << " KB"
           << endl;
    }
  }

#ifdef GECODE_HAS_QT
  void
  FlatZincSpace::branchWithPlugin(AST::Node* ann) {
    if (AST::Call* c = dynamic_cast<AST::Call*>(ann)) {
      QString pluginName(c->id.c_str());
      if (QLibrary::isLibrary(pluginName+".dll")) {
        pluginName += ".dll";
      } else if (QLibrary::isLibrary(pluginName+".dylib")) {
        pluginName = "lib" + pluginName + ".dylib";
      } else if (QLibrary::isLibrary(pluginName+".so")) {
        // Must check .so after .dylib so that Mac OS uses .dylib
        pluginName = "lib" + pluginName + ".so";
      }
      QPluginLoader pl(pluginName);
      QObject* plugin_o = pl.instance();
      if (!plugin_o) {
        throw FlatZinc::Error("FlatZinc",
          "Error loading plugin "+pluginName.toStdString()+
          ": "+pl.errorString().toStdString());
      }
      BranchPlugin* pb = qobject_cast<BranchPlugin*>(plugin_o);
      if (!pb) {
        throw FlatZinc::Error("FlatZinc",
        "Error loading plugin "+pluginName.toStdString()+
        ": does not contain valid PluginBrancher");
      }
      pb->branch(*this, c);
    }
  }
#else
  void
  FlatZincSpace::branchWithPlugin(AST::Node* ann) {
    throw FlatZinc::Error("FlatZinc",
      "Branching with plugins not supported (requires Qt support)");
  }
#endif

  void
  FlatZincSpace::run(std::ostream& out, const Printer& p,
                      const FlatZincOptions& opt, Support::Timer& t_total) {
    switch (_method) {
    case MIN:
    case MAX:
      runEngine<BAB>(out,p,opt,t_total);
      break;
    case SAT:
      runEngine<DFS>(out,p,opt,t_total);
      break;
    }
  }

  void
  FlatZincSpace::constrain(const Space& s) {
    if (_method == MIN)
      rel(*this, iv[_optVar], IRT_LE, 
                 static_cast<const FlatZincSpace*>(&s)->iv[_optVar].val());
    else
      rel(*this, iv[_optVar], IRT_GR,
                 static_cast<const FlatZincSpace*>(&s)->iv[_optVar].val());
  }

  Space*
  FlatZincSpace::copy(bool share) {
    return new FlatZincSpace(share, *this);
  }

  FlatZincSpace::Meth
  FlatZincSpace::method(void) const {
    return _method;
  }

  int
  FlatZincSpace::optVar(void) const {
    return _optVar;
  }

  void
  FlatZincSpace::print(std::ostream& out, const Printer& p) const {
    p.print(out, iv, bv
#ifdef GECODE_HAS_SET_VARS
    , sv
#endif
    );
  }

  void
  Printer::init(AST::Array* output) {
    _output = output;
  }

  void
  Printer::printElem(std::ostream& out,
                       AST::Node* ai,
                       const Gecode::IntVarArray& iv,
                       const Gecode::BoolVarArray& bv
#ifdef GECODE_HAS_SET_VARS
                       , const Gecode::SetVarArray& sv
#endif
                       ) const {
    int k;
    if (ai->isInt(k)) {
      out << k;
    } else if (ai->isIntVar()) {
      out << iv[ai->getIntVar()];
    } else if (ai->isBoolVar()) {
      if (bv[ai->getBoolVar()].min() == 1) {
        out << "true";
      } else if (bv[ai->getBoolVar()].max() == 0) {
        out << "false";
      } else {
        out << "false..true";
      }
#ifdef GECODE_HAS_SET_VARS
    } else if (ai->isSetVar()) {
      if (!sv[ai->getSetVar()].assigned()) {
        out << sv[ai->getSetVar()];
        return;
      }
      SetVarGlbRanges svr(sv[ai->getSetVar()]);
      if (!svr()) {
        out << "{}";
        return;
      }
      int min = svr.min();
      int max = svr.max();
      ++svr;
      if (svr()) {
        SetVarGlbValues svv(sv[ai->getSetVar()]);
        int i = svv.val();
        out << "{" << i;
        ++svv;
        for (; svv(); ++svv)
          out << ", " << svv.val();
        out << "}";
      } else {
        out << min << ".." << max;
      }
#endif
    } else if (ai->isBool()) {
      out << (ai->getBool() ? "true" : "false");
    } else if (ai->isSet()) {
      AST::SetLit* s = ai->getSet();
      if (s->interval) {
        out << s->min << ".." << s->max;
      } else {
        out << "{";
        for (unsigned int i=0; i<s->s.size(); i++) {
          out << s->s[i] << (i < s->s.size()-1 ? ", " : "}");
        }
      }
    } else if (ai->isString()) {
      std::string s = ai->getString();
      for (unsigned int i=0; i<s.size(); i++) {
        if (s[i] == '\\' && i<s.size()-1) {
          switch (s[i+1]) {
          case 'n': out << "\n"; break;
          case '\\': out << "\\"; break;
          case 't': out << "\t"; break;
          default: out << "\\" << s[i+1];
          }
          i++;
        } else {
          out << s[i];
        }
      }
    }
  }

  void
  Printer::print(std::ostream& out,
                   const Gecode::IntVarArray& iv,
                   const Gecode::BoolVarArray& bv
#ifdef GECODE_HAS_SET_VARS
                   ,
                   const Gecode::SetVarArray& sv
#endif
                   ) const {
    if (_output == NULL)
      return;
    for (unsigned int i=0; i< _output->a.size(); i++) {
      AST::Node* ai = _output->a[i];
      if (ai->isArray()) {
        AST::Array* aia = ai->getArray();
        int size = aia->a.size();
        out << "[";
        for (int j=0; j<size; j++) {
          printElem(out,aia->a[j],iv,bv
#ifdef GECODE_HAS_SET_VARS
          ,sv
#endif
          );
          if (j<size-1)
            out << ", ";
        }
        out << "]";
      } else if (ai->isCall("ifthenelse")) {
        AST::Array* aia = ai->getCall("ifthenelse")->getArgs(3);
        if (aia->a[0]->isBool()) {
          if (aia->a[0]->getBool())
            printElem(out, aia->a[1], iv,bv
#ifdef GECODE_HAS_SET_VARS
            ,sv
#endif
            );
          else
            printElem(out, aia->a[2], iv,bv
#ifdef GECODE_HAS_SET_VARS
            ,sv
#endif
            );
        } else if (aia->a[0]->isBoolVar()) {
          BoolVar b = bv[aia->a[0]->getBoolVar()];
          if (b.one())
            printElem(out, aia->a[1], iv,bv
#ifdef GECODE_HAS_SET_VARS
            ,sv
#endif
            );
          else if (b.zero())
            printElem(out, aia->a[2], iv,bv
#ifdef GECODE_HAS_SET_VARS
            ,sv
#endif
            );
          else
            std::cerr << "Error: Condition not fixed." << std::endl;
        } else {
          std::cerr << "Error: Condition not Boolean." << std::endl;        
        }
      } else {
        printElem(out,ai,iv,bv
#ifdef GECODE_HAS_SET_VARS
        ,sv
#endif
        );
      }
    }
  }

  Printer::~Printer(void) {
    delete _output;
  }

}}

// STATISTICS: flatzinc-any
