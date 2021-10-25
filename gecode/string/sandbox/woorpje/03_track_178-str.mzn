var string: J;
var string: I;
var string: H;
var string: R;
var string: V;
var string: G;
var string: N;
var string: M;
var string: P;
constraint
  ((J) ++ (G) ++ (I) ++ ("a") ++ (J) ++ ("a") ++ (J) ++ (G) ++ (I) ++ ("a") ++ (J) ++ ("b") ++ (J) ++ (J) ++ ("b") ++ (P) ++ (N) ++ ("b") ++ (V))
    =
    (("a") ++ (H) ++ ("a") ++ (I) ++ (J) ++ ("aaa") ++ (M) ++ ("aaa") ++ (M) ++ ("b") ++ (N) ++ (P) ++ (N) ++ (P) ++ ("b") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
