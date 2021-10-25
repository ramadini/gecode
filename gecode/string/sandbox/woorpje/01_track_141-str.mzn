var string: I;
var string: G;
var string: E;
var string: F;
constraint
  ((E) ++ (E) ++ (E) ++ (G)) =
    ((F) ++ (F) ++ (I) ++ ("aaaaaaaaa") ++ (G) ++ (G) ++ ("aaaaaaaaaa") ++ (G) ++ ("aaaaaaaa"));
solve satisfy;
