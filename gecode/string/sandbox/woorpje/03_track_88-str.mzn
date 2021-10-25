var string: J;
var string: I;
var string: H;
var string: F;
var string: L;
var string: O;
var string: M;
var string: P;
constraint
  (("aa") ++ (F) ++ (I) ++ ("a") ++ (F) ++ ("aaa") ++ (F) ++ (I) ++ ("a") ++ (F) ++ ("b") ++ (J) ++ ("aab") ++ (L) ++ (L) ++ ("b") ++ (O))
    =
    (("a") ++ (H) ++ (F) ++ ("a") ++ (L) ++ (L) ++ (L) ++ (L) ++ ("b") ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (P) ++ (P) ++ ("baa"));
solve satisfy;
