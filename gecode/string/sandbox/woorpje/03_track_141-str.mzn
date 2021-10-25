var string: H;
var string: G;
var string: E;
var string: F;
constraint
  ((G) ++ ("aa") ++ (G) ++ ("ab") ++ (G)) =
    (("a") ++ (E) ++ (F) ++ (H) ++ (H) ++ ("baa"));
solve satisfy;
