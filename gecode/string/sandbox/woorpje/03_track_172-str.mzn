var string: S;
var string: J;
var string: V;
var string: F;
var string: L;
var string: M;
var string: P;
var string: Q;
constraint
  ((J) ++ ("aaa") ++ (J) ++ (J) ++ ("aaa") ++ (J) ++ ("aaa") ++ (J) ++ (J) ++ ("aab") ++ (J) ++ (M) ++ ("ba") ++ (P) ++ ("b") ++ (V))
    =
    (("a") ++ (F) ++ (F) ++ (L) ++ (M) ++ (L) ++ (M) ++ ("b") ++ (Q) ++ (P) ++ (Q) ++ (P) ++ ("b") ++ (S) ++ (S) ++ ("baa"));
solve satisfy;
