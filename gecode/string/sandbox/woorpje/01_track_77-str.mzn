var string: C;
var string: F;
constraint
  ((C) ++ ("aaabab") ++ (C) ++ ("bb") ++ (C) ++ ("abbabbabbababb") ++ (C) ++ ("abaaaabaaaab") ++ (C) ++ ("bbbbabbbabaaabb") ++ (C) ++ ("b") ++ (C) ++ ("aabababaabaaabaabbbaaabb"))
    =
    (("ba") ++ (F) ++ (C) ++ ("bbaaaabababaa") ++ (C) ++ ("a") ++ (C) ++ ("bb") ++ (C) ++ ("abb"));
solve satisfy;
