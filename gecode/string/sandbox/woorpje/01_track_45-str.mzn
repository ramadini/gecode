var string: B;
var string: G;
constraint
  (("baaaaaaabaaa") ++ (B) ++ ("baabbbbbbbaababbaabbbaaabbbbb")) =
    (("baaaaaaabaaaabbbbbbabbbabbabbbaabbaaaababbabbbabbaa") ++ (G) ++ ("baabbbbbbbaababbaabbbaaabbbbb"));
solve satisfy;
