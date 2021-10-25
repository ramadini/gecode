var string: B;
var string: A;
var string: C;
var string: D;
constraint
  (("aaaabababbbbaaaabbbaaaaabbbbbabbbaab") ++ (A) ++ ("abbb")) =
    (("aaaabababbbbaaaabbbaaaaabbbbbabbbaabbbaaa") ++ (B) ++ ("b"));
constraint
  (("bababbabbabbbaabbabababbaababaaabaaaaa") ++ (C) ++ ("babbaaabbbaabaabaaaaaabbbaaaaabaaabaaaaaaabb"))
    =
    (("bababbabbabbbaabbabababbaababaaabaaaaabbaaaaabababbabbaaabbbaabaabaaaaaabbbaaaaabaaabaaaaaaabb") ++ (""));
constraint
  (("abaabbbbbbbabbabaabbabaaabaabbaabbaaabbabbbaabaabaaba") ++ (C) ++ ("abaabbbbaaabbbaabbbbabaaababb"))
    =
    (("abaabbbbbbbabbabaabbabaaabaabbaabbaaabbabbbaabaabaaba") ++ (C) ++ ("abaabbbbaaabbbaabbbbabaaababb"));
constraint
  (("aaaabaaaabbaabaaaabaaabbaabbbaabab") ++ (C) ++ ("bbbabaababaabaabbbbaaaabaaabbbabbabaabaabaabbbab"))
    =
    (("aaaabaaaabbaabaaaabaaabbaabbbaabab") ++ (C) ++ ("bbbabaababaabaabbbbaaaabaaabbbabbabaabaabaabbbab"));
constraint
  (("baaaaabbbabbbbaabbbbbbaabaaabaabbbbbbaabb") ++ (C) ++ ("babbbbaaaaabbbabaabaaabaaaababbaaababbaaa"))
    =
    (("baaaaabbbabbbbaabbbbbbaabaaabaabbbbbbaabbbbaaaaabababbabbbbaaaaabbbabaabaaabaaaababbaaababbaaa") ++ (""));
constraint
  (("aaabbabbaaaabbabbbbbbbaaa") ++ (C) ++ ("bbaaaaaababbbaabbabbbaaaabaaaabbbbbbaaabaabbaaaaaaabbabba"))
    =
    (("aaabbabbaaaabbabbbbbbbaaa") ++ (C) ++ ("bbaaaaaababbbaabbabbbaaaabaaaabbbbbbaaabaabbaaaaaaabbabba"));
constraint
  (("aababbaaaaaababbbbabbaabaaabbbababbaababbabababbbaababbbaabbbabaaaba") ++ (C) ++ ("baaaaabbabaaab"))
    =
    (("aababbaaaaaababbbbabbaabaaabbbababbaababbabababbbaababbbaabbbabaaababbaaaaabababbaaaaabbabaaab") ++ (""));
constraint
  (("aaaabbaabbaabaaaaaaabababbbb") ++ (C) ++ ("abbbaaababababaaaabbababbabbbbaaabaabbbaababaaabbbbaaa"))
    =
    (("aaaabbaabbaabaaaaaaabababbbbbbaaaaababababbbaaababababaaaabbababbabbbbaaabaabbbaababaaabbbbaaa") ++ (""));
constraint
  (("ababba") ++ (C) ++ ("baaabbbbabbbabaabbaaabaabbabbbbbbabbaababbaaababaaabbbaabbaaababbbbbaabaaaba"))
    =
    (("ababbabbaaaaabababbaaabbbbabbbabaabbaaabaabbabbbbbbabbaababbaaababaaabbbaabbaaababbbbbaabaaaba") ++ (""));
constraint
  (("ababbabbaaabbbbabaabaaaaaaaaababbabaaabbaaabbabbabaaa") ++ (C) ++ ("aaaaaabaaabbaaabbbbabaaabbaab"))
    =
    (("ababbabbaaabbbbabaabaaaaaaaaababbabaaabbaaabbabbabaaabbaaaaabababaaaaaabaaabbaaabbbbabaaabbaab") ++ (""));
constraint
  (("abbbbbbbaabaabbbbbabababbbaabbbbbbbabaaaaaabbaababbaabaaabbbabbbbabbbabaabbbaaaaabababababbbab") ++ (""))
    =
    (("abbbbbbbaabaabbbbbabababbbaabbbbbbbabaaaaaabbaababbaabaaabbbabbbbabbbabaab") ++ (C) ++ ("ababbbab"));
constraint
  (("aaabbbbaaaabaaaabbaaaabbaaaaabaaaabaaaa") ++ (C) ++ ("bbbabaabbbbbbbababababbbbbbbabaabbabbaabbbb"))
    =
    (("aaabbbbaaaabaaaabbaaaabbaaaaabaaaabaaaabbaaaaabababbbbabaabbbbbbbababababbbbbbbabaabbabbaabbbb") ++ (""));
constraint
  (("bbababbbaaaaabbbaaabaabbababbaabaabbbabbbbbaa") ++ (C) ++ ("ababaaabbbbaaabbaababbabaabaabbabbbaa"))
    =
    (("bbababbbaaaaabbbaaabaabbababbaabaabbbabbbbbaa") ++ (C) ++ ("ababaaabbbbaaabbaababbabaabaabbabbbaa"));
constraint
  (("abbbabbaaaaabababababbbabbbaababaabbbaaaaaabbabbabaaaabbbbaabbbaaaaaabaabbaaabaabaaabbaaababbb") ++ (""))
    =
    (("abbba") ++ (C) ++ ("ababbbabbbaababaabbbaaaaaabbabbabaaaabbbbaabbbaaaaaabaabbaaabaabaaabbaaababbb"));
constraint
  (("baaabbbaaaabbba") ++ (C) ++ ("bbbbaabaabaaaababbbabbbaaabababbbbaabbaabaabbabbbbababaaaababbaaaab"))
    =
    (("baaabbbaaaabbbabbaaaaabababbbbbaabaabaaaababbbabbbaaabababbbbaabbaabaabbabbbbababaaaababbaaaab") ++ (""));
constraint
  (("bbaabbbbabbaabbbbbbaabbaabaaaaaabbbabaabababbbbaabbaabbaabaabbaabbbbbbabbaaabbaba") ++ (C) ++ ("a"))
    =
    (("bbaabbbbabbaabbbbbbaabbaabaaaaaabbbabaabababbbbaabbaabbaabaabbaabbbbbbabbaaabbaba") ++ (C) ++ ("a"));
constraint
  (("baabababaabbbabbbbaaaabbbbbbbabababaaaaaabababbaaaabaaaabbbababaabaaababbaaaaabababaabababaaba") ++ (""))
    =
    (("baabababaabbbabbbbaaaabbbbbbbabababaaaaaabababbaaaabaaaabbbababaabaaaba") ++ (C) ++ ("aabababaaba"));
constraint
  (("abababaababbaaababaabbbabaab") ++ (D) ++ ("abbabaaabaababaababbbbbaaabaaaabb"))
    =
    (("abababaababbaaababaabbbabaabbaaa") ++ (C) ++ ("aabaababaababbbbbaaabaaaabb"));
constraint
  (("abaaabbbbaabaaabbaabbababbaaabbaabbaabbbbabaaabaaabbbbabaaabaababaaaababaabababab") ++ (B) ++ ("aaa"))
    =
    (("abaaabbbbaabaaabbaabbababbaaabbaabbaabbbbabaaabaaabbbbabaaabaababaaaababaababababbabaabbaaaaaa") ++ (""));
constraint
  (("abbbabababbaabaabaabababaaaababbbbabbaabbbaaaababaaaaababaababababbbbbbaaabaabb") ++ (B) ++ ("baaab"))
    =
    (("abbbabababbaabaabaabababaaaababbbbabbaabbbaaaababaaaaababaababababbbbbbaaabaabb") ++ (B) ++ ("baaab"));
constraint
  (("abaaabbbaaaaaaabaabbaaaab") ++ (B) ++ ("aaaabbbabbbabaabbbabbabbaaabbaaabaaabbbaabbbbbbabbbaabaaaaa"))
    =
    (("abaaabbbaaaaaaabaabbaaaabbabaabbaaaaaaabbbabbbabaabbbabbabbaaabbaaabaaabbbaabbbbbbabbbaabaaaaa") ++ (""));
constraint
  (("aabbbaaabaabbbaabbbaaaababa") ++ (B) ++ ("bbabbababbbbaaabbbbbbaabbaabbaaaaabbabaaaaabbaabbbaaababb"))
    =
    (("aabbbaaabaabbbaabbbaaaababa") ++ (B) ++ ("bbabbababbbbaaabbbbbbaabbaabbaaaaabbabaaaaabbaabbbaaababb"));
constraint
  (("abaaababbaabbaaabbabbaabaaabbbbbbababbbababbaaabbabaaaababbabaabbabbabaabbaaabbbabbbbbbbababba") ++ (""))
    =
    (("abaaababbaabbaaabbabbaabaaabbbbbbababbbababbaaabbabaaaababbabaabbab") ++ (B) ++ ("bbbabbbbbbbababba"));
constraint
  (("bbbab") ++ (B) ++ ("ababbbabbabaabbbaababbaababbbbbbababaaabbabaabaaabaabbbaaabbbbbbbbbbaabaabbabba"))
    =
    (("bbbab") ++ (B) ++ ("ababbbabbabaabbbaababbaababbbbbbababaaabbabaabaaabaabbbaaabbbbbbbbbbaabaabbabba"));
constraint
  (("aababbbaaaaabbbabbbaabbabbbababbbabaaaabaabababaaaabbbbbabaabbaaaaaabaaaabbabaabbabaabaaabbaab") ++ (""))
    =
    (("aababbbaaaaabbbabbbaabbabbbababbbabaaaabaabababaaaabbbb") ++ (B) ++ ("aaabaaaabbabaabbabaabaaabbaab"));
constraint
  (("bbbaababbbbbabaabba") ++ (B) ++ ("aabaababaaabaababbabaaabababbbbbababbaaaabaababbaaabaabbbbabaaabb"))
    =
    (("bbbaababbbbbabaabba") ++ (B) ++ ("aabaababaaabaababbabaaabababbbbbababbaaaabaababbaaabaabbbbabaaabb"));
constraint
  (("baabbbabbbabaabbaaabbbaabbbbababbbbabbaabbbbbababbbab") ++ (B) ++ ("babaaabaabbaaabbbbababbaabaabba"))
    =
    (("baabbbabbbabaabbaaabbbaabbbbababbbbabbaabbbbbababbbab") ++ (B) ++ ("babaaabaabbaaabbbbababbaabaabba"));
constraint
  (("bbbaaaaabbbabbbbbabbbabbaaaabaabbababaaabaabbabbaaabbbaabaab") ++ (B) ++ ("abaaabbaabbbbabbbaabbaba"))
    =
    (("bbbaaaaabbbabbbbbabbbabbaaaabaabbababaaabaabbabbaaabbbaabaabbabaabbaaaabaaabbaabbbbabbbaabbaba") ++ (""));
constraint
  (("babbaabbbbabaababbbbbabaabaabbbbbababbababbbbaaababbabbbbbbabbaaaababaabbabaabbaaaaaaaaabaaaaa") ++ (""))
    =
    (("babbaabbbbabaababbbbbabaabaabbbbbababbababbbbaaababbabbbbbbabbaaaababaab") ++ (B) ++ ("aaaaaabaaaaa"));
constraint
  (("aaabaabbbbbbababaabaaaaababaaabbaabaaabaabbaabbaabaaabbabaabbabbbbbbbbababaabbaaaabbbabbbaaabb") ++ (""))
    =
    (("aaabaabbbbbbababaabaaaaababaaabbaabaaabaabbaabbaabaaabbabaabbabbbbbbbba") ++ (B) ++ ("abbbabbbaaabb"));
constraint
  (("aabbabbb") ++ (B) ++ ("baaaabbbaaabaababbaaaabaaaaaababaabbabbbbbababbbbaaabaaaaaaaabaaabbaabbaaabb"))
    =
    (("aabbabbbbabaabbaaabaaaabbbaaabaababbaaaabaaaaaababaabbabbbbbababbbbaaabaaaaaaaabaaabbaabbaaabb") ++ (""));
constraint
  (("baaabbbbbaaaabbaabbbbbbabaabbaaaabbababbbabaaaabaabbaababbbbbabbabbabababaaabbaaaabaaaaaaaabba") ++ (""))
    =
    (("baaabbbbbaaaabbaabbbbb") ++ (B) ++ ("abbababbbabaaaabaabbaababbbbbabbabbabababaaabbaaaabaaaaaaaabba"));
constraint
  (("baababbabaabaabaababbabaaabaaaabbbbbabbbbabbaabababbbaaabaaabb") ++ (B) ++ ("babbbbababaaababbaaaaa"))
    =
    (("baababbabaabaabaababbabaaabaaaabbbbbabbbbabbaabababbbaaabaaabbbabaabbaaababbbbababaaababbaaaaa") ++ (""));
constraint
  (("aaababaababbbbbbabaaabbbbbaabbababbbaababaabbaabaaabaabaaababaabbaaaaabbabbabbbaaaaabababbbbbb") ++ (""))
    =
    (("aaababaababbbbbbabaaabbbbbaabbababbbaababaabbaabaaabaabaaa") ++ (B) ++ ("aabbabbabbbaaaaabababbbbbb"));
constraint
  (("aaabbbabababaabbababbbbabbbaaaaabbbbaabbabaaa") ++ (B) ++ ("bbaabaaaaabbaabbbbabbabbaabaaaaabbaabaa"))
    =
    (("aaabbbabababaabbababbbbabbbaaaaabbbbaabbabaaababaabbaaabbaabaaaaabbaabbbbabbabbaabaaaaabbaabaa") ++ (""));
constraint
  (("aaaabaaaabbabbbbbaabbbbaaabbbbbbabbbbaaabaabbbababaabbaaaaabbbbbbaaaaabaababbabaabaabbabaabbab") ++ (""))
    =
    (("aaaabaaaabbabbbbbaabbbbaaabbbbbbabbbbaaabaabbba") ++ (B) ++ ("aabbbbbbaaaaabaababbabaabaabbabaabbab"));
constraint
  (("bbbaabaaabbaaabaabaabbbaaabbbbbbbbaabbbbbaababbbaabababaaaaaabbabbbbaaaabba") ++ (B) ++ ("bbbbaabba"))
    =
    (("bbbaabaaabbaaabaabaabbbaaabbbbbbbbaabbbbbaababbbaabababaaaaaabbabbbbaaaabbababaabbaaabbbbaabba") ++ (""));
constraint
  (("babaaaaaaaabbbabababbbaaaaaaaababbabaaaababbbbbbaaabbabaabbaaaaabbabbbabbbbbabbabbbaabbabbbabb") ++ (""))
    =
    (("babaaaaaaaabbbabababbbaaaaaaaababbabaaaababbbbbbaaab") ++ (B) ++ ("aabbabbbabbbbbabbabbbaabbabbbabb"));
constraint
  (("aaaaabaaaababbbbaabbaaabbaaaabaaaaabababbaaaaabaa") ++ (B) ++ ("abbbaabaaaabaaaabaaaaabbababbabaaba"))
    =
    (("aaaaabaaaababbbbaabbaaabbaaaabaaaaabababbaaaaabaa") ++ (B) ++ ("abbbaabaaaabaaaabaaaaabbababbabaaba"));
constraint
  (("ababbbabbbaabbbbababbbababbaabbbbabaaabaabaaaaaaaa") ++ (B) ++ ("aaabaababaaababbbbaaabbbbabaabaaba"))
    =
    (("ababbbabbbaabbbbababbbababbaabbbbabaaabaabaaaaaaaa") ++ (B) ++ ("aaabaababaaababbbbaaabbbbabaabaaba"));
constraint
  (("babaabaabbababbaaaabbbaaaaababaababbbbbabbabbbaabbabbaaaaaaabaa") ++ (B) ++ ("aaabaababababbaaababb"))
    =
    (("babaabaabbababbaaaabbbaaaaababaababbbbbabbabbbaabbabbaaaaaaabaababaabbaaaaaabaababababbaaababb") ++ (""));
constraint
  (("abababaaabbabaababbbbbbabbaabbaabbaabbbaaabaaaababaaababbabaabb") ++ (B) ++ ("aaababbaaaabbbaabbaba"))
    =
    (("abababaaabbabaababbbbbbabbaabbaabbaabbbaaabaaaababaaababbabaabb") ++ (B) ++ ("aaababbaaaabbbaabbaba"));
constraint
  (("baababaabaabbbaaabbabababbabaabbaaabaababbabbaabbbbbbbbaababbabbbbbabbabbbbababbabbabaabaaaabb") ++ (""))
    =
    (("baababaabaabbbaaabbababab") ++ (B) ++ ("baababbabbaabbbbbbbbaababbabbbbbabbabbbbababbabbabaabaaaabb"));
constraint
  (("baabbabbbbbabaabbaaababbabaaaaababaabbbababbbabbbbbaabbbaaabbaababbbaababaaababaabbabbbbbaabba") ++ (""))
    =
    (("baabbabbbb") ++ (B) ++ ("babbabaaaaababaabbbababbbabbbbbaabbbaaabbaababbbaababaaababaabbabbbbbaabba"));
constraint
  (("bbaabaabaabbabb") ++ (B) ++ ("bbabaabbababbaaaaabaaaaabbbabbabbbbaaaabbababbaaabbbaabbbbbbbaaaababa"))
    =
    (("bbaabaabaabbabbbabaabbaaabbabaabbababbaaaaabaaaaabbbabbabbbbaaaabbababbaaabbbaabbbbbbbaaaababa") ++ (""));
constraint
  (("babbbaabbababbbababbbbbabb") ++ (B) ++ ("bbabaabbabbbbabbababbaabbabbabbbabbaaaabaaabbaababbbaaaabb"))
    =
    (("babbbaabbababbbababbbbbabbbabaabbaaabbabaabbabbbbabbababbaabbabbabbbabbaaaabaaabbaababbbaaaabb") ++ (""));
constraint
  (("baaaabbabaabaabbbbaabbbbbbaabbbaaabaabaabbbaaaaabbbaaabaaaaaababbbbbbbaabaababbbaababaabbaaaaa") ++ (""))
    =
    (("baaaabbabaabaabbbbaabbbbbbaabbbaaabaabaabbbaaaaabbbaaabaaaaaababbbbbbbaabaababbbaa") ++ (B) ++ ("aa"));
constraint
  (("bbbaaabaaaabababbaaaabbaaaa") ++ (B) ++ ("bbbababbaabbaabaaaabbbbbabaabbbbbbabaaaabababbbaabbaabbaa"))
    =
    (("bbbaaabaaaabababbaaaabbaaaa") ++ (B) ++ ("bbbababbaabbaabaaaabbbbbabaabbbbbbabaaaabababbbaabbaabbaa"));
constraint
  (("bbbbabbbabbababababaababaa") ++ (B) ++ ("bbbbaababbaaabbbbbbaaaabbbbababaaaaababaabaaabaabbaabbabba"))
    =
    (("bbbbabbbabbababababaababaa") ++ (B) ++ ("bbbbaababbaaabbbbbbaaaabbbbababaaaaababaabaaabaabbaabbabba"));
constraint
  (("aaabaabaababaabbaabaabbabaabbaaabbaababbbbbbaaabbbbbaaaabbbbbbbbaabbbaabbaaaaaaabbbaaaaaababba") ++ (""))
    =
    (("aaabaabaababaabbaabaab") ++ (B) ++ ("bbaababbbbbbaaabbbbbaaaabbbbbbbbaabbbaabbaaaaaaabbbaaaaaababba"));
constraint
  (("abbbaaababaababbabaabbbbbaaaababaabbaabbbabaaaaaabbabbbbababbababbbbbaabababbba") ++ (B) ++ ("aabaa"))
    =
    (("abbbaaababaababbabaabbbbbaaaababaabbaabbbabaaaaaabbabbbbababbababbbbbaabababbbababaabbaaaaabaa") ++ (""));
constraint
  (("bbbaaaabbaaababaabbaabaababbabaaabaabaaabbbaaabaabaaaaaaaaaabbbababaabbaaaabbabbabbabbbbaaabaa") ++ (""))
    =
    (("bbbaaaabbaaababaabbaabaababbabaaabaabaaabbbaaabaabaaaaaaaaaabbba") ++ (B) ++ ("abbabbabbabbbbaaabaa"));
constraint
  (("aabaaabbbbbabbbbaaabaabbabbababaaaababbaabaaaaabababbbbaabbaaaabaababbbbbb") ++ (B) ++ ("babaaabaab"))
    =
    (("aabaaabbbbbabbbbaaabaabbabbababaaaababbaabaaaaabababbbbaabbaaaabaababbbbbbbabaabbaaababaaabaab") ++ (""));
constraint
  (("aaabbbaababaabbbaabbbaabaaabbbbabbbbabbbabaabbaaabaabbbbbbabaabbbaaaabbbababbbababaabaaaababaa") ++ (""))
    =
    (("aaabbbaababaabbbaabbbaabaaabbbbabbbbabb") ++ (B) ++ ("baabbbbbbabaabbbaaaabbbababbbababaabaaaababaa"));
constraint
  (("baabbbaaaaabbbbaabbaabbbaabaaababbaaabaaabababbbaa") ++ (B) ++ ("aaaabbbbbaaabaababbaaababaaaaaaaab"))
    =
    (("baabbbaaaaabbbbaabbaabbbaabaaababbaaabaaabababbbaababaabbaaaaaaabbbbbaaabaababbaaababaaaaaaaab") ++ (""));
constraint
  (("aaabababbbbaaaabbbbabaaabaabbabaaaabbaabaabaabbaababaabbbabababaaaaaba") ++ (B) ++ ("baababbabababa"))
    =
    (("aaabababbbbaaaabbbbabaaabaabbabaaaabbaabaabaabbaababaabbbabababaaaaabababaabbaaabaababbabababa") ++ (""));
constraint
  (("aababaaabaaabababaabababb") ++ (B) ++ ("bbbaabbbaaaaabbbbabbaabbabbaabaabbbbbbabaaaabbaaababbbaaabb"))
    =
    (("aababaaabaaabababaabababb") ++ (B) ++ ("bbbaabbbaaaaabbbbabbaabbabbaabaabbbbbbabaaaabbaaababbbaaabb"));
constraint
  (("aabaaaabbabaaaaaaaabbbaaabaabbbababbbabbaabbab") ++ (B) ++ ("aabaabababaabaaababbbbabbbabbbbaaabbaa"))
    =
    (("aabaaaabbabaaaaaaaabbbaaabaabbbababbbabbaabbabbabaabbaaaaabaabababaabaaababbbbabbbabbbbaaabbaa") ++ (""));
constraint
  (("aaaabbbbbaabbbbabbaabbabaababababbaaaaaaaabbbabbbbaabbaaabababaaaabbbababbababaabbbabaabbaaabb") ++ (""))
    =
    (("aaaabbbbbaabbbbabbaabbabaababababbaaaaaaaabbbabbbbaabbaaabababaaaabbbababbababaabb") ++ (B) ++ ("bb"));
constraint
  (("abaabbbbbbbbbaabababbbbbaaaaaabbbaaabbba") ++ (B) ++ ("bbbaabbababaaaaabaaababaaabaaabbbbbaaabbbaaa"))
    =
    (("abaabbbbbbbbbaabababbbbbaaaaaabbbaaabbba") ++ (B) ++ ("bbbaabbababaaaaabaaababaaabaaabbbbbaaabbbaaa"));
constraint
  (("ababbbbbbbabababaaabababaabbaaabababaabbbaabaaabbabbbabbabababaaabbababbbbbabaabbaaabbabaababb") ++ (""))
    =
    (("ababbbbbbbabababaaabababaabbaaabababaabbbaabaaabbabbbabbabababaaabbababbbb") ++ (B) ++ ("bbabaababb"));
constraint
  (("baaaabbbaabbaaabababbabaaabaaaabbbaaabbbbbbabbaaaabbbababbbbbbbbbabaabbaaabbbaabbaabaaabaababb") ++ (""))
    =
    (("baaaabbbaabbaaabababbabaaabaaaabbbaaabbbbbbabbaaaabbbababbbbbbbb") ++ (B) ++ ("bbbaabbaabaaabaababb"));
constraint
  (("bbabbaaabaabbbbababababbabbababaababbbabbaaaaabaaabbbbbabbaaaabbabbaabbaabbba") ++ (B) ++ ("babaabb"))
    =
    (("bbabbaaabaabbbbababababbabbababaababbbabbaaaaabaaabbbbbabbaaaabbabbaabbaabbbababaabbaaababaabb") ++ (""));
constraint
  (("babaabbaabbbbabbbaaabaabbbbbbabbbaaaababababaabbbbbbbbbababababaaaaabbabababaabbaaaaaabbaabaab") ++ (""))
    =
    (("babaabbaabbbbabbbaaabaabbbbbbabbbaaaababababaabbbbbbbbbababababaaaaabbaba") ++ (B) ++ ("aaabbaabaab"));
constraint
  (("bbaabaaabaaaabbbababbbbaababbabaabbbbaaaaab") ++ (B) ++ ("aabbbababbaabaabbbaabbaaaabababaaaabaaabb"))
    =
    (("bbaabaaabaaaabbbababbbbaababbabaabbbbaaaaabbabaabbaaaaabbbababbaabaabbbaabbaaaabababaaaabaaabb") ++ (""));
constraint
  (("abbbbbabbbaaab") ++ (B) ++ ("babbabaaaaababababbbabbbabbbaaaaaaaababbbbababaaaaabbabbaaaabaababaaba"))
    =
    (("abbbbbabbbaaab") ++ (B) ++ ("babbabaaaaababababbbabbbabbbaaaaaaaababbbbababaaaaabbabbaaaabaababaaba"));
constraint
  (("aaabbbaaabbbababbbabbabaaaaaaaaabbbbaaabbaaabababaababbbababaaaababbabaa") ++ (B) ++ ("aabaaabababa"))
    =
    (("aaabbbaaabbbababbbabbabaaaaaaaaabbbbaaabbaaabababaababbbababaaaababbabaa") ++ (B) ++ ("aabaaabababa"));
constraint
  (("aaaaaaabbbabbaaaabaababaaabbabaabbabba") ++ (B) ++ ("bbbaabbababbababbaaabbaaabaaabbababbbababbbbba"))
    =
    (("aaaaaaabbbabbaaaabaababaaabbabaabbabbababaabbaaabbbaabbababbababbaaabbaaabaaabbababbbababbbbba") ++ (""));
constraint
  (("aabbabaaabbbbabaaababbbbaaabaaab") ++ (B) ++ ("aabbabaabbbaabbbabbbaababaabbbbbababaaababaaabbbbbbb"))
    =
    (("aabbabaaabbbbabaaababbbbaaabaaabbabaabbaaaaabbabaabbbaabbbabbbaababaabbbbbababaaababaaabbbbbbb") ++ (""));
constraint
  (("babbbbaaabbbabababbbbbaaaabbaaaaabbabaaaababaabbbabaaabbaababaaaaaabbbaababbabba") ++ (B) ++ ("abab"))
    =
    (("babbbbaaabbbabababbbbbaaaabbaaaaabbabaaaababaabbbabaaabbaababaaaaaabbbaababbabba") ++ (B) ++ ("abab"));
constraint
  (("abaaaabbababbbabbabbabababbbbbbaaaaaabbaabbbbbaabaaaaaa") ++ (B) ++ ("babaabbababbaaabaaaaabaaababb"))
    =
    (("abaaaabbababbbabbabbabababbbbbbaaaaaabbaabbbbbaabaaaaaa") ++ (B) ++ ("babaabbababbaaabaaaaabaaababb"));
constraint
  (("bbaaabaaab") ++ (B) ++ ("bbbbbbbaaaabababbbabaabbbbbababbabbbaababaaaaabbaabbbbbabababbbbbbaababbba"))
    =
    (("bbaaabaaabbabaabbaaabbbbbbbaaaabababbbabaabbbbbababbabbbaababaaaaabbaabbbbbabababbbbbbaababbba") ++ (""));
constraint
  (("baaaabbbbaabbabaabbabbaabaabaaabbbbbaaabbaababaaaabaaaabbaabbabaabbaaaaababbaaaababbaaaababbab") ++ (""))
    =
    (("baaaabbbbaabbabaabbabbaabaabaaabbbbbaaabbaababaaaabaaaabbaab") ++ (B) ++ ("aababbaaaababbaaaababbab"));
constraint
  (("aaaababbbaabbbaaaabaaababbaababaaabbaaabaabbabaabbaaabaabaaabbabaababbbbaabbbabababababaabaaaa") ++ (""))
    =
    (("aaaababbbaabbbaaaabaaababbaababaaabbaaabaab") ++ (B) ++ ("baabaaabbabaababbbbaabbbabababababaabaaaa"));
constraint
  (("aaabaabababbaababbbbbbabbbbabaabbaaaabaabbbabaaaaababbbbabaabaabbbbaabbbabaaaabaabbabaabbbabaa") ++ (""))
    =
    (("aaabaabababbaababbbbbbabbb") ++ (B) ++ ("abaabbbabaaaaababbbbabaabaabbbbaabbbabaaaabaabbabaabbbabaa"));
constraint
  (("baababbaababbbbba") ++ (B) ++ ("babbbaaabbbabaaaabbbaabababbbabbaaaaaaaaabbababbbbabbbbbbababbaaaaa"))
    =
    (("baababbaababbbbbababaabbaaababbbaaabbbabaaaabbbaabababbbabbaaaaaaaaabbababbbbabbbbbbababbaaaaa") ++ (""));
constraint
  (("b") ++ (A) ++ ("babbaaa") ++ (D) ++ ("bbaaabbba") ++ (A) ++ ("ba") ++ (A) ++ ("bbbbababaa") ++ (A) ++ ("aaaa"))
    =
    (("bababababbaaaaababbbbabbaaabbaaababaaaaa") ++ (C) ++ ("aabbbaabababa") ++ (A) ++ ("bbbbababaaababaaaaa"));
constraint
  (("bbbbabaabbbaaabaabbbbaa") ++ (A) ++ ("babbabaabbbbbaabbbbabbabaabaabbbaaaaabaabbaababbaaaabbbbbababbabaa"))
    =
    (("bbbbabaabbbaaabaabbbbaa") ++ (A) ++ ("babbabaabbbbbaabbbbabbabaabaabbbaaaaabaabbaababbaaaabbbbbababbabaa"));
constraint
  (("bbbbabaaaaabaaaa") ++ (A) ++ ("bbbbbababaabbbbaababaabaaaababbbbbabbbababababbbbbbaaabbabbbaababbaababaa"))
    =
    (("bbbbabaaaaabaaaa") ++ (A) ++ ("bbbbbababaabbbbaababaabaaaababbbbbabbbababababbbbbbaaabbabbbaababba") ++ (A) ++ ("a"));
constraint
  (("bbabaabba") ++ (A) ++ ("aabaaabbaaabbabbbbbaabbaabaabbbaaabbbbbaaaababbaaaaaaabaaaabbbbbaaaabbbababbbaab"))
    =
    (("bbabaabba") ++ (A) ++ ("aabaaabbaaabbabbbbbaabbaabaabbbaaabbbbbaaaababbaaaaaaabaaaabbbbbaaaabbbababbbaab"));
constraint
  (("aababbbabaabaaaaaababbabbbbbbabaaabbbbbbabbaabbaabbbababb") ++ (A) ++ ("abbaaaabbaabbbababbaababbabaaaba"))
    =
    (("aababbbabaabaaaaaababbabbbbbbabaaabbbbbbabbaabbaabbbababb") ++ (A) ++ ("abbaaaabbaabbbababbaababbabaaaba"));
constraint
  (("aabbababbaaaaabbaaabaabbabbbbaababaabbaaabbaaaabbbaa") ++ (A) ++ ("bbabbbbaaaaaaaabaaaabaabbbbbabbbbabbb"))
    =
    (("aabbababbaaaaabbaaabaabbabbbba") ++ (A) ++ ("abbaaabbaaaabbbaa") ++ (A) ++ ("bbabbbbaaaaaaaabaaaabaabbbbbabbbbabbb"));
constraint
  (("aabbaabbaaaaaabbbaaabbababbbbbbbaaababababbbbbbababb") ++ (A) ++ ("baaabbbabbabaabbbbabaabbababbbbbbabbb"))
    =
    (("aabbaabbaaaaaabbbaaabbababbbbbbbaaababababbbbbbababb") ++ (A) ++ ("baaabbbabbabaabbbbabaabbababbbbbbabbb"));
constraint
  (("abbbabbbbbbbbabaabbababbbbabbaababbbaaabbbbabbbb") ++ (A) ++ ("abbabbbbbaaaabaabbaabbabaaabbaabaaaaaaaaa"))
    =
    (("abbbabbbbbbbbabaabbababbbbabbaababbbaaabbbbabbbbababaabbabbbbbaaaabaabbaabbabaaabbaabaaaaaaaaa") ++ (""));
constraint
  (("bbbabaabbbaabbabaabbaabbaabbbbaaaaaaabbabbbbabaabaaaababbababaaaababbabaaaabaabaabaabaabaaaaaa") ++ (""))
    =
    (("bbbabaabbbaabbabaabbaabbaabbbbaaaaaaabbabbbbabaabaaaababb") ++ (A) ++ ("aaababbabaaaabaabaabaabaabaaaaaa"));
constraint
  (("bbbababaaaabaabaaaabbabbbabaaaba") ++ (A) ++ ("aaabaaabbbaabbabbaababbbbbbbbaababbbaaaaaaaaaabbaababbabb"))
    =
    (("bbb") ++ (A) ++ ("aaabaabaaaabbabbbabaaabaababaaaabaaabbbaabbabbaababbbbbbbbaababbbaaaaaaaaaabbaababbabb"));
constraint
  (("baaababbbbaaabaaababba") ++ (A) ++ ("babaaabbbbbaaabbbabaaabaabaabaaabbabbbabaaaaabbabaaabbbbaabbaabbbbb"))
    =
    (("baaababbbbaaabaaababba") ++ (A) ++ ("babaaabbbbbaaabbbabaaabaabaabaaabbabbbabaaaaabbabaaabbbbaabbaabbbbb"));
constraint
  (("bbbababbbbbba") ++ (A) ++ ("baabbbbabbbbabbaababaabbabaabbbaababbbaabbbaaaaaaaaababbbaaababbabbabbaababb"))
    =
    (("bbbababbbbbba") ++ (A) ++ ("baabbbbabbbbabbaababaabbabaabbbaababbbaabbbaaaaaaaaababbbaaababbabbabbaababb"));
constraint
  (("ababaaaabaababababaabaaaaba") ++ (A) ++ ("bbbabaaaa") ++ (A) ++ ("aababbbaabaaabbbaaaaabababbbabaaaabbaabbbbbbbbaa"))
    =
    ((A) ++ ("aaaba") ++ (A) ++ ("babaabaaaabaabababbbabaaaaababaaababbbaabaaabbbaaaa") ++ (A) ++ ("bbbabaaaabbaabbbbbbbbaa"));
constraint
  (("bbbaabbaaabbbbaba") ++ (A) ++ ("baaaaaabaabbabbbbabbabaabbbabbbbbbbbbbbababaabbaaabbbaabbaabababbbbaabaa"))
    =
    (("bbbaabbaaabbbbabaabababaaaaaabaabbabbbbabbabaabbbabbbbbbbbbbbababaabbaaabbbaabbaabababbbbaabaa") ++ (""));
constraint
  (("aaaaaabaaaaaabaaaabaaaabbaabaabababbabaabbbaaaabbbbaabbbbbaabaaaabaabbbbbababaaaabbabbaababaab") ++ (""))
    =
    (("aaaaaabaaaaaabaaaabaaaabbaaba") ++ (A) ++ ("bbabaabbbaaaabbbbaabbbbbaabaaaabaabbbbbababaaaabbabba") ++ (A) ++ ("ab"));
constraint
  (("baabbabbbabbbaaaaaabaaabbbabbbbbbaa") ++ (A) ++ ("abbbaabbbaaaabbbbbabbaaaaababbaaabaabbaaabbaabbbbbaaab"))
    =
    (("baabbabbbabbbaaaaaabaaabbbabbbbbbaa") ++ (A) ++ ("abbbaabbbaaaabbbbbabbaaaaababbaaabaabbaaabbaabbbbbaaab"));
constraint
  (("bbbaaaabbbbabbabaabbbabbabbbabbaaaababaaaaabbaabaabbabbbabbaaaaaaabbabaaabbaaaaabbbaabbaaaabab") ++ (""))
    =
    (("bbbaaaabbbbabbabaabbbabbabbbabbaaa") ++ (A) ++ ("aaaabbaabaabbabbbabbaaaaaaabbabaaabbaaaaabbbaabbaaaabab"));
constraint
  (("aaaabaaabaabbabbaaabbaaabbababbaaaababaaabbaabbbbabbbbaababbbaaaabaaaaababbabaabbbaaaabaababba") ++ (""))
    =
    (("aaaabaaabaabbabbaaabbaaabbababbaaa") ++ (A) ++ ("aabbaabbbbabbbbaababbbaaaabaaaaababbabaabbbaaaabaababba"));
constraint
  (("baabaabaabbaaabaabbabbbbabbabaabbbaaaabbababaababbabbaaaaaabaabbaaababaaabaabbbaaabbbbaababbba") ++ (""))
    =
    (("baabaabaabbaaabaabbabbbbabbabaabbbaaaabb") ++ (A) ++ ("ababbabbaaaaaabaabbaa") ++ (A) ++ ("aabaabbbaaabbbbaababbba"));
constraint
  (("bbbabababaabbbabaabaababbaaaaabaaaabbbbbbaabbaaabbbbbaaaa") ++ (A) ++ ("baaaabbabaabbbababbaaaababbaabbb"))
    =
    (("bbb") ++ (A) ++ ("baabbbabaabaababbaaaaabaaaabbbbbbaabbaaabbbbbaaaaabababaaaabbabaabbbababbaaaababbaabbb"));
constraint
  (("bbbabbabbbbbaaabbababbaaaaa") ++ (A) ++ ("bbabaabaaaaababbabaabbbaaababaaaabaaaaaaaabbbabaaaaabaaababbbb"))
    =
    (("bbbabbabbbbbaaabbababbaaaaaabababbabaabaaaaababbabaabbbaa") ++ (A) ++ ("aaabaaaaaaaabbbabaaaaabaaababbbb"));
constraint
  (("aaabaabaababaababaababbaaabaabbbbbabbbbaabbaababbabbaabbaabaaaaaabaabbbbabbbaaabbbbbbbabababbb") ++ (""))
    =
    (("aaabaabaababaababaababbaaabaabbbbbabbbbaabbaababbabbaabbaabaaaaaabaabbbbabbbaaabbbbbbb") ++ (A) ++ ("bbb"));
constraint
  (("baaabbaabbababbabbbababbbababbbbaabbbbaabbbaaaabbaababaaaaaaaba") ++ (A) ++ ("abbaaabbabbaabaaabaabbbaaa"))
    =
    (("baaabbaabbababbabbbababbbababbbbaabbbbaabbbaaaabba") ++ (A) ++ ("aaaaaabaababaabbaaabbabbaabaaabaabbbaaa"));
constraint
  (("bbabaaaaabbbbbaabababbabbaaababbaaabaaaaaaabbabaabaaabaaaaabbbbbaaaaaaabaabbbbabbaaababbbabbba") ++ (""))
    =
    (("bbabaaaaabbbbba") ++ (A) ++ ("bbabbaaababbaaabaaaaaaabbabaabaaabaaaaabbbbbaaaaaaabaabbbbabbaaababbbabbba"));
constraint
  (("bbbaabbbbbaaaaaabbabbbbabbb") ++ (A) ++ ("baabaabbaaaaaabbaaababbabaabbababbabbbbbbaaabababbbaaabbababaa"))
    =
    (("bbbaabbbbbaaaaaabbabbbbabbb") ++ (A) ++ ("baabaabbaaaaaabbaaababbabaabbababbabbbbbbaaabababbbaaabb") ++ (A) ++ ("a"));
constraint
  (("bbaabbabaabbabaaabbbabaabaabbbabaabbabba") ++ (A) ++ ("baabbaabbbbbbabaaaaabbaabaabaabaabbaaaabbabaaaaba"))
    =
    (("bbaabbabaabbabaaabbbabaabaabbbabaabbabba") ++ (A) ++ ("baabbaabbbbbbabaaaaabbaabaabaabaabbaaaabbabaaaaba"));
constraint
  (("aaaabababababbbabbbbbabaabbbabaabbababbaaabbbbbababbbaaaabbbbabbbaaabaaaabb") ++ (A) ++ ("baaababbbabbab"))
    =
    (("aaaabababababbbabbbbbabaabbbabaabbababbaaabbbbbababbbaaaabbbbabbbaaabaaaabbabababaaababbbabbab") ++ (""));
constraint
  (("bbbabbb") ++ (A) ++ ("bbaaaaabbbbaaaabbbbbbbbaaaaaaaabba") ++ (A) ++ ("abbaabbbbbabbbbbbbbbbbaabbaaaabbabbabbaaaab"))
    =
    (("bbbabbbabababbaaaaabbbbaaaabbbbbbbbaaaaaaaabba") ++ (A) ++ ("abbaabbbbbabbbbbbbbbbbaabbaaaabbabbabbaaaab"));
constraint
  (("abaababbaabbaabaaaaaa") ++ (A) ++ ("bbaaaaaababbabaaaaaaaabbbbaaaabbbbbbbaabaabbbbbababbaababbabbaaaaaab"))
    =
    (("abaababbaabbaabaaaaaa") ++ (A) ++ ("bbaaaaaababbabaaaaaaaabbbbaaaabbbbbbbaabaabbbbbababbaababbabbaaaaaab"));
constraint
  (("abaaabaabbbbaabbbaabaaaabbbbbbbaaabaabbbaabbabbabbabbabaaabb") ++ (A) ++ ("babbabbbaabaaababbbbbabbaaaba"))
    =
    (("abaaabaabbbbaabbbaabaaaabbbbbbbaaabaabbbaabbabbabbabbabaaabbababababbabbbaabaaababbbbbabbaaaba") ++ (""));
constraint
  (("baababbababbbabaabaabaabbaaabbbbbaabaabbbaabbabba") ++ (A) ++ ("abaabbaaaaaabbbbaabbbbabbbbbaabbbbbbaaba"))
    =
    (("baababbababbbabaabaabaabbaaabbbbbaabaabbbaabbabbaababaabaabbaaaaaabbbbaabbbbabbbbbaabbbbbbaaba") ++ (""));
constraint
  (("aabbbbbbababbabaaabbaaabaabbbabbbbabbbabbbaaaabaabababbbabaaaababababbaabbabbbbbbbabbaabbbabbb") ++ (""))
    =
    (("aabbbbbbababbabaaabbaaabaabbbabbbbabbbabbbaaaabaabababbbabaaa") ++ (A) ++ ("babbaabbabbbbbbbabbaabbbabbb"));
constraint
  (("aaaaabbabbabbabbbbbaaaaabbbabbbbbabaaaaabaaaaabbba") ++ (A) ++ ("ababbbaaaaabbaababbbaaaaaabbaaaabbbaaab"))
    =
    (("aaaaabbabbabbabbbbbaaaaabbbabbbbbabaaaaabaaaaabbba") ++ (A) ++ ("ababbbaaaaabbaababbbaaaaaabbaaaabbbaaab"));
constraint
  (("bbaaaababababbbabaabbabbaba") ++ (A) ++ ("bbbbabbbbbabbbbababaababbabbbbaabaaabbbbaaaabbbabbaaaabbaabaaa"))
    =
    (("bbaaaababababbbabaabbabbabaabababbbbabbbbbabbbbababaababbabbbbaabaaabbbbaaaabbbabbaaaabbaabaaa") ++ (""));
constraint
  (("baabbbbabbbaaaabbbaaaaaabbaabaaababbabaabbbbaabbabbbbbabaababb") ++ (A) ++ ("baabaabbbbaaaaabbbabaabaaaa"))
    =
    (("baabbbbabbbaaaabbbaaaaaabbaabaaababbabaabbbbaabbabbbbbabaababbabababaabaabbbbaaaaabbbabaabaaaa") ++ (""));
constraint
  (("abbbabbba") ++ (A) ++ ("babbaaabbaaaaaaabbaaaaaaaaabbbabbbbabbabbbababaabbbbabaababaabbbaabaaabbabaaaaaa"))
    =
    (("abbbabbba") ++ (A) ++ ("babbaaabbaaaaaaabbaaaaaaaaabbbabbbbabbabbb") ++ (A) ++ ("abbbbabaababaabbbaabaaabbabaaaaaa"));
constraint
  (("aaaabaaaabaabbbaaabbbbbbbbaaabbaa") ++ (A) ++ ("aabbbbaabbbbbababbbabaaa") ++ (A) ++ ("aabaaaabaabababbabaaababbaa"))
    =
    (("aaaabaaaabaabbbaaabbbbbbbbaaabbaaababaaabbbbaabbbbbababbbabaaa") ++ (A) ++ ("aabaaaaba") ++ (A) ++ ("bbabaaababbaa"));
constraint
  (("abbabbbbabbbbbaabababbbabbabbabbabaaabbbbbbbbaaaaabbaabbbbbabababaaaaabbbbbabbabbbbaaaaaaaaaab") ++ (""))
    =
    (("abbabbbbabbbbbaabababbbabbabbabbabaaabbbbbbbbaaaaabbaabbbbb") ++ (A) ++ ("baaaaabbbbbabbabbbbaaaaaaaaaab"));
constraint
  (("abbabbababbbaabb") ++ (A) ++ ("abbbbbabbaaaabbababbbbbabbbaabbabb") ++ (A) ++ ("baabaaaaaabaaaaaabbbbababbbabbbaab"))
    =
    (("abbabbababbbaabbababaabbbbbabbaaaabbababbbbbabbbaabbabb") ++ (A) ++ ("baabaaaaaabaaaaaabbbbababbbabbbaab"));
constraint
  (("aabaaababaabbbaaabbbaaababbbbaaaaaaaaaababbbbaaaaaababbaba") ++ (A) ++ ("bb") ++ (A) ++ ("baaaaaabaabbbaabbbb") ++ (A))
    =
    (("aabaa") ++ (A) ++ ("abbbaaabbbaaababbbbaaaaaaaaaababbbbaaaaaababbaba") ++ (A) ++ ("bbabababaaaaaabaabbbaabbbb") ++ (A));
constraint
  (("aaababaabbabbaabaaabbaaabbbabaabaaba") ++ (A) ++ ("bbaaaaaabbbaabababbbabaaabaaabbaabbbabbbabbababbabbaa"))
    =
    (("aaababaabbabbaabaaabbaaabbbabaabaabaabababbaaaaaabbbaabababbbabaaabaaabbaabbbabbbabbababbabbaa") ++ (""));
constraint
  (("aabbbaabbbba") ++ (A) ++ ("bbbabbabbaabbbbbbabbabaabbbaaababbabbbbaaaaabbabaabbbbbabbaabaaabbbbbbbbabaab"))
    =
    (("aabbbaabbbbaabababbbabbabbaabbbbbbabbabaabbbaaababbabbbbaaaaabbabaabbbbbabbaabaaabbbbbbbbabaab") ++ (""));
constraint
  (("ababbabbabaabbabbbabbababbaaabbbbabbabbbbbbbaaabbaaaababbababaababbbabaaabaabbbbaabaabaababbaa") ++ (""))
    =
    (("ababbabbabaabbabbbabbababbaaabbbbabbabbbbbbbaaabbaaaababb") ++ (A) ++ ("ababbbabaaabaabbbbaabaabaababbaa"));
constraint
  (("bbbbaaabaaabaaaabaaaba") ++ (A) ++ ("abbbabaaaaaaaabbabaaaaabaabbbbbabaabbabaabbbbbaaabaaaaabbbababbbbba"))
    =
    (("bbbbaaabaaabaaaabaaabaababaabbbabaaaaaaaabbabaaaaabaabbbbbabaabbabaabbbbbaaabaaaaabbbababbbbba") ++ (""));
constraint
  (("aaabbaaabbaaababbabaaba") ++ (A) ++ ("babbaaaaabaaabaaabbaaaabbababbbabbabaaabbabaaaaaaababbbababbabaaab"))
    =
    (("aaabbaaabbaaababbabaabaababababbaaaaabaaabaaabbaaaabbababbbabbabaaabbabaaaaaaababbbababbabaaab") ++ (""));
constraint
  (("ababbaabbabbaabbabaababaabaabbababbbbbbbbbababbbbbababbbaaababb") ++ (A) ++ ("aabbabbabaaaabaaabbabbabab"))
    =
    (("ababbaabbabbaabbabaababaabaabbababbbbbbbbbababbbbbababbbaaababbababaaabbabbabaaaabaaabbabbabab") ++ (""));
constraint
  (("aabaabba") ++ (A) ++ ("abbbbabaaaaababbabbabbaabaaabbaaaaaabbbbabbabbbbba") ++ (A) ++ ("baaabbaababbaabaaabbbbaabb"))
    =
    (("aabaabbaababaabbbbabaaaaababbabbabbaabaaabbaaaaaabbbbabbabbbbba") ++ (A) ++ ("baaabbaababbaabaaabbbbaabb"));
constraint
  (("aabaabbbbbbaabbbaabbbaabbbabababbbabababbabaaaaaababaaaaaabaababbabaaabbabaababaabbbbbbbaaabbb") ++ (""))
    =
    (("aabaabbbbbbaabbbaabbbaabbb") ++ (A) ++ ("bbbabababbabaaaaa") ++ (A) ++ ("aaaaabaababbabaaabbabaababaabbbbbbbaaabbb"));
constraint
  (("babbbbaabbabbabaaaaababbabaabbaabaaababbabbababaaabbaaabababaababbabbbbbbbababaabaabbbabbbaaab") ++ (""))
    =
    (("babbbbaabbabbabaaaaababbabaabbaabaaababbabbababaaabbaaabababaababbabbbbbbb") ++ (A) ++ ("abaabbbabbbaaab"));
constraint
  (("aabaaaaabaaabbaababbb") ++ (A) ++ ("abbbbaaaababbaaaaabaaabbababbaababbaaababbbaaabaababbbbbbbabbbbbbbbb"))
    =
    (("aabaaaaabaaabbaababbbababaabbbbaaaababbaaaaabaaabbababbaababbaaababbbaaabaababbbbbbbabbbbbbbbb") ++ (""));
constraint
  (("ababbababbbbabaabbaaabaabababaaabababbb") ++ (A) ++ ("abbabaabaabbaaababbaabbaba") ++ (A) ++ ("baabbbbbaaaabbabbab"))
    =
    (("ababbababbbbabaabbaaabaabababaa") ++ (A) ++ ("bbb") ++ (A) ++ ("abbabaabaabbaaababbaabbabaabababaabbbbbaaaabbabbab"));
constraint
  (("babbbbaaabbababaabbabbaaaaabbabaabbaaaabaaa") ++ (A) ++ ("ababbabbbaaabbabaabaaaabaaabaabbbbabbbaabbabba"))
    =
    (("babbbbaaabbababaabbabbaaaaabbabaabbaaaabaaa") ++ (A) ++ ("ababbabbbaaabbabaabaaaabaaabaabbbbabbbaabbabba"));
constraint
  (("bbbbbbaa") ++ (A) ++ ("b") ++ (A) ++ ("babaaaaaaabbbbaaaaaaaaaabbbbabaabaaabbbaabbbabbaaaabaabaaaabaabbaba") ++ (A) ++ ("aba"))
    =
    (("bbbbbbaa") ++ (A) ++ ("bababababaaaaaaabbbbaaaaaaaaaabbbbabaabaaabbbaabbbabbaaaabaabaaaabaabbabaababaaba"));
constraint
  ((A) ++ ("aaaaaaabbaaaababbaaaba") ++ (A) ++ ("abbaaaabaabbbabaabbbaaabbaaaaaaaaaaaabbabbababbbabbbbaaabbabbb"))
    =
    ((A) ++ ("aaaaaaabbaaaababbaaaba") ++ (A) ++ ("abbaaaabaabbbabaabbbaaabbaaaaaaaaaaaabbabbababbbabbbbaaabbabbb"));
constraint
  (("bbbabbabbaaabbbbabaaaabbaaaabbabbabbbbaaabbabbabbbabbbbababb") ++ (A) ++ ("babbbaaabababbbaaaabbbbabbabb"))
    =
    (("bbbabbabbaaabbbbabaaaabbaaaabbabbabbbbaaabbabbabbbabbbbababb") ++ (A) ++ ("babbbaa") ++ (A) ++ ("bbbaaaabbbbabbabb"));
constraint
  (("bbbaabababaabaabbaaababbaaaabaaabaabbabbbabbbbababbbbbaaaaaababb") ++ (A) ++ ("bbbbbbbaaabaaaaabababbaaa"))
    =
    (("bbbaabababaabaabbaaababbaaaabaaabaabbabbbabbbbababbbbbaaaaaababb") ++ (A) ++ ("bbbbbbbaaabaaaa") ++ (A) ++ ("bbaaa"));
constraint
  (("ababbbabbbbabbaaababbababbabaaabaabbbaabbbaaaaabbabbbb") ++ (A) ++ ("bbbbabaabbbbabbbabaaaabbaaabaaababb"))
    =
    (("ababbbabbbbabbaaababbababbabaaabaabbbaabbbaaaaabbabbbbabababbbbabaabbbbabbbabaaaabbaaabaaababb") ++ (""));
constraint
  (("bbbababbbabaaabbaa") ++ (A) ++ ("baababbbbbbaabbabaabbbaabbabbaabababbbbabbabbabbaabaabaaaabbbbaaaaaabab"))
    =
    (("bbbababbbabaaabbaaabababaababbbbbbaabbabaabbbaabbabba") ++ (A) ++ ("bbbbabbabbabbaabaabaaaabbbbaaaaaabab"));
constraint
  (("babbbbababbbabaabbbbbbbbaaaabbbabbababaaabbbabb") ++ (A) ++ ("bababbabbaaaaabbaaabbaabbaabaaabaabbbbbaba"))
    =
    (("babbbbababbbabaabbbbbbbbaaaabbbabb") ++ (A) ++ ("aabbbabbabababababbabbaaaaabbaaabbaabbaabaaabaabbbbbaba"));
constraint
  (("bbbaaabbbbbbbbbbaaaaabbbaabbaabaaaabbbbabbbabbbbbababbbbbaabababbbbbababbbabbbbbabbabaaaaaaaba") ++ (""))
    =
    (("bbbaaabbbbbbbbbbaaaaabbbaabbaabaaaabbbbabbbabbbbbababbbbba") ++ (A) ++ ("bbbbbababbbabbbbbabbabaaaaaaaba"));
constraint
  (("baabbaaabbbbbaa") ++ (A) ++ ("babaababbbbaabaabbaaaabbaabaabaaa") ++ (A) ++ ("bbaaabaaaabbabbbbabaabbbabbabbbbbabb"))
    =
    (("baabbaaabbbbbaa") ++ (A) ++ ("babaababbbbaabaabbaaaabbaabaabaaaabababbaaabaaaabbabbbbabaabbbabbabbbbbabb"));
constraint
  (("aaaaabbabbaaaabaaaaaaababaabbabbaabababbbbbabbabbbabbbbabaa") ++ (A) ++ ("bbaababaaabaaaaaabbaba") ++ (A) ++ ("aba"))
    =
    (("aaaaabbabbaaaabaaaaaa") ++ (A) ++ ("abbabba") ++ (A) ++ ("bbbbbabbabbbabbbbabaaabababbaababaaabaaaaaabbabaababaaba"));
constraint
  (("bbbaabbbbabbaabbaaaabbabaababbbbbabbbbbbbabbbbbaaabba") ++ (A) ++ ("aabbaabaabbbbbbaabbaababbbbbaaabaaaa"))
    =
    (("bbbaabbbbabbaabbaaaabbabaababbbbbabbbbbbbabbbbbaaabba") ++ (A) ++ ("aabbaabaabbbbbbaabbaababbbbbaaabaaaa"));
constraint
  (("aaa") ++ (A) ++ ("baaabbaababaababbbbbabbaaabababbbaaaababbaaaabbbabbbaaabbbbaababbabbbbbabaaabbabbbabab"))
    =
    (("aaa") ++ (A) ++ ("baaabbaababaababbbbbabbaaabababbbaaaababbaaaabbbabbbaaabbbbaababbabbbbbabaaabbabbbabab"));
constraint
  (("abbbabbaaaaba") ++ (A) ++ ("bbbaaaabbbbbaaaabbbabaabaaabb") ++ (A) ++ ("bbbbbabaabbbbaabbbabbbbabbbabbbabbbaaaabba"))
    =
    (("abbbabbaaaabaabababbbaaaabbbbbaaaabbbabaabaaabbabababbbbbabaabbbbaabbbabbbbabbbabbbabbbaaaabba") ++ (""));
constraint
  (("abbababbababbbbbaaba") ++ (A) ++ ("aababbbbabaababbabbaaba") ++ (A) ++ ("baaaabbaabbaaabbbbbaabbb") ++ (A) ++ ("babaaabaabaa"))
    =
    (("abbababbababbbbbaaba") ++ (A) ++ ("aababbbbabaababbabbaaba") ++ (A) ++ ("baaaabbaabbaaabbbbbaabbb") ++ (A) ++ ("babaaabaabaa"));
constraint
  (("aaaabaabaaabb") ++ (A) ++ ("aaaabaaababbaabbaaababaabbbaababbbbbaabababaaaabababbbaabaaaabaabbabbbbabbbb"))
    =
    (("aaaabaabaaabbababaaaaabaaababbaabbaaababaabbbaababbbbba") ++ (A) ++ ("baaaabababbbaabaaaabaabbabbbbabbbb"));
constraint
  (("aabaaaabbabaababbbaaaababbaba") ++ (A) ++ ("abbaabbbbabaaababbaabbbababbabbbbaaabbbbbbbbbaaaaabaaaababab"))
    =
    (("aabaaaabbabaababbbaaaababbabaababaabbaabbbbabaaababbaabbbababbabbbbaaabbbbbbbbbaaaaabaaaababab") ++ (""));
constraint
  (("bbabbbaabbaaababbaaabaababbbbbaaabaaaaabababbbabbaabaaababaaabaaabaaaabbaaaaaaabbabbabb") ++ (A) ++ ("bb"))
    =
    (("bbabbbaabbaaababbaaabaababbbbbaaabaaaa") ++ (A) ++ ("bbbabbaabaa") ++ (A) ++ ("aabaaabaaaabbaaaaaaabbabbabbabababb"));
constraint
  (("baaababbbaabbaababbbbaabbabaaaaaaaaabbaabbaaaaaabaababbaabababbabbbbaaaaabaaba") ++ (A) ++ ("bababbbbaba"))
    =
    (("baaababbbaabbaababbbbaabbabaaaaaaaaabbaabbaaaaaabaababbaabababbabbbbaaaaabaaba") ++ (A) ++ ("bababbbbaba"));
constraint
  (("abbababbbaabbaaaabaaaaaaabbbbbbbbbaaabbbabbabaaaabababbbbbbabbaaaaababaabbbbbaaabbbabbbb") ++ (A) ++ ("b"))
    =
    (("abbababbbaabbaaaabaaaaaaabbbbbbbbbaaabbbabbabaaaabababbbbbbabbaaaa") ++ (A) ++ ("abbbbbaaabbbabbbb") ++ (A) ++ ("b"));
constraint
  (("aababaababaaabbaabaaaabaababbaaabbbbbababbbaabababbbbabaaaaaabbbababbbabbbbaaababbaabbbababbba") ++ (""))
    =
    (("a") ++ (A) ++ (A) ++ ("aabbaabaaaabaababbaaabbbbbababbbaabababbbbabaaaaaabbbababbbabbbbaaababbaabbbababbba"));
constraint
  (("abbbabaaababbbaaabaabb") ++ (A) ++ ("bbaabaaaabaabbbabbaaabbaabbbbabaabaabbbbbaabbaaaabbbaaaaaabaabaabaa"))
    =
    (("abbbabaaababbbaaabaabbabababbaabaaaabaabbbabbaaabbaabbbbabaabaabbbbbaabbaaaabbbaaaaaabaabaabaa") ++ (""));
constraint
  (("aabbbbaababbaa") ++ (A) ++ ("abbaaaaabaaaaaabbbaabaabbbababaaabaaaabbabbaabbbabbabbbabbaaabbbaaaabaabaab"))
    =
    (("aabbbbaababbaa") ++ (A) ++ ("abbaaaaabaaaaaabbbaabaabbbababaaabaaaabbabbaabbbabbabbbabbaaabbbaaaabaabaab"));
constraint
  ((A) ++ ("aaababbaaaaababbabaabaaabaaaabbababbabaaabbabbbaabbbaabbaaabaaabbaaaabbaaa") ++ (A) ++ ("baabbaaaab"))
    =
    ((A) ++ ("aaababbaaaaababbabaabaaabaaaabbababbabaaabbabbbaabbbaabbaaabaaabbaaaabbaaa") ++ (A) ++ ("baabbaaaab"));
constraint
  (("bbbaaaaabbabaabbbbaabaaabbbababbabbbbbaaabbbababaaababbabbbbaaabbbbbabbababbaaababbbabbaaababa") ++ (""))
    =
    (("bbbaaaaabbabaabbbbaabaaabbbababbabbbbbaaabbb") ++ (A) ++ ("aababbabbbbaaabbbbbabbababbaaababbbabbaa") ++ (A));
constraint
  (("aabaabbaaabbaabbbbaa") ++ (A) ++ ("abbabbbaaababaaaaabbbabbbbbaaabbababbbaaaaaabbbabbbbba") ++ (A) ++ ("aaaaaabbab"))
    =
    (("aabaabbaaabbaabbbbaaababaabbabbbaaababaaaaabbbabbbbbaaabbababbbaaaaaabbbabbbbbaababaaaaaaabbab") ++ (""));
constraint
  (("baaaabaabbbbbaabaababbbbaabaabbabbbbabaababbbaaabbbbabbaabbbbaaabbabbbbbbaabbbabb") ++ (A) ++ ("abbaabab"))
    =
    (("baaaabaabbbbbaabaababbbbaabaabbabbbbabaababbbaaabbbbabbaabbbbaaabbabbbbbbaabbbabb") ++ (A) ++ ("abbaabab"));
constraint
  (("aaabbabbbaabaababbaabbaaaababbbabbabaabaaabbbaabaabbbabbaabaaabbbaabababaabaaaaababbabbabababb") ++ (""))
    =
    (("aaabbabbbaabaababbaabbaaaababbbabbabaabaaabbbaabaabbbabbaabaaabbba") ++ (A) ++ ("baabaaaaababbabbabababb"));
constraint
  (("aabaabaabbbabaabbaabbbaaaabbaaabbababaaaabbbabaaabaabbbbbaaaabaabbababaaababbabaababbabbaaabbb") ++ (""))
    =
    (("aabaabaabbbabaabbaabbbaaaabbaaabb") ++ (A) ++ ("aaabbbabaaabaabbbbbaaaabaabb") ++ (A) ++ ("aababbabaababbabbaaabbb"));
constraint
  (("bababaaabaaabaaaabbbbababbaabbaaabaaabbbaabaaabbbbbaabaabaabaababbabaabaaaabb") ++ (A) ++ ("baabbaaababb"))
    =
    (("b") ++ (A) ++ ("aabaaabaaaabbbbababbaabbaaabaaabbbaabaaabbbbbaabaabaabaababbabaabaaaabbabababaabbaaababb"));
constraint
  (("abbabbaaaaaaababbaaba") ++ (A) ++ ("bbaaaaabaaaabbaaaaabbbabaaaaaaabaabbabbbbabbababbabbaabaababbaaabaab"))
    =
    (("abbabbaaaaaaababbaaba") ++ (A) ++ ("bbaaaaabaaaabbaaaaabbbabaaaaaaabaabbabbbbabbababbabbaabaababbaaabaab"));
constraint
  (("abbabbb") ++ (A) ++ ("abaababbabbaaabbabbba") ++ (A) ++ ("bbbbabababbbabbaabbbaaba") ++ (A) ++ ("babbbbbbaaaaaaabaaaabaabbaa"))
    =
    (("abbabbbababaabaababbabbaaabbabbbaabababbbbabababbbabbaabbbaabaababababbbbbbaaaaaaabaaaabaabbaa") ++ (""));
constraint
  (("b") ++ (A) ++ ("bbbbbabaaabbbbaaaaaaabbbbabaabbbbaabaaaaaabbbbbaabaabbbaababbababbaabbaabbabbaabbabababa"))
    =
    (("babababbbbbabaaabbbbaaaaaaabbbbabaabbbbaabaaaaaabbbbbaabaabbbaababbababbaabbaabbabbaabbabababa") ++ (""));
constraint
  (("bbabbbabbaabaabaaabbbaabbbaaaabbbbbbbbaabbabbbabaabbabb") ++ (A) ++ ("bbaabbbaaababaababbabaaabbbaabbaaa"))
    =
    (("bbabbbabbaabaabaaabbbaabbbaaaabbbbbbbbaabbabbbabaabbabbabababbaabbbaa") ++ (A) ++ ("ababbabaaabbbaabbaaa"));
constraint
  (("babaaaabbabaabbbabbaaababbbabaaaababaabbbbbaaaaabaabbbbbbbbbaaaababbababbaababbababbbbb") ++ (A) ++ ("ab"))
    =
    (("babaaaabbabaabbbabbaaababbbabaaaababaabbbbbaaaaabaabbbbbbbbbaaaababbababbaababbababbbbbababaab") ++ (""));
constraint
  (("bbbbbaabbaaabaababbbabbaaaaaaaabbbbbabaabbaaababbabbaabbbbababaabbaaaaaaaabbbabaaababbbb") ++ (A) ++ ("a"))
    =
    (("bbbbbaabbaaabaababbbabbaaaaaaaabbbbbabaabbaaababbabbaabbbbababaabbaaaaaaaabbbabaaababbbbababaa") ++ (""));
constraint
  (("aaabaaaabaaaaabbabbbbbbbabbabbbbb") ++ (A) ++ ("bbbaaabbabbaabbbbbaabaabbbbabaabbbabbaaabaaabbabbaabbbbb"))
    =
    (("aaabaaaabaaaaabbabbbbbbbabbabbbbb") ++ (A) ++ ("bbbaaabbabbaabbbbbaabaabbbbabaabbbabbaaabaaabbabbaabbbbb"));
constraint
  (("aaaaaabaaaabbaaabbaabbababbbbaabaaaaaaaaaabbbbaabababaaaaabbaaaabbabaaabbbbbbbaabaaaabbbaabbab") ++ (""))
    =
    (("aaaaaabaaaabbaaabbaabbababbbbaabaaaaaaaaaabbbba") ++ (A) ++ ("baaaaabbaaaabbabaaabbbbbbbaabaaaabbbaabbab"));
constraint
  (("baaabaaabbabbaaa") ++ (A) ++ ("babaababaaabbabaababbabbababaabbabaabaaaabaaabbbbabbabb") ++ (A) ++ ("abbbbababbaba"))
    =
    (("baaabaaabbabbaaaababababaababaaabbabaababbabbababaabbabaabaaaabaaabbbbabbabbababaabbbbababbaba") ++ (""));
constraint
  (("aaabbbabbbaabbba") ++ (A) ++ ("baabbbbabaabaaababaabbbababbb") ++ (A) ++ ("bababbbbbaaaaabbababbabaa") ++ (A) ++ ("baabbbbba"))
    =
    (("aaabbbabbbaabbba") ++ (A) ++ ("baabbbbabaabaa") ++ (A) ++ ("abbbababbbabababababbbbbaaaaabbababbabaa") ++ (A) ++ ("baabbbbba"));
constraint
  (("bbaabbbbaaaababbbababbbabaaabbabbbbbabaabaabbbbaababbbbaaabbabbbbaababbbbbbbbbbabaaa") ++ (A) ++ ("ababb"))
    =
    (("bbaabbbbaaaababbbababbbabaaabbabbbbbabaabaabbbbaababbbbaaabbabbbbaababbbbbbbbbbabaaa") ++ (A) ++ ("ababb"));
constraint
  (("aabaaabbbaaaaaababbabbaaabaabbbaababababaabbbbbbabaababbaaababbbbaabbabbabaaaabaaa") ++ (A) ++ ("bbbaaaa"))
    =
    (("aabaaabbbaaaaaababbabbaaabaabbba") ++ (A) ++ ("babaabbbbbbabaababbaaababbbbaabbabbabaaaabaaa") ++ (A) ++ ("bbbaaaa"));
constraint
  (("bbaabbbaaabbabbaabbabbababbaabbabaababaabbababaababbbaaabaabbbbaabbabbbbbbbaabbbabbaaaaaabbaab") ++ (""))
    =
    (("bbaabbbaaabbabbaabbabbababbaabbabaababaabb") ++ (A) ++ ("ababbbaaabaabbbbaabbabbbbbbbaabbbabbaaaaaabbaab"));
constraint
  (("bbababaabaaaabababaaabbbbbabbaaabbaaaabaabbabbaaaaaabaaaaba") ++ (A) ++ (A) ++ ("abbabbbbbbaabaabaababbbaa"))
    =
    (("bb") ++ (A) ++ ("abaaaabababaaabbbbbabbaaabbaaaabaabbabbaaaaaabaaaaba") ++ (A) ++ ("ababaabbabbbbbbaabaabaababbbaa"));
constraint
  (("bbabbaaaaaaabbababbaabbabaaaababb") ++ (A) ++ ("aaaaaaabbbaababbbbaabbaababaaaaaaaaa") ++ (A) ++ ("baabbbbbaabbaba"))
    =
    (("bbabbaaaaaaabbababbaabbabaaaababbababaaaaaaaabbbaababbbbaabba") ++ (A) ++ ("aaaaaaaa") ++ (A) ++ ("baabbbbbaabbaba"));
constraint
  (("aabbabbaababbbabb") ++ (A) ++ ("b") ++ (A) ++ ("abbbbababbabbbabbaabbbababbbbbaaabbbababb") ++ (A) ++ ("aaaababbaabbbbaabbba"))
    =
    (("aabbabbaababbbabbababab") ++ (A) ++ ("abbbbababbabbbabbaabbbababbbbbaaabbbababb") ++ (A) ++ ("aaaababbaabbbbaabbba"));
constraint
  (("aabbabbbbaaaaabaaba") ++ (A) ++ (A) ++ (A) ++ ("baababbabbababbbbbabaababa") ++ (A) ++ ("aaabbbaabbbabbbababbabaaaabab"))
    =
    (("aabbabbbbaaaaabaaba") ++ (A) ++ (A) ++ (A) ++ ("baababbabbababbbbbaba") ++ (A) ++ (A) ++ ("aaabbbaabbbabbbababbabaaaabab"));
constraint
  (("abaaaabbbaababbbaabaabbbabaaaaabbaabababbabbabbabbbbbaaabbaaaabbaaabba") ++ (A) ++ ("aaabaababbabbbbbaaa"))
    =
    (("abaaaabbbaababbbaabaabbbabaaaaabbaabababbabbabbabbbbbaaabbaaaabbaaabbaababaaaabaababbabbbbbaaa") ++ (""));
constraint
  (("babbabbaaabaaaaaa") ++ (A) ++ ("bbbbaababbaaabbbbabaabbbbbaaaabbbbbaabaabbaabaaaabaaaabbabbbabaababbbaba"))
    =
    (("babbabbaaabaaaaaaabababbbbaababbaaabbbbabaabbbbbaaaabbbbbaabaabbaabaaaabaaaabbabbbabaababbbaba") ++ (""));
constraint
  (("ababbabbaabbaababaaabababbbbbbabaabbabbaababbbabbbbba") ++ (A) ++ ("abbaaa") ++ (A) ++ ("bbababba") ++ (A) ++ ("bbabaaabbbaa"))
    =
    (("ababbabbaabba") ++ (A) ++ ("a") ++ (A) ++ ("bbbbbbabaabbabbaababbbabbbbbaababaabbaaa") ++ (A) ++ ("bbababbaabababbabaaabbbaa"));
constraint
  (("babbbabbbaabbbbbbb") ++ (A) ++ ("bbaabababbbbaaababbbbb") ++ (A) ++ ("bbaabaaabbbabaabbaabbabbabbb") ++ (A) ++ ("aaaaabbabaa"))
    =
    (("babbbabbbaabbbbbbbabababba") ++ (A) ++ ("bbbbaaababbbbbabababbaabaaabbbabaabbaabbabbabbb") ++ (A) ++ ("aaaaabbabaa"));
constraint
  (("abbaabaaabbbababaabbabbaabbabaaaa") ++ (A) ++ ("b") ++ (A) ++ ("aaabaaabbaababbabaabbabbabbaabbbaaaaaaabbaabaaaaba"))
    =
    (("abbaabaaabbbababaabbabbaabbabaaaaabababababaaaabaaabbaababbabaabbabbabbaabbbaaaaaaabbaabaaaaba") ++ (""));
constraint
  (("abbbaaabaaaabbabbbaaabaaaaaabbbabbbaabababbbaabbaaaaaabaabaabbbabaabbbaababbbabbbababbbbaabbba") ++ (""))
    =
    (("abbbaaabaaaabbabbbaaabaaaaaabbbabbba") ++ (A) ++ ("bbbaabbaaaaaabaabaabbbabaabbbaababbbabbbababbbbaabbba"));
constraint
  (("abbaaabbaaabbbabbababbaaabbbbbbbbbabbbbaabbaabbaaaabaaabbaaabbaaaabaaaabba") ++ (A) ++ ("baabaababaaabaa"))
    =
    (("abbaaabbaaabbbabbababbaaabbbbbbbbbabbbbaabbaabbaaaabaaabbaaabbaaaabaaaabba") ++ (A) ++ ("baabaababaaabaa"));
constraint
  (("bbababaabaaabbabaaaabaabababbbbaaababbbaaabaabbabbabababbabbbaabbabaabaabbbbbbbbbbaaabaaabaaba") ++ (""))
    =
    (("bbababaabaaabbabaaaaba") ++ (A) ++ ("bbbbaaababbbaaabaabbabb") ++ (A) ++ ("bbabbbaabbabaabaabbbbbbbbbbaaabaaabaaba"));
constraint
  (("ababbbbbbbbabbaaabbabaababb") ++ (A) ++ ("aa") ++ (A) ++ ("aaabbabbbababbb") ++ (A) ++ ("bbbabababbaaaabbaaabaababbabbbbbabb"))
    =
    (("ababbbbbbbbabbaaabbabaababbababaaaababaaaabbabbbababbbabababbb") ++ (A) ++ ("bbaaaabbaaabaababbabbbbbabb"));
constraint
  (("aaabbabbbbabbababaaabbaabbbbabbabaaaaba") ++ (A) ++ ("bbaaabaaabbbbaaaababbbbaabbbbbaabaabaaaaaabbaaabab"))
    =
    (("aaabbabbbbabb") ++ (A) ++ ("aabbaabbbbabbabaaaabaabababbaaabaaabbbbaaaababbbbaabbbbbaabaabaaaaaabbaaabab"));
constraint
  (("aabbababbaabbaabbbaabaaababbbaabaa") ++ (A) ++ ("bbbbaabbbaaabaabaabaa") ++ (A) ++ ("bbbbabbaaabababbbaaaabbabaabb"))
    =
    (("aabbababbaabbaabbbaabaaababbbaabaaabababbbbaabbbaaabaabaabaa") ++ (A) ++ ("bbbbabbaaabababbbaaaabbabaabb"));
constraint
  (("babaaabbbbaaababbbbabbbabbbaabbbabbbbaabaaababbabbaabaababb") ++ (A) ++ ("baaabbbaabbbbabbaabb") ++ (A) ++ ("bbabb"))
    =
    (("babaaabbbbaaababbbbabbbabbbaabbbabbbbaabaaababbabbaabaababb") ++ (A) ++ ("baaabbbaabbbbabbaabbabababbabb"));
constraint
  (("aaabaababbabbaaababbaabbbbbababbaabaabaaababbaaabaabbabbbbaaabbabaababab") ++ (A) ++ ("babbaabbabaaabaaa"))
    =
    (("aaabaababbabbaaababbaabbbbbababbaabaabaaababbaaabaabbabbbbaaabbaba") ++ (A) ++ ("b") ++ (A) ++ ("babbaabbabaaabaaa"));
constraint
  (("babbbaaababaabaababbaaaaaabbbbaabaababbbababbbbaabaaaaaabbaabb") ++ (A) ++ ("babbbaabbbaaaabaaabaaaaaabb"))
    =
    (("babbbaaababaabaababbaaaaaabbbbaabaababbbababbbbaabaaaaaabbaabbababababbbaabbbaaaabaaabaaaaaabb") ++ (""));
constraint
  (("aaaabaaaababbbaabbbbbbababbaabbbbbbbbabbbabbbbbbbaaaabbaabaaaabbbaabbbabbbbabbbbabababababaaaa") ++ (""))
    =
    (("aaaabaaaababbbaabbbbbbababbaabbbbbbbbabbbabbbbbbbaaaabbaabaaaabbbaabbbabbbbabbbbababab") ++ (A) ++ ("aaa"));
constraint
  (("aabbbbabaaaabbabbbbbababababbabaa") ++ (A) ++ ("baabaabbbbbabaababaaaabbbaaaababbbbababbababbaaabbbababb"))
    =
    (("aabbbbabaaaabbabbbbb") ++ (A) ++ ("babbabaa") ++ (A) ++ ("baabaabbbbbabaababaaaabbbaaaababbbbababbababbaaabbbababb"));
constraint
  (("baabbbabbbabbabbbabbaabbbabaa") ++ (A) ++ ("aababaaababbbbbbababbbabbaababbababbaabbbababbbbbabbaabbaaba"))
    =
    (("baabbbabbbabbabbbabbaabbbabaaababaa") ++ (A) ++ ("aababbbbbbababbbabbaababbababbaabbbababbbbbabbaabbaaba"));
constraint
  (("bbaaabaaabbbbaabaababbbbaabbbabababa") ++ (A) ++ ("aababbaabaabbbbbaaa") ++ (A) ++ ("aaaabaaabbabbbabbaaabbabaaaaa"))
    =
    (("bbaaabaaabbbbaabaababbbbaabbb") ++ (A) ++ ("baababaaababbaabaabbbbbaaaababaaaaabaaabbabbbabbaaabbabaaaaa"));
constraint
  (("babbaaababbaabbaaaabababbabbababba") ++ (A) ++ ("bbbbbbaaaaaaabbaaaabbabbaaabaababaaaabbaaaaabaaabbbaaaa"))
    =
    (("babbaaababbaabbaaaabababbabbababbaabababbbbbbaaaaaaabbaaaabbabbaaaba") ++ (A) ++ ("aaabbaaaaabaaabbbaaaa"));
constraint
  (("baaaabaabbababbababaaaabbbbaaaabaabbbabbbbabaaabbaaaabbbbb") ++ (A) ++ ("abaabbbbabbbbbbabbbababaaabbbbb"))
    =
    (("baaaabaabbababb") ++ (A) ++ ("aaabbbbaaaabaabbbabbbbabaaabbaaaabbbbb") ++ (A) ++ ("abaabbbbabbbbbbabbbababaaabbbbb"));
constraint
  (("aabbbaababbbbbbbabbbaaabbbbba") ++ (A) ++ ("aabbaabbabababbbabbabbbbbabaaabaababbaaabbbbaaabbabbbabbaabb"))
    =
    (("aabbbaababbbbbbbabbbaaabbbbba") ++ (A) ++ ("aabbaabb") ++ (A) ++ ("bbbabbabbbbbabaaabaababbaaabbbbaaabbabbbabbaabb"));
constraint
  ((A) ++ ("aabbbbbb") ++ (A) ++ ("bbbbbaabb") ++ (A) ++ ("aabbaaaaaaababbaaaabbaabbabaaaaabbababbaaabaaaaaabbaaabaabaaaa"))
    =
    (("ababaaabbbbbb") ++ (A) ++ ("bbbbbaabbababaaabbaaaaaaababbaaaabbaabbabaaaaabbababbaaabaaaaaabbaaabaabaaaa"));
constraint
  (("ababbbabbbaabaaababbbabaabaaaaabbababaaabababbbbabbababbbbaabbbbbbaaaaaaaabbbabaaaaababbaabbab") ++ (""))
    =
    (("ababbbabbbaabaaababbbabaabaaaaabb") ++ (A) ++ ("a") ++ (A) ++ ("bbbbabbababbbbaabbbbbbaaaaaaaabbbabaaaaababbaabbab"));
constraint
  (("aaaa") ++ (A) ++ ("aaaabbbabbaaaabbbbbababbabaaaabaaaabbaababbbaabbbbbaaabbabbaabbbaaaabbbaabbbbaabbaaab"))
    =
    (("aaaaababaaaaabbbabbaaaabbbbbababbabaaaabaaaabbaababbbaabbbbbaaabbabbaabbbaaaabbbaabbbbaabbaaab") ++ (""));
constraint
  (("aaaabbabaaaabaababbababbaabbabbabbaabaaababbbbabaaabaaabaabaaabbabbaaaaabbbaaabaaaaababaa") ++ (A))
    =
    (("aaaabbabaaaabaababbababbaabbabbabbaabaaababbbbabaaabaaabaabaaabbabbaaaaabbbaaabaaaaababaaababa") ++ (""));
constraint
  (("baaaabbabaabbbbaabbabbaaaba") ++ (A) ++ ("aababbbbaabbbbbbabbabaabbbabbbbaaaaaaabbbbbaabaaabbbbaabaababa"))
    =
    (("baaaabbabaabbbbaabbabbaaabaababaaababbbbaabbbbbbabbabaabbbabbbbaaaaaaabbbbbaabaaabbbbaaba") ++ (A));
constraint
  (("babb") ++ (A) ++ ("bbabaaaaabbbaaaaabbaaababbabbabaaaaabbbaabbbbbbabbaaaaabaaabaaaaaaaaababbabaabbabaaba"))
    =
    (("babbabababbabaaaaabbbaaaaabbaaababbabbabaaaaabbbaabbbbbbabbaaaaabaaabaaaaaaaaababbabaabbabaaba") ++ (""));
constraint
  (("abbbabbabbaaababbbbaaabbbbbbbabbaaaaabaabbbabbbbababaaaaa") ++ (A) ++ ("abaaababbabaabbaaabbbbbaabaabbbb"))
    =
    (("abbbabbabbaaababbbbaaabbbbbbbabbaaaaabaabbbabbbbababaaaaaababaabaaababbabaabbaaabbbbbaabaabbbb") ++ (""));
constraint
  (("bbababbbaaaabbbabaabbbabbbabbaababaababbaaaaabaaaaabbabaaabaabbabaabbbbbabbbaaababbaababbabbab") ++ (""))
    =
    (("bbababbbaaaabbbabaabbbabbbabba") ++ (A) ++ ("ababbaaaaabaaaaabbabaaabaabbabaabbbbbabbbaaababbaababbabbab"));
constraint
  (("abbbbbaababababaaaaabbbabb") ++ (A) ++ ("aaaabbbbaababbbabbabbabaaaaaabbbbababbaaabbbbbabaabaaabbbbbaaaa"))
    =
    (("abbbbba") ++ (A) ++ ("babaaaaabbbabbababaaaaabbbbaababbbabbabbabaaaaaabbbbababbaaabbbbbabaabaaabbbbbaaaa"));
constraint
  (("baaaaaaabbabbbaaaaaaaabbbabbaaaaaaaabbbaaabbabaaabbaabbbabbbababbabbbaaabbbbabaabbb") ++ (A) ++ ("aabaaa"))
    =
    (("baaaaaaabbabbbaaaaaaaabbbabbaaaaaaaabbbaaabbabaaabbaabbbabbbababbabbbaaabbbbabaabbbababaaabaaa") ++ (""));
constraint
  (("babbaaaaaabaaabbbabbbbabbaabbaaaabaabaaababbbabbbaaaabbaaabbbaabbbabbbbabababbaabbababbbbbbaab") ++ (""))
    =
    (("babbaaaaaabaaabbbabbbbabbaabbaaaabaabaaababbbabbbaaaabbaaabbbaabbbabbbb") ++ (A) ++ ("bbaabbababbbbbbaab"));
constraint
  (("babbbbabaaabbbbaabaabaababaaabbaaaabbaa") ++ (A) ++ ("abbbabbaaaabaaabbbbaaaaaaaabbbbaabaaaabbbbaaabaaaa"))
    =
    (("babbbbabaaabbbbaabaabaababaaabbaaaabbaaababaabbbabbaaaabaaabbbbaaaaaaaabbbbaabaaaabbbbaaabaaaa") ++ (""));
constraint
  ((A) ++ ("aaaababaabbbabbaaaabbaabbbbbbbaabbbabaabaabbabbb") ++ (A) ++ ("bbababbaababbbbaaaaaabababbaaabbbbaa"))
    =
    ((A) ++ ("aaa") ++ (A) ++ ("abbbabbaaaabbaabbbbbbbaabbbabaabaabbabbbabababbababbaababbbbaaaaaabababbaaabbbbaa"));
constraint
  (("abaababaabaaababaaaabaa") ++ (A) ++ ("abaaaababababbbaaaaaababbbbaababbbbababbbbbabbaaaabbbabbbabaabaaaa"))
    =
    (("aba") ++ (A) ++ ("abaaababaaaabaaababaabaaa") ++ (A) ++ ("babbbaaaaaababbbbaababbbbababbbbbabbaaaabbbabbbabaabaaaa"));
constraint
  (("aba") ++ (A) ++ ("baaababababbaabbaaaabaababbabbabbbabbbbbbbbabbabaaabaaaabbbbaaaaaaabbbbbaaaabbaaaaaabb"))
    =
    (("aba") ++ (A) ++ ("baa") ++ (A) ++ ("babbaabbaaaabaababbabbabbbabbbbbbbbabbabaaabaaaabbbbaaaaaaabbbbbaaaabbaaaaaabb"));
solve satisfy;
