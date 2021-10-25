var string: I;
var string: H;
var string: K;
var string: R;
var string: F;
var string: G;
var string: L;
var string: M;
var string: P;
constraint
  ((F) ++ ("aaa") ++ (F) ++ ("aab") ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("b") ++ (L) ++ (L) ++ ("b") ++ (P))
    =
    (("a") ++ (G) ++ (H) ++ (K) ++ (L) ++ (L) ++ (K) ++ (L) ++ (L) ++ ("b") ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
