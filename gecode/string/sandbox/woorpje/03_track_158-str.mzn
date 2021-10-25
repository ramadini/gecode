var string: H;
var string: K;
var string: G;
var string: J;
var string: N;
var string: M;
constraint
  ((H) ++ ("aaaa") ++ (H) ++ ("aaab") ++ (H) ++ (H) ++ ("baab") ++ (N)) =
    (("a") ++ (G) ++ ("aaaaaa") ++ (J) ++ ("a") ++ (J) ++ ("a") ++ (J) ++ ("a") ++ (J) ++ ("ab") ++ (K) ++ ("a") ++ (K) ++ ("ab") ++ (M) ++ (M) ++ ("baa"));
solve satisfy;
