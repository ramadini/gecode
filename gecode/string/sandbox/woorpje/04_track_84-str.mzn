var string: B;
var string: A;
constraint
  (("bbaabb") ++ (A) ++ ("bbbbaabbabaabbabaaaababbabbbb")) =
    (("bbaabbbbabbbaabaabbb") ++ (B) ++ ("babbabbbb"));
constraint
  (("abbabbaaaaabbbaaaabbaabbabbbaabaababbbbbbaaaaab") ++ ("")) =
    (("abbabbaaaaabbbaaaabbaa") ++ (A) ++ ("abbbbbbaaaaab"));
constraint
  (("bbaabbbbaababab") ++ (A) ++ ("babbaababaabbaabaabb")) =
    (("bbaabbbbaababab") ++ (A) ++ ("babbaababaabbaabaabb"));
constraint
  (("bbbbabbabbaba") ++ (A) ++ ("abbaabbbbabbbbaaaaabbb")) =
    (("bbbbabbabbababbabbbaabaababbaabbbbabbbbaaaaabbb") ++ (""));
constraint
  (("aaaaa") ++ (A) ++ ("baaaaababaaabbabaabbbabaaaaaab")) =
    (("aaaaabbabbbaabaabbaaaaababaaabbabaabbbabaaaaaab") ++ (""));
constraint
  (("babbaabbbaaabbbababbbbbbabbbaabaabaaabbabbbbaba") ++ ("")) =
    (("babbaabbbaaabbbababbbb") ++ (A) ++ ("aaabbabbbbaba"));
constraint
  (("abbbbbabbbaabaababaaabbabaabbbbabaabbaabbababbb") ++ ("")) =
    (("abbb") ++ (A) ++ ("abaaabbabaabbbbabaabbaabbababbb"));
constraint
  (("abbaaaaabbbaabbbbaaaaabaaabbbabbbbbbaaabaaaaaaa") ++ ("")) =
    (("abbaaaaabbba") ++ (B) ++ ("aa"));
constraint
  (("aaa") ++ (A) ++ ("bababaaaabbbaaabbababaabaab")) =
    (("aaababa") ++ (B) ++ ("babababbababababaaaabbbaaa") ++ (B) ++ ("ababaabaab"));
constraint
  (("aabbbba") ++ (B) ++ ("bababaaabbbaaa") ++ (B) ++ ("baababbabbaaaabaaababa"))
    =
    (("aabbbbabbbababaaabbbaaa") ++ (B) ++ ("baababba") ++ (B) ++ ("aaaabaaababa"));
constraint
  (("a") ++ (B) ++ (B) ++ ("aaba") ++ (B) ++ ("ababa") ++ (B) ++ ("ababbaaaaabaaabababbbaaa") ++ (B) ++ ("bbb"))
    =
    (("a") ++ (B) ++ (B) ++ ("aababbababa") ++ (B) ++ ("ababbaaaaabaaababa") ++ (B) ++ ("baaa") ++ (B) ++ ("bbb"));
constraint
  (("a") ++ (B) ++ ("baabaababbababaa") ++ (B) ++ ("aaabaaa") ++ (B) ++ ("aabbababbbba") ++ (B) ++ ("abb"))
    =
    (("a") ++ (B) ++ ("baabaaba") ++ (B) ++ ("ababaa") ++ (B) ++ ("aaabaaa") ++ (B) ++ ("aabbababb") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B));
constraint
  (("baabaaaa") ++ (B) ++ ("ababaaaaba") ++ (B) ++ ("aaaabbababa") ++ (B) ++ ("aaba") ++ (B) ++ ("bbbaba"))
    =
    (("baabaaaa") ++ (B) ++ ("ababaaaababbaaaa") ++ (B) ++ ("ababa") ++ (B) ++ ("aaba") ++ (B) ++ ("bbbaba"));
constraint
  (("abbbababbabbaababa") ++ (B) ++ ("ba") ++ (B) ++ ("babb") ++ (B) ++ ("abbabaaabaaaa") ++ (B) ++ ("bb"))
    =
    (("a") ++ (B) ++ ("bababbabbaabababbba") ++ (B) ++ ("babb") ++ (B) ++ ("abbabaaabaaaabb") ++ (B));
constraint
  (("bbbaa") ++ (B) ++ ("baaaabbaaaabbbbbaaa") ++ (B) ++ ("baaaaabaa") ++ (B) ++ ("bbba") ++ (B) ++ ("ab"))
    =
    (("bbbaabbbaaaa") ++ (B) ++ ("aaaa") ++ (B) ++ (B) ++ ("baaabbbaaaaabaabbbbbabbab"));
constraint
  (("babaa") ++ (B) ++ ("babbbabbabbba") ++ (B) ++ ("baaaa") ++ (B) ++ ("aaaaabbba") ++ (B) ++ ("aa") ++ (B) ++ ("abb"))
    =
    (("babaabbbabbba") ++ (B) ++ ("abbbabbbaaaa") ++ (B) ++ ("aaaaa") ++ (B) ++ ("ba") ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B));
constraint
  (("ababaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aababaabaabaa") ++ (B) ++ ("aaaabaa") ++ (B) ++ ("ba") ++ (B) ++ ("b"))
    =
    (("ababaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aaabbaababaabaabaabbaaaabaabbba") ++ (B) ++ ("b"));
constraint
  (("aababa") ++ (B) ++ ("aabbababaabbbbbabbabaabaaabbaaaaaa") ++ (B) ++ (B) ++ ("b"))
    =
    (("aabababbaa") ++ (B) ++ ("ababaa") ++ (B) ++ ("bbba") ++ (B) ++ ("abaabaaa") ++ (B) ++ ("aaaaaabbbbb"));
constraint
  ((B) ++ ("aaaaaa") ++ (B) ++ ("aaaabbbbbaaabaa") ++ (B) ++ (B) ++ ("baaaabbbba") ++ (B) ++ ("abaaaa"))
    =
    (("bbaaaaaa") ++ (B) ++ ("aaaa") ++ (B) ++ ("bbbaaabaa") ++ (B) ++ (B) ++ ("baaaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("abaaaa"));
constraint
  (("aba") ++ (B) ++ ("aaabaabaabaaababaabbaabbbababaa") ++ (B) ++ ("baaaaa") ++ (B) ++ ("b"))
    =
    (("aba") ++ (B) ++ ("aaabaabaabaaababaa") ++ (B) ++ ("aabbbababaabbbaaaaabbb"));
constraint
  (("bb") ++ (B) ++ ("aabaa") ++ (B) ++ ("bbaa") ++ (B) ++ ("abaaaaabb") ++ (B) ++ ("bbba") ++ (B) ++ ("abaaaaabaa") ++ (B) ++ ("a"))
    =
    ((B) ++ (B) ++ ("aabaabbbbaa") ++ (B) ++ ("abaaaaabbbbbbba") ++ (B) ++ ("abaaaaabaabba"));
constraint
  (("aa") ++ (B) ++ (B) ++ ("baaba") ++ (B) ++ ("ababaaaaaaaabbbbaaaaaaaababaabaabb"))
    =
    (("aa") ++ (B) ++ (B) ++ ("baababbababaaaaaaaa") ++ (B) ++ (B) ++ ("aaaaaaaababaabaabb"));
constraint
  (("aaabaaaabaaaaaa") ++ (B) ++ ("bbbabaabababaa") ++ (B) ++ ("aba") ++ (B) ++ ("babbbaaab"))
    =
    (("aaabaaaabaaaaaa") ++ (B) ++ ("bbbabaabababaabbaba") ++ (B) ++ ("babbbaaab"));
constraint
  (("abaaaaabaaaaabbabbabaaa") ++ (B) ++ ("abaaabaababaaba") ++ (B) ++ (B) ++ ("aab"))
    =
    (("abaaaaabaaaaa") ++ (B) ++ ("a") ++ (B) ++ ("abaaabbabaaabaababaababbbbaab"));
constraint
  ((B) ++ ("bba") ++ (B) ++ ("aaaaabaabaaabababbbbbbbbababaaaabaabbaab")) =
    ((B) ++ (B) ++ ("a") ++ (B) ++ ("aaaaabaabaaabababbbb") ++ (B) ++ (B) ++ ("ababaaaabaa") ++ (B) ++ ("aab"));
constraint
  (("bbabaa") ++ (B) ++ ("aaabb") ++ (B) ++ ("bba") ++ (B) ++ ("a") ++ (B) ++ ("baaabaa") ++ (B) ++ ("babbaabbaabaa") ++ (B))
    =
    (("bbabaa") ++ (B) ++ ("aaabbbbbba") ++ (B) ++ ("a") ++ (B) ++ ("baaabaabbba") ++ (B) ++ ("aa") ++ (B) ++ ("aabaabb"));
constraint
  (("aaaabba") ++ (B) ++ ("ababa") ++ (B) ++ ("abbabbbaa") ++ (B) ++ ("bbaaaabb") ++ (B) ++ ("a") ++ (B) ++ ("babbabb"))
    =
    (("aaaabbabbababa") ++ (B) ++ ("abba") ++ (B) ++ ("baabb") ++ (B) ++ ("aaaa") ++ (B) ++ (B) ++ ("abbbabbabb"));
constraint
  (("bba") ++ (B) ++ ("babbabbabaaaba") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("babaabaaabaaabaaabbaa"))
    =
    ((B) ++ ("abbbabba") ++ (B) ++ ("abaaababb") ++ (B) ++ ("abbbabaabaaabaaabaaa") ++ (B) ++ ("aa"));
constraint
  (("aaababbbaabaaba") ++ (B) ++ ("abbba") ++ (B) ++ ("aaabbaaa") ++ (B) ++ ("bbbbabaaaabab"))
    =
    (("aaaba") ++ (B) ++ ("baabaababba") ++ (B) ++ ("ba") ++ (B) ++ ("aaa") ++ (B) ++ ("aaabbbbbbabaaaabab"));
constraint
  (("aaaaba") ++ (B) ++ ("abbaababbabb") ++ (B) ++ ("aabbaaaabb") ++ (B) ++ ("abaaabb") ++ (B) ++ ("ba") ++ (B))
    =
    (("aaaababbabbaaba") ++ (B) ++ ("abbbbaa") ++ (B) ++ ("aaaa") ++ (B) ++ (B) ++ ("abaaa") ++ (B) ++ ("bbbabb"));
constraint
  (("ba") ++ (B) ++ ("aaaabaabbbbaaa") ++ (B) ++ ("bbbbbaaba") ++ (B) ++ ("bbbaaaaaa") ++ (B) ++ ("aabbb"))
    =
    (("ba") ++ (B) ++ ("aaaabaabbbbaaabbbb") ++ (B) ++ ("baababb") ++ (B) ++ ("baaaaaa") ++ (B) ++ ("aa") ++ (B) ++ ("b"));
constraint
  (("aaaaaaaba") ++ (B) ++ (B) ++ ("baaababbbabbababaaaabaababaabbbbaa")) =
    (("aaaaaaaba") ++ (B) ++ ("bbbaaababbbabbababaaaabaababaabb") ++ (B) ++ ("aa"));
constraint
  (("bbbabbaaaa") ++ (B) ++ ("abaabaaaaaaababaaaaaabaaabbbaabbbab")) =
    ((B) ++ ("ba") ++ (B) ++ ("aaaabbabaabaaaaaaababaaaaaabaaa") ++ (B) ++ ("baa") ++ (B) ++ ("bab"));
constraint
  (("aaabbaaa") ++ (B) ++ ("ba") ++ (B) ++ ("bbaaaaabaaaaabaaabaabaababaaaabba"))
    =
    (("aaa") ++ (B) ++ ("aaabbba") ++ (B) ++ ("bbaaaaabaaaaabaaabaabaababaaaa") ++ (B) ++ ("a"));
constraint
  (("bb") ++ (B) ++ ("aabaabbbbbba") ++ (B) ++ ("aba") ++ (B) ++ (B) ++ ("baaaa") ++ (B) ++ (B) ++ ("baaa") ++ (B) ++ ("aabbaba"))
    =
    (("bbbbaabaabb") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aba") ++ (B) ++ ("bbbaaaabbbbbaaa") ++ (B) ++ ("aabbaba"));
constraint
  (("aaaaaabb") ++ (B) ++ ("a") ++ (B) ++ ("aaaa") ++ (B) ++ ("aaabbbaaababa") ++ (B) ++ ("abbabbababbba"))
    =
    (("aaaaaa") ++ (B) ++ ("bbabbaaaabbaaa") ++ (B) ++ ("baaabababba") ++ (B) ++ ("a") ++ (B) ++ ("ababbba"));
constraint
  (("aabababbabaaaaababa") ++ (B) ++ ("abaaabbabaaaababa") ++ (B) ++ ("bbbbbab"))
    =
    (("aabababbabaaaaababa") ++ (B) ++ ("abaaabbabaaaababa") ++ (B) ++ (B) ++ ("bbbab"));
constraint
  (("abbabbbaaa") ++ (B) ++ (B) ++ ("abba") ++ (B) ++ ("ba") ++ (B) ++ ("bbaaba") ++ (B) ++ ("abaabbaa") ++ (B) ++ ("aaabb"))
    =
    (("abba") ++ (B) ++ ("baaabb") ++ (B) ++ ("a") ++ (B) ++ ("abbba") ++ (B) ++ (B) ++ ("aaba") ++ (B) ++ ("abaabbaa") ++ (B) ++ ("aaa") ++ (B));
constraint
  (("babaabbaabbbaaaba") ++ (B) ++ ("aaabaaaaaababaabaa") ++ (B) ++ (B) ++ ("aba") ++ (B) ++ ("a"))
    =
    (("babaabbaabbbaaaba") ++ (B) ++ ("aaabaaaaaababaabaabbbbaba") ++ (B) ++ ("a"));
constraint
  (("bababaaabba") ++ (B) ++ ("abbaa") ++ (B) ++ (B) ++ ("bbaa") ++ (B) ++ ("abb") ++ (B) ++ ("aaaaaaaa") ++ (B) ++ ("abba"))
    =
    (("bababaaabbabbabbaabb") ++ (B) ++ (B) ++ ("aabba") ++ (B) ++ ("bbaaaaaaaa") ++ (B) ++ ("a") ++ (B) ++ ("a"));
constraint
  (("ababaa") ++ (B) ++ ("abaabababaaabbbababa") ++ (B) ++ ("ba") ++ (B) ++ ("ababbaa") ++ (B) ++ ("ba") ++ (B))
    =
    (("ababaa") ++ (B) ++ ("abaabababaaabbbabababbba") ++ (B) ++ ("ababbaabbbabb"));
constraint
  ((B) ++ ("babaabaa") ++ (B) ++ (B) ++ ("baaaba") ++ (B) ++ ("baabaabaaabbabbaabbaa") ++ (B) ++ ("ab"))
    =
    (("bbbabaabaabbbbbaaababbbaabaabaaabba") ++ (B) ++ ("aa") ++ (B) ++ ("aabbab"));
constraint
  (("a") ++ (B) ++ ("a") ++ (B) ++ ("bbabaabbbaabbbabbbaaaabaaababbbbaaaaa") ++ (B) ++ ("aa"))
    =
    (("abbabbbbabaabbbaa") ++ (B) ++ ("babbbaaaabaaaba") ++ (B) ++ ("bbaaaaa") ++ (B) ++ ("aa"));
constraint
  (("baaaabbbabbaaaaaaaaabaaaabbbaabaaabaa") ++ (B) ++ ("a") ++ (B) ++ ("babaa"))
    =
    (("baaaabbba") ++ (B) ++ ("aaaaaaaaabaaaa") ++ (B) ++ ("baabaaabaabbabbbabaa"));
constraint
  (("aaaabaaaaa") ++ (B) ++ ("a") ++ (B) ++ ("abaa") ++ (B) ++ ("abaa") ++ (B) ++ ("bababa") ++ (B) ++ ("aabaaba") ++ (B) ++ ("bab"))
    =
    (("aaaabaaaaabba") ++ (B) ++ ("abaabbabaa") ++ (B) ++ ("bababa") ++ (B) ++ ("aabaababbbab"));
constraint
  (("abbabbaabababa") ++ (B) ++ ("ba") ++ (B) ++ ("aa") ++ (B) ++ ("bbaaaaaaa") ++ (B) ++ ("abbaba") ++ (B) ++ ("bbba"))
    =
    (("abba") ++ (B) ++ ("aababababbbabbaabb") ++ (B) ++ ("aaaaaaa") ++ (B) ++ ("a") ++ (B) ++ ("aba") ++ (B) ++ ("bbba"));
constraint
  (("baaabaababbaaba") ++ (B) ++ ("abaaabaaaaa") ++ (B) ++ ("ababbba") ++ (B) ++ ("aa") ++ (B) ++ ("aaaa"))
    = (("baaabaababbaababbabaaabaaaaabbababbba") ++ (B) ++ ("aabbaaaa"));
constraint
  (("abbbabaaaa") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("aababaaabababbbaaaaababb") ++ (B) ++ ("a") ++ (B))
    =
    (("a") ++ (B) ++ ("babaaaabbbabbbbaababaaabababbbaaaaaba") ++ (B) ++ (B) ++ ("abb"));
constraint
  ((B) ++ ("baaababaaabaabbbaabaaababbbbaaabbbbabbbabaaba") ++ ("")) =
    ((B) ++ ("baaababaaabaabbbaabaaababb") ++ (B) ++ ("aaa") ++ (B) ++ ("bba") ++ (B) ++ ("babaaba"));
constraint
  (("a") ++ (B) ++ ("aa") ++ (B) ++ ("aaaaa") ++ (B) ++ ("bbababb") ++ (B) ++ ("bba") ++ (B) ++ ("aaaaa") ++ (B) ++ ("aababbbaabab"))
    =
    (("a") ++ (B) ++ ("aa") ++ (B) ++ ("aaaaa") ++ (B) ++ (B) ++ ("ababbbbbba") ++ (B) ++ ("aaaaa") ++ (B) ++ ("aaba") ++ (B) ++ ("baabab"));
constraint
  (("aba") ++ (B) ++ (B) ++ ("ababababbabbaabbbb") ++ (B) ++ ("abbaaababbbababa") ++ (B) ++ (B))
    =
    (("ababbbbababababba") ++ (B) ++ ("aabbbbbbabbaaababbbababa") ++ (B) ++ ("bb"));
constraint
  (("baaaaaa") ++ (B) ++ ("aabbbbaaaaabaa") ++ (B) ++ ("babaabaa") ++ (B) ++ ("ba") ++ (B) ++ ("a") ++ (B) ++ ("aaaba"))
    =
    (("baaaaaa") ++ (B) ++ ("aa") ++ (B) ++ ("bbaaaaabaa") ++ (B) ++ ("babaabaabbba") ++ (B) ++ ("a") ++ (B) ++ ("aaaba"));
constraint
  (("babbbbbaaaaaa") ++ (B) ++ ("aaabbabaaaba") ++ (B) ++ (B) ++ (B) ++ ("aaabababbbbaba"))
    =
    (("ba") ++ (B) ++ ("bbbaaaaaabbaaabbabaaababbbbbbaaababa") ++ (B) ++ (B) ++ ("aba"));
constraint
  (("bb") ++ (B) ++ (B) ++ ("abbaaaba") ++ (B) ++ ("bb") ++ (B) ++ ("aababbbaabababbababbaabbaba"))
    =
    (("bbbbbba") ++ (B) ++ ("aaaba") ++ (B) ++ (B) ++ ("bbaaba") ++ (B) ++ ("baabababbababbaabbaba"));
constraint
  (("aaaaa") ++ (B) ++ ("abaabaabaaaaaaabababaaaaa") ++ (B) ++ ("baababa") ++ (B) ++ ("ba") ++ (B))
    =
    (("aaaaa") ++ (B) ++ ("abaabaabaaaaaaabababaaaaa") ++ (B) ++ ("baababa") ++ (B) ++ ("ba") ++ (B));
constraint
  (("abaaaa") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("abbaabbbbbabbbb") ++ (B) ++ ("aaabaabbbabaaaab"))
    =
    (("abaaaabba") ++ (B) ++ ("a") ++ (B) ++ ("abbaabb") ++ (B) ++ ("ba") ++ (B) ++ ("bbbbaaabaabbbabaaaab"));
constraint
  (("baaa") ++ (B) ++ ("baaababababbbb") ++ (B) ++ (B) ++ (B) ++ ("aaaaaabaabaaabaaaaaaa"))
    =
    (("baaa") ++ (B) ++ ("baaabababa") ++ (B) ++ ("bbbb") ++ (B) ++ ("bbaaaaaabaabaaabaaaaaaa"));
constraint
  (("bbaabaababbaabbaabaaaaabaaabaa") ++ (B) ++ ("babb") ++ (B) ++ ("abbaabaaa"))
    = ((B) ++ ("aabaababbaabbaabaaaaabaaabaabbbabbbba") ++ (B) ++ ("aabaaa"));
constraint
  (("ababaaaaaababa") ++ (B) ++ ("ba") ++ (B) ++ ("baa") ++ (B) ++ ("aaabaabbaabb") ++ (B) ++ ("babaaaba"))
    =
    (("ababaaaaaababa") ++ (B) ++ ("ba") ++ (B) ++ ("baabbaaabaabbaa") ++ (B) ++ ("bbbabaaaba"));
constraint
  (("abbba") ++ (B) ++ ("baaaaaaaaba") ++ (B) ++ ("baababaabababaaa") ++ (B) ++ ("aaaa") ++ (B) ++ ("abb"))
    =
    (("abbbabbbaaaaaaaababbbaababaabababaaa") ++ (B) ++ ("aaaa") ++ (B) ++ ("a") ++ (B));
constraint
  (("abaaaaaaababb") ++ (B) ++ ("abbbabbbb") ++ (B) ++ ("aabb") ++ (B) ++ ("aababaabaabaa") ++ (B))
    =
    (("abaaaaaaaba") ++ (B) ++ ("bbabbbabbbb") ++ (B) ++ ("aa") ++ (B) ++ ("bbaababaabaabaabb"));
constraint
  (("baababbbabbbaabababb") ++ (B) ++ ("aaabaabbaa") ++ (B) ++ ("aabbbabbababb"))
    =
    (("baaba") ++ (B) ++ ("babbbaababa") ++ (B) ++ (B) ++ ("aaabaabbaabbaa") ++ (B) ++ ("ba") ++ (B) ++ ("aba") ++ (B));
constraint
  (("aaa") ++ (B) ++ ("ababbbaaababaaababbaababbaaa") ++ (B) ++ ("a") ++ (B) ++ ("babaaaabb"))
    = (("aaabbababbbaaababaaababbaaba") ++ (B) ++ ("aaabbabbbabaaaabb"));
constraint
  (("baabbbaabb") ++ (B) ++ ("baa") ++ (B) ++ (B) ++ ("ababbaaabaaaabba") ++ (B) ++ ("abaaa") ++ (B) ++ ("bba"))
    =
    (("baa") ++ (B) ++ ("baa") ++ (B) ++ (B) ++ ("baabbbbababbaaabaaaabba") ++ (B) ++ ("abaaa") ++ (B) ++ (B) ++ ("a"));
constraint
  (("baaaaba") ++ (B) ++ ("a") ++ (B) ++ ("bbbbbabaaaababbbabaabaaaaaa") ++ (B) ++ (B) ++ (B) ++ ("aa"))
    =
    (("baaaababba") ++ (B) ++ (B) ++ (B) ++ ("babaaaababbbabaabaaaaaabbbb") ++ (B) ++ ("aa"));
constraint
  (("ababaaaaaaaaabb") ++ (B) ++ ("bbbaaaaaaaabaababb") ++ (B) ++ ("aabbaabbbb"))
    =
    (("ababaaaaaaaaabbbb") ++ (B) ++ ("baaaaaaaabaaba") ++ (B) ++ ("bbaabbaabb") ++ (B));
constraint
  (("bbaaaaabbabbbaaaaa") ++ (B) ++ ("aaaaabba") ++ (B) ++ (B) ++ ("babbabbbaaaabab"))
    =
    ((B) ++ ("aaaaa") ++ (B) ++ ("abbbaaaaa") ++ (B) ++ ("aaaaabba") ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("abbbaaaabab"));
constraint
  (("aaaaba") ++ (B) ++ ("aa") ++ (B) ++ ("baabaaaaaba") ++ (B) ++ ("aabaaaaabb") ++ (B) ++ (B) ++ (B) ++ ("aabbbb"))
    =
    (("aaaaba") ++ (B) ++ ("aabbbaabaaaaaba") ++ (B) ++ ("aabaaaaabbbb") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B));
constraint
  (("ababa") ++ (B) ++ ("aa") ++ (B) ++ ("aabaa") ++ (B) ++ ("abaaabbbaababaaaabbbaa") ++ (B) ++ ("babab"))
    =
    (("ababa") ++ (B) ++ ("aabbaabaa") ++ (B) ++ ("abaaa") ++ (B) ++ ("baababaaaabbbaa") ++ (B) ++ ("babab"));
constraint
  (("abb") ++ (B) ++ ("abba") ++ (B) ++ ("aaaaaba") ++ (B) ++ ("aaaaaabbabaabaabaabbbaabaab"))
    =
    (("a") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("abbaaaaababbaaaaaa") ++ (B) ++ ("abaabaabaa") ++ (B) ++ ("baabaab"));
constraint
  (("aaaaba") ++ (B) ++ ("bba") ++ (B) ++ ("ababbababbbbbaba") ++ (B) ++ ("baabaaababbbaaba"))
    =
    (("aaaaba") ++ (B) ++ ("bba") ++ (B) ++ ("aba") ++ (B) ++ ("ababbbbbaba") ++ (B) ++ ("baabaaaba") ++ (B) ++ ("baaba"));
constraint
  ((B) ++ ("aaabaaabbaabba") ++ (B) ++ ("aaaaabbbaaabaabaaba") ++ (B) ++ (B) ++ ("baaaba"))
    =
    ((B) ++ ("aaabaaa") ++ (B) ++ ("aa") ++ (B) ++ ("abbaaaaabbbaaabaabaababbbbbaaaba"));
constraint
  (("baabbbaaaa") ++ (B) ++ ("aaaabababbba") ++ (B) ++ ("aaba") ++ (B) ++ ("babaaaabbaaaabb"))
    =
    (("baa") ++ (B) ++ ("baaaabbaaaabababbbabbaaba") ++ (B) ++ ("babaaaabbaaaabb"));
constraint
  ((B) ++ ("babb") ++ (B) ++ ("aa") ++ (B) ++ ("bbaabbaaaba") ++ (B) ++ ("bbabbabbbaaabbabbababb"))
    =
    (("bbba") ++ (B) ++ ("bbaa") ++ (B) ++ ("bbaa") ++ (B) ++ ("aaababbbba") ++ (B) ++ ("a") ++ (B) ++ ("baaa") ++ (B) ++ ("abbababb"));
constraint
  (("ababaaabaa") ++ (B) ++ (B) ++ ("baaaaababaaaaaaaa") ++ (B) ++ ("bbbaabaabaabbb"))
    =
    (("ababaaabaabb") ++ (B) ++ ("baaaaababaaaaaaaa") ++ (B) ++ (B) ++ ("baabaabaa") ++ (B) ++ ("b"));
constraint
  (("ababb") ++ (B) ++ ("abaaaaabbaaaaababb") ++ (B) ++ ("abaaba") ++ (B) ++ ("aabaaaabaaab"))
    = (("ababbbbabaaaaabbaaaaababbbbabaababbaabaaaabaaab") ++ (""));
constraint
  (("a") ++ (B) ++ ("baba") ++ (B) ++ ("baaababbbbbbababbbb") ++ (B) ++ ("ababbbbaaababbbbb"))
    =
    (("a") ++ (B) ++ ("bababbbaaababbbbbbaba") ++ (B) ++ (B) ++ ("bbaba") ++ (B) ++ ("bbaaababb") ++ (B) ++ ("b"));
constraint
  (("baabbbbbaaba") ++ (B) ++ ("aabaaababa") ++ (B) ++ ("baaabababa") ++ (B) ++ ("babbabbaa"))
    =
    (("baabb") ++ (B) ++ ("baababbaabaaababa") ++ (B) ++ ("baaabababa") ++ (B) ++ ("babba") ++ (B) ++ ("aa"));
constraint
  (("aaaaabbba") ++ (B) ++ ("abababb") ++ (B) ++ ("baaaabaaaaabaabaabbabbabaaa"))
    = (("aaaaabbbabbabababbbbbaaaabaaaaabaabaabba") ++ (B) ++ ("abaaa"));
constraint
  (("baabaababaabaaaa") ++ (B) ++ ("babaabbba") ++ (B) ++ ("aabbaababaaabbbbaa"))
    =
    (("baabaababaabaaaa") ++ (B) ++ ("babaabbbabbaabbaababaaa") ++ (B) ++ (B) ++ ("aa"));
constraint
  (("baa") ++ (B) ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("aaabbaa") ++ (B) ++ ("aaaaa") ++ (B) ++ ("aabaaaa") ++ (B) ++ ("baaaabbba"))
    =
    (("baabb") ++ (B) ++ (B) ++ ("babbaaabbaa") ++ (B) ++ ("aaaaabbaabaaaabbbaaaa") ++ (B) ++ ("ba"));
constraint
  (("bbbabaaabaaba") ++ (B) ++ ("bbaa") ++ (B) ++ ("abbabbba") ++ (B) ++ ("aaaabbababaaabab"))
    =
    ((B) ++ ("babaaabaaba") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("abbabbba") ++ (B) ++ ("aaaabbababaaabab"));
constraint
  (("aaa") ++ (B) ++ ("bbaaaababbaaaa") ++ (B) ++ (B) ++ ("aaababaa") ++ (B) ++ ("abb") ++ (B) ++ ("ababaabaa"))
    =
    (("aaabb") ++ (B) ++ ("aaaaba") ++ (B) ++ ("aaaabb") ++ (B) ++ ("aaababaabbabbbbababaabaa"));
constraint
  (("abbabaabbaaabbbaaabaaaabbbbbaaaaabb") ++ (B) ++ ("a") ++ (B) ++ ("aaaaaab"))
    =
    (("abbabaabbaaabbbaaabaaaabbbbbaaaaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aaaaaab"));
constraint
  (("babbaa") ++ (B) ++ ("bbba") ++ (B) ++ ("abababbaaaabb") ++ (B) ++ ("ba") ++ (B) ++ ("baaaa") ++ (B) ++ ("aa") ++ (B) ++ ("bbb"))
    =
    (("ba") ++ (B) ++ ("aabbbbba") ++ (B) ++ ("abababbaaaa") ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("baaaabbaa") ++ (B) ++ (B) ++ ("b"));
constraint
  ((B) ++ ("abaaaaaaaaa") ++ (B) ++ ("aabbabaaaaaaaabaabaaa") ++ (B) ++ ("bbbabaaaa"))
    =
    (("bbabaaaaaaaaa") ++ (B) ++ ("aa") ++ (B) ++ ("abaaaaaaaabaabaaa") ++ (B) ++ (B) ++ ("babaaaa"));
constraint
  (("aaa") ++ (B) ++ ("bbbbaababbbbabaabaaa") ++ (B) ++ ("abbabaaabbaaabaaaaba"))
    =
    (("aaabbbbbbaaba") ++ (B) ++ (B) ++ ("abaabaaa") ++ (B) ++ ("abbabaaa") ++ (B) ++ ("aaabaaaaba"));
constraint
  (("ababaababbbaba") ++ (B) ++ ("bb") ++ (B) ++ ("aaa") ++ (B) ++ ("babababbbabbaaabaabbba"))
    =
    (("ababaababbbaba") ++ (B) ++ ("bbbbaaabbbababa") ++ (B) ++ ("ba") ++ (B) ++ ("aaabaa") ++ (B) ++ ("ba"));
constraint
  (("baaababb") ++ (B) ++ ("baabaaaaa") ++ (B) ++ ("baaaaabaa") ++ (B) ++ ("babaaaaaaabbbaa"))
    =
    (("baaababbbbbaabaaaaa") ++ (B) ++ ("baaaaabaa") ++ (B) ++ ("babaaaaaaa") ++ (B) ++ ("baa"));
constraint
  (("baabbaaaaaaabaabb") ++ (B) ++ ("aaabaabaabb") ++ (B) ++ ("baaaabbabbbabab"))
    =
    (("baabbaaaaaaabaabbbbaaabaabaa") ++ (B) ++ (B) ++ ("baaaabba") ++ (B) ++ ("babab"));
constraint
  ((B) ++ ("abaa") ++ (B) ++ ("bbbaaabbaa") ++ (B) ++ ("bbaabababaaabb") ++ (B) ++ ("abbabb") ++ (B) ++ (B) ++ ("a"))
    =
    (("bbabaa") ++ (B) ++ ("bbbaaabbaa") ++ (B) ++ ("bbaabababaaa") ++ (B) ++ (B) ++ ("abbabb") ++ (B) ++ ("bba"));
constraint
  (("a") ++ (B) ++ ("baaaabbabbbabababbbaaaaabaabaaabaabbbabb") ++ (B) ++ ("ba"))
    =
    (("a") ++ (B) ++ ("baaaa") ++ (B) ++ ("abbbababa") ++ (B) ++ ("baaaaabaabaaabaabbba") ++ (B) ++ (B) ++ ("ba"));
constraint
  (("aaaaaaabbbbaababbbbaaaaaabaa") ++ (B) ++ ("aaaa") ++ (B) ++ ("babaabaabaa"))
    =
    (("aaaaaaa") ++ (B) ++ (B) ++ ("aaba") ++ (B) ++ (B) ++ ("aaaaaabaa") ++ (B) ++ ("aaaabbbabaabaabaa"));
constraint
  (("babaaaa") ++ (B) ++ (B) ++ (B) ++ ("aaaaaaaabababaaaa") ++ (B) ++ ("abaabbba") ++ (B) ++ ("bbbaa"))
    =
    (("babaaaa") ++ (B) ++ ("bb") ++ (B) ++ ("aaaaaaaabababaaaa") ++ (B) ++ ("abaabbba") ++ (B) ++ (B) ++ ("baa"));
constraint
  ((B) ++ ("aaaaabaa") ++ (B) ++ (B) ++ (B) ++ ("babbabb") ++ (B) ++ ("baa") ++ (B) ++ ("baa") ++ (B) ++ ("aaaaabbbbabb"))
    =
    (("bbaaaaabaabbbb") ++ (B) ++ ("ba") ++ (B) ++ ("abb") ++ (B) ++ ("baa") ++ (B) ++ ("baa") ++ (B) ++ ("aaaaa") ++ (B) ++ (B) ++ ("a") ++ (B));
constraint
  (("aabba") ++ (B) ++ ("baabaa") ++ (B) ++ (B) ++ ("aabaaabbaababaabbbaabaaa") ++ (B) ++ ("baba"))
    =
    (("aa") ++ (B) ++ ("abbbaabaabbbbaabaaabbaababaa") ++ (B) ++ ("baabaaabbbaba"));
constraint
  (("aaababbababaaabbaa") ++ (B) ++ ("abbbabba") ++ (B) ++ ("aaaaaabbbabbbaaaa"))
    =
    (("aaaba") ++ (B) ++ ("ababaaabbaa") ++ (B) ++ ("a") ++ (B) ++ ("babbabbaaaaaabbbabbbaaaa"));
constraint
  (("a") ++ (B) ++ ("babababaaaaa") ++ (B) ++ ("a") ++ (B) ++ ("bababbabaa") ++ (B) ++ ("baababbababbbbb"))
    =
    (("a") ++ (B) ++ ("babababaaaaa") ++ (B) ++ ("abbbaba") ++ (B) ++ ("abaa") ++ (B) ++ ("baababbaba") ++ (B) ++ ("bbb"));
constraint
  (("a") ++ (B) ++ ("aabaaaaaaababaaaa") ++ (B) ++ ("babaaaaabbaa") ++ (B) ++ ("bbaaababbbb"))
    =
    (("abbaabaaaaaaababaaaabbbabaaaaa") ++ (B) ++ ("aa") ++ (B) ++ ("bbaaababb") ++ (B));
constraint
  (("aaaabaaaababbaaababaabaaabbbbabbbaaaabaababaaba") ++ ("")) =
    (("aaaabaaaababbaaababaabaaa") ++ (B) ++ ("bba") ++ (B) ++ ("baaaabaababaaba"));
constraint
  (("abbbbabaabaa") ++ (B) ++ ("ababaaabaa") ++ (B) ++ ("aaabaaba") ++ (B) ++ (B) ++ ("bb") ++ (B) ++ ("a") ++ (B) ++ ("ab"))
    =
    (("abbbbabaabaabbababaaabaa") ++ (B) ++ ("aaabaababb") ++ (B) ++ (B) ++ ("bba") ++ (B) ++ ("ab"));
constraint
  (("aaabbaaaaabbbbaa") ++ (B) ++ ("aaabaaaaaabaa") ++ (B) ++ ("abaaabbabaaaaa"))
    =
    (("aaa") ++ (B) ++ ("aaaaa") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("aaabaaaaaabaa") ++ (B) ++ ("abaaabbabaaaaa"));
constraint
  (("aabaaaa") ++ (B) ++ ("aaaabbaba") ++ (B) ++ (B) ++ ("abaa") ++ (B) ++ ("bbabbbbababaabbaaba"))
    =
    (("aabaaaabbaaaa") ++ (B) ++ ("ababbbbabaabbbba") ++ (B) ++ (B) ++ ("ababaa") ++ (B) ++ ("aaba"));
constraint
  (("abaaabababbbababaabaa") ++ (B) ++ ("baaaaabbabaabbabbaaaabaa")) =
    (("abaaabababbbababaabaa") ++ (B) ++ ("baaaaa") ++ (B) ++ ("abaa") ++ (B) ++ ("a") ++ (B) ++ ("aaaabaa"));
constraint
  (("bbbabaaaaaaabaa") ++ (B) ++ ("aa") ++ (B) ++ ("abaaaba") ++ (B) ++ ("aaabb") ++ (B) ++ ("a") ++ (B) ++ ("bbaaabb"))
    =
    (("bbbabaaaaaaabaa") ++ (B) ++ ("aabbabaaaba") ++ (B) ++ ("aaa") ++ (B) ++ ("bbabbbbaaabb"));
constraint
  (("aabaaabaaabaabaabbaabaa") ++ (B) ++ ("aaaaa") ++ (B) ++ ("aabaaba") ++ (B) ++ ("bb") ++ (B) ++ ("aa"))
    =
    (("aabaaabaaabaabaa") ++ (B) ++ ("aabaabbaaaaabbaabaaba") ++ (B) ++ (B) ++ ("bbaa"));
constraint
  ((B) ++ (B) ++ ("ba") ++ (B) ++ ("aabbabbaaaaaaa") ++ (B) ++ ("aaaaaabbbababababa") ++ (B) ++ ("aab"))
    =
    ((B) ++ (B) ++ ("ba") ++ (B) ++ ("aabba") ++ (B) ++ ("aaaaaaa") ++ (B) ++ ("aaaaaa") ++ (B) ++ ("bababababa") ++ (B) ++ ("aab"));
constraint
  (("babaaa") ++ (B) ++ ("bb") ++ (B) ++ (B) ++ ("abaaabaaababbabbba") ++ (B) ++ ("ba") ++ (B) ++ ("babababba"))
    =
    (("babaaabbbbbbbbabaaabaaababbabbba") ++ (B) ++ ("babbbababa") ++ (B) ++ ("a"));
constraint
  (("baaa") ++ (B) ++ ("baa") ++ (B) ++ ("baaaaba") ++ (B) ++ ("aaaba") ++ (B) ++ ("abbba") ++ (B) ++ ("bbbbaa") ++ (B) ++ (B) ++ ("aab"))
    =
    (("baaa") ++ (B) ++ ("baa") ++ (B) ++ ("baaaababbaaaba") ++ (B) ++ ("abbbabbbb") ++ (B) ++ ("aabb") ++ (B) ++ ("aab"));
constraint
  (("baa") ++ (B) ++ ("aaabbbaa") ++ (B) ++ ("bbbabbaabaaababaaabb") ++ (B) ++ ("baaabaabab"))
    =
    (("baa") ++ (B) ++ ("aaa") ++ (B) ++ ("baa") ++ (B) ++ ("bbba") ++ (B) ++ ("aabaaababaaa") ++ (B) ++ (B) ++ ("baaabaabab"));
constraint
  (("aabaaaa") ++ (B) ++ ("a") ++ (B) ++ ("baa") ++ (B) ++ ("bba") ++ (B) ++ ("a") ++ (B) ++ ("babaaaaa") ++ (B) ++ ("bbaaababbbaa"))
    =
    (("aabaaaabba") ++ (B) ++ ("baabb") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("babaaaaa") ++ (B) ++ (B) ++ ("aaababbbaa"));
constraint
  (("abaabbabbabababaaba") ++ (B) ++ ("aaabaaaba") ++ (B) ++ (B) ++ ("abaaabababbab"))
    =
    (("abaabbabbabababaaba") ++ (B) ++ ("aaabaaaba") ++ (B) ++ ("bbabaaababa") ++ (B) ++ ("ab"));
constraint
  (("abba") ++ (B) ++ ("baa") ++ (B) ++ ("abbbaba") ++ (B) ++ ("aba") ++ (B) ++ ("a") ++ (B) ++ ("baaaaaba") ++ (B) ++ ("baabbbaab"))
    =
    (("a") ++ (B) ++ ("abbbaabbabbbababbababbabbbaaaaaba") ++ (B) ++ ("baa") ++ (B) ++ ("baab"));
constraint
  ((B) ++ ("baabbaa") ++ (B) ++ ("abaabaababb") ++ (B) ++ (B) ++ ("baabbabaaaa") ++ (B) ++ (B) ++ ("baabaa"))
    =
    (("bbbaabbaa") ++ (B) ++ ("abaabaababb") ++ (B) ++ ("bbbaa") ++ (B) ++ ("abaaaabb") ++ (B) ++ ("baabaa"));
constraint
  ((B) ++ ("a") ++ (B) ++ ("aabbaaaabba") ++ (B) ++ ("a") ++ (B) ++ ("bbaaababbaaaa") ++ (B) ++ ("abba") ++ (B) ++ ("babaa"))
    =
    (("bbabbaabbaaaabba") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aaaba") ++ (B) ++ ("aaaa") ++ (B) ++ ("a") ++ (B) ++ ("abbbabaa"));
constraint
  (("bbbaabaaaabbbaababaaaabbabababababbabbbabaababa") ++ ("")) =
    (("bbbaabaaaabbbaababaaaa") ++ (B) ++ ("abababababbabbbabaababa"));
constraint
  (("bababaaaaaaaaabbaaaabb") ++ (B) ++ (B) ++ ("baa") ++ (B) ++ ("aababaabaaaabbaa"))
    =
    (("bababaaaaaaaaa") ++ (B) ++ ("aaaa") ++ (B) ++ (B) ++ (B) ++ ("baabbaababaabaaaa") ++ (B) ++ ("aa"));
constraint
  (("aabbbaaabbaabaabaaabbabababbba") ++ (B) ++ ("abbbbbabaaa") ++ (B) ++ ("ba"))
    =
    (("aa") ++ (B) ++ ("baaabbaabaabaaabbababa") ++ (B) ++ ("babbabb") ++ (B) ++ ("babaaa") ++ (B) ++ ("ba"));
constraint
  (("bababaaaaaa") ++ (B) ++ ("ba") ++ (B) ++ ("aaaaa") ++ (B) ++ ("aabaababbbaaabbbaaababa"))
    = (("bababaaaaaabbbabbaaaaabbaabaaba") ++ (B) ++ ("baaabbbaaababa"));
constraint
  (("aaababa") ++ (B) ++ ("aabaaababaaaabaaba") ++ (B) ++ ("aaaaaababbaaaaaabb"))
    = (("aaababa") ++ (B) ++ ("aabaaababaaaabaababbaaaaaababbaaaaaa") ++ (B));
constraint
  ((B) ++ ("bbababa") ++ (B) ++ ("baaaaaaaabbbbababaabababbbabbbbaaa") ++ (B))
    =
    (("bbbbabababbbaaaaaaaa") ++ (B) ++ ("bbababaababa") ++ (B) ++ ("babbbbaaa") ++ (B));
constraint
  (("aaaababbaaabaaaba") ++ (B) ++ ("abbaabaabaabbbbbaabaababbbbb")) =
    (("aaaababbaaabaaababba") ++ (B) ++ ("aabaabaabbbbbaabaaba") ++ (B) ++ ("bbb"));
constraint
  (("a") ++ (B) ++ ("abaaababaaaba") ++ (B) ++ ("aaaabbbbabaabaaaabaababb") ++ (B) ++ ("aba"))
    =
    (("abbabaaababaaababbaaaabb") ++ (B) ++ ("abaabaaaabaababb") ++ (B) ++ ("aba"));
constraint
  (("baaba") ++ (B) ++ (B) ++ ("abbbaba") ++ (B) ++ ("babaaa") ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ ("aaaababbaaba") ++ (B))
    =
    (("baababbbbabbbababbbabaaa") ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ ("aaaababbaababb"));
constraint
  (("aabaaba") ++ (B) ++ ("aaaa") ++ (B) ++ ("baa") ++ (B) ++ ("baaaaaababaabaaabb") ++ (B) ++ ("babaaba"))
    =
    (("aabaababbaaaa") ++ (B) ++ ("baa") ++ (B) ++ ("baaaaaababaabaaabbbbbabaaba"));
constraint
  (("a") ++ (B) ++ ("baabbabaabbbabababbabababaaaa") ++ (B) ++ ("bababaaabaaaa"))
    = (("abbbaabbabaabbbababa") ++ (B) ++ ("abababaaaabbbababaaabaaaa"));
constraint
  (("ba") ++ (B) ++ ("ababb") ++ (B) ++ ("aabaaaabaaaaa") ++ (B) ++ ("bb") ++ (B) ++ ("aaaaaaa") ++ (B) ++ ("bbaaba") ++ (B))
    =
    (("ba") ++ (B) ++ ("aba") ++ (B) ++ (B) ++ ("aabaaaabaaaaabbbb") ++ (B) ++ ("aaaaaaa") ++ (B) ++ ("bbaaba") ++ (B));
constraint
  ((B) ++ ("abaaabbabaa") ++ (B) ++ ("bbbaaaa") ++ (B) ++ ("bababbababaaaa") ++ (B) ++ ("bbaabaa"))
    =
    ((B) ++ ("abaaa") ++ (B) ++ ("abaa") ++ (B) ++ ("bbbaaaabbbaba") ++ (B) ++ ("ababaaaa") ++ (B) ++ (B) ++ ("aabaa"));
constraint
  (("bb") ++ (B) ++ (B) ++ ("baa") ++ (B) ++ ("bbbabaaaba") ++ (B) ++ ("bb") ++ (B) ++ (B) ++ ("babaabaabaaa") ++ (B) ++ ("abab"))
    =
    ((B) ++ ("bb") ++ (B) ++ ("baa") ++ (B) ++ ("bbbabaaababb") ++ (B) ++ ("bb") ++ (B) ++ ("babaabaabaaabbabab"));
constraint
  ((B) ++ ("baa") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("abbaabaaabaaba") ++ (B) ++ ("ababbaaabba") ++ (B) ++ (B) ++ ("a"))
    =
    (("bbbaabbbbaabbbba") ++ (B) ++ ("aabaaabaababbaba") ++ (B) ++ ("aaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a"));
constraint
  (("aaa") ++ (B) ++ ("aa") ++ (B) ++ ("aba") ++ (B) ++ ("abbbb") ++ (B) ++ ("baaabbbabaabaaa") ++ (B) ++ ("baabbbbaa"))
    =
    (("aaa") ++ (B) ++ ("aabbababbabb") ++ (B) ++ (B) ++ ("baaabbbabaabaaa") ++ (B) ++ ("baa") ++ (B) ++ (B) ++ ("aa"));
constraint
  (("aabbaaba") ++ (B) ++ ("ababbabbaaa") ++ (B) ++ (B) ++ ("bba") ++ (B) ++ ("abbaababaaabaaaaa"))
    =
    (("aabbaaba") ++ (B) ++ ("aba") ++ (B) ++ ("a") ++ (B) ++ ("aaabb") ++ (B) ++ ("bbabbabbaababaaabaaaaa"));
constraint
  (("aababaabbaabbaba") ++ (B) ++ ("abbbaabbbbabaabb") ++ (B) ++ ("bbbbbaa") ++ (B) ++ ("ba"))
    =
    (("aababaa") ++ (B) ++ ("aabbaba") ++ (B) ++ ("a") ++ (B) ++ ("baabb") ++ (B) ++ ("abaa") ++ (B) ++ (B) ++ ("bbbbbaa") ++ (B) ++ ("ba"));
constraint
  (("a") ++ (B) ++ ("bbbabaaaaaaaaaaabaaabbbaababba") ++ (B) ++ ("babbbabaabab"))
    =
    (("abbbbbabaaaaaaaaaaabaaa") ++ (B) ++ ("baababba") ++ (B) ++ ("ba") ++ (B) ++ ("babaabab"));
constraint
  (("baa") ++ (B) ++ ("bb") ++ (B) ++ ("baaaa") ++ (B) ++ ("bbaabaaa") ++ (B) ++ ("ababababababbbaabaaab"))
    =
    (("baabbbb") ++ (B) ++ ("baaaa") ++ (B) ++ (B) ++ ("aabaaa") ++ (B) ++ ("abababababa") ++ (B) ++ ("baabaaab"));
constraint
  (("a") ++ (B) ++ ("baaa") ++ (B) ++ ("bbaaaababaaaaabaabaaaba") ++ (B) ++ ("aa") ++ (B) ++ ("aaaa") ++ (B) ++ ("a") ++ (B))
    =
    (("abbbaaa") ++ (B) ++ ("bbaaaababaaaaabaabaaaba") ++ (B) ++ ("aabbaaaabbabb"));
constraint
  (("abaabaababaaaa") ++ (B) ++ ("abaaabb") ++ (B) ++ ("bababaa") ++ (B) ++ ("aababbbbaa") ++ (B) ++ ("b"))
    =
    (("abaabaababaaaa") ++ (B) ++ ("abaaa") ++ (B) ++ ("bbbababaabbaababbbbaa") ++ (B) ++ ("b"));
constraint
  (("abababaa") ++ (B) ++ ("aaabbabbaaabaaaaaaaabaaaba") ++ (B) ++ (B) ++ ("aaaabaa"))
    =
    (("abababaa") ++ (B) ++ ("aaa") ++ (B) ++ ("abbaaabaaaaaaaabaaaba") ++ (B) ++ ("bbaaaabaa"));
constraint
  (("bbaaaaabaaabaa") ++ (B) ++ ("aaaabbabaaabaa") ++ (B) ++ ("babaaaaaabbbaaa"))
    = ((B) ++ ("aaaaabaaabaabbaaaabbabaaabaa") ++ (B) ++ ("babaaaaaabbbaaa"));
constraint
  (("babbbabaaabba") ++ (B) ++ ("bbbbaaaaaabb") ++ (B) ++ ("baabb") ++ (B) ++ ("bbbbbaa") ++ (B) ++ ("aa"))
    =
    (("ba") ++ (B) ++ ("babaaabbabb") ++ (B) ++ (B) ++ ("aaaaaabbbbbaa") ++ (B) ++ ("bbbbbbbaabbaa"));
constraint
  (("aaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("baaaba") ++ (B) ++ ("bababaa") ++ (B) ++ ("aabbbaa") ++ (B) ++ ("bbbaababa"))
    =
    (("aaabbaaa") ++ (B) ++ ("bbbaaababbbababaabbaabbbaa") ++ (B) ++ (B) ++ ("baababa"));
constraint
  (("abaa") ++ (B) ++ ("abaabababbbaaaabbaabaaaba") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aabaaab"))
    =
    (("abaabbabaabababbbaaaa") ++ (B) ++ ("aabaaababbbba") ++ (B) ++ ("bbaabaaab"));
constraint
  (("aaaababaaa") ++ (B) ++ ("abb") ++ (B) ++ ("bbbbaa") ++ (B) ++ (B) ++ ("aaabaabbaabbabababaa"))
    =
    (("aaaababaaa") ++ (B) ++ ("a") ++ (B) ++ ("bbbb") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("aaabaa") ++ (B) ++ ("aabbabababaa"));
constraint
  (("baa") ++ (B) ++ ("bba") ++ (B) ++ ("bababaa") ++ (B) ++ ("aabaabaaabababa") ++ (B) ++ ("aba") ++ (B) ++ ("a") ++ (B) ++ ("baa"))
    =
    (("baabbbba") ++ (B) ++ ("bababaabbaabaabaaabababa") ++ (B) ++ ("ababbabbbaa"));
constraint
  (("aabaaaaaababbabba") ++ (B) ++ ("baabaabbbaaaabaaababaaababab")) =
    (("aabaaaaaaba") ++ (B) ++ ("abbabbbaabaa") ++ (B) ++ ("baaaabaaababaaababab"));
constraint
  (("abaaaabaaabaabaabaabaaaabbabbbb") ++ (B) ++ ("baa") ++ (B) ++ ("aaaaba") ++ (B) ++ ("b"))
    =
    (("abaaaabaaabaabaabaabaaaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("bbbaa") ++ (B) ++ ("aaaaba") ++ (B) ++ ("b"));
constraint
  (("bba") ++ (B) ++ ("baa") ++ (B) ++ ("bbaaaabaa") ++ (B) ++ ("ababbababaa") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("abbaaaa"))
    =
    ((B) ++ ("a") ++ (B) ++ ("baabbbbaaaabaabbaba") ++ (B) ++ ("ababaabbba") ++ (B) ++ ("bbabbaaaa"));
constraint
  (("bb") ++ (B) ++ (B) ++ ("bbba") ++ (B) ++ (B) ++ ("baababbbbaabbaaaaba") ++ (B) ++ ("bbbbaaa") ++ (B) ++ ("aba"))
    =
    (("bbbbbbbbba") ++ (B) ++ (B) ++ ("baababbbbaabbaaaababbbb") ++ (B) ++ ("aaabbaba"));
solve satisfy;
