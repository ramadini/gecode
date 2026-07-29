#include <gecode/dashed.hh>

void dashed_header_syntax(Gecode::Home home,
                          Gecode::ListVar x,
                          Gecode::ListVar y,
                          Gecode::ListVar z,
                          Gecode::BoolVar b,
                          Gecode::IntVar n) {
  Gecode::rel(home, x, Gecode::IRT_EQ, y);
  Gecode::rel(home, x, Gecode::IRT_NQ, y, Gecode::eqv(b));
  Gecode::concat(home, x, y, z);
  Gecode::length(home, z, n);
}
