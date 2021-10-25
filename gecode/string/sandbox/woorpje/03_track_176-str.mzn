var string: S;
var string: J;
var string: T;
var string: H;
var string: R;
var string: I;
var string: L;
var string: N;
var string: M;
var string: P;
constraint
  ((J) ++ ("a") ++ (J) ++ ("b") ++ (L) ++ (L) ++ (L) ++ (L) ++ ("b") ++ (N) ++ (P) ++ ("b") ++ (S))
    =
    (("a") ++ (H) ++ (H) ++ ("a") ++ (I) ++ (N) ++ (M) ++ (M) ++ (M) ++ (N) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (R) ++ (R) ++ (R) ++ (R) ++ ("b") ++ (T) ++ (T) ++ ("baa"));
solve satisfy;
