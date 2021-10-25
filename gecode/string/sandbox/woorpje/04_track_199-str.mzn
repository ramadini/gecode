var string: I;
var string: H;
var string: B;
var string: G;
constraint
  ((I) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("b") ++ (I) ++ ("a") ++ (B) ++ (B) ++ (B) ++ (H) ++ (B) ++ ("aaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("a"))
    =
    (("abaaa") ++ (B) ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B) ++ (B) ++ (I) ++ ("ab") ++ (B) ++ (B) ++ (B) ++ ("baaab") ++ (B) ++ ("a") ++ (B) ++ ("ab") ++ (B) ++ ("aaa") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("a"));
constraint
  ((B) ++ ("a") ++ (B) ++ (B) ++ ("baa") ++ (B) ++ ("aab") ++ (B) ++ ("aaba") ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("abbba") ++ (B) ++ ("ba") ++ (B) ++ ("aab") ++ (B) ++ (B) ++ ("baab") ++ (B) ++ ("aa"))
    =
    ((B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("aabaa") ++ (B) ++ (B) ++ ("aababb") ++ (B) ++ ("ababb") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (I) ++ (B) ++ (B) ++ ("bbaa") ++ (B) ++ ("baa"));
constraint
  (("a") ++ (B) ++ ("bababa") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("aaaab") ++ (I) ++ ("aab") ++ (B) ++ ("ababa") ++ (B) ++ ("baa") ++ (B) ++ ("abab") ++ (B))
    =
    (("a") ++ (B) ++ ("ba") ++ (B) ++ ("a") ++ (B) ++ ("abbaa") ++ (B) ++ (B) ++ ("aaaa") ++ (B) ++ (I) ++ ("aa") ++ (B) ++ (B) ++ ("aba") ++ (B) ++ ("a") ++ (B) ++ ("baa") ++ (B) ++ ("a") ++ (B) ++ ("ab") ++ (B));
constraint
  ((B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ ("babbb") ++ (B) ++ (B) ++ ("baaaaaab") ++ (I) ++ ("b") ++ (B) ++ ("aaaaaaa") ++ (B) ++ ("a") ++ (B) ++ ("abbab"))
    =
    ((B) ++ ("aaba") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("bbaaaaaa") ++ (B) ++ (I) ++ ("b") ++ (B) ++ ("aaaaaaaba") ++ (B) ++ ("abbab"));
constraint
  (("aa") ++ (B) ++ ("bbbaaa") ++ (I) ++ ("b") ++ (B) ++ ("aab") ++ (B) ++ ("aab") ++ (B) ++ ("ab") ++ (B) ++ ("aaaba") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("babbaab"))
    =
    (("aab") ++ (B) ++ (B) ++ ("baaaa") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("aabbaa") ++ (B) ++ ("ba") ++ (B) ++ ("baaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("aab"));
constraint
  (("ababbaaaba") ++ (B) ++ (B) ++ (I) ++ ("baab") ++ (B) ++ ("aaaabb") ++ (B) ++ (B) ++ (B) ++ ("baa") ++ (B) ++ (I) ++ (B) ++ (B) ++ ("bb"))
    =
    (("a") ++ (B) ++ ("a") ++ (B) ++ ("baaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (I) ++ (B) ++ ("aab") ++ (B) ++ ("aaaabb") ++ (B) ++ ("bbbaa") ++ (B) ++ (I) ++ (B) ++ (B) ++ (B) ++ (B));
constraint
  (("ba") ++ (B) ++ ("ba") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("baaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aaa") ++ (B) ++ ("babb") ++ (B) ++ ("aaaaabbaaba") ++ (B) ++ ("abb"))
    =
    (("bab") ++ (B) ++ ("abab") ++ (B) ++ (B) ++ ("baaa") ++ (B) ++ ("babaaa") ++ (B) ++ ("ba") ++ (B) ++ ("bbaaaaab") ++ (B) ++ ("aababab") ++ (B));
constraint
  (("a") ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ (I) ++ ("aaa") ++ (B) ++ ("aabaaaaabaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ ("abaaaaa"))
    =
    ((I) ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aaaa") ++ (I) ++ ("baaaaabaabaaab") ++ (B) ++ ("ba") ++ (B) ++ ("aaaaa"));
constraint
  (("a") ++ (B) ++ ("baab") ++ (I) ++ ("b") ++ (B) ++ (B) ++ ("aaaab") ++ (G) ++ ("a") ++ (B) ++ ("aaab") ++ (I) ++ ("a") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("ba"))
    =
    (("ab") ++ (B) ++ ("aa") ++ (B) ++ ("abaa") ++ (B) ++ (B) ++ (B) ++ ("aaaabaa") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("b") ++ (I) ++ ("a") ++ (B) ++ (I) ++ ("abb") ++ (B) ++ (B) ++ (B) ++ ("a"));
constraint
  (("aa") ++ (B) ++ ("bb") ++ (B) ++ ("b") ++ (B) ++ ("aab") ++ (B) ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("baaaa") ++ (B) ++ ("abbabbbbb") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("ba") ++ (B) ++ ("aaa"))
    =
    (("aab") ++ (B) ++ ("bb") ++ (B) ++ ("baa") ++ (B) ++ (B) ++ (B) ++ ("baabbaaaaba") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("bbbbbabba") ++ (B) ++ ("aaa"));
constraint
  (("ab") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aaa") ++ (I) ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("abaab") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("baaaba") ++ (B) ++ (I) ++ ("a") ++ (B) ++ (B))
    =
    (("a") ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aaa") ++ (I) ++ (B) ++ ("bb") ++ (B) ++ (I) ++ ("bbaaabbbaaa") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("aaab") ++ (B));
constraint
  (("aaa") ++ (B) ++ ("aabbaaaaa") ++ (B) ++ ("bab") ++ (B) ++ ("aa") ++ (B) ++ ("ab") ++ (B) ++ ("aaaaaabb") ++ (B) ++ ("aaabababb"))
    =
    (("aa") ++ (I) ++ ("b") ++ (B) ++ ("aaaaa") ++ (B) ++ ("ba") ++ (B) ++ ("baabab") ++ (B) ++ ("aaaaaa") ++ (B) ++ ("b") ++ (B) ++ ("aaababab") ++ (B));
constraint
  (("ab") ++ (G) ++ (B) ++ ("a") ++ (I) ++ ("aaa") ++ (B) ++ ("ba") ++ (B) ++ ("b") ++ (B) ++ ("bbababbba") ++ (B) ++ ("aaa") ++ (B) ++ ("aaaa"))
    =
    (("abaa") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("bba") ++ (I) ++ ("aaa") ++ (B) ++ ("ba") ++ (B) ++ ("bbb") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("abaaa") ++ (B) ++ ("aaaa"));
constraint
  ((B) ++ ("abb") ++ (B) ++ ("b") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("aba") ++ (B) ++ (B) ++ ("aaba") ++ (B) ++ ("b") ++ (B) ++ ("babbaaabbbaaaaa"))
    =
    (("babb") ++ (B) ++ ("bbaa") ++ (B) ++ ("baba") ++ (B) ++ ("abab") ++ (B) ++ ("aa") ++ (B) ++ ("abb") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("aaabb") ++ (B) ++ ("aaaaa"));
constraint
  (("ab") ++ (B) ++ ("bab") ++ (B) ++ ("babba") ++ (I) ++ ("aaab") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("ba") ++ (B) ++ (I) ++ (B) ++ ("baaaaa") ++ (B) ++ ("b"))
    =
    (("abbba") ++ (B) ++ ("bba") ++ (B) ++ ("ba") ++ (I) ++ ("aaa") ++ (B) ++ ("bbaab") ++ (B) ++ (B) ++ ("ab") ++ (I) ++ ("bbaaaaa") ++ (B) ++ ("b"));
constraint
  (("aba") ++ (B) ++ ("aa") ++ (B) ++ ("ba") ++ (B) ++ ("aaaaaaba") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aaabb") ++ (B) ++ (B) ++ (B))
    =
    (("ab") ++ (I) ++ (B) ++ ("b") ++ (I) ++ ("aaaababbabbbbbaba") ++ (B) ++ ("b") ++ (B) ++ (I) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("bb"));
constraint
  ((B) ++ ("a") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aabba") ++ (B) ++ ("aabaa") ++ (B) ++ (B) ++ ("bba") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("bbaa") ++ (B) ++ ("a") ++ (B) ++ ("baab"))
    =
    (("b") ++ (I) ++ ("a") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("b") ++ (I) ++ ("baa") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("ababa") ++ (B) ++ ("a") ++ (B) ++ ("bbaaba") ++ (B) ++ ("baab"));
constraint
  ((B) ++ ("aaaa") ++ (I) ++ (B) ++ ("bba") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("baaaa") ++ (I) ++ ("bbbbabbbbbab") ++ (B))
    =
    ((B) ++ ("aaaa") ++ (I) ++ ("b") ++ (B) ++ ("bab") ++ (B) ++ ("b") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("aaaa") ++ (I) ++ (B) ++ ("b") ++ (B) ++ ("ba") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("b"));
constraint
  (("a") ++ (B) ++ ("abb") ++ (B) ++ ("aa") ++ (B) ++ ("baa") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("ab") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("aa") ++ (B) ++ ("ba") ++ (B) ++ ("ab") ++ (I) ++ ("baa"))
    =
    (("abab") ++ (B) ++ ("baab") ++ (B) ++ ("aab") ++ (B) ++ ("aa") ++ (B) ++ ("ab") ++ (B) ++ ("aaa") ++ (B) ++ ("bbbaa") ++ (B) ++ ("ba") ++ (B) ++ ("a") ++ (B) ++ (I) ++ (B) ++ ("aa"));
constraint
  ((B) ++ ("baaaaaaaaabaaa") ++ (B) ++ ("abaaaaaab") ++ (B) ++ ("ab") ++ (I) ++ ("ababaaabaa"))
    =
    (("b") ++ (B) ++ ("aaaaaaaaa") ++ (B) ++ ("aaab") ++ (I) ++ ("aaaa") ++ (B) ++ ("ba") ++ (B) ++ ("a") ++ (B) ++ ("aaa") ++ (B) ++ (I) ++ (I));
constraint
  (("aa") ++ (B) ++ ("aabb") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (I) ++ ("ab") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("abbaaba") ++ (B) ++ ("baa"))
    =
    (("aabaa") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("aab") ++ (I) ++ ("abbab") ++ (B) ++ ("abba") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ ("baa"));
constraint
  (("ba") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aabaaab") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (I) ++ ("aa") ++ (I) ++ (B) ++ ("b") ++ (B))
    =
    (("ba") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("bb") ++ (B) ++ ("ab") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("ba") ++ (I) ++ ("a") ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("abaaaaabaab") ++ (B) ++ ("b"));
constraint
  (("ab") ++ (B) ++ ("aaab") ++ (B) ++ (G) ++ ("aababaaaa") ++ (B) ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("bbbaa") ++ (B) ++ (B) ++ (B))
    =
    (("a") ++ (B) ++ ("baaab") ++ (B) ++ ("aabbb") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("abaaaa") ++ (B) ++ ("bb") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("bb") ++ (B) ++ ("bbaabb") ++ (B));
constraint
  ((B) ++ (B) ++ ("b") ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("bba") ++ (B) ++ ("aba") ++ (B) ++ (B) ++ ("ba") ++ (I) ++ ("aba") ++ (B) ++ ("aabb") ++ (B) ++ ("babbaa"))
    =
    ((B) ++ ("bb") ++ (B) ++ ("abbabbbb") ++ (B) ++ ("ba") ++ (B) ++ ("aba") ++ (B) ++ ("bbaa") ++ (B) ++ ("aaab") ++ (I) ++ (B) ++ (B) ++ (B) ++ (B) ++ ("abbaa"));
constraint
  (("aa") ++ (B) ++ ("bbaaaa") ++ (B) ++ ("a") ++ (B) ++ ("baaabb") ++ (B) ++ (B) ++ ("aba") ++ (B) ++ ("baab") ++ (I) ++ (G) ++ ("aaa"))
    =
    (("aa") ++ (B) ++ ("b") ++ (B) ++ ("aaaaba") ++ (B) ++ (B) ++ ("aaabbb") ++ (B) ++ ("aba") ++ (B) ++ ("baa") ++ (B) ++ ("abaaaa") ++ (B) ++ ("bb") ++ (B) ++ (B) ++ ("aaa"));
constraint
  (("baaabbaaab") ++ (B) ++ ("bbabaa") ++ (B) ++ ("b") ++ (B) ++ ("baaa") ++ (B) ++ ("bbaba") ++ (B) ++ ("bbaabbaa") ++ (B) ++ ("ab"))
    =
    (("baaa") ++ (B) ++ (B) ++ ("aaab") ++ (B) ++ (B) ++ ("b") ++ (I) ++ (B) ++ (B) ++ (B) ++ ("baaabbba") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("aabbaabab"));
constraint
  ((B) ++ ("aba") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("abb") ++ (B) ++ ("b") ++ (B) ++ ("bbaa") ++ (B) ++ ("baa") ++ (B) ++ ("a") ++ (B) ++ (I) ++ (B) ++ ("a") ++ (B) ++ ("abaa") ++ (B) ++ ("aaaaa"))
    =
    ((B) ++ ("aba") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("bbbb") ++ (B) ++ ("aa") ++ (B) ++ ("baa") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("aaba") ++ (B) ++ (I) ++ ("baaaaa"));
constraint
  (("a") ++ (B) ++ (B) ++ ("baaabab") ++ (B) ++ ("baaaaa") ++ (B) ++ ("aba") ++ (B) ++ ("baa") ++ (B) ++ ("baa") ++ (B) ++ (B) ++ ("abab") ++ (B) ++ (I) ++ ("aa"))
    =
    (("abb") ++ (B) ++ ("aaabab") ++ (B) ++ ("baaaaaba") ++ (B) ++ ("a") ++ (B) ++ ("baab") ++ (B) ++ ("aab") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (I) ++ ("aa"));
constraint
  (("aaabbbba") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("aaabbaaab") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aa") ++ (I) ++ ("aaab") ++ (B) ++ ("baaa"))
    =
    (("aaa") ++ (B) ++ (B) ++ ("bbab") ++ (B) ++ ("ab") ++ (B) ++ ("aaab") ++ (B) ++ ("aaabb") ++ (B) ++ ("abbaaabaaaaa") ++ (B) ++ ("bbaaa"));
constraint
  (("abbaaba") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aaaaaba") ++ (B) ++ ("aaaaa") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("baaab") ++ (B) ++ ("abaa") ++ (B) ++ (B) ++ ("ba"))
    =
    (("a") ++ (B) ++ ("baababb") ++ (I) ++ ("aaab") ++ (I) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("baaab") ++ (B) ++ (I) ++ (B) ++ (B) ++ ("ba"));
constraint
  (("aaaa") ++ (B) ++ ("a") ++ (B) ++ ("aab") ++ (B) ++ ("a") ++ (B) ++ ("bab") ++ (B) ++ (B) ++ (I) ++ ("bb") ++ (I) ++ ("abaababb") ++ (I) ++ ("aa"))
    =
    (("aaaababaa") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ (I) ++ (B) ++ (B) ++ ("abaa") ++ (I) ++ ("ba") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aaaa"));
constraint
  ((B) ++ ("aaaaaaaaaa") ++ (B) ++ ("a") ++ (B) ++ ("abbaa") ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ ("baabaa") ++ (B) ++ ("aab") ++ (I) ++ ("aba"))
    =
    ((B) ++ ("aaaaaaaaaa") ++ (B) ++ ("aba") ++ (B) ++ ("baa") ++ (B) ++ ("aaba") ++ (B) ++ ("baa") ++ (B) ++ ("aabaab") ++ (I) ++ ("a") ++ (B) ++ ("a"));
constraint
  (("a") ++ (B) ++ ("ba") ++ (B) ++ (I) ++ ("aaaba") ++ (B) ++ ("baa") ++ (B) ++ (B) ++ ("a") ++ (I) ++ ("aa") ++ (B) ++ ("b") ++ (B) ++ ("aa") ++ (I) ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (B))
    =
    (("a") ++ (B) ++ (B) ++ ("aba") ++ (B) ++ ("aaaaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aab") ++ (B) ++ ("a") ++ (I) ++ ("aab") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ ("bab"));
constraint
  ((B) ++ ("b") ++ (B) ++ ("baaaba") ++ (B) ++ ("baaba") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aaba") ++ (B) ++ (I) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ (I) ++ ("aa") ++ (B) ++ ("ab"))
    =
    (("b") ++ (B) ++ ("bbaaaba") ++ (B) ++ (B) ++ ("aababba") ++ (B) ++ (B) ++ ("aabababaaabbbabaaaabab"));
constraint
  ((B) ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("bab") ++ (B) ++ ("ab") ++ (B) ++ ("b") ++ (I) ++ (B) ++ (B) ++ ("aaaaaabbb") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("aaaab") ++ (B) ++ ("aa"))
    =
    ((B) ++ (B) ++ ("ababbabbab") ++ (B) ++ ("babaabbaaaaaa") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("abaaaab") ++ (B) ++ ("aa"));
constraint
  (("aaaabba") ++ (I) ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("abab") ++ (B) ++ ("b") ++ (B) ++ (B) ++ (B) ++ ("aaaaa") ++ (B) ++ ("bababaa") ++ (B) ++ ("aaa"))
    =
    (("aaaa") ++ (B) ++ (B) ++ ("aabaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("ba") ++ (B) ++ ("abb") ++ (B) ++ ("b") ++ (B) ++ ("baaaaa") ++ (B) ++ ("bab") ++ (I) ++ ("baaa"));
constraint
  ((B) ++ ("a") ++ (B) ++ ("bb") ++ (B) ++ ("bababbabba") ++ (B) ++ ("abba") ++ (B) ++ ("ba") ++ (I) ++ (B) ++ ("aa") ++ (B) ++ ("bbbaa") ++ (B) ++ (B) ++ ("abb"))
    =
    ((B) ++ ("abbb") ++ (B) ++ (B) ++ ("aba") ++ (B) ++ (B) ++ ("abba") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("aa") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("aabba") ++ (B) ++ (B));
constraint
  ((B) ++ ("aabb") ++ (B) ++ (I) ++ ("b") ++ (I) ++ ("bba") ++ (B) ++ ("aaaabaaabbaaaa") ++ (B) ++ ("bb") ++ (B) ++ ("aba") ++ (B) ++ (B))
    =
    ((B) ++ ("aab") ++ (B) ++ (B) ++ (I) ++ (B) ++ ("a") ++ (B) ++ ("aabb") ++ (I) ++ ("aabaaabbaaaabb") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ (B));
constraint
  ((B) ++ ("ba") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aaaa") ++ (B) ++ ("ba") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("bba") ++ (B) ++ ("aab") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaaaa"))
    =
    ((B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ (I) ++ ("aa") ++ (B) ++ ("ba") ++ (B) ++ ("aa") ++ (B) ++ ("bb") ++ (B) ++ (I) ++ (B) ++ (B) ++ (B) ++ ("aaabaaa") ++ (B) ++ ("baaaaa"));
constraint
  ((B) ++ (B) ++ (B) ++ ("aaab") ++ (B) ++ ("aaab") ++ (B) ++ ("aa") ++ (B) ++ (I) ++ ("babaaa") ++ (B) ++ (B) ++ ("baaabbbb") ++ (B) ++ ("bbaaa"))
    =
    ((B) ++ ("bbaaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ ("aa") ++ (B) ++ (I) ++ ("ab") ++ (B) ++ (B) ++ ("aaab") ++ (B) ++ ("bb") ++ (B) ++ ("bbaaa"));
constraint
  ((B) ++ ("a") ++ (B) ++ ("baababbabaabb") ++ (B) ++ (I) ++ ("aaa") ++ (B) ++ ("aaaa") ++ (B) ++ ("aabb") ++ (B) ++ ("a") ++ (B) ++ ("aaaaa"))
    =
    (("babbaaba") ++ (B) ++ (B) ++ (I) ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("aaaa") ++ (I) ++ ("aa") ++ (B) ++ ("aa") ++ (B) ++ ("b") ++ (B) ++ (I) ++ ("aaa"));
constraint
  (("aaa") ++ (B) ++ (B) ++ ("bbb") ++ (I) ++ ("aaaab") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (B) ++ ("bbbaaaaba") ++ (B) ++ ("aaaaa") ++ (B) ++ ("ab"))
    =
    (("aaa") ++ (B) ++ ("b") ++ (B) ++ (B) ++ (B) ++ ("abaaaaaabbba") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aaaa") ++ (B) ++ ("abaaaaa") ++ (B) ++ ("ab"));
constraint
  ((I) ++ ("ab") ++ (B) ++ (B) ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("b") ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("aaa") ++ (B) ++ ("a") ++ (B) ++ ("baaabb") ++ (B) ++ ("b"))
    =
    (("a") ++ (B) ++ ("aaa") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("b") ++ (I) ++ ("aa") ++ (I) ++ ("ababaaa") ++ (B) ++ ("abbaaa") ++ (B) ++ (B) ++ ("bb"));
constraint
  (("a") ++ (B) ++ ("babaaa") ++ (B) ++ ("bba") ++ (B) ++ ("ab") ++ (B) ++ ("b") ++ (B) ++ ("aabbaab") ++ (B) ++ ("b") ++ (B) ++ ("ababaa") ++ (B) ++ (B) ++ ("baabab"))
    =
    (("abb") ++ (I) ++ ("abbbaba") ++ (B) ++ ("bbbaabbaab") ++ (B) ++ ("b") ++ (B) ++ ("aba") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("baa") ++ (B) ++ ("a") ++ (B));
constraint
  (("baaabaaaabb") ++ (B) ++ ("aa") ++ (B) ++ ("aba") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("aaaa") ++ (I) ++ ("ba") ++ (I) ++ (I) ++ ("b"))
    =
    (("baaa") ++ (B) ++ ("aaaa") ++ (B) ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("ababbab") ++ (B) ++ ("aaaaabaaba") ++ (I) ++ ("a") ++ (B) ++ ("aa") ++ (B));
constraint
  (("baa") ++ (B) ++ ("bbaabbaaa") ++ (B) ++ (B) ++ ("a") ++ (I) ++ ("ba") ++ (B) ++ ("babb") ++ (I) ++ (B) ++ (B) ++ ("abaa") ++ (B) ++ ("abaa"))
    =
    ((B) ++ ("aab") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("baaa") ++ (B) ++ (B) ++ ("aabaa") ++ (B) ++ ("abba") ++ (B) ++ ("babaa") ++ (B) ++ ("ba") ++ (B) ++ ("aa") ++ (B) ++ (I));
constraint
  (("ab") ++ (I) ++ ("abbabbbab") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (I) ++ (B) ++ ("aaaa") ++ (B) ++ (I) ++ ("b") ++ (G))
    =
    (("aba") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("bbab") ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (I) ++ (B) ++ ("aaaa") ++ (B) ++ ("abaabaab") ++ (B) ++ ("b") ++ (B) ++ (B));
constraint
  (("aaaaab") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("aaabbaabaabbb") ++ (B) ++ (I) ++ ("aaa") ++ (B) ++ ("abab"))
    =
    (("aaaaab") ++ (B) ++ ("babb") ++ (B) ++ ("a") ++ (B) ++ ("bbaaabba") ++ (I) ++ ("b") ++ (B) ++ (B) ++ ("babaaaaa") ++ (B) ++ ("aba") ++ (B));
constraint
  (("aabaa") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (I) ++ ("a") ++ (B) ++ ("bb") ++ (B) ++ ("bbaab") ++ (B) ++ (B) ++ ("aaaab") ++ (B) ++ ("bab") ++ (B) ++ ("b") ++ (B) ++ ("ba") ++ (B) ++ ("ab"))
    =
    (("a") ++ (I) ++ ("bab") ++ (B) ++ ("a") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("aab") ++ (B) ++ (B) ++ ("aaaab") ++ (B) ++ ("babb") ++ (B) ++ ("b") ++ (B) ++ ("abab"));
constraint
  (("baabaa") ++ (B) ++ (B) ++ ("bababba") ++ (B) ++ (B) ++ ("aabaabbbaba") ++ (B) ++ (B) ++ ("aaaabbbb") ++ (B) ++ ("a") ++ (B) ++ ("a"))
    =
    (("ba") ++ (I) ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("aabb") ++ (B) ++ ("a") ++ (B) ++ ("abbaaaab") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aba"));
constraint
  ((B) ++ ("aaaa") ++ (B) ++ ("abbaaaaaaa") ++ (B) ++ ("aaa") ++ (I) ++ ("aab") ++ (G) ++ (B) ++ ("babbbba"))
    =
    (("baaaabab") ++ (B) ++ ("aaaaaaabaaa") ++ (I) ++ ("aa") ++ (B) ++ ("aa") ++ (B) ++ ("bbbbbba") ++ (B) ++ ("b") ++ (B) ++ ("ba"));
constraint
  ((B) ++ (B) ++ ("aaa") ++ (B) ++ ("baa") ++ (I) ++ ("aaaa") ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("baaa") ++ (I) ++ ("aaa") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("a"))
    =
    ((B) ++ (B) ++ ("aaa") ++ (B) ++ ("baa") ++ (I) ++ ("aaaab") ++ (B) ++ (B) ++ ("abbaaaa") ++ (B) ++ ("aaaaa") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aba"));
constraint
  ((I) ++ (B) ++ ("aaaaab") ++ (B) ++ (B) ++ ("babbbaab") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("abbabaababaaba") ++ (B))
    =
    (("abaabaaaaa") ++ (B) ++ ("bbbabb") ++ (B) ++ ("aabbaaabba") ++ (B) ++ ("b") ++ (I) ++ (B) ++ (I) ++ (B) ++ ("a") ++ (B));
constraint
  (("ab") ++ (B) ++ ("aab") ++ (B) ++ ("aabaaaa") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("babb") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (I) ++ (B) ++ ("baab") ++ (B) ++ ("ba"))
    =
    (("ab") ++ (B) ++ ("aa") ++ (B) ++ ("ba") ++ (I) ++ ("aabb") ++ (B) ++ ("bbab") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aaaba") ++ (B) ++ ("aab") ++ (B) ++ ("aa") ++ (B) ++ ("bba"));
constraint
  ((B) ++ ("aaa") ++ (B) ++ ("a") ++ (G) ++ ("bab") ++ (B) ++ ("baaa") ++ (B) ++ (I) ++ ("baa") ++ (I) ++ ("bba") ++ (B) ++ ("aabb") ++ (B))
    =
    (("baaa") ++ (B) ++ ("aaabb") ++ (B) ++ ("bbbabbbaaa") ++ (B) ++ (I) ++ ("baaa") ++ (B) ++ ("aabba") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ (B));
constraint
  ((B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aba") ++ (B) ++ ("b") ++ (B) ++ ("aaaaaabaab") ++ (B) ++ ("ababb") ++ (B) ++ ("a") ++ (B) ++ (I) ++ ("bbaa") ++ (B) ++ ("a"))
    =
    (("b") ++ (B) ++ ("aaabbabab") ++ (B) ++ ("baaaaaa") ++ (B) ++ ("aabbabab") ++ (B) ++ (B) ++ ("aba") ++ (B) ++ ("aa") ++ (B) ++ ("baa") ++ (B) ++ ("a"));
constraint
  (("b") ++ (B) ++ ("a") ++ (B) ++ (I) ++ (B) ++ ("abbba") ++ (I) ++ ("aaaaaaaaaaaabbaabaaaaabb"))
    =
    (("bba") ++ (B) ++ ("abaaba") ++ (B) ++ ("bba") ++ (I) ++ ("aaaaaaaaaaaa") ++ (B) ++ (B) ++ ("a") ++ (I) ++ ("aaabb"));
constraint
  (("baab") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B) ++ (B) ++ ("bbaa") ++ (B) ++ ("a") ++ (B) ++ ("abaaaabbab") ++ (B) ++ (I) ++ (G))
    =
    (("baa") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("aab") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("aabab") ++ (I) ++ ("aabba") ++ (B) ++ (B) ++ (I) ++ ("aab") ++ (B) ++ (B) ++ (B) ++ ("b"));
constraint
  (("b") ++ (B) ++ (I) ++ (B) ++ ("baaabb") ++ (B) ++ ("aabaaa") ++ (B) ++ (B) ++ ("aaba") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("abb") ++ (B) ++ ("bb") ++ (B) ++ ("bb") ++ (B) ++ ("ab"))
    =
    ((B) ++ (B) ++ ("abaab") ++ (B) ++ ("aaabbba") ++ (I) ++ ("ab") ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("babb") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("ab"));
constraint
  (("ba") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("aaaa") ++ (B) ++ ("b") ++ (B) ++ ("aaaaaabaa") ++ (B) ++ ("aaabb") ++ (B) ++ ("aaabab") ++ (B) ++ (B) ++ (B))
    =
    (("ba") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("babbaaaab") ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ ("aabaaabb") ++ (B) ++ ("aaaba") ++ (B) ++ ("bbb"));
constraint
  (("a") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("abaababa") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("aaaa") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aa") ++ (B) ++ ("bba") ++ (B) ++ ("a"))
    =
    (("ab") ++ (B) ++ ("aaa") ++ (B) ++ (I) ++ (B) ++ ("abab") ++ (B) ++ ("bab") ++ (B) ++ (B) ++ ("aaaa") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("babaa") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("a"));
constraint
  (("ab") ++ (B) ++ (B) ++ ("baaabbaaa") ++ (B) ++ ("babbba") ++ (B) ++ (B) ++ (B) ++ ("bba") ++ (B) ++ (B) ++ ("bbbba") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("a"))
    =
    (("abb") ++ (B) ++ ("baaa") ++ (B) ++ (B) ++ ("aaab") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("abbb") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("b") ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (B) ++ ("a"));
constraint
  (("abbaba") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (G) ++ ("babaa") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("ba") ++ (B) ++ ("ab") ++ (B) ++ ("b") ++ (B) ++ ("babbabb"))
    =
    (("ab") ++ (B) ++ ("a") ++ (B) ++ ("abbaa") ++ (B) ++ ("aabb") ++ (B) ++ (B) ++ (B) ++ ("b") ++ (I) ++ ("b") ++ (B) ++ ("b") ++ (B) ++ ("baba") ++ (B) ++ ("bb") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("bab") ++ (B));
constraint
  (("baba") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aaaaabab") ++ (B) ++ (I) ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("ba") ++ (B) ++ ("abbbab") ++ (B) ++ ("b") ++ (B) ++ ("aab") ++ (B) ++ ("b"))
    =
    ((B) ++ ("abab") ++ (B) ++ ("abaaaaabab") ++ (B) ++ (I) ++ ("bb") ++ (B) ++ ("bbaba") ++ (B) ++ ("b") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B) ++ (B));
constraint
  (("a") ++ (B) ++ ("bb") ++ (B) ++ ("aabba") ++ (B) ++ ("abb") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aba") ++ (B) ++ ("b") ++ (B) ++ (B) ++ (B) ++ ("aaaaaaba") ++ (B) ++ ("baba") ++ (B) ++ (B) ++ ("a"))
    =
    (("abb") ++ (B) ++ ("baabba") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("ababb") ++ (B) ++ ("b") ++ (B) ++ ("aaaaaaba") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a"));
constraint
  ((I) ++ (B) ++ ("aaaaaabbbaaa") ++ (B) ++ ("abbbbaaaa") ++ (B) ++ ("aa") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("b") ++ (B) ++ ("aaa"))
    =
    (("abaa") ++ (B) ++ ("aaaaaabb") ++ (B) ++ ("aaaba") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aaa") ++ (I) ++ ("babbbb") ++ (B) ++ ("bbaaa"));
constraint
  (("aaab") ++ (I) ++ ("abba") ++ (B) ++ ("a") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("aaabab") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("aba") ++ (B) ++ ("ba") ++ (B) ++ ("a") ++ (B) ++ ("baba"))
    =
    (("aaaba") ++ (B) ++ ("aaa") ++ (B) ++ ("ba") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("aaabab") ++ (B) ++ ("b") ++ (B) ++ ("babab") ++ (B) ++ ("ababba") ++ (B) ++ ("a"));
constraint
  (("a") ++ (B) ++ ("a") ++ (B) ++ ("baab") ++ (B) ++ ("ab") ++ (I) ++ ("aaba") ++ (B) ++ (I) ++ ("b") ++ (B) ++ ("abbabbba") ++ (B) ++ (B) ++ ("bb") ++ (B) ++ ("baa"))
    =
    (("a") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (I) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ (I) ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("bab") ++ (B) ++ (B) ++ (B) ++ ("bbaa"));
constraint
  (("a") ++ (B) ++ ("a") ++ (B) ++ (I) ++ (I) ++ (B) ++ (B) ++ ("babbbab") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ (B) ++ ("babb") ++ (B) ++ ("aabaaaa") ++ (B) ++ ("ba"))
    =
    (("abab") ++ (I) ++ ("abaabbbabbba") ++ (B) ++ ("baabb") ++ (B) ++ ("ba") ++ (B) ++ ("bbaabaaaa") ++ (B) ++ ("ba"));
constraint
  (("baabbabbab") ++ (B) ++ ("aaab") ++ (B) ++ ("abba") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (B) ++ ("baaaaaaaaaa") ++ (B) ++ ("aab"))
    =
    (("baabba") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("aaab") ++ (B) ++ ("a") ++ (B) ++ ("ba") ++ (B) ++ ("ba") ++ (B) ++ ("babbaaaaaaaaaa") ++ (B) ++ ("aa") ++ (B));
constraint
  ((B) ++ ("aaa") ++ (B) ++ ("abaabba") ++ (I) ++ ("bbaaa") ++ (B) ++ ("a") ++ (G) ++ (B) ++ ("ab") ++ (B) ++ ("aabab") ++ (B) ++ ("a") ++ (B))
    =
    (("baaababaabba") ++ (I) ++ ("b") ++ (B) ++ ("aa") ++ (I) ++ ("ab") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("babbaabab") ++ (B) ++ ("a") ++ (B));
constraint
  (("aaaaba") ++ (B) ++ ("b") ++ (B) ++ ("aaa") ++ (B) ++ ("aaab") ++ (B) ++ ("aaaba") ++ (B) ++ (B) ++ (B) ++ ("abbaa") ++ (B) ++ (B) ++ ("aabaab") ++ (B) ++ ("bb"))
    =
    (("aaaabab") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("abb") ++ (B) ++ ("a") ++ (B) ++ ("baa") ++ (B) ++ ("baa") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("b") ++ (B));
constraint
  ((B) ++ ("b") ++ (B) ++ ("aa") ++ (B) ++ ("ababa") ++ (B) ++ ("bbaabbababb") ++ (I) ++ ("a") ++ (B) ++ ("abb") ++ (B) ++ ("aaaaaba") ++ (B) ++ ("b"))
    =
    ((B) ++ ("b") ++ (B) ++ ("aa") ++ (B) ++ ("aba") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("baabbabab") ++ (B) ++ (I) ++ ("a") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("aaaaabab") ++ (B));
constraint
  (("a") ++ (B) ++ ("a") ++ (B) ++ ("bbbaaab") ++ (B) ++ ("abbbbaaa") ++ (B) ++ ("aaa") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("bbb") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("ababab"))
    =
    (("aba") ++ (B) ++ ("b") ++ (B) ++ ("baaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("bbbaa") ++ (I) ++ ("abaab") ++ (B) ++ ("bbb") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("ababab"));
constraint
  (("aaaba") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("aa") ++ (I) ++ ("baaa") ++ (B) ++ ("baaaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("bab") ++ (B) ++ ("a") ++ (B) ++ ("abb"))
    =
    (("aaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (I) ++ (B) ++ (B) ++ ("aaabaa") ++ (B) ++ ("aaa") ++ (B) ++ ("baaaabbab") ++ (B) ++ ("abba") ++ (B) ++ ("ab") ++ (B));
constraint
  (("aaaabaaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aabbabaaa") ++ (B) ++ ("a") ++ (B) ++ ("baaaba") ++ (B) ++ ("ba") ++ (B) ++ ("aaab") ++ (B) ++ ("a") ++ (B) ++ (B))
    =
    (("aaaa") ++ (B) ++ ("aaababbaa") ++ (B) ++ ("babaaababbaaababb") ++ (I) ++ ("a") ++ (B) ++ (B) ++ ("abb"));
constraint
  ((B) ++ ("bb") ++ (B) ++ ("ababb") ++ (B) ++ ("a") ++ (B) ++ ("baabba") ++ (B) ++ ("a") ++ (B) ++ ("abbab") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("aaaa"))
    =
    (("bb") ++ (B) ++ ("baba") ++ (B) ++ ("b") ++ (B) ++ ("abbaab") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("aaaa"));
constraint
  (("aaa") ++ (B) ++ (B) ++ ("bbaa") ++ (B) ++ ("ba") ++ (B) ++ ("babaaaa") ++ (B) ++ ("b") ++ (G) ++ ("aaa") ++ (B) ++ (B) ++ ("a") ++ (I) ++ ("aaa"))
    =
    (("aaabbb") ++ (B) ++ ("aa") ++ (B) ++ ("babb") ++ (I) ++ ("aabbaa") ++ (B) ++ (B) ++ ("bb") ++ (B) ++ ("aaab") ++ (B) ++ ("a") ++ (I) ++ ("aaa"));
constraint
  (("aaa") ++ (B) ++ (B) ++ ("aa") ++ (I) ++ ("baabb") ++ (B) ++ ("aba") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("aabbbab") ++ (B) ++ ("aaba") ++ (B) ++ ("aa") ++ (B) ++ ("a"))
    =
    (("aaabbaaabaa") ++ (B) ++ ("aa") ++ (B) ++ ("bba") ++ (B) ++ ("ab") ++ (B) ++ ("aabaab") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("baa") ++ (B) ++ (I) ++ ("ba"));
constraint
  (("b") ++ (B) ++ ("aa") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ (B) ++ ("abbba") ++ (B) ++ ("a") ++ (B) ++ ("bba") ++ (B) ++ ("aabb") ++ (G) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("bb"))
    =
    ((B) ++ (B) ++ ("aab") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("abbba") ++ (B) ++ ("ab") ++ (B) ++ ("b") ++ (I) ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("abbabab") ++ (B) ++ ("b"));
constraint
  (("a") ++ (B) ++ ("baba") ++ (B) ++ ("ab") ++ (B) ++ ("abab") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("bbab") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaaaa") ++ (B) ++ ("baaab") ++ (B) ++ (B))
    =
    (("a") ++ (B) ++ ("ba") ++ (B) ++ ("aba") ++ (B) ++ (B) ++ ("aba") ++ (B) ++ ("bba") ++ (B) ++ ("b") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaaaa") ++ (B) ++ ("baaabb") ++ (B));
constraint
  (("b") ++ (B) ++ (I) ++ ("abaaabb") ++ (B) ++ ("ababa") ++ (B) ++ ("b") ++ (I) ++ (B) ++ ("aabbaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("aa"))
    =
    ((B) ++ (B) ++ (I) ++ ("a") ++ (B) ++ ("aaa") ++ (B) ++ ("b") ++ (B) ++ ("aba") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aabaa") ++ (B) ++ (B) ++ ("aab") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ (I));
constraint
  ((I) ++ ("b") ++ (B) ++ ("abba") ++ (B) ++ ("ba") ++ (B) ++ ("baaab") ++ (B) ++ ("abba") ++ (B) ++ ("ab") ++ (B) ++ ("aaa") ++ (B) ++ ("aa") ++ (B) ++ ("aababab"))
    =
    ((I) ++ ("bbab") ++ (B) ++ ("a") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("aaab") ++ (B) ++ ("abbaba") ++ (B) ++ ("baaa") ++ (B) ++ ("aabaa") ++ (B) ++ ("a") ++ (B) ++ ("ab"));
constraint
  (("b") ++ (B) ++ (G) ++ (B) ++ ("baba") ++ (B) ++ (B) ++ (B) ++ ("aba") ++ (B) ++ ("a") ++ (G) ++ ("baaab") ++ (I) ++ ("aa") ++ (B) ++ (B))
    =
    ((B) ++ ("baa") ++ (B) ++ ("bb") ++ (B) ++ (B) ++ ("bba") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("abaaa") ++ (B) ++ (B) ++ ("bb") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("abaaaa") ++ (B) ++ ("b"));
constraint
  (("ba") ++ (B) ++ (B) ++ ("aaabaab") ++ (B) ++ ("a") ++ (B) ++ ("ab") ++ (B) ++ ("baabaaabba") ++ (I) ++ (B) ++ ("ababb") ++ (B) ++ ("aaab"))
    =
    ((B) ++ ("abbaaa") ++ (B) ++ ("aa") ++ (B) ++ ("ba") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("a") ++ (I) ++ ("a") ++ (B) ++ ("ba") ++ (I) ++ (B) ++ ("aba") ++ (B) ++ (B) ++ (B) ++ ("aaab"));
constraint
  ((B) ++ (B) ++ (B) ++ ("abbbabab") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aaaaabba") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("baaaab") ++ (B) ++ ("a"))
    =
    ((B) ++ (B) ++ ("ba") ++ (B) ++ ("bba") ++ (B) ++ ("ab") ++ (B) ++ ("ab") ++ (B) ++ ("aaaaa") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("aaab") ++ (B) ++ ("bab") ++ (B) ++ ("baaaabba"));
constraint
  (("bbb") ++ (B) ++ ("bb") ++ (B) ++ (B) ++ (I) ++ ("aaaa") ++ (B) ++ ("abbb") ++ (I) ++ (B) ++ ("ab") ++ (I) ++ ("aba") ++ (B) ++ (I) ++ ("a") ++ (B))
    =
    (("b") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("bb") ++ (B) ++ (I) ++ ("aaaabab") ++ (B) ++ ("b") ++ (I) ++ ("ba") ++ (B) ++ (I) ++ ("a") ++ (B) ++ ("ababaaa") ++ (B));
constraint
  (("b") ++ (B) ++ ("aabaaaaaaaba") ++ (B) ++ (B) ++ ("aaabbaaa") ++ (B) ++ ("bbbaabbaab") ++ (B) ++ ("ab") ++ (B) ++ ("bab"))
    =
    ((B) ++ (B) ++ ("aa") ++ (B) ++ ("aaaaaaababbaaa") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("b") ++ (B) ++ ("baab") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("a") ++ (B));
constraint
  (("abaa") ++ (B) ++ ("aba") ++ (B) ++ ("b") ++ (B) ++ ("aaaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ (H) ++ (B) ++ ("a") ++ (B) ++ ("abbbabab"))
    =
    ((I) ++ ("baba") ++ (B) ++ ("b") ++ (B) ++ ("aaaa") ++ (B) ++ ("ab") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("aaab") ++ (B) ++ ("ababba") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("abab"));
constraint
  (("baa") ++ (B) ++ ("bbba") ++ (B) ++ (B) ++ ("abb") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("baaa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (B) ++ ("abbaaaa") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B))
    =
    ((B) ++ ("aab") ++ (B) ++ ("b") ++ (B) ++ ("abbab") ++ (B) ++ ("b") ++ (B) ++ ("b") ++ (B) ++ ("baaa") ++ (B) ++ ("aaab") ++ (B) ++ (B) ++ ("abbaaaabb") ++ (B) ++ ("ab") ++ (B) ++ (B));
constraint
  (("ababa") ++ (B) ++ (I) ++ ("bbaaa") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aabbaaa") ++ (I) ++ ("aabbabaa"))
    =
    (("a") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ (I) ++ ("bbaaa") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("baa") ++ (B) ++ ("baaaa") ++ (B) ++ ("aaaa") ++ (B) ++ ("b") ++ (I));
constraint
  (("aaba") ++ (B) ++ ("b") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ (B) ++ ("aaaa") ++ (B) ++ ("ab") ++ (I) ++ ("aaa") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("aa") ++ (B) ++ ("ab") ++ (B) ++ ("abaab"))
    =
    (("aabab") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B) ++ (B) ++ ("aaaa") ++ (B) ++ ("a") ++ (B) ++ ("abaaaaa") ++ (B) ++ ("ab") ++ (B) ++ ("baa") ++ (B) ++ ("ab") ++ (B) ++ (I) ++ ("b"));
constraint
  ((B) ++ ("b") ++ (B) ++ (B) ++ ("aba") ++ (B) ++ ("baaaaab") ++ (B) ++ ("aabaab") ++ (B) ++ ("baaba") ++ (B) ++ (I) ++ ("baaaaaa") ++ (B) ++ ("a"))
    =
    (("b") ++ (B) ++ ("bbaba") ++ (B) ++ ("baaaaa") ++ (B) ++ (B) ++ ("a") ++ (I) ++ ("bbbaa") ++ (B) ++ ("aba") ++ (B) ++ ("aabaaaaaaba"));
constraint
  (("b") ++ (B) ++ ("aababab") ++ (B) ++ (B) ++ ("aaaaa") ++ (B) ++ ("aa") ++ (I) ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ (I) ++ ("b") ++ (B) ++ ("aaa") ++ (B) ++ ("bbba"))
    =
    ((B) ++ ("baaba") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("aaaaabaaabaa") ++ (B) ++ ("bab") ++ (B) ++ (I) ++ ("b") ++ (B) ++ ("aaa") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("a"));
constraint
  (("a") ++ (I) ++ (G) ++ ("b") ++ (B) ++ ("aa") ++ (B) ++ ("aba") ++ (G) ++ (I) ++ ("babbbaa") ++ (B) ++ ("aa") ++ (B))
    =
    (("a") ++ (I) ++ ("aabbbb") ++ (B) ++ (B) ++ ("baa") ++ (B) ++ (I) ++ ("abbbbb") ++ (I) ++ ("babbba") ++ (I) ++ (B));
constraint
  ((B) ++ (B) ++ ("baaaaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("baaaaaaa") ++ (B) ++ ("b") ++ (B) ++ ("bab") ++ (B) ++ ("ababb") ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (B) ++ ("ab") ++ (B) ++ ("b"))
    =
    (("bbbaaaaabab") ++ (B) ++ (B) ++ ("aaaaaaa") ++ (B) ++ (B) ++ ("bbab") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("abba") ++ (B) ++ ("abbb"));
constraint
  (("b") ++ (B) ++ ("baaaaaa") ++ (B) ++ (B) ++ ("baaab") ++ (B) ++ ("aababb") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("aaaa") ++ (B) ++ (B) ++ ("baaaa"))
    =
    ((B) ++ ("bbaaaaaab") ++ (B) ++ (B) ++ ("aaabbaababbbaabbaabaaaabb") ++ (B) ++ ("aaaa"));
constraint
  (("aaa") ++ (B) ++ ("ba") ++ (B) ++ (B) ++ ("aaaa") ++ (B) ++ (B) ++ ("baa") ++ (I) ++ (B) ++ ("a") ++ (I) ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("aaa") ++ (I) ++ ("a") ++ (B))
    =
    (("aaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("baaaa") ++ (B) ++ (B) ++ ("baa") ++ (I) ++ ("ba") ++ (I) ++ (B) ++ ("aaab") ++ (B) ++ ("aaa") ++ (I) ++ ("ab"));
constraint
  (("ba") ++ (B) ++ ("b") ++ (B) ++ ("aaaaabbaa") ++ (B) ++ ("ba") ++ (B) ++ ("aabbbabaa") ++ (B) ++ ("aaabb") ++ (B) ++ (B) ++ ("baa") ++ (B) ++ ("aaa"))
    =
    (("ba") ++ (B) ++ (B) ++ ("baaaaa") ++ (B) ++ (B) ++ ("aabb") ++ (I) ++ ("b") ++ (B) ++ (B) ++ ("abaabaaa") ++ (B) ++ (B) ++ ("bbba") ++ (I) ++ ("a"));
constraint
  (("a") ++ (B) ++ ("aa") ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("abbbaaaba") ++ (B) ++ ("ab") ++ (B) ++ ("bba") ++ (B) ++ ("aaaa") ++ (B) ++ (B) ++ ("a") ++ (B))
    =
    ((I) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("baabb") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("aaababa") ++ (B) ++ (B) ++ ("b") ++ (B) ++ (I) ++ ("aab") ++ (B) ++ ("ab"));
constraint
  (("a") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("a") ++ (B) ++ ("ababa") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("ba") ++ (B) ++ ("b") ++ (B) ++ ("ba") ++ (B) ++ ("baba") ++ (B) ++ ("abaaabbba"))
    =
    (("a") ++ (B) ++ ("a") ++ (B) ++ ("bb") ++ (B) ++ ("ababababa") ++ (B) ++ (B) ++ ("bbba") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("ab") ++ (I) ++ ("a") ++ (B) ++ (B) ++ ("ba"));
constraint
  (("abb") ++ (B) ++ ("aaa") ++ (B) ++ ("a") ++ (B) ++ ("aaaab") ++ (B) ++ ("baaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("aaaaba") ++ (B) ++ ("babbaa") ++ (B) ++ ("ab"))
    =
    (("ab") ++ (B) ++ (B) ++ ("aaaba") ++ (B) ++ ("aaaabb") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ ("ab") ++ (I) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ ("ba") ++ (B) ++ ("baa") ++ (B) ++ ("a") ++ (B));
constraint
  (("bb") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("babaaba") ++ (I) ++ ("babaabbaa") ++ (B) ++ ("b") ++ (B) ++ (I) ++ ("aaba") ++ (B) ++ ("a") ++ (B) ++ ("a"))
    =
    (("bb") ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ (I) ++ (B) ++ ("aa") ++ (B) ++ ("aab") ++ (I) ++ (B) ++ ("baabb") ++ (B) ++ (I) ++ ("aa") ++ (B) ++ ("aba") ++ (B) ++ ("a"));
constraint
  (("b") ++ (B) ++ ("b") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("baabbaababba") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("ab") ++ (B) ++ ("aaaaa") ++ (I) ++ ("b") ++ (B) ++ ("abaa"))
    =
    (("bbbb") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aabbaa") ++ (B) ++ ("abbaba") ++ (B) ++ (B) ++ ("bab") ++ (B) ++ ("aaaaaabaa") ++ (B) ++ ("babaa"));
constraint
  (("a") ++ (I) ++ ("b") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ ("ababaabaa") ++ (B) ++ ("aaaabbabaa") ++ (B) ++ (B) ++ ("baababb"))
    =
    (("a") ++ (I) ++ (B) ++ (B) ++ ("baaba") ++ (B) ++ ("a") ++ (B) ++ ("abaa") ++ (B) ++ ("aabaaaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("aab") ++ (B) ++ (B) ++ ("aabab") ++ (B));
constraint
  (("aaaa") ++ (B) ++ ("aba") ++ (B) ++ ("ba") ++ (B) ++ (I) ++ ("aabab") ++ (B) ++ ("a") ++ (B) ++ ("aaba") ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (G) ++ (B) ++ ("a"))
    =
    (("aaaababa") ++ (B) ++ ("ba") ++ (B) ++ ("abaaaabab") ++ (B) ++ ("a") ++ (B) ++ ("aab") ++ (I) ++ (B) ++ ("aaabbbb") ++ (B) ++ ("ba"));
constraint
  ((B) ++ (B) ++ ("baab") ++ (B) ++ ("aabaabb") ++ (I) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("abb") ++ (B) ++ ("baa") ++ (B) ++ ("a") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (I) ++ ("a"))
    =
    ((B) ++ ("b") ++ (B) ++ ("aab") ++ (B) ++ ("aa") ++ (B) ++ ("aab") ++ (B) ++ (I) ++ ("a") ++ (B) ++ ("bba") ++ (B) ++ (B) ++ ("bbaa") ++ (B) ++ (I) ++ ("abba") ++ (B) ++ ("aaa"));
constraint
  (("aba") ++ (B) ++ ("baa") ++ (B) ++ ("abaaa") ++ (B) ++ (B) ++ ("aaaabaaa") ++ (B) ++ ("bbaabbb") ++ (B) ++ (I) ++ ("ba") ++ (I))
    =
    (("aba") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (I) ++ ("ab") ++ (B) ++ ("aaaabaaa") ++ (B) ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("bbbabaa") ++ (B) ++ ("a") ++ (I));
constraint
  (("aa") ++ (B) ++ (B) ++ (B) ++ ("aaaaaa") ++ (B) ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("aabaaa") ++ (B) ++ ("bbbaabababbaabbab") ++ (B))
    =
    (("aa") ++ (B) ++ ("b") ++ (B) ++ ("aaaaaabb") ++ (B) ++ ("ba") ++ (B) ++ ("aabaaa") ++ (B) ++ ("b") ++ (B) ++ ("baaba") ++ (B) ++ ("abbaab") ++ (B) ++ ("ab") ++ (B));
constraint
  ((I) ++ ("b") ++ (B) ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("b") ++ (B) ++ (I) ++ (I) ++ (B) ++ ("bbabaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("baaa") ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B))
    =
    ((I) ++ ("bb") ++ (B) ++ ("bba") ++ (B) ++ ("bb") ++ (I) ++ ("abaab") ++ (B) ++ (B) ++ (I) ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("bb") ++ (B) ++ ("ab"));
constraint
  (("baaa") ++ (B) ++ ("bab") ++ (B) ++ ("ba") ++ (B) ++ ("a") ++ (B) ++ ("bbbb") ++ (B) ++ ("aa") ++ (B) ++ ("aaa") ++ (B) ++ (B) ++ (I) ++ (B) ++ ("aaba") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("ab"))
    =
    ((B) ++ ("aaab") ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("aba") ++ (B) ++ ("bbbb") ++ (B) ++ ("aabaaa") ++ (B) ++ ("b") ++ (I) ++ ("baaba") ++ (B) ++ ("bb") ++ (B) ++ ("a") ++ (B));
constraint
  (("baaaaaaa") ++ (B) ++ ("b") ++ (B) ++ ("ab") ++ (B) ++ (G) ++ (B) ++ ("aabaaa") ++ (B) ++ (B) ++ (B) ++ ("aaabb") ++ (B) ++ ("baaa") ++ (B))
    =
    ((B) ++ ("aaaaaaabb") ++ (B) ++ ("a") ++ (B) ++ ("baabb") ++ (B) ++ (B) ++ (B) ++ ("baabaaa") ++ (B) ++ ("bbaaa") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aaa") ++ (B));
constraint
  (("b") ++ (B) ++ (B) ++ ("aa") ++ (I) ++ (B) ++ (B) ++ ("aaaaaabbabaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("aaabaabba") ++ (B) ++ ("a") ++ (B) ++ ("a"))
    =
    ((B) ++ (B) ++ (B) ++ ("aa") ++ (I) ++ ("b") ++ (B) ++ ("aaaaaab") ++ (B) ++ (I) ++ ("bab") ++ (B) ++ ("b") ++ (B) ++ ("aa") ++ (I) ++ (B) ++ ("baba") ++ (B) ++ ("a"));
constraint
  (("aaa") ++ (B) ++ ("aba") ++ (B) ++ ("ab") ++ (B) ++ ("baaaaabab") ++ (B) ++ (B) ++ ("aaaaba") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("baaaaa") ++ (B) ++ ("a"))
    =
    (("aaa") ++ (B) ++ ("ababa") ++ (B) ++ ("b") ++ (B) ++ ("aaaaa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("aaaaba") ++ (B) ++ ("bbbab") ++ (B) ++ ("aaaaaba"));
constraint
  ((I) ++ ("a") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("bb") ++ (B) ++ ("ab") ++ (B) ++ ("abaab") ++ (B) ++ (I) ++ ("abbaaabaabaaaaaa") ++ (B))
    =
    ((I) ++ ("a") ++ (B) ++ ("bbb") ++ (B) ++ (B) ++ ("bab") ++ (B) ++ (I) ++ ("b") ++ (B) ++ (I) ++ ("a") ++ (B) ++ (B) ++ ("aa") ++ (I) ++ (B) ++ ("aaaaaa") ++ (B));
constraint
  ((B) ++ ("bbaaa") ++ (B) ++ ("bbaaabab") ++ (B) ++ ("ba") ++ (I) ++ (B) ++ ("a") ++ (B) ++ ("aaaaaa") ++ (B) ++ ("baa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("aa"))
    =
    ((B) ++ (B) ++ (B) ++ ("aaa") ++ (B) ++ ("bbaaa") ++ (B) ++ ("a") ++ (B) ++ ("bbaabaab") ++ (I) ++ ("aaaa") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("baa"));
constraint
  (("aa") ++ (B) ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("bbaabbb") ++ (B) ++ ("aab") ++ (B) ++ ("a") ++ (B) ++ ("bab") ++ (B) ++ (B) ++ ("ababaaaaaaaaaab"))
    =
    (("aa") ++ (B) ++ (B) ++ (B) ++ ("ba") ++ (B) ++ ("b") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("bbaabbabbabb") ++ (B) ++ ("a") ++ (B) ++ ("abaaaaaaaaaab"));
constraint
  (("a") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("abaabaaba") ++ (B) ++ ("a") ++ (B) ++ ("ab") ++ (G) ++ ("a") ++ (B) ++ ("aa") ++ (B) ++ (I) ++ (I) ++ ("a") ++ (B))
    =
    (("a") ++ (B) ++ ("bba") ++ (B) ++ (I) ++ ("baababa") ++ (B) ++ ("a") ++ (B) ++ ("aa") ++ (B) ++ ("b") ++ (B) ++ ("b") ++ (B) ++ ("abaa") ++ (B) ++ (I) ++ (I) ++ ("ab"));
constraint
  ((G) ++ (I) ++ (B) ++ (B) ++ ("babb") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aab") ++ (I) ++ ("a") ++ (G) ++ ("aaaaaa"))
    =
    (("aa") ++ (B) ++ ("bb") ++ (B) ++ (B) ++ (I) ++ (B) ++ ("bba") ++ (B) ++ ("b") ++ (B) ++ ("abbaa") ++ (B) ++ (I) ++ ("aaab") ++ (B) ++ (B) ++ ("bbaaaaaa"));
constraint
  ((B) ++ (B) ++ ("b") ++ (I) ++ ("babaaaabaabbabb") ++ (I) ++ ("a") ++ (I) ++ ("aabab") ++ (B) ++ ("abbab"))
    =
    (("bb") ++ (B) ++ (I) ++ (B) ++ ("abaaa") ++ (I) ++ ("b") ++ (B) ++ ("ab") ++ (B) ++ ("a") ++ (B) ++ ("aaa") ++ (I) ++ ("aa") ++ (B) ++ ("ab") ++ (B) ++ ("abba") ++ (B));
constraint
  (("aab") ++ (B) ++ ("a") ++ (G) ++ (B) ++ ("aaabaaa") ++ (B) ++ (B) ++ (I) ++ ("ba") ++ (I) ++ (B) ++ ("ba") ++ (B) ++ ("baaaaa"))
    =
    (("aabbaaabb") ++ (B) ++ ("bb") ++ (B) ++ ("aaabaaa") ++ (B) ++ (B) ++ (I) ++ (B) ++ ("a") ++ (I) ++ (B) ++ ("bab") ++ (B) ++ ("aaaaa"));
constraint
  (("ab") ++ (B) ++ ("ba") ++ (B) ++ ("aaaaa") ++ (B) ++ ("baaaa") ++ (B) ++ ("babbaaaababab") ++ (B) ++ ("babbba") ++ (B) ++ ("a") ++ (B) ++ ("a"))
    =
    (("ab") ++ (B) ++ ("b") ++ (I) ++ ("aaa") ++ (B) ++ ("baaaab") ++ (B) ++ ("abbaaaa") ++ (B) ++ ("a") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("a"));
constraint
  (("aaab") ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("baaa") ++ (B) ++ (B) ++ ("baba") ++ (B) ++ ("bbbaabbab") ++ (I) ++ (B) ++ ("aabba") ++ (B) ++ ("ab"))
    =
    (("aaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("a") ++ (B) ++ ("baaabb") ++ (B) ++ ("a") ++ (B) ++ ("abb") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("baba") ++ (B) ++ ("aa") ++ (B) ++ ("aa") ++ (B) ++ ("babab"));
constraint
  ((I) ++ ("abbaaba") ++ (B) ++ ("abbbbb") ++ (B) ++ (B) ++ (B) ++ (B) ++ ("b") ++ (B) ++ ("aabbb") ++ (I) ++ ("baa") ++ (B) ++ (B) ++ (I))
    =
    ((I) ++ ("a") ++ (B) ++ ("baa") ++ (B) ++ ("a") ++ (B) ++ ("abbb") ++ (B) ++ ("bb") ++ (B) ++ ("b") ++ (B) ++ ("b") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ (B) ++ (I) ++ (B) ++ ("aabb") ++ (I));
constraint
  (("aaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("babb") ++ (B) ++ ("baaaaa") ++ (B) ++ ("babbaaab") ++ (B) ++ ("b") ++ (B) ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ ("aaa"))
    =
    (("aaabbab") ++ (B) ++ ("a") ++ (B) ++ ("ba") ++ (B) ++ ("bbbaaaaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aaabb") ++ (B) ++ ("b") ++ (B) ++ ("aa") ++ (B) ++ ("abaaa"));
constraint
  (("bab") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("aaba") ++ (B) ++ ("aaba") ++ (B) ++ (I) ++ ("aa") ++ (B) ++ ("abb") ++ (B) ++ ("aa") ++ (B) ++ ("ba") ++ (I) ++ (B) ++ ("aaba"))
    =
    ((B) ++ ("abba") ++ (B) ++ ("baab") ++ (I) ++ ("ba") ++ (B) ++ (I) ++ ("aa") ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("a") ++ (I) ++ ("baa") ++ (B) ++ ("a"));
constraint
  (("a") ++ (B) ++ ("baaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ (B) ++ (I) ++ ("aa") ++ (B) ++ ("aaaaaaaaaa") ++ (B) ++ ("ba") ++ (B) ++ (I) ++ ("ab"))
    =
    (("abbaaa") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("ab") ++ (B) ++ (B) ++ ("abaaaabaaaaaaaaaa") ++ (B) ++ ("baba") ++ (B) ++ ("aaa") ++ (B));
constraint
  (("bba") ++ (G) ++ ("ba") ++ (B) ++ ("a") ++ (B) ++ ("abbbb") ++ (B) ++ ("ba") ++ (I) ++ (B) ++ ("bbaba") ++ (B) ++ (B) ++ ("ab") ++ (B) ++ ("ba") ++ (B) ++ ("a"))
    =
    (("bbaaa") ++ (B) ++ ("b") ++ (B) ++ (B) ++ (B) ++ ("baba") ++ (B) ++ ("ab") ++ (B) ++ ("bb") ++ (B) ++ ("ba") ++ (I) ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("abba") ++ (B) ++ ("b") ++ (B) ++ ("aba"));
constraint
  (("a") ++ (B) ++ ("a") ++ (B) ++ ("aaaaa") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ ("baaaababbaaaa") ++ (B) ++ ("abaa") ++ (B) ++ (B) ++ (B) ++ ("baa") ++ (B) ++ ("b") ++ (B) ++ (B))
    =
    (("ab") ++ (I) ++ ("aa") ++ (I) ++ (B) ++ (B) ++ (B) ++ ("aaaaba") ++ (B) ++ ("baaaa") ++ (B) ++ ("a") ++ (B) ++ ("aa") ++ (B) ++ ("b") ++ (B) ++ ("baa") ++ (B) ++ (B) ++ ("bb"));
constraint
  (("ab") ++ (I) ++ ("abbaaa") ++ (I) ++ (B) ++ (B) ++ ("aaba") ++ (B) ++ ("aaaabaabbaaaab") ++ (B) ++ ("ab") ++ (B) ++ ("a"))
    =
    (("a") ++ (B) ++ (I) ++ ("a") ++ (B) ++ ("baaaabaabbaa") ++ (B) ++ ("abaaaa") ++ (B) ++ ("aa") ++ (B) ++ ("baaaa") ++ (B) ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("a"));
constraint
  (("baaa") ++ (B) ++ (B) ++ (I) ++ (B) ++ ("aabbaaaaaab") ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ ("ba") ++ (B) ++ ("b") ++ (B) ++ ("aababab"))
    =
    (("baaa") ++ (B) ++ (B) ++ ("abaabaab") ++ (B) ++ ("aaaaaa") ++ (B) ++ ("b") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("bab") ++ (B) ++ ("a") ++ (B) ++ (B) ++ ("baaba") ++ (B) ++ ("a") ++ (B));
constraint
  ((I) ++ ("ba") ++ (I) ++ ("abb") ++ (I) ++ ("a") ++ (B) ++ ("aa") ++ (B) ++ (B) ++ (B) ++ ("aaaaa") ++ (I) ++ ("aaa") ++ (B) ++ ("baaba"))
    =
    (("abaabaabaaab") ++ (B) ++ (I) ++ ("abaa") ++ (B) ++ (B) ++ ("baaaaaa") ++ (B) ++ ("aaaaa") ++ (B) ++ ("baa") ++ (B) ++ ("a"));
solve satisfy;
