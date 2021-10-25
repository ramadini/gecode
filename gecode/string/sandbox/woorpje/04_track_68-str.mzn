var string: A;
constraint
  (("baaababbbabaababbbbbaabababbaaaaababbababbbbbbaaababaaababaababbbbbaabba") ++ (""))
    = (("baaababbbabaa") ++ (A) ++ ("bba"));
constraint
  (("bababaabbaabbbabbababaabaabbababbabbbaaaaaaabaabbabbbaaababbbaaababbabbb") ++ (""))
    = (("babab") ++ (A) ++ ("aaaaabaabbabbbaaababbbaaababbabbb"));
constraint
  (("aaabaabbbaaab") ++ (A) ++ ("bbbabbbbabaabbaaaabbbbaababbbababaabbbaabbbbb"))
    =
    (("aaabaabbbaaab") ++ (A) ++ ("bbbabbbbabaabbaaaabbbbaababbbababaabbbaabbbbb"));
constraint
  (("bbbbbbbaabaaabbbbabaaaa") ++ (A) ++ ("baaaabaaababbbbbabbbbbaaaaaaaaababa"))
    =
    (("bbbbbbbaabaaabbbbabaaaa") ++ (A) ++ ("baaaabaaababbbbbabbbbbaaaaaaaaababa"));
constraint
  (("baaaaabaabbbbaababababaababbbbbbaaabaaabaa") ++ (A) ++ ("baabbaabbbaaabaa"))
    =
    (("baaaaabaabbbbaababababaababbbbbbaaabaaabaaabbbbaaaabaabbbaabbaabbbaaabaa") ++ (""));
constraint
  (("babaaabbbbabbbaaabbbbababaabbabbababaaaabbababaaaaaaaba") ++ (A) ++ ("baa"))
    =
    (("babaaabbbbabbbaaabbbbababaabbabbababaaaabbababaaaaaaabaabbbbaaaabaabbbaa") ++ (""));
constraint
  (("baabaabbbabba") ++ (A) ++ ("abaaaaaaabaabbbaabbbabba")) =
    (("baabaabbbabba") ++ (A) ++ ("abaaaaaaabaabbbaabbbabba"));
constraint
  (("bbb") ++ (A) ++ ("babaa")) =
    (("bbbaaaababaaaabaabbbbbababaabbbbaaabababababbabbbaababaaaaababbaaabbabaa") ++ (""));
constraint
  (("abaababbbaabbabbbabbab") ++ (A) ++ ("babbbabbbabbaababbabababbaaaaaaabb"))
    =
    (("abaababbbaabbabbbabbabbbbaabbbababbbabbabbbabbbabbaababbabababbaaaaaaabb") ++ (""));
constraint
  (("baa") ++ (A) ++ ("bbaaaabb") ++ (A) ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("aa") ++ (A) ++ ("aaaaab") ++ (A) ++ ("aaaaaba") ++ (A) ++ ("abbbabbaaba") ++ (A) ++ ("bbbaaaaabaaab") ++ (A) ++ ("babaabaaa") ++ (A) ++ ("b"))
    =
    (("baabb") ++ (A) ++ ("aaaa") ++ (A) ++ ("b") ++ (A) ++ ("b") ++ (A) ++ ("a") ++ (A) ++ ("aabaaaaabbaaaaabababb") ++ (A) ++ ("a") ++ (A) ++ ("baa") ++ (A) ++ ("a") ++ (A) ++ ("b") ++ (A) ++ ("baaaaabaaab") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("aa") ++ (A) ++ ("aaa") ++ (A) ++ ("b"));
constraint
  ((A) ++ ("b") ++ (A) ++ (A) ++ ("aba") ++ (A) ++ ("abba") ++ (A) ++ ("baabaa") ++ (A) ++ ("aab") ++ (A) ++ ("baab") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("bbbba") ++ (A) ++ (A) ++ ("aaabbaaa") ++ (A) ++ (A) ++ ("aaaa") ++ (A) ++ ("abaaaaabbbababaaba"))
    =
    (("bb") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("abba") ++ (A) ++ (A) ++ ("aabaabaa") ++ (A) ++ ("b") ++ (A) ++ ("aa") ++ (A) ++ ("ba") ++ (A) ++ ("b") ++ (A) ++ ("b") ++ (A) ++ ("bab") ++ (A) ++ ("aaab") ++ (A) ++ ("aaa") ++ (A) ++ (A) ++ ("aaaa") ++ (A) ++ ("abaaaaa") ++ (A) ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("aaba"));
constraint
  (("aabaa") ++ (A) ++ (A) ++ ("aaba") ++ (A) ++ ("a") ++ (A) ++ ("ba") ++ (A) ++ ("a") ++ (A) ++ ("bb") ++ (A) ++ ("aaba") ++ (A) ++ (A) ++ (A) ++ (A) ++ ("baa") ++ (A) ++ ("ba") ++ (A) ++ ("ba") ++ (A) ++ (A) ++ ("aaabaab") ++ (A) ++ ("aaaaaabaaa") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ (A) ++ (A) ++ ("aa") ++ (A) ++ ("abb"))
    =
    (("aa") ++ (A) ++ ("aab") ++ (A) ++ ("aababa") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("ab") ++ (A) ++ (A) ++ ("baa") ++ (A) ++ ("abb") ++ (A) ++ ("bbaabbab") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("aaabaa") ++ (A) ++ ("baaaaaabaaaba") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("baa") ++ (A) ++ ("abb"));
constraint
  (("bb") ++ (A) ++ ("aba") ++ (A) ++ (A) ++ ("baa") ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("aaba") ++ (A) ++ (A) ++ ("aabaaaaaaaaab") ++ (A) ++ ("b") ++ (A) ++ ("baaabaa") ++ (A) ++ ("ba") ++ (A) ++ (A) ++ ("aaaa") ++ (A) ++ ("aa") ++ (A) ++ ("ab") ++ (A) ++ ("baa") ++ (A) ++ ("bbb") ++ (A) ++ ("aa") ++ (A) ++ ("b"))
    =
    ((A) ++ ("bba") ++ (A) ++ ("a") ++ (A) ++ (A) ++ (A) ++ ("aa") ++ (A) ++ ("bbbaabab") ++ (A) ++ ("aa") ++ (A) ++ ("aaaaaaaaa") ++ (A) ++ ("bb") ++ (A) ++ (A) ++ ("aaa") ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("abbaaaa") ++ (A) ++ ("aaba") ++ (A) ++ ("bbaa") ++ (A) ++ ("bbbbaabb"));
constraint
  (("ba") ++ (A) ++ ("ba") ++ (A) ++ (A) ++ ("baa") ++ (A) ++ ("baaaba") ++ (A) ++ ("aaaaa") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("ababa") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("abba") ++ (A) ++ ("baa") ++ (A) ++ ("b") ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("bbaaaa") ++ (A) ++ ("abaaa") ++ (A) ++ ("aabb"))
    =
    (("babbabb") ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("aaaba") ++ (A) ++ ("aaaaaba") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("ab") ++ (A) ++ ("ababa") ++ (A) ++ ("a") ++ (A) ++ ("bab") ++ (A) ++ ("aab") ++ (A) ++ (A) ++ ("aabb") ++ (A) ++ ("baaaaba") ++ (A) ++ ("aaabaa") ++ (A) ++ ("b"));
constraint
  (("aaab") ++ (A) ++ (A) ++ ("ab") ++ (A) ++ ("baabb") ++ (A) ++ ("bb") ++ (A) ++ ("aaaaa") ++ (A) ++ (A) ++ ("baaa") ++ (A) ++ ("aaaaab") ++ (A) ++ ("babaaaba") ++ (A) ++ ("aab") ++ (A) ++ ("aabb") ++ (A) ++ ("abbbbabbaaa") ++ (A) ++ (A) ++ (A) ++ ("aba"))
    =
    (("aaa") ++ (A) ++ ("b") ++ (A) ++ ("ab") ++ (A) ++ ("baab") ++ (A) ++ ("bbb") ++ (A) ++ ("aaaaab") ++ (A) ++ ("baaa") ++ (A) ++ ("aaaaabb") ++ (A) ++ ("a") ++ (A) ++ ("aaa") ++ (A) ++ ("abaab") ++ (A) ++ ("aabbba") ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("a") ++ (A) ++ ("baaa") ++ (A) ++ (A) ++ (A) ++ ("aba"));
constraint
  (("ba") ++ (A) ++ ("b") ++ (A) ++ ("bb") ++ (A) ++ ("a") ++ (A) ++ ("aaab") ++ (A) ++ ("aaaa") ++ (A) ++ ("aa") ++ (A) ++ ("a") ++ (A) ++ ("ababaaaba") ++ (A) ++ (A) ++ ("aaabaaaabbab") ++ (A) ++ ("aa") ++ (A) ++ ("abaababa") ++ (A) ++ (A) ++ ("baa") ++ (A) ++ ("aababb"))
    =
    ((A) ++ ("ab") ++ (A) ++ ("b") ++ (A) ++ ("b") ++ (A) ++ ("a") ++ (A) ++ ("aaa") ++ (A) ++ (A) ++ ("aaaa") ++ (A) ++ ("aaba") ++ (A) ++ ("aba") ++ (A) ++ ("aaaba") ++ (A) ++ (A) ++ ("aaabaaaa") ++ (A) ++ ("babbaa") ++ (A) ++ ("abaababa") ++ (A) ++ ("bbaa") ++ (A) ++ ("aabab") ++ (A));
constraint
  ((A) ++ ("a") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("b") ++ (A) ++ ("a") ++ (A) ++ ("baa") ++ (A) ++ (A) ++ ("aba") ++ (A) ++ (A) ++ ("aba") ++ (A) ++ ("aaaabaaaaaaabbaa") ++ (A) ++ ("abbaaaabaaaaaabaab") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("aababaaaa"))
    =
    (("bab") ++ (A) ++ ("abbba") ++ (A) ++ ("baab") ++ (A) ++ ("abab") ++ (A) ++ ("a") ++ (A) ++ ("abaaaabaaaaaaabbaabab") ++ (A) ++ ("aaaabaaaaaa") ++ (A) ++ ("aabb") ++ (A) ++ ("abaa") ++ (A) ++ ("a") ++ (A) ++ ("aaaa"));
constraint
  (("baaaa") ++ (A) ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("aaaa") ++ (A) ++ ("baabaab") ++ (A) ++ (A) ++ ("baaaa") ++ (A) ++ ("baa") ++ (A) ++ ("aaaba") ++ (A) ++ ("aaaabbbaaaaa") ++ (A) ++ ("aaabb") ++ (A) ++ ("ab") ++ (A) ++ ("abaa") ++ (A) ++ (A) ++ ("aaa"))
    =
    ((A) ++ ("aaaa") ++ (A) ++ (A) ++ ("aab") ++ (A) ++ ("aaaa") ++ (A) ++ ("baabaa") ++ (A) ++ ("b") ++ (A) ++ ("baaaa") ++ (A) ++ ("baa") ++ (A) ++ ("aaababaaaa") ++ (A) ++ ("b") ++ (A) ++ ("aaaaabaaab") ++ (A) ++ ("ba") ++ (A) ++ (A) ++ ("abaa") ++ (A) ++ ("baaa"));
constraint
  (("bb") ++ (A) ++ ("aaabaab") ++ (A) ++ ("a") ++ (A) ++ ("baa") ++ (A) ++ ("ba") ++ (A) ++ ("a") ++ (A) ++ ("b") ++ (A) ++ (A) ++ (A) ++ ("baaaabb") ++ (A) ++ ("ab") ++ (A) ++ (A) ++ ("bb") ++ (A) ++ (A) ++ ("aab") ++ (A) ++ ("baa") ++ (A) ++ (A) ++ ("aabb") ++ (A) ++ ("aaaab") ++ (A) ++ ("bba") ++ (A) ++ ("a") ++ (A) ++ ("baa") ++ (A))
    =
    (("b") ++ (A) ++ ("baaa") ++ (A) ++ ("aab") ++ (A) ++ ("ab") ++ (A) ++ ("aab") ++ (A) ++ ("a") ++ (A) ++ ("abbb") ++ (A) ++ (A) ++ (A) ++ ("aaaa") ++ (A) ++ ("bba") ++ (A) ++ ("bbbbb") ++ (A) ++ ("aa") ++ (A) ++ (A) ++ (A) ++ ("aa") ++ (A) ++ ("baab") ++ (A) ++ (A) ++ ("aaaab") ++ (A) ++ (A) ++ ("baba") ++ (A) ++ (A) ++ ("aa") ++ (A));
constraint
  (("ababb") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ (A) ++ (A) ++ ("bbbaab") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("bb") ++ (A) ++ ("b") ++ (A) ++ ("b") ++ (A) ++ (A) ++ ("aa") ++ (A) ++ ("aaabba") ++ (A) ++ (A) ++ (A) ++ (A) ++ ("bab") ++ (A) ++ ("baa") ++ (A) ++ ("bbaaa") ++ (A) ++ ("aa") ++ (A) ++ ("ba") ++ (A) ++ ("a") ++ (A) ++ ("abaa") ++ (A) ++ ("a"))
    =
    (("aba") ++ (A) ++ ("bbbababb") ++ (A) ++ ("bbbaab") ++ (A) ++ ("abb") ++ (A) ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("bb") ++ (A) ++ ("aabaaab") ++ (A) ++ ("a") ++ (A) ++ ("bbb") ++ (A) ++ ("a") ++ (A) ++ ("bbaab") ++ (A) ++ ("baaabaa") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("abaa") ++ (A) ++ ("a"));
constraint
  (("baa") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("aaabaaabb") ++ (A) ++ (A) ++ (A) ++ ("abaab") ++ (A) ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("bbaa") ++ (A) ++ ("a") ++ (A) ++ ("abaa") ++ (A) ++ ("aabbbaab") ++ (A) ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("ababaaa") ++ (A) ++ ("aaaaaa"))
    =
    (("baa") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("ba") ++ (A) ++ ("aaabaaa") ++ (A) ++ (A) ++ ("bb") ++ (A) ++ ("a") ++ (A) ++ ("aa") ++ (A) ++ ("b") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("aaba") ++ (A) ++ ("a") ++ (A) ++ ("aabaa") ++ (A) ++ ("b") ++ (A) ++ ("aabb") ++ (A) ++ ("a") ++ (A) ++ ("ba") ++ (A) ++ ("a") ++ (A) ++ ("aaabaaaaaa"));
constraint
  (("aba") ++ (A) ++ ("abaaababbaaa") ++ (A) ++ ("aabaab") ++ (A) ++ ("ba") ++ (A) ++ ("ab") ++ (A) ++ (A) ++ (A) ++ ("bb") ++ (A) ++ ("aba") ++ (A) ++ ("baaaaaabbaab") ++ (A) ++ ("aaa") ++ (A) ++ ("baaabaa") ++ (A) ++ ("a") ++ (A) ++ (A) ++ (A) ++ (A) ++ ("baa"))
    =
    (("a") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("aaa") ++ (A) ++ ("ab") ++ (A) ++ ("aaabaa") ++ (A) ++ ("aab") ++ (A) ++ (A) ++ ("ababb") ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("baba") ++ (A) ++ ("baaaaaa") ++ (A) ++ ("baab") ++ (A) ++ ("aaa") ++ (A) ++ ("baaa") ++ (A) ++ ("aababbb") ++ (A) ++ ("baa"));
constraint
  (("aabba") ++ (A) ++ ("abaa") ++ (A) ++ (A) ++ ("b") ++ (A) ++ (A) ++ ("aaaa") ++ (A) ++ (A) ++ ("ab") ++ (A) ++ ("abaabaa") ++ (A) ++ (A) ++ ("aaaababa") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("aaaaa") ++ (A) ++ ("ba") ++ (A) ++ ("babba") ++ (A) ++ ("abbaa") ++ (A) ++ ("aaaa") ++ (A) ++ ("b"))
    =
    (("aa") ++ (A) ++ (A) ++ ("aba") ++ (A) ++ ("aa") ++ (A) ++ ("b") ++ (A) ++ (A) ++ ("baaaa") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("babaa") ++ (A) ++ ("aab") ++ (A) ++ ("aaaabababbabaaaaa") ++ (A) ++ ("babba") ++ (A) ++ (A) ++ ("aba") ++ (A) ++ ("baabaaaab") ++ (A));
constraint
  (("bbaa") ++ (A) ++ ("abbaaa") ++ (A) ++ ("aa") ++ (A) ++ ("aaaaaaabb") ++ (A) ++ ("aaabaa") ++ (A) ++ ("a") ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("aabab") ++ (A) ++ (A) ++ (A) ++ ("abaa") ++ (A) ++ (A) ++ ("aaaaabbaa") ++ (A) ++ ("aa") ++ (A) ++ ("a") ++ (A) ++ ("baaa") ++ (A))
    =
    ((A) ++ ("baa") ++ (A) ++ ("abbaaabaa") ++ (A) ++ ("aaaaaaa") ++ (A) ++ (A) ++ (A) ++ ("aaa") ++ (A) ++ ("aaba") ++ (A) ++ ("aabbaa") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("abaabbaaaaab") ++ (A) ++ ("aabaabab") ++ (A) ++ ("aaab"));
constraint
  ((A) ++ ("baaabaaa") ++ (A) ++ ("abbb") ++ (A) ++ ("aaaaab") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("aaaaaba") ++ (A) ++ ("a") ++ (A) ++ ("ab") ++ (A) ++ (A) ++ (A) ++ ("abbbb") ++ (A) ++ ("aaaabbbaabbba") ++ (A) ++ ("baab") ++ (A) ++ ("b") ++ (A) ++ ("aa") ++ (A) ++ ("a"))
    =
    (("b") ++ (A) ++ ("aaa") ++ (A) ++ ("aaabab") ++ (A) ++ (A) ++ (A) ++ ("aaaaabba") ++ (A) ++ ("abaaaaababababb") ++ (A) ++ ("bab") ++ (A) ++ ("bbbaaaabbbaab") ++ (A) ++ ("ba") ++ (A) ++ ("baa") ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("aaba"));
constraint
  (("a") ++ (A) ++ ("aaa") ++ (A) ++ ("ababaaaa") ++ (A) ++ ("baaabbab") ++ (A) ++ (A) ++ ("bb") ++ (A) ++ (A) ++ ("abba") ++ (A) ++ (A) ++ ("ab") ++ (A) ++ ("a") ++ (A) ++ (A) ++ (A) ++ ("aba") ++ (A) ++ ("aa") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("baa") ++ (A) ++ ("a") ++ (A) ++ ("aaaaa") ++ (A) ++ ("b") ++ (A) ++ ("baa") ++ (A) ++ ("b"))
    =
    (("abaaaba") ++ (A) ++ ("a") ++ (A) ++ ("aaaa") ++ (A) ++ (A) ++ ("aaab") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("bbb") ++ (A) ++ (A) ++ ("ab") ++ (A) ++ ("a") ++ (A) ++ ("babbabb") ++ (A) ++ ("ababaababa") ++ (A) ++ ("baa") ++ (A) ++ ("abaaaaa") ++ (A) ++ (A) ++ (A) ++ (A) ++ ("aab") ++ (A));
constraint
  (("bab") ++ (A) ++ (A) ++ ("baaaaa") ++ (A) ++ (A) ++ ("aaaabaaaaaabbb") ++ (A) ++ ("babaa") ++ (A) ++ ("ab") ++ (A) ++ ("ba") ++ (A) ++ ("abba") ++ (A) ++ ("aba") ++ (A) ++ ("aa") ++ (A) ++ ("abab") ++ (A) ++ ("b") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("aabba") ++ (A) ++ (A) ++ ("b"))
    =
    (("babbbbaaaaabbaaaa") ++ (A) ++ ("aaaaaabbb") ++ (A) ++ ("ba") ++ (A) ++ ("aa") ++ (A) ++ ("abb") ++ (A) ++ ("ababba") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("aa") ++ (A) ++ ("aba") ++ (A) ++ ("b") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("babaa") ++ (A) ++ ("ba") ++ (A) ++ (A) ++ (A));
constraint
  (("abaaa") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("aaa") ++ (A) ++ ("abaaaaa") ++ (A) ++ ("aab") ++ (A) ++ ("a") ++ (A) ++ ("aaab") ++ (A) ++ ("b") ++ (A) ++ (A) ++ (A) ++ ("aaa") ++ (A) ++ (A) ++ (A) ++ ("ba") ++ (A) ++ ("baabaa") ++ (A) ++ ("bbaaaabbaba") ++ (A) ++ ("abaa"))
    =
    (("abaaa") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("aab") ++ (A) ++ ("aaaba") ++ (A) ++ ("aaaaa") ++ (A) ++ ("aabba") ++ (A) ++ ("aaa") ++ (A) ++ ("b") ++ (A) ++ ("bbbaaab") ++ (A) ++ ("b") ++ (A) ++ ("ab") ++ (A) ++ ("aabaa") ++ (A) ++ ("bbaaaab") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("aa"));
constraint
  (("baaaababaab") ++ (A) ++ (A) ++ ("aabb") ++ (A) ++ ("aaa") ++ (A) ++ ("abaaaaababb") ++ (A) ++ ("b") ++ (A) ++ ("abbababbb") ++ (A) ++ ("aababa") ++ (A) ++ (A) ++ ("baaabab") ++ (A) ++ ("ba") ++ (A) ++ ("b") ++ (A) ++ ("aaa") ++ (A) ++ ("b"))
    =
    (("baaaa") ++ (A) ++ ("a") ++ (A) ++ ("aab") ++ (A) ++ ("baab") ++ (A) ++ (A) ++ ("aaababaaaaabab") ++ (A) ++ ("bbba") ++ (A) ++ (A) ++ ("aba") ++ (A) ++ (A) ++ ("bbaa") ++ (A) ++ ("a") ++ (A) ++ ("ab") ++ (A) ++ ("baaa") ++ (A) ++ ("ab") ++ (A) ++ ("bab") ++ (A) ++ ("baaabb"));
constraint
  ((A) ++ (A) ++ ("abab") ++ (A) ++ ("b") ++ (A) ++ ("aaabbbaaaaaabaab") ++ (A) ++ ("abab") ++ (A) ++ ("aaa") ++ (A) ++ (A) ++ ("aaaaab") ++ (A) ++ (A) ++ (A) ++ ("ab") ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("abaa") ++ (A) ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("aba") ++ (A) ++ (A) ++ ("baabaab"))
    =
    (("b") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ (A) ++ (A) ++ ("baaa") ++ (A) ++ (A) ++ (A) ++ ("aaaaaa") ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("abab") ++ (A) ++ ("aaa") ++ (A) ++ (A) ++ ("aaaaab") ++ (A) ++ ("b") ++ (A) ++ ("abbbbba") ++ (A) ++ ("aab") ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("a") ++ (A) ++ ("abbbaabaab"));
constraint
  (("aaaabb") ++ (A) ++ (A) ++ ("baaaaa") ++ (A) ++ ("b") ++ (A) ++ ("abaa") ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("aa") ++ (A) ++ ("abba") ++ (A) ++ (A) ++ ("baaba") ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("abba") ++ (A) ++ ("ababaabba") ++ (A) ++ ("b") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("bab") ++ (A) ++ ("a") ++ (A) ++ ("aa"))
    =
    (("aaaa") ++ (A) ++ (A) ++ (A) ++ ("bbaaaaa") ++ (A) ++ (A) ++ (A) ++ ("abaabbbbaa") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("a") ++ (A) ++ (A) ++ (A) ++ ("aababaabbab") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("aab") ++ (A) ++ ("a") ++ (A) ++ ("bb") ++ (A) ++ ("ab") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("aa"));
constraint
  (("bbb") ++ (A) ++ ("a") ++ (A) ++ ("babaaab") ++ (A) ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("ab") ++ (A) ++ (A) ++ (A) ++ ("aa") ++ (A) ++ ("b") ++ (A) ++ ("baabaabbaaabbaa") ++ (A) ++ ("a") ++ (A) ++ ("bbaba") ++ (A) ++ ("abaabaaab") ++ (A) ++ ("a") ++ (A) ++ ("ba") ++ (A) ++ ("ab") ++ (A) ++ (A) ++ ("a"))
    =
    ((A) ++ (A) ++ ("b") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("aaa") ++ (A) ++ (A) ++ (A) ++ ("ba") ++ (A) ++ ("a") ++ (A) ++ ("bbbaa") ++ (A) ++ ("bb") ++ (A) ++ ("aa") ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("aaa") ++ (A) ++ ("baa") ++ (A) ++ ("abb") ++ (A) ++ ("ababa") ++ (A) ++ ("aabaaa") ++ (A) ++ ("ba") ++ (A) ++ (A) ++ ("abab") ++ (A) ++ ("ba"));
constraint
  (("babab") ++ (A) ++ ("b") ++ (A) ++ ("aaaaa") ++ (A) ++ ("baabaaa") ++ (A) ++ ("b") ++ (A) ++ (A) ++ ("bba") ++ (A) ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("bb") ++ (A) ++ (A) ++ ("aababbbbababa") ++ (A) ++ ("a") ++ (A) ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("aba") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("aa"))
    =
    ((A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("bbaaaaabbaabaaa") ++ (A) ++ (A) ++ (A) ++ ("bb") ++ (A) ++ ("ab") ++ (A) ++ (A) ++ ("ab") ++ (A) ++ ("b") ++ (A) ++ ("baa") ++ (A) ++ ("a") ++ (A) ++ (A) ++ (A) ++ (A) ++ ("ababababb") ++ (A) ++ ("abaabbabab") ++ (A) ++ ("aba") ++ (A) ++ ("aa"));
constraint
  (("bababaaababaa") ++ (A) ++ ("abaaabbaab") ++ (A) ++ (A) ++ ("babbabaaa") ++ (A) ++ ("a") ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("ba") ++ (A) ++ ("aaaaaabab") ++ (A) ++ ("ababaaba") ++ (A) ++ ("aa") ++ (A) ++ (A) ++ ("ab") ++ (A) ++ ("a"))
    =
    (("ba") ++ (A) ++ ("abaaababaa") ++ (A) ++ ("a") ++ (A) ++ ("aaa") ++ (A) ++ (A) ++ ("aa") ++ (A) ++ (A) ++ (A) ++ ("bab") ++ (A) ++ ("a") ++ (A) ++ ("aaa") ++ (A) ++ ("a") ++ (A) ++ ("aabb") ++ (A) ++ ("a") ++ (A) ++ ("aaaaaaba") ++ (A) ++ ("baba") ++ (A) ++ ("aa") ++ (A) ++ ("abaabba") ++ (A) ++ ("ba"));
solve satisfy;
