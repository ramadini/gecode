var string: J;
var string: H;
var string: I;
var string: G;
var string: F;
var string: L;
var string: O;
var string: M;
var string: P;
constraint
  ((G) ++ ("a") ++ (H) ++ (F) ++ (G) ++ ("a") ++ (G) ++ ("a") ++ (H) ++ (F) ++ (G) ++ ("b") ++ (I) ++ (I) ++ (L) ++ (L) ++ ("b") ++ (L) ++ (L) ++ ("b") ++ (O))
    =
    (("a") ++ (G) ++ (H) ++ (H) ++ ("a") ++ (H) ++ ("a") ++ (H) ++ (J) ++ (J) ++ (J) ++ (J) ++ ("b") ++ (M) ++ (L) ++ (M) ++ (L) ++ ("b") ++ (P) ++ (P) ++ ("baa"));
solve satisfy;
