var string: B;
var string: F;
constraint
  (("abba") ++ (F) ++ ("bbaaabaaaaaba") ++ (B) ++ ("abbbaaba") ++ (F) ++ (F) ++ ("bbbabbabaa") ++ (F) ++ ("aaaa") ++ (F) ++ ("aaaaabbabaaabab"))
    =
    (("abbaababbaa") ++ (F) ++ ("aaa") ++ (F) ++ ("abb") ++ (F) ++ ("babaaabaabaabbaababbbaaba") ++ (F) ++ (F) ++ ("bbbabbabaaabaaaaaabaaaaaabb") ++ (F) ++ ("aabab"));
solve satisfy;
