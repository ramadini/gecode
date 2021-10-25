var string: I;
var string: H;
var string: G;
var string: F;
var string: J;
var string: N;
var string: M;
constraint
  (("aa") ++ (G) ++ ("aa") ++ (G) ++ ("aaa") ++ (G) ++ ("aa") ++ (G) ++ ("baa") ++ (I) ++ ("ab") ++ (J) ++ ("ab") ++ (N))
    =
    (("a") ++ (F) ++ (G) ++ (G) ++ ("a") ++ (H) ++ (H) ++ ("ba") ++ (J) ++ ("a") ++ (J) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
