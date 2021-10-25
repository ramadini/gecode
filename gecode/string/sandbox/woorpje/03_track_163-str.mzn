var string: H;
var string: K;
var string: I;
var string: F;
var string: J;
var string: L;
var string: M;
constraint
  ((H) ++ (H) ++ ("a") ++ (H) ++ (H) ++ ("b") ++ (I) ++ (I) ++ (I) ++ ("ab") ++ (K) ++ (K) ++ ("b") ++ (M))
    =
    (("a") ++ (H) ++ (F) ++ ("a") ++ (F) ++ ("a") ++ (J) ++ (J) ++ (J) ++ (J) ++ (J) ++ (J) ++ (J) ++ (J) ++ ("b") ++ (L) ++ (L) ++ (L) ++ (L) ++ ("b") ++ (L) ++ (L) ++ ("baa"));
solve satisfy;
