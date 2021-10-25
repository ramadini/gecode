var string: H;
var string: K;
var string: G;
var string: E;
var string: L;
var string: M;
constraint
  ((H) ++ ("aa") ++ (H) ++ ("ab") ++ (K) ++ ("ab") ++ (L)) =
    (("a") ++ (E) ++ ("a") ++ (G) ++ (H) ++ (H) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
