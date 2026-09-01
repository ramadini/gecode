#include <gecode/flatzinc.hh>
#include <gecode/string.hh>

#include <cassert>
#include <sstream>

using namespace Gecode;

int
main(void) {
  String::DashedString::_DECOMP_REGEX = true;
  FlatZinc::FlatZincOptions options("Dashed FlatZinc test");
  std::ostringstream errors;

  Rnd invalid_random(0);
  FlatZinc::FlatZincSpace* invalid_space =
    new FlatZinc::FlatZincSpace(invalid_random);
  FlatZinc::Printer invalid_printer;
  std::istringstream invalid_model(
    "var string: text;\n"
    "constraint str_reg(text, \"a\");\n"
    "invalid\n");
  assert(FlatZinc::parse(invalid_model, invalid_printer, options, errors,
                        invalid_space, invalid_random) == NULL);
  delete invalid_space;

  Rnd valid_random(0);
  FlatZinc::Printer valid_printer;
  std::istringstream valid_model(
    "var string: text :: output_var;\n"
    "constraint str_eq(text, \"b\");\n"
    "solve satisfy;\n");
  FlatZinc::FlatZincSpace* valid_space =
    FlatZinc::parse(valid_model, valid_printer, options, errors,
                    NULL, valid_random);
  assert(valid_space != NULL);
  assert(valid_space->status() != SS_FAILED);
  delete valid_space;

  Rnd dfa_random(0);
  FlatZinc::Printer dfa_printer;
  std::istringstream dfa_model(
    "array [1..2] of int: transitions = [2, 0];\n"
    "var bool: accepted;\n"
    "constraint str_dfa_reif(\"a\", 2, {\"a\"}, transitions, 1, 2..2, "
      "accepted);\n"
    "constraint bool_eq(accepted, true);\n"
    "solve satisfy;\n");
  FlatZinc::FlatZincSpace* dfa_space =
    FlatZinc::parse(dfa_model, dfa_printer, options, errors,
                    NULL, dfa_random);
  assert(dfa_space != NULL);
  assert(dfa_space->status() != SS_FAILED);
  delete dfa_space;

  Rnd alphabet_random(0);
  FlatZinc::Printer alphabet_printer;
  std::istringstream valid_alphabet_model(
    "var string: x = \"ab\";\n"
    "constraint str_alphabet(x, {\"a\", \"b\"});\n"
    "solve satisfy;\n");
  FlatZinc::FlatZincSpace* alphabet_space =
    FlatZinc::parse(valid_alphabet_model, alphabet_printer, options, errors,
                    NULL, alphabet_random);
  assert(alphabet_space != NULL);
  assert(alphabet_space->status() != SS_FAILED);
  delete alphabet_space;

  std::istringstream invalid_alphabet_model(
    "var string: x = \"a\";\n"
    "constraint str_alphabet(x, {\"a\", \"b\"});\n"
    "solve satisfy;\n");
  alphabet_space =
    FlatZinc::parse(invalid_alphabet_model, alphabet_printer, options, errors,
                    NULL, alphabet_random);
  assert(alphabet_space != NULL);
  assert(alphabet_space->status() == SS_FAILED);
  delete alphabet_space;

  std::istringstream empty_alphabet_model(
    "var string: x = \"\";\n"
    "constraint str_alphabet(x, {});\n"
    "solve satisfy;\n");
  alphabet_space =
    FlatZinc::parse(empty_alphabet_model, alphabet_printer, options, errors,
                    NULL, alphabet_random);
  assert(alphabet_space != NULL);
  assert(alphabet_space->status() != SS_FAILED);
  delete alphabet_space;

  std::istringstream active_domain_model(
    "var string: x;\n"
    "constraint str_chars(x, {\"a\", \"b\"});\n"
    "constraint str_len(x, 1..2);\n"
    "solve satisfy;\n");
  alphabet_space =
    FlatZinc::parse(active_domain_model, alphabet_printer, options, errors,
                    NULL, alphabet_random);
  assert(alphabet_space != NULL);
  assert(alphabet_space->status() != SS_FAILED);
  FlatZinc::FlatZincSpace* cloned_space =
    static_cast<FlatZinc::FlatZincSpace*>(alphabet_space->clone());
  assert(cloned_space->status() != SS_FAILED);
  delete cloned_space;
  delete alphabet_space;

  Rnd nfa_random(0);
  FlatZinc::FlatZincSpace* nfa_space =
    new FlatZinc::FlatZincSpace(nfa_random);
  FlatZinc::Printer nfa_printer;
  std::istringstream nfa_model(
    "array [1..2] of set of int: transitions = [{2}, {}];\n"
    "constraint str_nfa(\"a\", 2, {\"a\"}, transitions, 1, 2..2);\n"
    "solve satisfy;\n");
  bool nfa_rejected = false;
  try {
    (void) FlatZinc::parse(nfa_model, nfa_printer, options, errors,
                          nfa_space, nfa_random);
  } catch (const FlatZinc::Error& error) {
    nfa_rejected = error.toString().find("str_nfa") != std::string::npos;
  }
  assert(nfa_rejected);
  delete nfa_space;
  return 0;
}