var string: I;
var string: G;
var string: F;
var string: H;
constraint
  ((F) ++ ("aaaa") ++ (F) ++ ("aaaba") ++ (G) ++ (G) ++ ("ab") ++ (I) ++ ("ba"))
    =
    (("a") ++ (G) ++ (G) ++ (G) ++ ("aa") ++ (H) ++ (G) ++ (G) ++ (H) ++ (G) ++ (G) ++ ("baaaab") ++ (I) ++ (I) ++ ("baa"));
solve satisfy;
