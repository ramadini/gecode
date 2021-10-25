var string: H;
var string: G;
var string: F;
var string: J;
constraint
  ((F) ++ ("aaaaa") ++ (F) ++ ("aaaab") ++ (H) ++ ("ab") ++ (H) ++ ("aba")) =
    (("a") ++ (G) ++ ("aaa") ++ (G) ++ (G) ++ (G) ++ (G) ++ ("b") ++ (H) ++ (H) ++ (H) ++ (H) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
