var string: I;
var string: K;
var string: F;
var string: E;
var string: M;
constraint
  (("a") ++ (F) ++ (F) ++ (F) ++ ("aa") ++ (F) ++ (F) ++ (F) ++ ("b") ++ (K) ++ ("aba"))
    =
    (("a") ++ (E) ++ (E) ++ (K) ++ (I) ++ (K) ++ (I) ++ ("b") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
