var string: H;
var string: I;
var string: N;
var string: G;
var string: L;
constraint
  ((H) ++ ("a") ++ (H) ++ ("aa") ++ (H) ++ ("a") ++ (H) ++ ("ab") ++ (H) ++ (H) ++ ("b") ++ (L))
    =
    (("a") ++ (G) ++ (I) ++ (H) ++ (I) ++ (H) ++ ("b") ++ (N) ++ (N) ++ ("baa"));
solve satisfy;
