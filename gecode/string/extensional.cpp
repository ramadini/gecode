#include <gecode/string.hh>
#include <gecode/string/extensional.hh>
#include <gecode/string/extensional/parse-reg.hpp>

namespace Gecode { namespace String {

  namespace {

    // Active nodes retain their explicit membership. Inactive component strings
    // are epsilon, so concat and element remain unconditional.
    class AlternativePoster : public RegExVisitor {
    private:
      Home home;
      StringVar x;
      BoolVar active;
      VarArgs& variables;

    public:
      AlternativePoster(Home home0, StringVar x0, BoolVar active0,
                        VarArgs& variables0)
        : home(home0), x(x0), active(active0), variables(variables0) {}

      void
      visit(const EmptyEx&) {
        rel(home, x, STRT_EQ, StringVar(home, ""));
      }

      void
      visit(const CharEx& node) {
        StringVar literal(home, StringVal::decode_utf8(node.get_string()));
        rel(home, x, STRT_EQ, literal, Reify(active, RM_IMP));
      }

      void
      visit(const RangeEx& node) {
        const int lower = node.min_length();
        if (lower < 0 || lower > node.max_length()) {
          home.fail();
          return;
        }
        const int target_max = x.max_length();
        const int upper = std::min(node.max_length(), target_max);
        const NSIntSet symbols = node.get_chars(x.may_chars());
        if (symbols.empty() || upper == 0) {
          rel(home, x, STRT_EQ, StringVar(home, ""));
        } else {
          NSBlocks blocks({NSBlock(symbols, 0, upper)});
          rel(home, x, STRT_DOM, blocks, 0, upper);
        }
        if (home.failed())
          return;
        // Keep the selected minimum even when the effective alphabet is empty.
        if (lower > target_max) {
          rel(home, active, IRT_EQ, 0);
        } else if (lower > 0) {
          IntVar length_var(home, 0, upper);
          variables.iva << length_var;
          length(home, x, length_var);
          rel(home, length_var, IRT_GQ, lower, Reify(active, RM_IMP));
        }
      }

      void
      visit(const StarEx& node) {
        // Existing explicit stars admit epsilon, including the intersection
        // and literal-power routes that native compilation cannot replace.
        node.post(home, x, variables);
      }

      void
      visit(const ConcatEx& node) {
        if (node.left().empty()) {
          node.right().post_alternative(home, x, active, variables);
          return;
        }
        if (node.right().empty()) {
          node.left().post_alternative(home, x, active, variables);
          return;
        }
        const NSIntSet alphabet = x.may_chars();
        const int upper = x.max_length();
        StringVar left(home, alphabet, 0, upper);
        variables.sva << left;
        StringVar right(home, alphabet, 0, upper);
        variables.sva << right;
        node.left().post_alternative(home, left, active, variables);
        node.right().post_alternative(home, right, active, variables);
        rel(home, left, right, STRT_CAT, x);
      }

      void
      visit(const InterEx& node) {
        node.left().post_alternative(home, x, active, variables);
        node.right().post_alternative(home, x, active, variables);
      }

      void
      visit(const UnionEx& node) {
        const NSIntSet alphabet = x.may_chars();
        const int upper = x.max_length();
        StringVar left(home, alphabet, 0, upper);
        variables.sva << left;
        StringVar right(home, alphabet, 0, upper);
        variables.sva << right;
        IntVar selector(home, 1, 2);
        variables.iva << selector;
        BoolVar select_left(home, 0, 1);
        variables.bva << select_left;
        BoolVar select_right(home, 0, 1);
        variables.bva << select_right;
        BoolVar left_active(home, 0, 1);
        variables.bva << left_active;
        BoolVar right_active(home, 0, 1);
        variables.bva << right_active;
        rel(home, selector, IRT_EQ, 1, Reify(select_left));
        rel(home, selector, IRT_EQ, 2, Reify(select_right));
        rel(home, active, BOT_AND, select_left, left_active);
        rel(home, active, BOT_AND, select_right, right_active);
        rel(home, active, BOT_OR, select_left, 1);
        element(home, StringVarArgs() << left << right, selector, x);
        node.left().post_alternative(home, left, left_active, variables);
        node.right().post_alternative(home, right, right_active, variables);
      }
    };

  } // namespace

  void
  RegEx::post_alternative(Home home, StringVar x, BoolVar active,
                          VarArgs& variables) const {
    GECODE_POST;
    BoolVar empty(home, 0, 1);
    variables.bva << empty;
    rel(home, x, STRT_EQ, StringVar(home, ""), Reify(empty));
    rel(home, active, BOT_OR, empty, 1);
    if (home.failed())
      return;
    AlternativePoster poster(home, x, active, variables);
    accept(poster);
  }

  void
  RegEx::post_automaton(Home home, StringVar x) const {
    GECODE_POST;
    TrimDFAHandle automaton(symbolic_dfa(x.may_chars()));
    GECODE_ES_FAIL(Reg::post(home, StringView(x), automaton.get()));
  }

  trimDFA*
  RegEx::symbolic_dfa(const NSIntSet& active_alphabet) const {
    return symbolic_dfa(active_alphabet, false, false);
  }

  trimDFA*
  RegEx::symbolic_dfa(const NSIntSet& active_alphabet,
                      bool leading_wildcard, bool trailing_wildcard) const {
    std::vector<NSIntSet> sets;
    collect_symbol_sets(active_alphabet, sets);
    if (leading_wildcard || trailing_wildcard) {
      std::vector<NSIntSet> active_sets;
      active_sets.reserve(sets.size() + 1);
      for (const NSIntSet& set : sets) {
        NSIntSet overlap(set);
        overlap.intersect(active_alphabet);
        if (!overlap.empty())
          active_sets.push_back(std::move(overlap));
      }
      sets.swap(active_sets);
    }
    if ((leading_wildcard || trailing_wildcard) && !active_alphabet.empty())
      sets.push_back(active_alphabet);

    std::vector<NSIntSet> partitions;
    for (const NSIntSet& set : sets) {
      NSIntSet remaining(set);
      const unsigned int count = partitions.size();
      for (unsigned int i = 0; i < count; ++i) {
        NSIntSet overlap(partitions[i]);
        overlap.intersect(set);
        if (overlap.empty())
          continue;
        remaining.exclude(overlap);
        if (!(overlap == partitions[i])) {
          NSIntSet outside(partitions[i]);
          outside.exclude(set);
          partitions[i] = std::move(overlap);
          partitions.push_back(std::move(outside));
        }
      }
      if (!remaining.empty())
        partitions.push_back(std::move(remaining));
    }

    RegLanguage pattern = symbolic_reg(partitions, active_alphabet);
    if (leading_wildcard || trailing_wildcard) {
      const RegLanguage sigma_star =
          symbolic_class(active_alphabet, partitions).star();
      if (leading_wildcard)
        pattern = sigma_star.concat(pattern);
      if (trailing_wildcard)
        pattern = pattern.concat(sigma_star);
    }
    return new trimDFA(pattern.dfa(), partitions);
  }

}}

namespace Gecode {

  namespace {

    void
    post_native_regex(Home home, StringVar x, const String::RegEx& expression) {
      if (expression.empty()) {
        rel(home, x, STRT_EQ, StringVar(home, ""));
        return;
      }
      const String::InterEx* intersection =
        dynamic_cast<const String::InterEx*>(&expression);
      if (intersection != nullptr) {
        post_native_regex(home, x, intersection->left());
        post_native_regex(home, x, intersection->right());
        return;
      }
      expression.post_automaton(home, x);
    }

    void
    post_native_regex(Home home, StringVar x, const String::RegEx& expression,
                      BoolVar b, ReifyMode rm) {
      if (expression.empty()) {
        rel(home, x, STRT_EQ, StringVar(home, ""), Reify(b, rm));
        return;
      }
      const String::InterEx* intersection =
        dynamic_cast<const String::InterEx*>(&expression);
      if (intersection == nullptr) {
        String::TrimDFAHandle automaton(
          expression.symbolic_dfa(x.may_chars()));
        String::CompDFAHandle complete(
          new String::compDFA(*automaton, x.may_chars()));
        switch (rm) {
          case RM_IMP:
            GECODE_ES_FAIL((String::ReReg<Int::BoolView, RM_IMP>::post(
              home, String::StringView(x), complete.get(), b)));
            break;
          case RM_PMI:
            GECODE_ES_FAIL((String::ReReg<Int::BoolView, RM_PMI>::post(
              home, String::StringView(x), complete.get(), b)));
            break;
          default:
            GECODE_ES_FAIL((String::ReReg<Int::BoolView, RM_EQV>::post(
              home, String::StringView(x), complete.get(), b)));
        }
        return;
      }

      BoolVar left(home, 0, 1);
      BoolVar right(home, 0, 1);
      BoolVar conjunction(home, 0, 1);
      post_native_regex(home, x, intersection->left(), left, RM_EQV);
      post_native_regex(home, x, intersection->right(), right, RM_EQV);
      rel(home, left, BOT_AND, right, conjunction);
      switch (rm) {
        case RM_IMP:
          rel(home, b, IRT_LQ, conjunction);
          break;
        case RM_PMI:
          rel(home, conjunction, IRT_LQ, b);
          break;
        default:
          rel(home, conjunction, IRT_EQ, b);
      }
    }

  }

  void
  extensional(Home home, StringVar x, String::RegEx* r) {
    if (Gecode::String::DashedString::_DECOMP_REGEX && r->decomp()) {
      VarArgs v;
      r->post(home, x, v);
    }
    else
      post_native_regex(home, x, *r);
  }

  void
  extensional(Home home, StringVar x, std::string regex, VarArgs& v) {
    std::unique_ptr<String::RegEx> r = String::RegExParser(regex).parse();
    if (Gecode::String::DashedString::_DECOMP_REGEX && r->decomp())
      r->post(home, x, v);
    else
      post_native_regex(home, x, *r);
  }

  void
  extensional(Home home, StringVar x, std::string regex) {
    std::unique_ptr<String::RegEx> r = String::RegExParser(regex).parse();
    if (Gecode::String::DashedString::_DECOMP_REGEX && !r->intro_vars()) {
      VarArgs v;
      r->post(home, x, v);
    }
    else
      post_native_regex(home, x, *r);
  }

  void
  extensional(
    Home home, StringVar x, const DFA& d, BoolVar b, ReifyMode rm = RM_EQV
  ) {
    switch (rm) {
      case RM_IMP:
        GECODE_ES_FAIL((String::ReReg<Int::BoolView, RM_IMP>
          ::post(home, x, d, b)));
        return;
      case RM_PMI:
        GECODE_ES_FAIL((String::ReReg<Int::BoolView, RM_PMI>
          ::post(home, x, d, b)));
        return;
      default:
        GECODE_ES_FAIL((String::ReReg<Int::BoolView, RM_EQV>
          ::post(home, x, d, b)));
    }
  }

  void
  extensional(
    Home home, StringVar x, std::string regex, BoolVar b, ReifyMode rm = RM_EQV
  ) {
    std::unique_ptr<String::RegEx> r = String::RegExParser(regex).parse();
    post_native_regex(home, x, *r, b, rm);
  }

  void
  extensional(Home home, StringVar x, DFA d) {
    GECODE_ES_FAIL((String::Reg::post(home, x, d)));
  }

  void
  extensional(Home home, StringVar x, StringVarArgs y) {
    GECODE_POST;
    if (y.size() == 0) {
      home.fail();
      return;
    }
    IntVar i(home, 1, y.size());
    element(home, y, i, x);
  }

  void
  extensional(
    Home home, StringVar x, const std::vector<String::StringVal>& y
  ) {
    GECODE_POST;
    if (y.empty()) {
      home.fail();
      return;
    }
    IntVar i(home, 1, y.size());
    StringVarArgs a;
    for (const String::StringVal& value : y)
      a << StringVar(home, value);
    element(home, a, i, x);
  }

  void
  extensional(Home home, StringVar x, const std::vector<std::string>& y) {
    GECODE_POST;
    if (y.empty()) {
      home.fail();
      return;
    }
    IntVar i(home, 1, y.size());
    StringVarArgs a;
    for (auto& s : y)
      a << StringVar(home, s);
    element(home, a, i, x);
  }

}

// STATISTICS: string-post
