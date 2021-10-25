var string: H;
var string: G;
var string: E;
constraint
  ((E) ++ (E) ++ ("aaa") ++ (E) ++ (E) ++ ("aabaab") ++ (G)) =
    (("aa") ++ (E) ++ ("aa") ++ (G) ++ ("a") ++ (G) ++ ("ab") ++ (H) ++ (H) ++ ("baa"));
solve satisfy;
