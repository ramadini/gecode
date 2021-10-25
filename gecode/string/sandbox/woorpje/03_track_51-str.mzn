var string: H;
var string: G;
var string: F;
var string: J;
var string: L;
var string: M;
constraint
  (("a") ++ (H) ++ (H) ++ (G) ++ (H) ++ ("aa") ++ (H) ++ (H) ++ (G) ++ (H) ++ ("b") ++ (H) ++ (J) ++ ("b") ++ (J) ++ (J) ++ ("b") ++ (L))
    =
    (("a") ++ (F) ++ (H) ++ (H) ++ (H) ++ ("aaa") ++ (J) ++ (J) ++ (J) ++ ("a") ++ (J) ++ (J) ++ (J) ++ ("ba") ++ (L) ++ ("a") ++ (L) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
