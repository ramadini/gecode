var string: K;
var string: J;
var string: N;
var string: M;
var string: D;
constraint
  ((K) ++ ("aaa") ++ (M) ++ ("b") ++ (J) ++ ("accbcbbb") ++ (N) ++ ("accbaa"))
    =
    (("a") ++ (N) ++ ("bcaaaaaabaacbcaab") ++ (N) ++ ("aba") ++ (D) ++ ("abacaaccbcbbb") ++ (N) ++ ("accbaa"));
solve satisfy;
