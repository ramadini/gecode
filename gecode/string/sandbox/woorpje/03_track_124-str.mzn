var string: H;
var string: G;
var string: F;
var string: E;
constraint
  ((F) ++ ("aa") ++ (F) ++ ("aba") ++ (F) ++ ("b") ++ (H)) =
    (("a") ++ (E) ++ ("aa") ++ (G) ++ (F) ++ (G) ++ (F) ++ ("b") ++ (G) ++ (G) ++ ("baa"));
solve satisfy;
