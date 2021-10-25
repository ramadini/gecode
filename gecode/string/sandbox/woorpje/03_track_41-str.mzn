var string: I;
var string: K;
var string: G;
var string: F;
var string: L;
var string: O;
constraint
  ((G) ++ ("aaa") ++ (G) ++ ("aab") ++ (K) ++ (L) ++ ("b") ++ (L) ++ ("ba"))
    =
    (("a") ++ (F) ++ (F) ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (O) ++ (O) ++ (O) ++ (O) ++ ("b") ++ (O) ++ (O) ++ ("baa"));
solve satisfy;
