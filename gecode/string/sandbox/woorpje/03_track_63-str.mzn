var string: I;
var string: K;
var string: R;
var string: G;
var string: L;
var string: M;
var string: P;
constraint
  ((G) ++ ("a") ++ (G) ++ ("b") ++ (I) ++ (I) ++ (K) ++ (I) ++ ("b") ++ (L) ++ ("ab") ++ (P))
    =
    (("a") ++ (I) ++ ("aaaa") ++ (L) ++ (K) ++ ("a") ++ (L) ++ (K) ++ ("ab") ++ (M) ++ ("a") ++ (M) ++ ("ab") ++ (R) ++ (R) ++ ("baa"));
solve satisfy;
