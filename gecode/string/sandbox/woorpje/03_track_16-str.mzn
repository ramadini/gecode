var string: H;
var string: K;
var string: D;
var string: E;
constraint
  ((D) ++ (E) ++ ("a") ++ (D) ++ (E) ++ ("b") ++ (H)) =
    (("aa") ++ (H) ++ (K) ++ (K) ++ ("baa"));
solve satisfy;
