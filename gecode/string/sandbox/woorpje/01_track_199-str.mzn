var string: I;
var string: K;
var string: N;
var string: M;
constraint
  ((K) ++ (M) ++ ("a") ++ (N) ++ ("aaaaa")) =
    ((M) ++ (I) ++ ("aaaaaa") ++ (M) ++ ("aa") ++ (M) ++ ("aa") ++ (M) ++ (M) ++ (M) ++ ("aaaa") ++ (M) ++ ("aa") ++ (M) ++ (M) ++ (M) ++ (M) ++ ("a"));
solve satisfy;
