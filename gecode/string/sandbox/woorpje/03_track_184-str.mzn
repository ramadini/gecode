var string: I;
var string: G;
var string: F;
var string: E;
var string: J;
constraint
  ((F) ++ ("a") ++ (F) ++ ("aa") ++ (F) ++ ("a") ++ (F) ++ ("ab") ++ (G) ++ (G) ++ ("b") ++ (J))
    = (("aa") ++ (F) ++ (E) ++ ("aaaab") ++ (I) ++ (I) ++ ("baa"));
solve satisfy;
