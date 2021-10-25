var string: I;
var string: H;
var string: G;
var string: F;
var string: C;
constraint
  (("aa") ++ (I) ++ ("a") ++ (I) ++ ("b") ++ (I) ++ (I) ++ (H) ++ (G) ++ (I) ++ ("a"))
    = (("aabab") ++ (I) ++ ("bb") ++ (H) ++ ("aaaaa") ++ (I) ++ ("abaaabba"));
constraint
  (("aaaaaaa") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ (H) ++ ("a") ++ (I) ++ ("aaa") ++ (I) ++ ("aaaa"))
    =
    (("aaaaaaaba") ++ (I) ++ ("aba") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("aaabaaaa"));
constraint
  (("a") ++ (F) ++ ("a") ++ (H) ++ ("aaaa") ++ (I) ++ (F) ++ ("abb") ++ (I) ++ ("aaa"))
    =
    (("a") ++ (I) ++ ("ab") ++ (I) ++ ("aa") ++ (I) ++ ("baaaa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("ba") ++ (I) ++ ("b") ++ (I) ++ ("aaa"));
constraint
  (("aab") ++ (I) ++ (I) ++ ("b") ++ (I) ++ ("aa") ++ (I) ++ (F) ++ (I) ++ (I) ++ ("bbaa") ++ (I) ++ ("aaaa"))
    =
    (("aab") ++ (I) ++ (I) ++ ("bba") ++ (H) ++ ("ab") ++ (I) ++ ("b") ++ (I) ++ ("bbaa") ++ (I) ++ ("aaaa"));
constraint
  (("abaaababa") ++ (H) ++ (I) ++ ("aab") ++ (I) ++ ("abaaaba") ++ (I)) =
    (("abaaaba") ++ (I) ++ ("a") ++ (H) ++ ("baa") ++ (I) ++ ("ba") ++ (I) ++ ("aaa") ++ (I) ++ ("a") ++ (I));
constraint
  (("aa") ++ (I) ++ ("ab") ++ (I) ++ (H) ++ (H) ++ ("babaaaaaaa") ++ (H)) =
    (("aa") ++ (I) ++ ("a") ++ (I) ++ (F) ++ ("a") ++ (I) ++ ("bbabaaaaaaa") ++ (H));
constraint
  (("aabaaabaabababb") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ (H) ++ ("ba"))
    =
    (("aa") ++ (I) ++ ("aaabaababa") ++ (I) ++ (I) ++ (I) ++ ("ababab") ++ (I) ++ ("ba"));
constraint
  (("a") ++ (H) ++ ("a") ++ (I) ++ ("a") ++ (H) ++ ("ababa") ++ (I) ++ ("aaaaa") ++ (I) ++ ("aab"))
    =
    (("aab") ++ (I) ++ ("abaa") ++ (I) ++ ("ba") ++ (I) ++ ("a") ++ (I) ++ ("abaaaaa") ++ (I) ++ ("aa") ++ (I));
constraint
  (("b") ++ (I) ++ (C) ++ ("bbba") ++ (H) ++ ("b") ++ (I) ++ (I) ++ ("aaaa") ++ (I) ++ ("aa"))
    =
    (("b") ++ (I) ++ (I) ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("b") ++ (I) ++ (I) ++ (I) ++ ("aabb") ++ (I) ++ (I) ++ (I) ++ ("aaaa") ++ (I) ++ ("aa"));
constraint
  ((I) ++ ("aabaabaa") ++ (I) ++ ("ba") ++ (H) ++ (I) ++ (I) ++ ("a") ++ (F) ++ (H))
    =
    (("baa") ++ (I) ++ ("aa") ++ (I) ++ ("aab") ++ (I) ++ ("aa") ++ (I) ++ ("bbba") ++ (F) ++ ("ab") ++ (I));
constraint
  (("a") ++ (I) ++ ("a") ++ (F) ++ ("a") ++ (I) ++ ("a") ++ (H) ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (H) ++ ("b") ++ (I) ++ ("ab"))
    =
    (("ababa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("aab") ++ (I) ++ (I) ++ ("aabaa") ++ (I) ++ ("b") ++ (I) ++ (I) ++ ("ab"));
constraint
  (("a") ++ (I) ++ (I) ++ (I) ++ ("aba") ++ (F) ++ (F) ++ ("aa") ++ (I) ++ (I) ++ ("aaaaaa"))
    =
    (("a") ++ (I) ++ ("b") ++ (I) ++ ("ababa") ++ (I) ++ ("b") ++ (F) ++ ("a") ++ (H) ++ ("aaaaaa"));
constraint
  (("a") ++ (I) ++ ("aaaaaa") ++ (I) ++ ("aaaab") ++ (H) ++ (H) ++ ("bbaaa"))
    =
    (("abaaaaaa") ++ (I) ++ ("aaaaba") ++ (I) ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("bbaaa"));
constraint
  (("a") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("abaaabaaa") ++ (I) ++ ("a") ++ (F) ++ ("aa") ++ (I))
    =
    (("a") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (I) ++ ("aaa") ++ (I) ++ ("aaaba") ++ (I) ++ ("ab") ++ (I) ++ ("aab"));
constraint
  (("abaaabaab") ++ (I) ++ (I) ++ ("aaa") ++ (I) ++ ("aa") ++ (I) ++ (F) ++ ("aab"))
    =
    (("a") ++ (I) ++ ("aaa") ++ (I) ++ ("aa") ++ (I) ++ ("bbaaa") ++ (I) ++ ("a") ++ (H) ++ ("ab") ++ (I) ++ ("aa") ++ (I));
constraint
  ((I) ++ (I) ++ ("abbab") ++ (I) ++ ("bba") ++ (H) ++ ("aa") ++ (I) ++ ("ab") ++ (I) ++ ("a") ++ (H) ++ ("a"))
    =
    ((I) ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("bb") ++ (I) ++ ("aab") ++ (I) ++ ("aaba") ++ (I) ++ (I) ++ ("aab") ++ (I) ++ ("a"));
constraint
  (("aaaaaaaaaa") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("aaa") ++ (I) ++ (I) ++ ("bbaa"))
    =
    (("aaaaaaaaaa") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("aa") ++ (H) ++ (I) ++ ("baa"));
constraint
  (("baa") ++ (F) ++ ("bb") ++ (I) ++ ("baa") ++ (I) ++ (I) ++ ("a") ++ (H) ++ ("abaaaa"))
    =
    (("baaba") ++ (I) ++ ("bbbb") ++ (I) ++ ("aab") ++ (I) ++ ("aa") ++ (I) ++ ("babaaaa"));
constraint
  ((I) ++ (I) ++ ("baaaa") ++ (H) ++ ("aababa") ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (I) ++ ("aaa"))
    =
    ((I) ++ ("b") ++ (I) ++ ("aaaa") ++ (H) ++ ("aaba") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("abaaa"));
constraint
  ((I) ++ (I) ++ ("aa") ++ (H) ++ ("aaabb") ++ (I) ++ ("aaa") ++ (I) ++ (H) ++ ("aa") ++ (I) ++ (I) ++ ("b"))
    =
    (("b") ++ (I) ++ ("aaa") ++ (I) ++ ("baaa") ++ (I) ++ (I) ++ ("baaa") ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("aa") ++ (I) ++ (I) ++ (I));
constraint
  ((H) ++ ("b") ++ (I) ++ ("baaab") ++ (F) ++ (I) ++ (I) ++ ("aaa") ++ (I) ++ (I) ++ (I) ++ (I) ++ ("aa"))
    =
    (("abb") ++ (I) ++ (I) ++ (I) ++ ("aaa") ++ (I) ++ (I) ++ ("ab") ++ (I) ++ ("bbaaab") ++ (I) ++ (I) ++ (I) ++ ("aa"));
constraint
  (("aabaaaaa") ++ (I) ++ ("b") ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ ("baa") ++ (H) ++ ("ab"))
    =
    (("aabaaaaab") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ (I) ++ (I) ++ ("baaab") ++ (I) ++ ("ab"));
constraint
  (("ba") ++ (I) ++ ("aba") ++ (I) ++ ("abaab") ++ (I) ++ ("aa") ++ (I) ++ ("b") ++ (I) ++ ("baaabaa"))
    =
    ((I) ++ ("abababa") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("b") ++ (I) ++ ("aaabaa"));
constraint
  ((H) ++ ("a") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (H) ++ ("aaaaabaa") ++ (I) ++ ("aa"))
    =
    (("ab") ++ (I) ++ ("a") ++ (I) ++ ("ba") ++ (I) ++ ("abaa") ++ (I) ++ ("baaaaabaa") ++ (I) ++ ("aa"));
constraint
  ((H) ++ ("aabaa") ++ (I) ++ ("b") ++ (I) ++ ("aaa") ++ (H) ++ ("baabaaa") ++ (I))
    =
    (("ab") ++ (I) ++ ("aa") ++ (I) ++ ("aab") ++ (I) ++ ("baaa") ++ (H) ++ ("baabaaa") ++ (I));
constraint
  (("baaaa") ++ (I) ++ (H) ++ (I) ++ ("a") ++ (I) ++ ("aaa") ++ (H) ++ (I) ++ ("aa") ++ (H) ++ ("a"))
    =
    (("baaaa") ++ (I) ++ ("ab") ++ (I) ++ ("ba") ++ (I) ++ ("aaaab") ++ (I) ++ ("baaabba"));
constraint
  ((I) ++ (H) ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("bb") ++ (I) ++ ("bbaba") ++ (I) ++ ("abaa"))
    =
    ((I) ++ ("a") ++ (I) ++ (I) ++ ("ba") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (H) ++ ("bb") ++ (I) ++ (I) ++ ("aba") ++ (I) ++ ("abaa"));
constraint
  (("aa") ++ (I) ++ (H) ++ ("baaabaa") ++ (I) ++ ("a") ++ (I) ++ ("aaaa") ++ (H) ++ ("aa"))
    =
    (("aa") ++ (I) ++ ("a") ++ (I) ++ (I) ++ (I) ++ ("aaabaa") ++ (I) ++ ("abaaaaab") ++ (I) ++ ("aa"));
constraint
  (("baaaaaaab") ++ (I) ++ ("b") ++ (I) ++ ("baa") ++ (H) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("aab"))
    =
    (("baaaaaaa") ++ (I) ++ ("bbbbaaa") ++ (I) ++ ("ba") ++ (I) ++ ("abaa") ++ (I));
constraint
  (("bb") ++ (I) ++ (I) ++ ("a") ++ (H) ++ ("a") ++ (I) ++ ("aaa") ++ (H) ++ (I) ++ ("aa") ++ (H) ++ ("b") ++ (I) ++ ("b"))
    =
    (("bbb") ++ (I) ++ ("a") ++ (H) ++ ("abaaaab") ++ (I) ++ ("baaa") ++ (I) ++ (I) ++ ("b") ++ (I) ++ (I));
constraint
  (("a") ++ (I) ++ ("aaaaaa") ++ (I) ++ ("aba") ++ (H) ++ (I) ++ (I) ++ (I) ++ ("aa") ++ (H) ++ ("ab"))
    =
    (("a") ++ (I) ++ ("aaaaaa") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("b") ++ (I) ++ (I) ++ ("baaa") ++ (I) ++ ("ba") ++ (I));
constraint
  (("b") ++ (I) ++ (H) ++ (I) ++ ("a") ++ (I) ++ ("aaa") ++ (I) ++ ("abaa") ++ (I) ++ (H) ++ (I) ++ ("abaa"))
    =
    (("b") ++ (I) ++ ("abb") ++ (I) ++ ("a") ++ (I) ++ ("aaa") ++ (I) ++ ("abaaba") ++ (I) ++ (I) ++ (I) ++ ("abaa"));
constraint
  (("aaaaaa") ++ (I) ++ ("a") ++ (I) ++ ("b") ++ (I) ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("bbbbb") ++ (I) ++ ("baaa"))
    =
    (("aaaaaa") ++ (F) ++ ("b") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ ("bb") ++ (I) ++ ("b") ++ (I) ++ ("baaa"));
constraint
  (("aaa") ++ (I) ++ ("ab") ++ (H) ++ (I) ++ (I) ++ ("b") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("b") ++ (I) ++ (I) ++ (I) ++ (I) ++ ("baa"))
    =
    (("aaaba") ++ (I) ++ ("abbbb") ++ (I) ++ ("baa") ++ (I) ++ (I) ++ ("b") ++ (I) ++ (I) ++ ("bb") ++ (I) ++ ("aa"));
constraint
  ((I) ++ ("b") ++ (C) ++ ("aab") ++ (C) ++ ("a") ++ (I) ++ ("aba") ++ (I) ++ ("aa"))
    =
    ((I) ++ ("bb") ++ (I) ++ (I) ++ (H) ++ ("aa") ++ (I) ++ ("b") ++ (I) ++ ("ba") ++ (I) ++ ("baba") ++ (I) ++ ("abaa"));
constraint
  (("a") ++ (H) ++ (I) ++ (H) ++ ("baa") ++ (H) ++ ("aaababaaaaa")) =
    (("aa") ++ (I) ++ ("b") ++ (I) ++ (H) ++ (I) ++ ("aaa") ++ (I) ++ (I) ++ ("aaaba") ++ (I) ++ ("aaaaa"));
constraint
  (("a") ++ (I) ++ (I) ++ ("b") ++ (I) ++ ("aaaaabba") ++ (C) ++ (F) ++ ("a") ++ (I))
    =
    (("abbbbaaaaab") ++ (I) ++ ("abb") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ (H) ++ ("ab"));
constraint
  ((I) ++ ("aaababa") ++ (I) ++ (I) ++ ("baabaaaabbaa") ++ (I) ++ (I) ++ ("a"))
    =
    ((I) ++ ("aaaba") ++ (I) ++ ("abbbaa") ++ (I) ++ ("aaaa") ++ (I) ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("a"));
constraint
  (("aaaba") ++ (I) ++ ("aa") ++ (I) ++ ("abaaab") ++ (I) ++ ("aa") ++ (I) ++ ("abaaaa"))
    =
    (("aaa") ++ (I) ++ ("a") ++ (I) ++ ("aababaaab") ++ (I) ++ ("aababaaaa"));
constraint
  ((I) ++ ("abaaaaaaaa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("aaa") ++ (F) ++ ("b"))
    =
    (("ba") ++ (I) ++ ("aaaaaaaa") ++ (I) ++ ("ba") ++ (I) ++ ("abaaabab") ++ (I) ++ (I));
constraint
  (("abaa") ++ (I) ++ ("a") ++ (I) ++ ("aabbbbbbba") ++ (F) ++ ("aaaa")) =
    (("a") ++ (I) ++ ("aababaab") ++ (I) ++ ("bb") ++ (I) ++ ("bba") ++ (I) ++ ("a") ++ (I) ++ ("baaaa"));
constraint
  (("a") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ (H) ++ ("a") ++ (I) ++ (I) ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("baa") ++ (I) ++ ("a") ++ (H))
    =
    (("ababa") ++ (I) ++ ("a") ++ (I) ++ (F) ++ ("baa") ++ (I) ++ ("b") ++ (I) ++ ("aa") ++ (I) ++ ("aab") ++ (I));
constraint
  (("aba") ++ (I) ++ ("aa") ++ (I) ++ ("aaaa") ++ (I) ++ (I) ++ ("aabbaaa") ++ (I) ++ ("aa") ++ (I) ++ (I))
    =
    (("ababaa") ++ (I) ++ ("aaaa") ++ (I) ++ ("baabbaaabaa") ++ (I) ++ ("b"));
constraint
  (("a") ++ (I) ++ (F) ++ (I) ++ (I) ++ ("bbb") ++ (I) ++ (I) ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("aaaabbaa"))
    =
    (("a") ++ (I) ++ (I) ++ ("ab") ++ (I) ++ ("bbb") ++ (I) ++ ("bb") ++ (I) ++ (I) ++ ("ba") ++ (I) ++ ("aaaab") ++ (I) ++ ("aa"));
constraint
  (("bb") ++ (F) ++ ("abaaa") ++ (F) ++ ("aaba") ++ (F) ++ ("ba")) =
    ((I) ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("ba") ++ (I) ++ ("aaa") ++ (I) ++ ("abbaaba") ++ (I) ++ ("a") ++ (I) ++ ("b") ++ (I) ++ ("a"));
constraint
  (("aaaaa") ++ (I) ++ ("aaabaaaaaaa") ++ (I) ++ ("aa") ++ (I) ++ ("aaaa")) =
    (("aaaaabaaa") ++ (I) ++ ("aaaaaaa") ++ (I) ++ ("aabaaaa"));
constraint
  (("b") ++ (I) ++ (I) ++ (I) ++ ("aaabaa") ++ (I) ++ ("a") ++ (F) ++ ("bbb") ++ (C))
    =
    (("bb") ++ (I) ++ ("baaa") ++ (I) ++ ("aa") ++ (I) ++ ("abab") ++ (I) ++ ("bbbb") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("b"));
constraint
  (("aa") ++ (H) ++ (I) ++ ("aaaaa") ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (H) ++ (I) ++ ("baba") ++ (I))
    =
    (("aaabb") ++ (I) ++ ("aaaaa") ++ (I) ++ ("aa") ++ (I) ++ ("aa") ++ (I) ++ ("bbba") ++ (I) ++ ("ab"));
constraint
  (("bbb") ++ (I) ++ (I) ++ (I) ++ ("a") ++ (H) ++ ("aa") ++ (I) ++ (C) ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("bb"))
    =
    (("b") ++ (I) ++ ("b") ++ (I) ++ ("bbaa") ++ (I) ++ ("baa") ++ (I) ++ ("b") ++ (I) ++ ("ba") ++ (I) ++ (I) ++ ("b") ++ (I) ++ ("ab") ++ (I) ++ ("b"));
constraint
  (("aaaa") ++ (I) ++ ("aaaabaaaaaa") ++ (I) ++ ("aaa") ++ (I) ++ ("aaaa")) =
    (("aaaabaaaabaaaaaabaaabaaaa") ++ (""));
constraint
  (("aabaaaaaaa") ++ (I) ++ (I) ++ (I) ++ ("abaaa") ++ (I) ++ ("aaaaa") ++ (I))
    =
    (("aa") ++ (I) ++ ("aaaaaaab") ++ (I) ++ ("ba") ++ (I) ++ ("aaabaaaaab"));
constraint
  (("aaabbab") ++ (I) ++ ("aaaaaaaaaaaaabba") ++ (I)) =
    (("aaa") ++ (I) ++ ("ba") ++ (I) ++ ("baaaaaaaaaaaaa") ++ (I) ++ ("bab"));
constraint
  ((F) ++ ("b") ++ (I) ++ ("baaaaaaaabaaaaaabba")) =
    ((I) ++ ("a") ++ (I) ++ (I) ++ (I) ++ (I) ++ ("baaaaaaaabaaaaaabba"));
constraint
  (("baaa") ++ (I) ++ (I) ++ ("bbb") ++ (I) ++ (I) ++ (I) ++ ("aaaa") ++ (H) ++ (C))
    =
    (("baaabbb") ++ (I) ++ (I) ++ (I) ++ ("bbaaaaa") ++ (I) ++ (I) ++ ("b") ++ (I) ++ ("b") ++ (H));
constraint
  (("aa") ++ (I) ++ ("ba") ++ (H) ++ ("aa") ++ (I) ++ ("bba") ++ (I) ++ ("aaa") ++ (I) ++ ("aaaaba"))
    =
    (("aa") ++ (I) ++ (I) ++ ("a") ++ (H) ++ ("aa") ++ (I) ++ ("b") ++ (I) ++ ("a") ++ (I) ++ ("aaabaaaaba"));
constraint
  (("bab") ++ (I) ++ ("aaa") ++ (H) ++ ("a") ++ (I) ++ ("aaa") ++ (I) ++ ("a") ++ (I) ++ ("aababaa"))
    = (("ba") ++ (I) ++ ("baaaabbabaaababaababaa"));
constraint
  (("ab") ++ (I) ++ (H) ++ (H) ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("aabaaa") ++ (H) ++ (I) ++ (I))
    =
    (("a") ++ (I) ++ ("ba") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("b") ++ (I) ++ ("a") ++ (I) ++ ("abaabaaaa") ++ (I) ++ ("bb") ++ (I));
constraint
  (("aa") ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (F) ++ ("aaba") ++ (H) ++ ("ba") ++ (H) ++ ("a") ++ (I))
    =
    (("aabaa") ++ (I) ++ ("aba") ++ (I) ++ (I) ++ ("aa") ++ (I) ++ ("aab") ++ (I) ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("ab"));
constraint
  (("aababaaaa") ++ (I) ++ (F) ++ ("aaa") ++ (F) ++ (I) ++ ("bb") ++ (I)) =
    (("aababaaaab") ++ (F) ++ ("aaa") ++ (I) ++ (H) ++ (I) ++ ("bb") ++ (I));
constraint
  (("aaab") ++ (I) ++ ("baabaabaaaab") ++ (F) ++ ("a") ++ (H)) =
    (("aaab") ++ (I) ++ ("baa") ++ (I) ++ ("aa") ++ (I) ++ ("aaaa") ++ (I) ++ (F) ++ ("a") ++ (H));
constraint
  (("b") ++ (I) ++ ("aabaa") ++ (F) ++ (H) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("baba") ++ (I) ++ ("aa"))
    =
    (("bbaabaa") ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("ab") ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (I) ++ ("a") ++ (I) ++ ("abaa"));
constraint
  ((I) ++ ("aaabaab") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (I) ++ ("babaa") ++ (I) ++ (I) ++ ("ba"))
    =
    ((I) ++ ("aaa") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("ab") ++ (I) ++ ("abaab") ++ (I) ++ (I) ++ ("a"));
constraint
  ((H) ++ ("ab") ++ (H) ++ (H) ++ (I) ++ (I) ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("a") ++ (F) ++ ("aa"))
    =
    (("ab") ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (I) ++ ("abb") ++ (I) ++ ("b") ++ (I) ++ ("aab") ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (I) ++ ("aa"));
constraint
  (("aaaba") ++ (I) ++ (H) ++ (F) ++ ("ba") ++ (I) ++ ("aaaa") ++ (H) ++ ("ba"))
    =
    (("aaaba") ++ (F) ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("ba") ++ (I) ++ ("aaaa") ++ (H) ++ (I) ++ ("a"));
constraint
  (("aaba") ++ (I) ++ (I) ++ ("aa") ++ (H) ++ ("ababaab") ++ (I) ++ ("a") ++ (I) ++ ("aaab"))
    =
    (("aa") ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("aaab") ++ (I) ++ ("aba") ++ (I) ++ ("aa") ++ (I) ++ ("babaaab"));
constraint
  ((I) ++ (F) ++ (I) ++ ("aaaa") ++ (I) ++ ("aab") ++ (I) ++ (F) ++ ("ba") ++ (F))
    =
    (("b") ++ (I) ++ ("a") ++ (I) ++ ("b") ++ (I) ++ ("aaaabaa") ++ (I) ++ (I) ++ (I) ++ (H) ++ (I) ++ ("abab") ++ (I));
constraint
  ((H) ++ ("abaab") ++ (H) ++ ("a") ++ (F) ++ (I) ++ ("aabaaaaa")) =
    ((H) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("bbaa") ++ (I) ++ ("aaaaa"));
constraint
  (("abbbaabaaaaaaaaaa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("aaab")) =
    (("a") ++ (I) ++ ("b") ++ (I) ++ ("aa") ++ (I) ++ ("aaaaaaaaa") ++ (H) ++ ("a") ++ (I) ++ ("aaab"));
constraint
  (("aaa") ++ (H) ++ ("aa") ++ (H) ++ (I) ++ (I) ++ ("b") ++ (F) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("bb"))
    =
    (("aaa") ++ (H) ++ ("aaa") ++ (I) ++ (I) ++ ("bb") ++ (I) ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("b"));
constraint
  (("bbbaa") ++ (H) ++ ("aaabb") ++ (I) ++ ("abaabb") ++ (I) ++ ("aaa") ++ (I))
    =
    ((I) ++ ("b") ++ (I) ++ ("aa") ++ (H) ++ ("aaa") ++ (I) ++ ("bbabaa") ++ (I) ++ (I) ++ (I) ++ ("aaa") ++ (I));
constraint
  (("aaaaa") ++ (F) ++ ("a") ++ (I) ++ ("b") ++ (I) ++ ("a") ++ (H) ++ (I) ++ ("bba") ++ (I) ++ ("aaa"))
    =
    (("aaaaa") ++ (I) ++ ("ab") ++ (I) ++ (H) ++ ("baa") ++ (I) ++ ("bbbba") ++ (I) ++ ("aaa"));
constraint
  (("aaabb") ++ (I) ++ (I) ++ ("ab") ++ (I) ++ ("aabaa") ++ (H) ++ ("abaaaaa"))
    =
    (("aaa") ++ (I) ++ (I) ++ (I) ++ ("bab") ++ (I) ++ ("aabaaab") ++ (I) ++ ("abaaaaa"));
constraint
  ((I) ++ (I) ++ ("a") ++ (H) ++ ("babbaaa") ++ (F) ++ ("abba") ++ (I) ++ ("abb"))
    =
    (("b") ++ (I) ++ ("aa") ++ (I) ++ ("b") ++ (I) ++ ("ab") ++ (I) ++ ("aaabab") ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("a") ++ (F));
constraint
  (("aaaaa") ++ (H) ++ ("a") ++ (I) ++ ("aaab") ++ (C) ++ (I) ++ (I) ++ ("aa") ++ (I))
    = (("aaaaa") ++ (H) ++ ("abaaab") ++ (I) ++ ("bbabbb") ++ (I) ++ ("aab"));
constraint
  (("abba") ++ (F) ++ ("aaba") ++ (I) ++ (I) ++ ("aaa") ++ (I) ++ ("aababb") ++ (I))
    =
    (("ab") ++ (I) ++ ("a") ++ (I) ++ ("abbaa") ++ (I) ++ (H) ++ ("aaabaaba") ++ (I) ++ ("bb"));
constraint
  (("aabaa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ (I) ++ (H) ++ ("aaa") ++ (F) ++ ("aabab"))
    =
    (("aabaa") ++ (I) ++ (I) ++ ("ab") ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("aaa") ++ (I) ++ ("a") ++ (I) ++ ("baa") ++ (I) ++ ("ab"));
constraint
  (("bb") ++ (H) ++ (I) ++ ("b") ++ (I) ++ ("ba") ++ (I) ++ ("aaa") ++ (I) ++ ("aaa") ++ (F) ++ (I) ++ ("ab"))
    =
    ((I) ++ (I) ++ (H) ++ ("bbb") ++ (I) ++ ("a") ++ (I) ++ ("aaa") ++ (I) ++ ("aaa") ++ (I) ++ ("ab") ++ (I) ++ ("bab"));
constraint
  (("b") ++ (I) ++ ("abaabbabaa") ++ (H) ++ (I) ++ ("bababa") ++ (I) ++ ("aa"))
    =
    (("b") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("babaaab") ++ (I) ++ ("b") ++ (I) ++ ("aba") ++ (I) ++ ("a") ++ (I) ++ ("aa"));
constraint
  ((C) ++ (I) ++ ("aaaaaa") ++ (I) ++ ("abaabababaa")) =
    (("b") ++ (I) ++ (I) ++ ("a") ++ (I) ++ (I) ++ (I) ++ ("aaaaaa") ++ (I) ++ ("abaa") ++ (I) ++ ("ababaa"));
constraint
  (("bb") ++ (I) ++ ("baaaaab") ++ (I) ++ ("a") ++ (I) ++ ("aaaaaaaabaaa")) =
    ((I) ++ (I) ++ ("b") ++ (I) ++ ("aaaaa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("aaaaaaaabaaa"));
constraint
  (("a") ++ (I) ++ (I) ++ ("ababaa") ++ (H) ++ ("ba") ++ (I) ++ ("abaaaba") ++ (H))
    = (("abba") ++ (I) ++ ("abaaa") ++ (I) ++ ("bbababaaabaab") ++ (I));
constraint
  ((I) ++ (I) ++ ("aaaaa") ++ (H) ++ ("baaaaa") ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ ("a") ++ (I))
    =
    ((I) ++ ("baaaaaab") ++ (I) ++ (I) ++ ("aaaaa") ++ (I) ++ ("a") ++ (F) ++ ("bab"));
constraint
  (("aaaabaabaaa") ++ (H) ++ ("ab") ++ (H) ++ ("aa") ++ (H) ++ ("b")) =
    (("aaaa") ++ (I) ++ ("aabaaaa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("aaab") ++ (I) ++ ("b"));
constraint
  ((I) ++ (F) ++ (I) ++ (I) ++ ("b") ++ (I) ++ ("baabb") ++ (I) ++ ("aa") ++ (I) ++ ("aaaaa") ++ (I) ++ ("a"))
    =
    ((I) ++ ("ba") ++ (I) ++ ("b") ++ (I) ++ (I) ++ (I) ++ (I) ++ (I) ++ ("aab") ++ (I) ++ ("baabaaaaa") ++ (I) ++ ("a"));
constraint
  (("aa") ++ (I) ++ ("aaaa") ++ (H) ++ ("baa") ++ (H) ++ ("aaaa") ++ (I) ++ ("bb") ++ (I) ++ ("b"))
    =
    (("aabaaaaabbbaaa") ++ (I) ++ (I) ++ ("aaaa") ++ (I) ++ ("b") ++ (I) ++ (I) ++ (I));
constraint
  ((H) ++ ("aaaba") ++ (I) ++ ("aa") ++ (H) ++ ("aba") ++ (I) ++ ("aaa") ++ (H) ++ (I))
    =
    (("a") ++ (I) ++ (I) ++ ("aaababaaa") ++ (I) ++ (I) ++ ("ababaaaa") ++ (I) ++ (I) ++ (I));
constraint
  (("bbaaa") ++ (I) ++ ("aa") ++ (F) ++ (I) ++ ("aaabaa") ++ (F) ++ ("ab")) =
    ((I) ++ ("baaabaaba") ++ (I) ++ ("bbaaa") ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (I) ++ ("bab"));
constraint
  (("aa") ++ (I) ++ (I) ++ (I) ++ ("aaaaaba") ++ (I) ++ ("aa") ++ (I) ++ (H) ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("a"))
    =
    (("aabbbaaaaa") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (I) ++ ("bbaba") ++ (I) ++ ("a"));
constraint
  ((H) ++ (I) ++ ("b") ++ (F) ++ ("ababa") ++ (H) ++ ("b") ++ (I) ++ (C)) =
    (("abb") ++ (I) ++ (I) ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("abaa") ++ (I) ++ (I) ++ (I) ++ ("bbbba") ++ (I) ++ (I));
constraint
  (("aaaaab") ++ (F) ++ ("a") ++ (H) ++ (I) ++ ("abaaaaa") ++ (I) ++ (I) ++ ("a"))
    =
    (("aaaaabba") ++ (I) ++ ("baa") ++ (I) ++ (I) ++ ("ba") ++ (I) ++ ("aaaa") ++ (H) ++ ("a"));
constraint
  ((H) ++ ("bba") ++ (F) ++ ("aaa") ++ (I) ++ ("abaaa") ++ (F) ++ ("bb")) =
    ((H) ++ (I) ++ ("baba") ++ (I) ++ (I) ++ ("aaaba") ++ (I) ++ ("aaa") ++ (I) ++ ("abb") ++ (I) ++ ("b"));
constraint
  (("bbb") ++ (I) ++ (I) ++ (H) ++ (I) ++ ("aba") ++ (I) ++ ("aa") ++ (H) ++ (I) ++ (I) ++ (I) ++ (I) ++ ("aba"))
    =
    (("b") ++ (I) ++ (I) ++ (I) ++ ("b") ++ (H) ++ (I) ++ ("aba") ++ (I) ++ ("aaa") ++ (I) ++ ("b") ++ (I) ++ (I) ++ (I) ++ (I) ++ ("aba"));
constraint
  (("baaabaa") ++ (H) ++ ("baaa") ++ (I) ++ ("ba") ++ (I) ++ ("abaa") ++ (I) ++ ("aa"))
    =
    (("baaabaaa") ++ (I) ++ ("b") ++ (I) ++ ("aaa") ++ (I) ++ ("bababaabaa"));
constraint
  (("b") ++ (I) ++ ("b") ++ (I) ++ (I) ++ ("aaa") ++ (C) ++ (I) ++ ("aba") ++ (I) ++ ("abaaaa"))
    =
    (("bb") ++ (I) ++ ("b") ++ (I) ++ ("aaa") ++ (I) ++ ("bbabbbababa") ++ (I) ++ ("aaaa"));
constraint
  (("aaa") ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (H) ++ (I) ++ ("aaaaa") ++ (F) ++ ("baab"))
    =
    (("aaababa") ++ (I) ++ ("abb") ++ (I) ++ ("aaaaaba") ++ (I) ++ (I) ++ (I) ++ ("aa") ++ (I));
constraint
  (("aa") ++ (I) ++ ("abab") ++ (I) ++ (H) ++ ("bbaba") ++ (I) ++ (I) ++ (H) ++ ("b") ++ (I) ++ (I) ++ (I))
    =
    (("aaba") ++ (I) ++ ("a") ++ (I) ++ ("ba") ++ (I) ++ (I) ++ ("b") ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (F) ++ (I) ++ (I) ++ ("b") ++ (I));
constraint
  (("abbaab") ++ (F) ++ (I) ++ (I) ++ (I) ++ (I) ++ ("abaaaba") ++ (I) ++ ("aab"))
    =
    (("a") ++ (I) ++ ("baab") ++ (I) ++ ("a") ++ (I) ++ ("bb") ++ (I) ++ ("b") ++ (I) ++ ("abaaaba") ++ (I) ++ ("aa") ++ (I));
constraint
  (("a") ++ (H) ++ (H) ++ (I) ++ ("aaaaaaaabab") ++ (I) ++ ("aab") ++ (I) ++ (I))
    =
    (("aab") ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("baaaaaaaa") ++ (I) ++ ("ab") ++ (I) ++ ("aab") ++ (I) ++ ("b"));
constraint
  ((F) ++ (I) ++ ("aab") ++ (I) ++ ("aa") ++ (I) ++ (H) ++ (I) ++ ("a") ++ (I) ++ ("aab") ++ (F))
    =
    (("ba") ++ (I) ++ (I) ++ ("baabbaaba") ++ (I) ++ ("b") ++ (I) ++ ("a") ++ (I) ++ ("aab") ++ (I) ++ ("ab") ++ (I));
constraint
  ((H) ++ ("b") ++ (I) ++ ("bbababababaaab") ++ (H) ++ ("aab")) =
    (("ab") ++ (I) ++ ("bbbbaba") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("aaa") ++ (I) ++ (H) ++ ("aab"));
constraint
  (("a") ++ (I) ++ ("b") ++ (I) ++ (I) ++ ("baaa") ++ (I) ++ (I) ++ (I) ++ ("ab") ++ (I) ++ ("a") ++ (I) ++ ("bbbaa") ++ (I) ++ ("ba"))
    =
    ((H) ++ (I) ++ (I) ++ (I) ++ ("aaab") ++ (I) ++ (I) ++ ("ab") ++ (I) ++ ("a") ++ (I) ++ ("bb") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("a"));
constraint
  ((I) ++ (F) ++ (I) ++ ("aaba") ++ (H) ++ (F) ++ ("a") ++ (I) ++ (I) ++ ("ba") ++ (I) ++ ("ab"))
    =
    ((I) ++ ("bab") ++ (I) ++ ("baa") ++ (I) ++ ("aab") ++ (I) ++ (I) ++ ("ab") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ ("aba") ++ (I));
constraint
  ((I) ++ ("baaabaaa") ++ (H) ++ ("aaaaa") ++ (H) ++ ("aa") ++ (I) ++ (I) ++ ("b"))
    = ((I) ++ ("baaabaaaabbaaaaa") ++ (H) ++ ("a") ++ (H) ++ ("b"));
constraint
  (("aa") ++ (I) ++ ("b") ++ (H) ++ (I) ++ ("aaaa") ++ (H) ++ ("aabaa") ++ (I) ++ ("a") ++ (I) ++ ("aa"))
    =
    (("aabba") ++ (I) ++ ("bbaaaaa") ++ (I) ++ (I) ++ ("aa") ++ (I) ++ ("aaba") ++ (I) ++ ("aa"));
constraint
  (("aa") ++ (H) ++ ("aaaaa") ++ (F) ++ ("bbaa") ++ (I) ++ ("aaa") ++ (I) ++ ("a") ++ (I))
    =
    (("aaa") ++ (I) ++ ("baaaaa") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ (I) ++ ("aabaaa") ++ (I) ++ ("a") ++ (I));
constraint
  (("a") ++ (I) ++ ("aba") ++ (I) ++ ("a") ++ (I) ++ ("aaaaa") ++ (H) ++ ("baa") ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (I))
    =
    (("a") ++ (I) ++ ("aba") ++ (I) ++ ("a") ++ (I) ++ ("aaaaa") ++ (H) ++ ("baa") ++ (I) ++ ("aaba") ++ (I));
constraint
  (("a") ++ (I) ++ ("a") ++ (H) ++ ("a") ++ (I) ++ ("aababbb") ++ (C) ++ ("a") ++ (I) ++ (I) ++ ("a"))
    =
    (("a") ++ (I) ++ ("aab") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ (H) ++ ("b") ++ (I) ++ ("bba") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("ba"));
constraint
  ((I) ++ ("aaaaa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("aba") ++ (I) ++ ("aaabaa") ++ (H) ++ ("b"))
    =
    ((I) ++ ("aaaa") ++ (H) ++ ("a") ++ (I) ++ ("baba") ++ (I) ++ ("aaabaaa") ++ (I) ++ (I) ++ ("b"));
constraint
  (("abaabaa") ++ (I) ++ ("abaabba") ++ (I) ++ ("abaaa") ++ (I) ++ ("bab")) =
    (("a") ++ (I) ++ ("aabaa") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("abaaab") ++ (I) ++ ("a") ++ (I));
constraint
  (("a") ++ (I) ++ ("bbb") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("b") ++ (I) ++ ("aba") ++ (H) ++ (I) ++ ("baa") ++ (H) ++ (I))
    =
    (("a") ++ (I) ++ (I) ++ ("b") ++ (I) ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("bba") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("bbaa") ++ (H) ++ ("b"));
constraint
  ((H) ++ ("aaa") ++ (I) ++ ("aaabb") ++ (I) ++ ("aaa") ++ (I) ++ ("aaa") ++ (I) ++ ("a") ++ (I) ++ ("aa"))
    =
    (("a") ++ (I) ++ (I) ++ ("aaabaaa") ++ (I) ++ ("b") ++ (I) ++ ("aaa") ++ (I) ++ ("aaababaa"));
constraint
  (("ab") ++ (I) ++ ("ba") ++ (I) ++ ("aaaaaaaba") ++ (I) ++ ("baaa") ++ (H) ++ (I) ++ ("a"))
    =
    (("a") ++ (I) ++ (I) ++ ("ba") ++ (I) ++ ("aaaaaaa") ++ (I) ++ ("ab") ++ (I) ++ ("aaaab") ++ (I) ++ ("ba"));
constraint
  (("b") ++ (I) ++ ("aaabaaa") ++ (H) ++ ("a") ++ (F) ++ ("b") ++ (I) ++ ("aaaa") ++ (I) ++ (I))
    =
    (("bbaaabaaaab") ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ (I) ++ ("aaaa") ++ (I) ++ (I));
constraint
  ((H) ++ ("ababaa") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ (I) ++ (I) ++ (I) ++ ("baaa") ++ (I) ++ ("aab"))
    =
    (("a") ++ (I) ++ ("ba") ++ (I) ++ ("abaa") ++ (I) ++ ("aabbb") ++ (I) ++ (I) ++ ("baaa") ++ (I) ++ ("aa") ++ (I));
constraint
  ((I) ++ ("abaa") ++ (I) ++ ("aab") ++ (C) ++ ("bbab") ++ (I) ++ ("aba") ++ (I) ++ ("a"))
    =
    ((I) ++ ("a") ++ (I) ++ ("aabaabbb") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("a"));
constraint
  (("ba") ++ (I) ++ (I) ++ (I) ++ ("babaaabaaa") ++ (I) ++ ("ba") ++ (I) ++ ("aaaa") ++ (I) ++ ("a"))
    =
    ((I) ++ ("a") ++ (I) ++ ("b") ++ (I) ++ (I) ++ ("abaaa") ++ (I) ++ ("aa") ++ (H) ++ ("abaaaaba"));
constraint
  (("bbaa") ++ (H) ++ ("aaa") ++ (I) ++ ("aaaaab") ++ (F) ++ ("abab")) =
    ((I) ++ (I) ++ ("aaabbaaa") ++ (I) ++ ("aaaaa") ++ (I) ++ (I) ++ ("a") ++ (I) ++ ("ba") ++ (I) ++ ("a") ++ (I));
constraint
  (("aaabaab") ++ (H) ++ (H) ++ ("aa") ++ (F) ++ ("aaaaab")) =
    (("aaabaa") ++ (I) ++ ("a") ++ (I) ++ (I) ++ (H) ++ ("aa") ++ (I) ++ ("a") ++ (I) ++ ("baaaaa") ++ (I));
constraint
  (("aba") ++ (I) ++ ("aaaba") ++ (H) ++ (F) ++ ("baabb") ++ (I) ++ ("aaa"))
    =
    (("a") ++ (I) ++ ("abaaa") ++ (I) ++ ("aa") ++ (I) ++ ("bba") ++ (I) ++ ("b") ++ (I) ++ ("aa") ++ (I) ++ ("b") ++ (I) ++ ("aaa"));
constraint
  (("b") ++ (I) ++ ("aba") ++ (I) ++ ("aa") ++ (I) ++ ("b") ++ (I) ++ ("abaa") ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("aa"))
    =
    (("b") ++ (I) ++ ("ababaa") ++ (I) ++ ("b") ++ (I) ++ ("abaa") ++ (I) ++ ("aaba") ++ (I) ++ ("a") ++ (I) ++ ("aa"));
constraint
  (("aa") ++ (I) ++ ("b") ++ (I) ++ (I) ++ ("abaaaabaaaaa") ++ (H) ++ (I) ++ ("aa") ++ (I))
    =
    (("aa") ++ (I) ++ (I) ++ (I) ++ ("babaaaabaaaaaa") ++ (I) ++ (I) ++ ("baa") ++ (I));
constraint
  ((H) ++ ("aba") ++ (H) ++ ("a") ++ (I) ++ ("abaa") ++ (C) ++ ("baab")) =
    (("abba") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("aba") ++ (I) ++ ("aa") ++ (I) ++ ("b") ++ (I) ++ ("abb") ++ (I) ++ ("aab"));
constraint
  (("a") ++ (I) ++ (F) ++ ("baaa") ++ (I) ++ ("b") ++ (I) ++ ("baa") ++ (H) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ ("b"))
    =
    ((H) ++ ("ab") ++ (I) ++ ("baa") ++ (H) ++ (I) ++ (I) ++ ("aaa") ++ (I) ++ ("ba") ++ (I) ++ ("aab") ++ (I));
constraint
  ((I) ++ ("aa") ++ (H) ++ (I) ++ ("aa") ++ (I) ++ (I) ++ (I) ++ (I) ++ ("a") ++ (H) ++ ("a") ++ (I) ++ (H) ++ ("a") ++ (I) ++ ("b"))
    =
    (("baaa") ++ (I) ++ (I) ++ ("baab") ++ (I) ++ ("bbaa") ++ (I) ++ ("ba") ++ (I) ++ ("abba") ++ (I) ++ ("b"));
constraint
  (("abab") ++ (I) ++ ("aaa") ++ (I) ++ ("ababaababababaaa")) =
    (("abab") ++ (I) ++ ("aaababa") ++ (I) ++ ("aaba") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("aaa"));
constraint
  (("a") ++ (I) ++ (F) ++ ("ba") ++ (I) ++ ("aaababaa") ++ (I) ++ ("aa") ++ (H) ++ ("ab"))
    =
    (("ab") ++ (I) ++ ("a") ++ (I) ++ ("b") ++ (I) ++ ("a") ++ (I) ++ ("aaababaa") ++ (I) ++ ("aaab") ++ (I) ++ ("a") ++ (I));
constraint
  ((I) ++ ("aba") ++ (C) ++ (H) ++ ("a") ++ (H) ++ ("a") ++ (F) ++ ("aaa")) =
    ((I) ++ ("abab") ++ (I) ++ ("ba") ++ (I) ++ ("ba") ++ (I) ++ ("baab") ++ (I) ++ ("a") ++ (F) ++ ("aaa"));
constraint
  ((I) ++ ("abaaaaa") ++ (F) ++ ("ab") ++ (I) ++ (I) ++ (I) ++ ("a") ++ (H) ++ ("abaa"))
    = ((I) ++ ("abaaaaabab") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ ("baabbabaa"));
constraint
  (("bb") ++ (I) ++ ("aabaaaaaaaa") ++ (H) ++ ("a") ++ (I) ++ ("a") ++ (H) ++ (I) ++ (I))
    =
    (("b") ++ (I) ++ ("baabaaaaaaaaab") ++ (I) ++ ("abaa") ++ (I) ++ (I) ++ (I) ++ ("b"));
constraint
  (("aa") ++ (I) ++ ("aaa") ++ (I) ++ (F) ++ ("aa") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (C) ++ ("ab"))
    =
    (("aa") ++ (I) ++ ("aaabbabbaa") ++ (I) ++ ("a") ++ (I) ++ ("abb") ++ (I) ++ ("ab") ++ (I) ++ ("a") ++ (I));
constraint
  ((H) ++ ("aaa") ++ (I) ++ ("a") ++ (I) ++ ("abaa") ++ (H) ++ ("baababa") ++ (I) ++ ("a"))
    =
    (("a") ++ (I) ++ (I) ++ ("aaaba") ++ (I) ++ ("a") ++ (I) ++ ("aaa") ++ (I) ++ ("bbaa") ++ (I) ++ ("a") ++ (I) ++ ("aba"));
constraint
  (("baaa") ++ (I) ++ (I) ++ ("aabaaaa") ++ (I) ++ ("aba") ++ (F) ++ (I) ++ ("aab"))
    =
    (("baaa") ++ (I) ++ (I) ++ ("aabaaaababa") ++ (I) ++ ("ab") ++ (I) ++ (I) ++ ("aab"));
constraint
  (("ab") ++ (H) ++ ("aa") ++ (I) ++ ("aaa") ++ (H) ++ ("aba") ++ (H) ++ ("a") ++ (I) ++ ("aba"))
    =
    (("aba") ++ (I) ++ (I) ++ ("aabaaaab") ++ (I) ++ ("a") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("ababa"));
constraint
  ((I) ++ ("a") ++ (I) ++ ("baa") ++ (I) ++ ("b") ++ (H) ++ ("baabaaa") ++ (I) ++ ("ababa") ++ (I))
    =
    (("bab") ++ (I) ++ ("aa") ++ (I) ++ ("ba") ++ (I) ++ ("bbaabaaaba") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I));
constraint
  ((I) ++ (I) ++ ("aa") ++ (I) ++ ("a") ++ (I) ++ (I) ++ (H) ++ (I) ++ ("a") ++ (I) ++ ("b") ++ (H) ++ ("baaaaa") ++ (I))
    =
    ((I) ++ (I) ++ ("aababbab") ++ (I) ++ (F) ++ ("a") ++ (I) ++ ("bbaaaaa") ++ (I));
constraint
  (("a") ++ (I) ++ (F) ++ ("aaaaaa") ++ (I) ++ (I) ++ ("ba") ++ (H) ++ ("aaabab"))
    =
    (("a") ++ (I) ++ (I) ++ ("a") ++ (I) ++ (I) ++ ("aaaaaab") ++ (I) ++ ("baab") ++ (I) ++ ("aaa") ++ (I) ++ ("a") ++ (I));
constraint
  (("aa") ++ (F) ++ ("abbb") ++ (I) ++ (I) ++ ("aaaa") ++ (H) ++ ("baabba"))
    =
    (("aabab") ++ (I) ++ ("a") ++ (I) ++ ("bb") ++ (I) ++ (I) ++ ("aaaaa") ++ (I) ++ ("b") ++ (I) ++ ("aab") ++ (I) ++ ("a"));
constraint
  (("aaaaaa") ++ (H) ++ ("b") ++ (I) ++ ("baa") ++ (H) ++ ("abaababa")) =
    (("aaaaaaa") ++ (I) ++ ("bbb") ++ (I) ++ ("aaab") ++ (I) ++ ("abaa") ++ (I) ++ ("aba"));
constraint
  (("bbb") ++ (I) ++ ("bb") ++ (I) ++ (I) ++ ("abaaaaba") ++ (I) ++ ("ab") ++ (H) ++ ("aa") ++ (I))
    =
    (("b") ++ (I) ++ (I) ++ (I) ++ ("bbb") ++ (I) ++ ("abaaaaba") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("baab"));
constraint
  (("aaa") ++ (I) ++ ("a") ++ (F) ++ ("a") ++ (H) ++ ("ab") ++ (I) ++ ("baaa") ++ (H) ++ ("bb"))
    =
    (("aaa") ++ (I) ++ ("a") ++ (I) ++ ("ab") ++ (I) ++ ("aab") ++ (I) ++ ("ab") ++ (I) ++ ("baaa") ++ (H) ++ ("bb"));
constraint
  (("aab") ++ (I) ++ ("aaabbaaaaba") ++ (H) ++ ("a") ++ (H) ++ (I) ++ ("b") ++ (I))
    =
    (("aab") ++ (I) ++ ("aaa") ++ (I) ++ ("baaaa") ++ (I) ++ ("aabbaabbb") ++ (I) ++ ("b"));
constraint
  (("a") ++ (I) ++ ("aaaab") ++ (I) ++ ("aa") ++ (H) ++ ("aababaaabbab")) =
    (("a") ++ (I) ++ ("aaa") ++ (H) ++ ("aaab") ++ (I) ++ ("aababaaa") ++ (I) ++ (I) ++ ("ab"));
constraint
  (("abb") ++ (I) ++ (I) ++ ("baaaa") ++ (I) ++ ("aabbaab") ++ (H) ++ (I) ++ (I) ++ ("aa"))
    =
    ((H) ++ ("bb") ++ (I) ++ ("aaaa") ++ (I) ++ ("aa") ++ (I) ++ ("baaba") ++ (I) ++ ("b") ++ (I) ++ (I) ++ ("aa"));
constraint
  ((I) ++ (H) ++ ("abba") ++ (I) ++ ("ba") ++ (I) ++ ("a") ++ (H) ++ ("aaaaa") ++ (I) ++ ("aab"))
    =
    ((I) ++ ("ab") ++ (I) ++ (H) ++ ("a") ++ (I) ++ ("ba") ++ (I) ++ ("a") ++ (H) ++ ("aaaaa") ++ (I) ++ ("aa") ++ (I));
constraint
  (("bbbaaa") ++ (H) ++ ("a") ++ (F) ++ ("bbbbaaaaaaa")) =
    (("bb") ++ (I) ++ ("aaa") ++ (H) ++ ("a") ++ (I) ++ ("abb") ++ (I) ++ ("b") ++ (I) ++ (I) ++ ("aaaaaaa"));
constraint
  ((I) ++ ("b") ++ (I) ++ ("aabbaa") ++ (I) ++ (H) ++ ("baaaaaa") ++ (I) ++ (F))
    =
    ((I) ++ (I) ++ (I) ++ ("aab") ++ (I) ++ ("aabab") ++ (I) ++ ("baaaaa") ++ (H) ++ ("a") ++ (I) ++ (I));
constraint
  (("ab") ++ (H) ++ ("aa") ++ (I) ++ ("b") ++ (I) ++ ("baabaa") ++ (H) ++ ("b") ++ (I) ++ (I) ++ ("b") ++ (I) ++ ("a"))
    =
    (("a") ++ (I) ++ (H) ++ ("aabbbbaa") ++ (I) ++ ("aaa") ++ (I) ++ (I) ++ (I) ++ ("b") ++ (I) ++ ("bba"));
constraint
  (("ababaaa") ++ (I) ++ ("aba") ++ (I) ++ ("aaaaaa") ++ (I) ++ ("aaaaaa")) =
    (("ababaaababa") ++ (I) ++ ("aaaaaabaaaaaa"));
constraint
  (("a") ++ (H) ++ ("a") ++ (I) ++ ("abaa") ++ (I) ++ (I) ++ (H) ++ ("aa") ++ (I) ++ ("a") ++ (H) ++ ("a") ++ (I) ++ (I))
    =
    (("aab") ++ (I) ++ ("a") ++ (I) ++ ("a") ++ (I) ++ ("aabb") ++ (H) ++ ("aa") ++ (I) ++ ("aab") ++ (F));
solve satisfy;
