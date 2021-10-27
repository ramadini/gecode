%var string: C;
%var string: F;
%constraint
%  ((C) ++ ("aaabab") ++ (C) ++ ("bb") ++ (C) ++ ("abbabbabbababb") ++ (C) ++ ("abaaaabaaaab") ++ (C) ++ ("bbbbabbbabaaabb") ++ (C) ++ ("b") ++ (C) ++ ("aabababaabaaabaabbbaaabb"))
%    =
%    (("ba") ++ (F) ++ (C) ++ ("bbaaaabababaa") ++ (C) ++ ("a") ++ (C) ++ ("bb") ++ (C) ++ ("abb"));
%solve satisfy;FIXME:

var string of {"a","b"}: C;
var string(62) of {"a","b"}: F;
constraint
  C ++ "aaababbaabbbaaabbabbabbababbbaaabaaaabaaaabbaabbbbabbbabaaabbbaabbaaaabababaabaaabaabbbaaabb"
  =
  "ba" ++ F ++ "baabbaaaabababaa" ++ C ++ "a" ++ C ++ "bbbaaabb";
solve satisfy;

constraint str_len(F)=62 /\ str_char_at(C, 1) = "b";
