var string: S;
var string: J;
var string: H;
var string: K;
var string: R;
var string: G;
var string: L;
var string: M;
var string: P;
var string: Q;
constraint
  ((G) ++ ("aaa") ++ (G) ++ ("aab") ++ (K) ++ (K) ++ ("b") ++ (P) ++ ("a") ++ (P) ++ (P) ++ ("b") ++ (S) ++ ("b") ++ (S))
    =
    (("a") ++ (H) ++ ("aaa") ++ (L) ++ (J) ++ ("a") ++ (L) ++ (J) ++ ("ab") ++ (P) ++ ("a") ++ (M) ++ (P) ++ ("a") ++ (M) ++ ("b") ++ (Q) ++ (R) ++ (Q) ++ (R) ++ ("baabaa"));
solve satisfy;
