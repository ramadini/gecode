var string: B;
var string: F;
var string: E;
constraint
  (("bbaaabbbab") ++ (B) ++ ("babbbbbabbbbbabbbabb") ++ (B) ++ ("bbba") ++ (E) ++ ("aaabb") ++ (B) ++ ("aaaaa") ++ (B) ++ ("bbaabbbaabbbbbbbbaabababaa"))
    =
    (("bbaaabbbabbab") ++ (B) ++ ("bbbbabbbbbabb") ++ (B) ++ ("bbabbbbaaabb") ++ (B) ++ ("babaabaaaababb") ++ (F) ++ ("aabbbabaaaaababbbaabbbaabbbbbbbbaabababaa"));
solve satisfy;
