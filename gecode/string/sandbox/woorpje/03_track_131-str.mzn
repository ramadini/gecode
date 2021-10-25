var string: I;
var string: R;
var string: H;
var string: F;
var string: L;
var string: P;
constraint
  ((F) ++ ("a") ++ (I) ++ (H) ++ (I) ++ ("a") ++ (F) ++ ("a") ++ (I) ++ (H) ++ (I) ++ ("baa") ++ (I) ++ (I) ++ ("b") ++ (L) ++ (L) ++ ("b") ++ (R))
    =
    (("aa") ++ (I) ++ (H) ++ (H) ++ (I) ++ ("a") ++ (L) ++ ("a") ++ (I) ++ (L) ++ ("a") ++ (I) ++ ("b") ++ (L) ++ ("a") ++ (L) ++ ("ab") ++ (P) ++ (P) ++ ("baa"));
solve satisfy;
