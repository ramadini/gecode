var string: J;
var string: T;
var string: R;
var string: V;
var string: G;
var string: L;
var string: N;
var string: O;
var string: M;
var string: P;
var string: Q;
constraint
  ((J) ++ (G) ++ ("a") ++ (J) ++ (G) ++ ("b") ++ (J) ++ ("aa") ++ (J) ++ (M) ++ ("a") ++ (J) ++ ("b") ++ (P) ++ (O) ++ (P) ++ ("b") ++ (Q) ++ (P) ++ ("b") ++ (T))
    =
    (("aaaaaa") ++ (G) ++ ("aaaaaa") ++ (L) ++ (L) ++ ("b") ++ (N) ++ ("aa") ++ (N) ++ ("aab") ++ (Q) ++ (R) ++ (Q) ++ (R) ++ ("b") ++ (V) ++ (V) ++ ("baa"));
solve satisfy;
