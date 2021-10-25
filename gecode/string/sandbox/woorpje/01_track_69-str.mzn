var string: D;
var string: A;
constraint
  (("b") ++ (D) ++ ("bbbabbabbaabababababbabaabbbbaa") ++ (D) ++ (D) ++ ("babbbbba") ++ (D) ++ (D) ++ (A) ++ ("bbabababbbaabaababaaaaab") ++ (D) ++ ("bbbababb") ++ (D) ++ ("ababaaaaaabbbabbaabbab"))
    =
    (("baabbbbabbabb") ++ (D) ++ ("ababababbabaabbbbaaaabaabbabbbbba") ++ (D) ++ ("aabbbbbbababaabaababba") ++ (D) ++ ("bb") ++ (D) ++ ("bbb") ++ (D) ++ ("babababbb") ++ (D) ++ (D) ++ ("abaaaaabaabbbbababbaabababaaaaaabbbabb") ++ (D) ++ ("bab"));
solve satisfy;
