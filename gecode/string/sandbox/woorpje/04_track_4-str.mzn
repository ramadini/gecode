var string: B;
var string: A;
var string: C;
constraint
  (("a") ++ (A) ++ ("aaa")) = (("aabbaabaaab") ++ (C) ++ ("aaaaaababbaaa"));
constraint
  ((C) ++ ("abbbab") ++ (A) ++ ("b") ++ (C) ++ (C) ++ ("aabaab") ++ (C) ++ ("aaa") ++ (C) ++ ("baaabbbbba") ++ (C) ++ (C) ++ ("baab"))
    =
    ((C) ++ ("abbbabbbbbaabbbba") ++ (C) ++ ("aabaabbaaaa") ++ (C) ++ ("baaa") ++ (B) ++ ("aba") ++ (C) ++ ("baab"));
constraint
  (("bb") ++ (C) ++ ("baabaaa") ++ (C) ++ ("b") ++ (C) ++ ("ab") ++ (C) ++ ("bb") ++ (C) ++ ("abbbab") ++ (C) ++ ("ababb") ++ (C) ++ ("aaabbbb") ++ (C) ++ ("a") ++ (C) ++ ("a"))
    =
    (("bb") ++ (C) ++ (C) ++ ("abaaababbaabbabb") ++ (C) ++ ("abb") ++ (C) ++ ("bbaababbbaaaabbbb") ++ (C) ++ ("abaa"));
constraint
  (("bbb") ++ (C) ++ ("bbabaabbbbbaaabaaabbaaabbaaa") ++ (C) ++ ("aaaa") ++ (C) ++ ("b") ++ (C) ++ (C) ++ ("bbaabb"))
    =
    (("bbbbab") ++ (C) ++ ("baabbbb") ++ (C) ++ ("aabaaabbaaab") ++ (C) ++ ("aabaaaaa") ++ (C) ++ ("b") ++ (C) ++ ("babbaabb"));
constraint
  (("babbbb") ++ (C) ++ ("b") ++ (C) ++ ("bbabaab") ++ (C) ++ ("bbbbaa") ++ (C) ++ ("ba") ++ (B) ++ ("b") ++ (C) ++ ("aabaaaa") ++ (C) ++ ("aabaa"))
    =
    ((C) ++ (B) ++ ("ab") ++ (C) ++ ("b") ++ (C) ++ ("baab") ++ (C) ++ ("bbbbaababa") ++ (B) ++ ("bbaaabaaaabaaa") ++ (C) ++ ("a"));
constraint
  (("ba") ++ (C) ++ ("a") ++ (C) ++ ("baab") ++ (C) ++ ("aaaaaba") ++ (C) ++ ("a") ++ (C) ++ ("abb") ++ (C) ++ ("b") ++ (C) ++ ("abbbabb") ++ (C) ++ ("a") ++ (C) ++ ("bbbb") ++ (C) ++ ("b"))
    =
    (("babaaba") ++ (C) ++ ("ab") ++ (C) ++ ("aaaaababaa") ++ (C) ++ ("abbbabbaabbbabb") ++ (C) ++ ("aba") ++ (B) ++ ("ab"));
constraint
  (("aaa") ++ (C) ++ ("bb") ++ (C) ++ ("ababaaaaaaaab") ++ (C) ++ ("baabb") ++ (C) ++ (B) ++ ("aab") ++ (C) ++ ("baaaa") ++ (C) ++ ("bbbb"))
    =
    (("aaa") ++ (C) ++ ("bb") ++ (C) ++ ("aba") ++ (C) ++ ("aaaaaaab") ++ (C) ++ (C) ++ ("abbbabbbbbaab") ++ (C) ++ (C) ++ ("aaa") ++ (C) ++ ("bbbb"));
constraint
  (("ba") ++ (C) ++ ("bba") ++ (C) ++ ("baabaab") ++ (C) ++ ("bbaabbbaaaabababbaaaaaab") ++ (C) ++ ("aabbaaaa"))
    =
    (("ba") ++ (C) ++ ("b") ++ (C) ++ (C) ++ (C) ++ ("a") ++ (C) ++ ("ab") ++ (C) ++ ("bbaabbbaaaa") ++ (C) ++ (C) ++ ("b") ++ (C) ++ ("aaaaabbaaab") ++ (C) ++ ("aaa"));
constraint
  (("bb") ++ (C) ++ ("aabbaab") ++ (C) ++ ("b") ++ (C) ++ ("ba") ++ (C) ++ ("baaabaaab") ++ (C) ++ ("a") ++ (C) ++ ("bbbbaabb") ++ (C) ++ ("aabbbaab"))
    =
    (("bbbaaab") ++ (C) ++ ("abbab") ++ (C) ++ (C) ++ ("babaaabaaabbaa") ++ (C) ++ ("bbb") ++ (C) ++ ("abbbaaabbbaab"));
constraint
  (("baabbbbbba") ++ (C) ++ ("baaa") ++ (C) ++ ("aab") ++ (C) ++ ("ab") ++ (C) ++ ("a") ++ (C) ++ ("bbbb") ++ (C) ++ ("ababab") ++ (C) ++ ("babaaabb"))
    =
    ((C) ++ ("a") ++ (B) ++ (C) ++ (C) ++ (C) ++ ("aabaaabbaabbaababbbbbaaba") ++ (C) ++ ("b") ++ (C) ++ (C) ++ (C) ++ ("aabb"));
constraint
  (("baa") ++ (C) ++ ("abbababbbabbbaabbabab") ++ (C) ++ ("baabbaab") ++ (C) ++ ("aa") ++ (C) ++ ("abbaabbbba"))
    =
    ((C) ++ ("abaab") ++ (C) ++ (C) ++ ("bbbabb") ++ (C) ++ ("abba") ++ (C) ++ ("bbabaab") ++ (C) ++ ("abbaaabaab") ++ (C) ++ ("abbbba"));
constraint
  (("b") ++ (C) ++ ("ab") ++ (C) ++ ("aaaaba") ++ (C) ++ ("baaaa") ++ (B) ++ ("a") ++ (C) ++ (C) ++ ("aabb") ++ (C) ++ ("aba") ++ (C) ++ ("bb") ++ (C) ++ ("aaabbbb"))
    =
    (("b") ++ (C) ++ ("abbaaaaa") ++ (C) ++ ("ba") ++ (C) ++ ("aaabbbb") ++ (C) ++ ("babaaabb") ++ (C) ++ ("a") ++ (C) ++ (C) ++ ("bb") ++ (C) ++ ("aaabbbb"));
constraint
  (("aa") ++ (C) ++ ("a") ++ (C) ++ ("aabbbbabb") ++ (C) ++ ("aaaaaaab") ++ (C) ++ ("abaaaaaaaa") ++ (C) ++ ("baaaabb") ++ (C) ++ ("b") ++ (C))
    =
    (("aabaabaaabbbbabb") ++ (C) ++ ("aaaaaaab") ++ (C) ++ ("a") ++ (C) ++ ("aaaaaaa") ++ (C) ++ ("baaaabb") ++ (C) ++ ("b") ++ (C));
constraint
  (("aabbaaabaaaaaaaaa") ++ (C) ++ (C) ++ (C) ++ ("b") ++ (C) ++ ("abba") ++ (C) ++ ("abbaaaa") ++ (C) ++ (C) ++ ("aabbaab") ++ (C))
    =
    (("aabbaaa") ++ (C) ++ ("aaaaaaaababababbaab") ++ (C) ++ (C) ++ ("ab") ++ (C) ++ ("aaaba") ++ (C) ++ ("aabbaab") ++ (C));
constraint
  (("aaaabaaaabab") ++ (C) ++ ("aaabbbab") ++ (C) ++ ("abbb") ++ (C) ++ ("a") ++ (C) ++ ("aa") ++ (C) ++ ("b") ++ (C) ++ ("aba") ++ (C) ++ ("baababa"))
    =
    (("aaaa") ++ (C) ++ ("aaababbaaaabbbab") ++ (C) ++ ("abbbbaabaaabab") ++ (C) ++ ("a") ++ (C) ++ (C) ++ ("baa") ++ (C) ++ ("ba"));
constraint
  (("aaaabb") ++ (C) ++ ("bbbabbaa") ++ (C) ++ ("b") ++ (C) ++ (B) ++ ("bba") ++ (C) ++ ("a") ++ (C) ++ (C) ++ ("ba") ++ (C) ++ ("ab") ++ (C) ++ ("aa") ++ (C) ++ ("aaab"))
    =
    (("aaaabb") ++ (C) ++ ("bbbabbaabab") ++ (C) ++ (B) ++ ("b") ++ (C) ++ ("baa") ++ (C) ++ ("bababaab") ++ (C) ++ ("aa") ++ (C) ++ ("aaab"));
constraint
  (("abaaaabbaaaaabbbaab") ++ (C) ++ ("aaaaabbbababaaaaaaa") ++ (B) ++ ("bbbaaaa"))
    =
    (("abaaaabbaaaaabb") ++ (C) ++ ("ab") ++ (C) ++ ("aaaaabb") ++ (C) ++ (C) ++ (C) ++ ("aaaaaa") ++ (B) ++ ("bb") ++ (C) ++ ("aaa"));
solve satisfy;
