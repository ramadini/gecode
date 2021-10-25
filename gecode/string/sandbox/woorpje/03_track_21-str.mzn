var string: N;
var string: J;
var string: I;
var string: F;
var string: G;
var string: L;
var string: O;
var string: Q;
constraint
  ((F) ++ ("aaa") ++ (I) ++ (F) ++ ("a") ++ (F) ++ ("aaa") ++ (I) ++ (F) ++ ("baa") ++ (J) ++ ("b") ++ (O) ++ (L) ++ ("ba"))
    =
    (("a") ++ (G) ++ (G) ++ (L) ++ (I) ++ (L) ++ (I) ++ ("b") ++ (N) ++ (L) ++ (N) ++ (L) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
