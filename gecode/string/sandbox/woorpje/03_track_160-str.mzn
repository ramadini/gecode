var string: H;
var string: K;
var string: I;
var string: G;
var string: J;
var string: M;
constraint
  ((H) ++ ("aa") ++ (H) ++ ("ab") ++ (K) ++ (J) ++ ("b") ++ (K)) =
    (("aaa") ++ (G) ++ ("a") ++ (H) ++ (I) ++ (H) ++ (I) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
