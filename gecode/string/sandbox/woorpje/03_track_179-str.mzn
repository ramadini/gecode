var string: S;
var string: N;
var string: J;
var string: I;
var string: H;
var string: V;
var string: G;
var string: L;
var string: P;
var string: Q;
constraint
  ((J) ++ (I) ++ ("a") ++ (J) ++ (I) ++ ("b") ++ (L) ++ ("ab") ++ (P) ++ (P) ++ ("b") ++ (S))
    =
    (("a") ++ (H) ++ (G) ++ ("a") ++ (N) ++ (N) ++ ("b") ++ (Q) ++ (Q) ++ (Q) ++ (Q) ++ ("b") ++ (V) ++ (V) ++ ("baa"));
solve satisfy;
