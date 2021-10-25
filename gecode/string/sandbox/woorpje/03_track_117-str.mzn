var string: H;
var string: K;
var string: I;
var string: G;
var string: E;
var string: F;
constraint
  ((E) ++ (E) ++ (E) ++ (E) ++ ("a") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("b") ++ (G) ++ ("ab") ++ (K))
    =
    (("a") ++ (F) ++ ("a") ++ (E) ++ (E) ++ (H) ++ (G) ++ (H) ++ (G) ++ ("b") ++ (I) ++ (I) ++ ("baa"));
solve satisfy;
