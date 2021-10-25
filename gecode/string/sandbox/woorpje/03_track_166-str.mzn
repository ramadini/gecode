var string: S;
var string: J;
var string: T;
var string: I;
var string: K;
var string: U;
var string: L;
var string: M;
var string: P;
var string: Q;
constraint
  ((I) ++ ("aaaaaa") ++ (I) ++ ("aaaaab") ++ (K) ++ (K) ++ (K) ++ ("aaa") ++ (K) ++ (K) ++ ("b") ++ (P) ++ ("aab") ++ (S) ++ (S) ++ ("b") ++ (T))
    =
    (("aaaaaaaa") ++ (J) ++ ("aa") ++ (L) ++ (K) ++ ("a") ++ (M) ++ (L) ++ (K) ++ ("a") ++ (M) ++ ("b") ++ (M) ++ ("a") ++ (M) ++ ("aba") ++ (Q) ++ ("a") ++ (Q) ++ ("b") ++ (U) ++ (U) ++ ("baa"));
solve satisfy;
