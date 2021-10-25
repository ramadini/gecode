var string: S;
var string: J;
var string: I;
var string: K;
var string: H;
var string: V;
var string: G;
var string: L;
var string: O;
var string: M;
var string: P;
var string: Q;
constraint
  (("aaaaaa") ++ (G) ++ (I) ++ ("aaaaaaaa") ++ (G) ++ (I) ++ ("aba") ++ (J) ++ (K) ++ (M) ++ (K) ++ ("b") ++ (M) ++ (O) ++ (O) ++ (P) ++ ("b") ++ (P) ++ (S) ++ ("b") ++ (V))
    =
    (("a") ++ (H) ++ (G) ++ (M) ++ (L) ++ (M) ++ ("a") ++ (J) ++ (M) ++ (L) ++ (M) ++ ("a") ++ (J) ++ ("b") ++ (O) ++ (O) ++ (M) ++ (O) ++ (O) ++ (O) ++ (M) ++ (O) ++ ("b") ++ (Q) ++ (P) ++ (Q) ++ (P) ++ ("baabaa"));
solve satisfy;
