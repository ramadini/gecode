var string: H;
var string: K;
var string: I;
var string: G;
var string: E;
constraint
  (("aa") ++ (E) ++ (E) ++ ("aaa") ++ (E) ++ (E) ++ ("b") ++ (H) ++ ("ab") ++ (K))
    =
    (("aa") ++ (G) ++ (G) ++ (G) ++ ("a") ++ (H) ++ ("a") ++ (H) ++ ("b") ++ (I) ++ (I) ++ ("baa"));
solve satisfy;
