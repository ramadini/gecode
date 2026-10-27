namespace Gecode { namespace String {

  typedef std::pair<int, unsigned int> MatchLanguageTransition;
  typedef std::vector<std::vector<MatchLanguageTransition>>
    MatchLanguageTransitions;

  forceinline unsigned int
  match_language_cardinality(const MatchLanguageTransitions& outgoing,
                             const std::vector<unsigned char>& accepting) {
    const int states = static_cast<int>(outgoing.size());
    std::vector<std::vector<int>> incoming(states);
    for (int source = 0; source < states; ++source)
      for (const MatchLanguageTransition& transition : outgoing[source])
        incoming[transition.first].push_back(source);

    std::vector<unsigned char> productive(states, 0);
    std::vector<int> queue;
    queue.reserve(states);
    for (int state = 0; state < states; ++state)
      if (accepting[state]) {
        productive[state] = 1;
        queue.push_back(state);
      }
    for (unsigned int head = 0; head < queue.size(); ++head) {
      const int target = queue[head];
      for (int source : incoming[target])
        if (!productive[source]) {
          productive[source] = 1;
          queue.push_back(source);
        }
    }
    if (!productive[0])
      return 0;

    std::vector<unsigned char> live(states, 0);
    queue.clear();
    queue.push_back(0);
    live[0] = 1;
    for (unsigned int head = 0; head < queue.size(); ++head) {
      const int source = queue[head];
      for (const MatchLanguageTransition& transition : outgoing[source]) {
        const int target = transition.first;
        if (productive[target] && !live[target]) {
          live[target] = 1;
          queue.push_back(target);
        }
      }
    }

    std::vector<int> indegree(states, 0);
    int live_states = 0;
    for (int source = 0; source < states; ++source)
      if (live[source]) {
        ++live_states;
        for (const MatchLanguageTransition& transition : outgoing[source]) {
          const int target = transition.first;
          if (live[target])
            ++indegree[target];
        }
      }

    std::vector<int> order;
    order.reserve(live_states);
    queue.clear();
    for (int state = 0; state < states; ++state)
      if (live[state] && indegree[state] == 0)
        queue.push_back(state);
    for (unsigned int head = 0; head < queue.size(); ++head) {
      const int source = queue[head];
      order.push_back(source);
      for (const MatchLanguageTransition& transition : outgoing[source]) {
        const int target = transition.first;
        if (live[target] && --indegree[target] == 0)
          queue.push_back(target);
      }
    }

    // A cycle on a path from the start state to a final state generates
    // infinitely many accepted words.
    if (static_cast<int>(order.size()) != live_states)
      return 2;

    std::vector<unsigned int> words(states, 0);
    for (std::vector<int>::reverse_iterator state = order.rbegin();
         state != order.rend(); ++state) {
      unsigned int count = accepting[*state] ? 1 : 0;
      for (const MatchLanguageTransition& transition : outgoing[*state]) {
        const int target = transition.first;
        if (live[target]) {
          const unsigned int contribution =
            transition.second > 1 && words[target] > 0
              ? 2 : transition.second * words[target];
          count += contribution;
          if (count > 1) {
            count = 2;
            break;
          }
        }
      }
      words[*state] = count;
    }
    return words[0];
  }

  forceinline unsigned int
  match_language_cardinality(const DFA& dfa) {
    MatchLanguageTransitions outgoing(dfa.n_states());
    for (DFA::Transitions transition(dfa); transition(); ++transition)
      outgoing[transition.i_state()].push_back(
        MatchLanguageTransition(transition.o_state(), 1));
    std::vector<unsigned char> accepting(dfa.n_states(), 0);
    for (int state = dfa.final_fst(); state < dfa.final_lst(); ++state)
      accepting[state] = 1;
    return match_language_cardinality(outgoing, accepting);
  }

  forceinline unsigned int
  match_language_cardinality(const trimDFA& dfa) {
    MatchLanguageTransitions outgoing(dfa.n_states);
    for (int source = 0; source < dfa.n_states; ++source)
      for (const std::pair<NSIntSet, int>& transition : dfa.delta[source])
        outgoing[source].push_back(MatchLanguageTransition(
          transition.second,
          static_cast<unsigned int>(std::min(2, transition.first.size()))));
    std::vector<unsigned char> accepting(dfa.n_states, 0);
    for (int state = 0; state < dfa.n_states; ++state)
      accepting[state] = dfa.accepting(state);
    return match_language_cardinality(outgoing, accepting);
  }

  forceinline bool
  match_language_is_singleton(const DFA& dfa) {
    return match_language_cardinality(dfa) == 1;
  }

  forceinline bool
  match_language_is_singleton(const trimDFA& dfa) {
    return match_language_cardinality(dfa) == 1;
  }

  forceinline MatchImplementation
  match_default_implementation(const DFA& dfa) {
    return match_language_is_singleton(dfa) ? MATCH_DFA : MATCH_NFA;
  }

  forceinline MatchImplementation
  match_default_implementation(const trimDFA& dfa) {
    return match_language_is_singleton(dfa) ? MATCH_DFA : MATCH_NFA;
  }

  forceinline ExecStatus
  post_match(Home home, StringView x, const string& re,
             Gecode::Int::IntView i, MatchImplementation implementation) {
    std::unique_ptr<RegEx> expression = RegExParser(re).parse();

    // MATCH is defined only for proper patterns. In particular, accepting
    // epsilon is a violated posting precondition rather than a separate
    // public semantics for empty patterns.
    if (expression->has_empty())
      return ES_FAILED;

    const NSIntSet active_alphabet = x.may_chars();
    TrimDFAHandle language(expression->symbolic_dfa(active_alphabet));
    const unsigned int cardinality = match_language_cardinality(*language);
    if (cardinality == 0)
      return me_failed(i.eq(home, 0)) ? ES_FAILED : ES_OK;

    TrimDFAHandle pref(
      expression->symbolic_dfa(active_alphabet, false, true));
    TrimDFAHandle full(
      expression->symbolic_dfa(active_alphabet, true, true));

    if (pref->accepting_states().empty())
      return me_failed(i.eq(home, 0)) ? ES_FAILED : ES_OK;
    assert(full->accepting(1) && full->accepting_states().size() == 1);
    assert(full->neighbours(1) == NSIntSet(1));
    assert(pref->accepting(1) && pref->accepting_states().size() == 1);
    const int min_length = pref->min_word_length();
    assert(min_length > 0);

    GECODE_ME_CHECK(i.gq(home, 0));
    GECODE_ME_CHECK(
      i.lq(home, std::max(0, x.max_length() - min_length + 1)));
    if (!i.in(0))
      GECODE_ME_CHECK(x.lb(home, i.min() + min_length - 1));

    if (implementation == MATCH_AUTO)
      implementation = match_default_implementation(*language);
    return implementation == MATCH_DFA
      ? MatchDFA::post(home, x, i, full.get(), pref.get(), min_length)
      : MatchNFA::post(home, x, i, pref.get(), full.get(), min_length);
  }

}}

// STATISTICS: string-prop
