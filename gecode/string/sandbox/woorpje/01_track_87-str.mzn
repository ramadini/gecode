var string: I;
var string: M;
var string: J;
constraint
  (("cbbbbbabcacabcbc") ++ (J) ++ ("ccccbacccaccaccaaaaaaabbbaccccaaabbacaacaaabbc"))
    =
    (("cb") ++ (I) ++ ("bbbabb") ++ (M) ++ ("aaaaaabbbaccccaaabbacaacaaabbc"));
solve satisfy;
