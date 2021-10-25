var string: B;
var string: E;
var string: C;
constraint
  (("bbaaa") ++ (E) ++ ("aaababbbabaaaabaaaabbba")) =
    (("bb") ++ (B) ++ ("bbbbaaaa") ++ (C) ++ ("babbbaaabbbabbaaabaababaaabbbaaaabaaababbbabaaaabaaaabbba"));
solve satisfy;
