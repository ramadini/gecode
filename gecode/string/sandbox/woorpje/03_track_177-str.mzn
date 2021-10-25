var string: N;
var string: J;
var string: H;
var string: R;
var string: V;
var string: G;
var string: L;
var string: M;
var string: P;
var string: Q;
constraint
  ((J) ++ (G) ++ (G) ++ ("aa") ++ (J) ++ (G) ++ (G) ++ ("ab") ++ (L) ++ (L) ++ ("b") ++ (P) ++ (P) ++ ("b") ++ (V))
    =
    (("a") ++ (G) ++ ("a") ++ (G) ++ ("a") ++ (H) ++ ("a") ++ (G) ++ (H) ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (Q) ++ (N) ++ (Q) ++ (N) ++ ("b") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
