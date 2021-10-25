var string: B;
var string: A;
constraint
  (("aababbaabbaaaabbbabaaabbbbbabbbaabbbbabbb") ++ (A) ++ ("aaba")) =
    (("aab") ++ (B) ++ ("babbaaaabbbaabbababbaaabbaababababaabbaabababbabaaabbabbaaba"));
solve satisfy;
