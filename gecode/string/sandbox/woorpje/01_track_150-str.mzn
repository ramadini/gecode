var string: G;
var string: B;
var string: F;
var string: E;
constraint
  ((F) ++ ("aaaaaa") ++ (B) ++ (F) ++ ("aaaaaa") ++ (E) ++ ("aaaaa")) =
    ((B) ++ ("aaaaaa") ++ (F) ++ (F) ++ (G) ++ ("aaaaaaaaaaaaa"));
solve satisfy;
