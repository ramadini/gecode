#ifndef __GECODE_STRING_EXTENSIONAL_FLATZINC_HPP__
#define __GECODE_STRING_EXTENSIONAL_FLATZINC_HPP__

#include <gecode/string/extensional/parse-reg.hpp>
#include <gecode/flatzinc/parser.hh>

namespace Gecode { namespace String {

  namespace FlatZincAST = Gecode::FlatZinc::AST;

  class FlatZincRegExBuilder : public RegExVisitor {
  private:
    FlatZincAST::StringVar* target;
    FlatZinc::ParserState* state;

    void emit(const RegEx& expression, FlatZincAST::StringVar* target0) {
      FlatZincAST::StringVar* previous = target;
      target = target0;
      expression.accept(*this);
      target = previous;
    }

  public:
    FlatZincRegExBuilder(FlatZinc::ParserState* state0)
      : target(nullptr), state(state0) {}

    void add(const RegEx& expression, FlatZincAST::StringVar* target0) {
      emit(expression, target0);
    }

    virtual void visit(const EmptyEx&) {
      FlatZincAST::Array* constraint = new FlatZincAST::Array(2);
      constraint->a[0] = new FlatZincAST::StringVar(target->i);
      constraint->a[1] = new FlatZincAST::StringDom("");
      state->constraints.push_back(
        new FlatZinc::ConExpr("str_eq", constraint, nullptr));
    }

    virtual void visit(const ConcatEx& expression) {
      const RegEx& left = expression.left();
      const RegEx& right = expression.right();
      if (left.empty()) {
        if (right.empty()) {
          FlatZincAST::Array* constraint = new FlatZincAST::Array(2);
          constraint->a[0] = new FlatZincAST::StringVar(target->i);
          constraint->a[1] = new FlatZincAST::StringDom("");
          state->constraints.push_back(
            new FlatZinc::ConExpr("str_eq", constraint, nullptr));
          return;
        }
        emit(right, target);
      } else if (right.empty()) {
        emit(left, target);
      } else {
        FlatZincAST::StringVar* left_var =
          new FlatZincAST::StringVar(state->stringvars.size());
        state->stringvars.push_back(FlatZinc::varspec("",
          new FlatZinc::StringVarSpec(
            FlatZinc::Option<FlatZincAST::StringDom*>::some(
              new FlatZincAST::StringDom()), false, false)));
        FlatZincAST::StringVar* right_var =
          new FlatZincAST::StringVar(state->stringvars.size());
        state->stringvars.push_back(FlatZinc::varspec("",
          new FlatZinc::StringVarSpec(
            FlatZinc::Option<FlatZincAST::StringDom*>::some(
              new FlatZincAST::StringDom()), false, false)));
        emit(left, left_var);
        emit(right, right_var);
        FlatZincAST::Array* constraint = new FlatZincAST::Array(3);
        constraint->a[0] = left_var;
        constraint->a[1] = right_var;
        constraint->a[2] = new FlatZincAST::StringVar(target->i);
        state->constraints.push_back(
          new FlatZinc::ConExpr("str_concat", constraint, nullptr));
      }
    }

    virtual void visit(const InterEx& expression) {
      const RegEx& left = expression.left();
      const RegEx& right = expression.right();
      if (left.empty() && right.empty()) {
        FlatZincAST::Array* constraint = new FlatZincAST::Array(2);
        constraint->a[0] = new FlatZincAST::StringVar(target->i);
        constraint->a[1] = new FlatZincAST::StringDom("");
        state->constraints.push_back(
          new FlatZinc::ConExpr("str_eq", constraint, nullptr));
        return;
      }
      emit(left, target);
      emit(right, target);
    }

    virtual void visit(const UnionEx& expression) {
      if (expression.empty()) {
        FlatZincAST::Array* constraint = new FlatZincAST::Array(2);
        constraint->a[0] = new FlatZincAST::StringVar(target->i);
        constraint->a[1] = new FlatZincAST::StringDom("");
        state->constraints.push_back(
          new FlatZinc::ConExpr("str_eq", constraint, nullptr));
        return;
      }
      FlatZincAST::StringVar* left_var =
        new FlatZincAST::StringVar(state->stringvars.size());
      state->stringvars.push_back(FlatZinc::varspec("",
        new FlatZinc::StringVarSpec(
          FlatZinc::Option<FlatZincAST::StringDom*>::some(
            new FlatZincAST::StringDom()), false, false)));
      FlatZincAST::StringVar* right_var =
        new FlatZincAST::StringVar(state->stringvars.size());
      state->stringvars.push_back(FlatZinc::varspec("",
        new FlatZinc::StringVarSpec(
          FlatZinc::Option<FlatZincAST::StringDom*>::some(
            new FlatZincAST::StringDom()), false, false)));
      emit(expression.left(), left_var);
      emit(expression.right(), right_var);
      FlatZincAST::Array* alternatives = new FlatZincAST::Array(2);
      alternatives->a[0] = left_var;
      alternatives->a[1] = right_var;
      FlatZincAST::IntVar* index =
        new FlatZincAST::IntVar(state->intvars.size());
      state->intvars.push_back(FlatZinc::varspec("",
        new FlatZinc::IntVarSpec(
          FlatZinc::Option<FlatZincAST::SetLit*>::some(
            new FlatZincAST::SetLit(1, 2)), false, false)));
      FlatZincAST::Array* constraint = new FlatZincAST::Array(3);
      constraint->a[0] = index;
      constraint->a[1] = alternatives;
      constraint->a[2] = new FlatZincAST::StringVar(target->i);
      state->constraints.push_back(new FlatZinc::ConExpr(
        "array_var_string_element", constraint, nullptr));
    }

    virtual void visit(const StarEx& expression) {
      const RegEx& base = expression.expression();
      if (expression.decomp() && DashedString::_DECOMP_REGEX) {
        if (base.has_concat()) {
          string value = base.get_string();
          FlatZincAST::IntVar* count =
            new FlatZincAST::IntVar(state->intvars.size());
          state->intvars.push_back(FlatZinc::varspec("",
            new FlatZinc::IntVarSpec(
              FlatZinc::Option<FlatZincAST::SetLit*>::some(
                new FlatZincAST::SetLit(
                  0, DashedString::_MAX_STR_LENGTH)), false, false)));
          FlatZincAST::StringVar* base_var =
            new FlatZincAST::StringVar(state->stringvars.size());
          state->stringvars.push_back(FlatZinc::varspec("",
            new FlatZinc::StringVarSpec(
              FlatZinc::Option<FlatZincAST::StringDom*>::some(
                new FlatZincAST::StringDom(value)), false, false)));
          FlatZincAST::Array* constraint = new FlatZincAST::Array(3);
          constraint->a[0] = base_var;
          constraint->a[1] = count;
          constraint->a[2] = new FlatZincAST::StringVar(target->i);
          state->constraints.push_back(
            new FlatZinc::ConExpr("str_pow", constraint, nullptr));
        } else {
          FlatZincAST::Array* constraint = new FlatZincAST::Array(2);
          constraint->a[0] = new FlatZincAST::StringVar(target->i);
          constraint->a[1] = new FlatZincAST::StringDom(base.get_chars());
          state->constraints.push_back(
            new FlatZinc::ConExpr("str_chars", constraint, nullptr));
        }
        return;
      }
      FlatZincAST::Array* constraint = new FlatZincAST::Array(2);
      constraint->a[0] = new FlatZincAST::StringVar(target->i);
      constraint->a[1] = new FlatZincAST::DFA(expression.dfa());
      state->constraints.push_back(
        new FlatZinc::ConExpr("str_dfa", constraint, nullptr));
    }

    virtual void visit(const CharEx& expression) {
      FlatZincAST::Array* constraint = new FlatZincAST::Array(2);
      constraint->a[0] = new FlatZincAST::StringVar(target->i);
      constraint->a[1] = new FlatZincAST::StringDom(expression.get_string());
      state->constraints.push_back(
        new FlatZinc::ConExpr("str_eq", constraint, nullptr));
    }

    virtual void visit(const RangeEx& expression) {
      if (expression.min_char() > expression.max_char() ||
          expression.min_length() > expression.max_length()) {
        state->fg->fail();
        return;
      }
      FlatZincAST::Array* length_constraint = new FlatZincAST::Array(2);
      length_constraint->a[0] = new FlatZincAST::StringVar(target->i);
      if (expression.min_length() == expression.max_length())
        length_constraint->a[1] =
          new FlatZincAST::IntLit(expression.min_length());
      else
        length_constraint->a[1] = new FlatZincAST::SetLit(
          expression.min_length(), expression.max_length());
      state->constraints.push_back(
        new FlatZinc::ConExpr("str_len", length_constraint, nullptr));

      FlatZincAST::Array* chars_constraint = new FlatZincAST::Array(2);
      chars_constraint->a[0] = new FlatZincAST::StringVar(target->i);
      chars_constraint->a[1] =
        new FlatZincAST::StringDom(expression.get_chars());
      state->constraints.push_back(
        new FlatZinc::ConExpr("str_chars", chars_constraint, nullptr));
    }
  };

  forceinline void
  add_to_flatzinc(const RegEx& expression, FlatZincAST::StringVar* target,
                  FlatZinc::ParserState* state) {
    FlatZincRegExBuilder builder(state);
    builder.add(expression, target);
  }

}}

#endif