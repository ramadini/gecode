var string: H;
var string: F;
var string: D;
constraint
  (("a") ++ (F) ++ ("aa") ++ (F) ++ ("b") ++ (F)) =
    (("aa") ++ (D) ++ (H) ++ (H) ++ ("baa"));
solve satisfy;
