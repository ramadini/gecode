var string: S;
var string: J;
var string: H;
var string: K;
var string: R;
var string: V;
var string: G;
var string: L;
var string: M;
var string: P;
constraint
  ((G) ++ ("aa") ++ (J) ++ (J) ++ ("aaaaa") ++ (G) ++ ("aa") ++ (J) ++ (J) ++ ("aaaab") ++ (K) ++ (K) ++ ("a") ++ (K) ++ (K) ++ ("aa") ++ (K) ++ ("b") ++ (P) ++ (P) ++ (P) ++ (M) ++ ("b") ++ (S) ++ (S) ++ ("b") ++ (V))
    =
    (("a") ++ (J) ++ ("aaaa") ++ (J) ++ (H) ++ (L) ++ (L) ++ (K) ++ ("a") ++ (M) ++ (L) ++ (L) ++ (K) ++ ("a") ++ (M) ++ ("ba") ++ (M) ++ (P) ++ (P) ++ ("a") ++ (M) ++ (P) ++ (P) ++ ("b") ++ (R) ++ ("a") ++ (R) ++ ("abaabaa"));
solve satisfy;
