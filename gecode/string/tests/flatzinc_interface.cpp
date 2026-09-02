#include <gecode/flatzinc.hh>
#include <gecode/search.hh>

#include <cassert>
#include <sstream>
#include <string>

using namespace Gecode;

std::string
solve(const std::string& model) {
  FlatZinc::Printer printer;
  std::ostringstream errors;
  std::istringstream input(model);
  FlatZinc::FlatZincSpace* root = FlatZinc::parse(input, printer, errors);
  assert(root != NULL);
  assert(errors.str().empty());

  FlatZinc::FlatZincOptions options("FlatZinc string test");
  root->createBranchers(printer, root->solveAnnotations(), options, true,
                        errors);
  DFS<FlatZinc::FlatZincSpace> engine(root);
  FlatZinc::FlatZincSpace* solution = engine.next();
  assert(solution != NULL);

  std::ostringstream output;
  solution->print(output, printer);
  delete solution;
  return output.str();
}

bool
satisfiable(const std::string& model) {
  FlatZinc::Printer printer;
  std::ostringstream errors;
  std::istringstream input(model);
  FlatZinc::FlatZincSpace* root = FlatZinc::parse(input, printer, errors);
  assert(root != NULL);
  assert(errors.str().empty());
  if (root->status() == SS_FAILED) {
    delete root;
    return false;
  }

  FlatZinc::FlatZincOptions options("FlatZinc string test");
  root->createBranchers(printer, root->solveAnnotations(), options, true,
                        errors);
  DFS<FlatZinc::FlatZincSpace> engine(root);
  FlatZinc::FlatZincSpace* solution = engine.next();
  bool result = solution != NULL;
  delete solution;
  return result;
}

int
main(void) {
  assert(solve(
    "var string: text :: output_var;\n"
    "constraint str_eq(text, \"b\");\n"
    "solve satisfy;\n") == "text = b;\n");

  assert(solve(
    "string: expected = \"cat\";\n"
    "var string: source = expected;\n"
    "var string: text :: output_var = source;\n"
    "constraint str_eq(text, \"cat\");\n"
    "solve satisfy;\n") == "text = cat;\n");

  std::string disequality = solve(
    "var string: text :: output_var;\n"
    "constraint str_ne(text, \"b\");\n"
    "solve satisfy;\n");
  assert(disequality.find("text = ") == 0);
  assert(disequality != "text = b;\n");

  assert(solve(
    "var int: n :: output_var;\n"
    "var string: text = \"cat\";\n"
    "constraint str_len(text, n);\n"
    "solve satisfy;\n") == "n = 3;\n");

  assert(solve(
    "var int: n :: output_var;\n"
    "var string: text = \"\\\\\";\n"
    "constraint str_len(text, n);\n"
    "solve satisfy;\n") == "n = 1;\n");

  assert(solve(
    "var int: n :: output_var;\n"
    "var string: text = \"\\x5cA\";\n"
    "constraint str_len(text, n);\n"
    "solve satisfy;\n") == "n = 2;\n");

  assert(solve(
    "var int: n :: output_var;\n"
    "var string: text = \"a\\x00\";\n"
    "constraint str_len(text, n);\n"
    "solve satisfy;\n") == "n = 2;\n");

  assert(solve(
    "var int: code :: output_var;\n"
    "var string(1) of {\"\\x00\"}: text;\n"
    "constraint str_len(text, 1);\n"
    "constraint str_char2code(text, code);\n"
    "solve satisfy;\n") == "code = 0;\n");

  assert(satisfiable(
    "array [1..1] of var string: text;\n"
    "constraint str_eq(text[1], \"ok\");\n"
    "solve satisfy;\n"));

  assert(!satisfiable(
    "var string(1): text = \"ab\";\n"
    "solve satisfy;\n"));

  assert(!satisfiable(
    "var string of {\"a\"}: text = \"b\";\n"
    "solve satisfy;\n"));

  assert(!satisfiable(
    "var string: source = \"ab\";\n"
    "var string(1): text = source;\n"
    "solve satisfy;\n"));

  assert(satisfiable(
    "string: pattern = \"\\(\";\n"
    "var string: text = \"(\";\n"
    "constraint str_reg(text, pattern);\n"
    "solve satisfy;\n"));

  assert(satisfiable(
    "var string: text = \"ab\";\n"
    "constraint str_alphabet(text, {\"a\", \"b\"});\n"
    "solve satisfy;\n"));

  assert(!satisfiable(
    "var string: text = \"a\";\n"
    "constraint str_alphabet(text, {\"a\", \"b\"});\n"
    "solve satisfy;\n"));

  assert(satisfiable(
    "var string: text = \"\";\n"
    "constraint str_alphabet(text, {});\n"
    "solve satisfy;\n"));

  assert(satisfiable(
    "array [1..2] of int: transitions = [2, 0];\n"
    "var bool: accepted;\n"
    "constraint str_dfa_reif(\"a\", 2, {\"a\"}, transitions, 1, 2..2, "
      "accepted);\n"
    "constraint bool_eq(accepted, true);\n"
    "solve satisfy;\n"));

  assert(satisfiable(
    "array [1..0] of int: transitions = [];\n"
    "constraint str_dfa(\"\", 1, {}, transitions, 1, 1..1);\n"
    "solve satisfy;\n"));

  assert(!satisfiable(
    "var string(1) of {\"a\", \"b\"}: x;\n"
    "var string(1) of {\"a\", \"b\"}: y;\n"
    "var string(1) of {\"a\", \"b\"}: z;\n"
    "constraint str_len(x, 1);\n"
    "constraint str_len(y, 1);\n"
    "constraint str_len(z, 1);\n"
    "constraint str_ne(x, y);\n"
    "constraint str_ne(x, z);\n"
    "constraint str_ne(y, z);\n"
    "solve satisfy;\n"));

  assert(solve(
    "var int: code :: output_var;\n"
    "var string(1) of {\"\\xff\"}: text;\n"
    "constraint str_len(text, 1);\n"
    "constraint str_char2code(text, code);\n"
    "solve satisfy;\n") == "code = 255;\n");

  assert(solve(
    "var string: text :: output_var;\n"
    "constraint str_concat(\"ca\", \"t\", text);\n"
    "solve satisfy;\n") == "text = cat;\n");

  assert(solve(
    "var string: text :: output_var;\n"
    "constraint str_sub(\"concatenate\", 1, 3, text);\n"
    "solve satisfy;\n") == "text = con;\n");

  assert(solve(
    "var int: n :: output_var;\n"
    "constraint str_find(\"cat\", \"concatenate\", n);\n"
    "solve satisfy;\n") == "n = 4;\n");

  assert(solve(
    "var string: text :: output_var;\n"
    "constraint str_replace(\"cat\", \"a\", \"o\", text);\n"
    "solve satisfy;\n") == "text = cot;\n");

  assert(solve(
    "var bool: b :: output_var;\n"
    "constraint str_contains_reif(\"concatenate\", \"cat\", b);\n"
    "solve satisfy;\n") == "b = true;\n");

  FlatZinc::Printer printer;
  std::ostringstream errors;
  std::istringstream inconsistent(
    "var string: text;\n"
    "constraint str_eq(text, \"b\");\n"
    "constraint str_ne(text, \"b\");\n"
    "solve satisfy;\n");
  FlatZinc::FlatZincSpace* root =
    FlatZinc::parse(inconsistent, printer, errors);
  assert(root != NULL);
  assert(root->status() == SS_FAILED);
  delete root;
  return 0;
}