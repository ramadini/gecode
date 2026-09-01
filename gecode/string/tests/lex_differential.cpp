#include <gecode/string.hh>

#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>

using namespace Gecode;
using namespace Gecode::String;

namespace {

  int
  min_length(const NSBlocks& domain) {
    int length = 0;
    for (const NSBlock& block : domain)
      length += block.l;
    return length;
  }

  int
  max_length(const NSBlocks& domain) {
    int length = 0;
    for (const NSBlock& block : domain)
      length += block.u;
    return length;
  }

  StringVar
  make_string(Space& home, const NSBlocks& source) {
    NSBlocks domain(source);
    return StringVar(home, domain, min_length(domain), max_length(domain));
  }

  void enumerate_blocks(const NSBlocks&, unsigned int, std::string&,
                        std::set<std::string>&);

  void
  enumerate_characters(const NSBlocks& domain, unsigned int block,
                       int remaining, std::string& value,
                       std::set<std::string>& values) {
    if (remaining == 0) {
      enumerate_blocks(domain, block + 1, value, values);
      return;
    }
    for (NSIntSet::iterator character(domain[block].S); character();
         ++character) {
      value.push_back(static_cast<char>(*character));
      enumerate_characters(domain, block, remaining - 1, value, values);
      value.erase(value.size() - 1);
    }
  }

  void
  enumerate_blocks(const NSBlocks& domain, unsigned int block,
                   std::string& value, std::set<std::string>& values) {
    if (block == domain.size()) {
      values.insert(value);
      return;
    }
    for (int count = domain[block].l; count <= domain[block].u; ++count)
      enumerate_characters(domain, block, count, value, values);
  }

  std::set<std::string>
  concretizations(const NSBlocks& domain) {
    std::set<std::string> values;
    std::string value;
    enumerate_blocks(domain, 0, value, values);
    return values;
  }

  class LexModel : public Space {
  public:
    StringVar left;
    StringVar right;

    LexModel(const NSBlocks& left_domain, const NSBlocks& right_domain,
             bool strict)
      : left(make_string(*this, left_domain)),
        right(make_string(*this, right_domain)) {
      rel(*this, left, strict ? STRT_LEXLT : STRT_LEXLQ, right);
    }

    LexModel(LexModel& model) : Space(model) {
      left.update(*this, model.left);
      right.update(*this, model.right);
    }

    virtual Space* copy(void) {
      return new LexModel(*this);
    }

    void constrain(const std::string& left_value,
                   const std::string& right_value) {
      rel(*this, left, STRT_EQ, StringVar(*this, left_value));
      rel(*this, right, STRT_EQ, StringVar(*this, right_value));
    }
  };

  bool
  check_pair(const NSBlocks& left_domain, const NSBlocks& right_domain,
             bool strict) {
    const std::set<std::string> left_values = concretizations(left_domain);
    const std::set<std::string> right_values = concretizations(right_domain);
    bool oracle_satisfiable = false;
    for (const std::string& left : left_values)
      for (const std::string& right : right_values)
        oracle_satisfiable |= strict ? left < right : left <= right;

    LexModel initial(left_domain, right_domain, strict);
    const bool propagator_satisfiable = initial.status() != SS_FAILED;
    if (propagator_satisfiable != oracle_satisfiable) {
      std::cerr << "LEX extrema mismatch: " << left_domain
                << (strict ? " < " : " <= ") << right_domain << std::endl;
      return false;
    }
    if (!propagator_satisfiable)
      return true;

    for (const std::string& left : left_values) {
      for (const std::string& right : right_values) {
        LexModel* concrete = static_cast<LexModel*>(initial.clone());
        concrete->constrain(left, right);
        const bool actual = concrete->status() != SS_FAILED;
        delete concrete;
        const bool expected = strict ? left < right : left <= right;
        if (actual != expected) {
          std::cerr << "LEX support mismatch: <" << left << ">"
                    << (strict ? " < " : " <= ") << '<' << right << ">\n"
                    << "left domain: " << left_domain << "\n"
                    << "right domain: " << right_domain << "\n"
                    << "left after propagation: " << initial.left << "\n"
                    << "right after propagation: " << initial.right
                    << std::endl;
          return false;
        }
      }
    }
    return true;
  }

  NSIntSet
  characters(unsigned int mask, const std::string& alphabet) {
    NSIntSet result;
    for (unsigned int i = 0; i < alphabet.size(); ++i)
      if (mask & (1U << i))
        result.add(static_cast<unsigned char>(alphabet[i]));
    return result;
  }

  NSBlocks
  random_domain(std::mt19937& random) {
    static const int bounds[][2] = {{0, 1}, {1, 1}, {0, 2}, {1, 2}};
    const std::string alphabet("Aab");
    NSBlocks domain;
    const int blocks = 1 + static_cast<int>(random() % 3);
    for (int i = 0; i < blocks; ++i) {
      const unsigned int mask = 1 + random() % 7;
      const int* selected = bounds[random() % 4];
      domain.push_back(NSBlock(characters(mask, alphabet),
                               selected[0], selected[1]));
    }
    return domain;
  }

}

int
main(void) {
  std::vector<NSBlock> blocks;
  const std::string alphabet("ab");
  for (unsigned int mask = 1; mask < 4; ++mask) {
    const NSIntSet set = characters(mask, alphabet);
    blocks.push_back(NSBlock(set, 0, 1));
    blocks.push_back(NSBlock(set, 1, 1));
  }

  std::vector<NSBlocks> domains;
  for (const NSBlock& block : blocks)
    domains.push_back(NSBlocks(1, block));
  for (const NSBlock& first : blocks)
    for (const NSBlock& second : blocks) {
      NSBlocks domain;
      domain.push_back(first);
      domain.push_back(second);
      domains.push_back(domain);
    }

  for (const NSBlocks& left : domains)
    for (const NSBlocks& right : domains)
      if (!check_pair(left, right, false) ||
          !check_pair(left, right, true))
        return 1;

  // LEX-1: max(a? {A,a}? b) is b, not ab.
  NSBlocks lex1;
  lex1.push_back(NSBlock(NSIntSet('a'), 0, 1));
  NSIntSet middle('A');
  middle.add('a');
  lex1.push_back(NSBlock(middle, 0, 1));
  lex1.push_back(NSBlock(NSIntSet('b'), 1, 1));
  if (!check_pair(NSBlocks("b"), lex1, false))
    return 1;

  std::mt19937 random(0x4c455831U);
  int checked = 0;
  while (checked < 200) {
    const NSBlocks left = random_domain(random);
    const NSBlocks right = random_domain(random);
    if (concretizations(left).size() > 32 ||
        concretizations(right).size() > 32)
      continue;
    if (!check_pair(left, right, false) ||
        !check_pair(left, right, true))
      return 1;
    ++checked;
  }

  std::cout << "LEX differential tests passed" << std::endl;
  return 0;
}
