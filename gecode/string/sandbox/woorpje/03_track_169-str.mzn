var string: J;
var string: I;
var string: K;
var string: G;
var string: L;
var string: O;
var string: M;
var string: P;
var string: Q;
constraint
  ((I) ++ (G) ++ ("aaaaa") ++ (I) ++ (G) ++ ("aaaabaaaa") ++ (K) ++ (K) ++ ("aab") ++ (L) ++ (M) ++ ("ab") ++ (O) ++ (O) ++ ("b") ++ (P))
    =
    (("aaaaaaaaaaaaaaaaa") ++ (J) ++ ("a") ++ (K) ++ ("a") ++ (J) ++ ("a") ++ (K) ++ ("ab") ++ (M) ++ ("a") ++ (M) ++ (M) ++ (M) ++ ("a") ++ (M) ++ (M) ++ ("b") ++ (O) ++ (M) ++ (O) ++ (M) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
