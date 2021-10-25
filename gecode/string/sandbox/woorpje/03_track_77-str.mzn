var string: J;
var string: I;
var string: H;
var string: L;
var string: N;
var string: O;
var string: M;
var string: P;
constraint
  ((I) ++ (H) ++ ("aaa") ++ (I) ++ (H) ++ ("aab") ++ (I) ++ (I) ++ ("b") ++ (M) ++ ("ab") ++ (N) ++ (N) ++ ("b") ++ (O))
    =
    (("a") ++ (H) ++ (I) ++ ("aa") ++ (J) ++ (J) ++ ("aa") ++ (J) ++ (J) ++ (J) ++ ("aa") ++ (J) ++ ("b") ++ (L) ++ (L) ++ (L) ++ (L) ++ ("ba") ++ (M) ++ ("a") ++ (M) ++ ("b") ++ (P) ++ (P) ++ ("baa"));
solve satisfy;
