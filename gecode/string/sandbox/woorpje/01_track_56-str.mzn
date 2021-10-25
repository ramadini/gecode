var string: C;
var string: F;
constraint
  (("bbbaaabbaaaaaaab") ++ (C) ++ ("abbbabbaababbababaaabbbbbbbbaabbbbaabbabbaabbabaaabbbbbaababaaaba"))
    =
    (("bbbaaabbaaaaaaabaabaabbaab") ++ (F) ++ ("bbababaaabbbbbbbbaabbbbaabbabbaabbabaaabbbbbaababaaaba"));
solve satisfy;
