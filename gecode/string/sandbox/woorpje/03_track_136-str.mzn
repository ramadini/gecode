var string: I;
var string: H;
var string: F;
var string: J;
var string: N;
var string: O;
var string: L;
constraint
  ((F) ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aaa") ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ (F) ++ ("aabaa") ++ (I) ++ ("b") ++ (L) ++ (O) ++ ("b") ++ (O))
    =
    (("a") ++ (H) ++ ("a") ++ (F) ++ ("aaaaa") ++ (J) ++ ("aa") ++ (J) ++ ("b") ++ (N) ++ (L) ++ (N) ++ (L) ++ ("baabaa"));
solve satisfy;
