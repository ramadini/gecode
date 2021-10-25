var string: B;
var string: D;
constraint
  (("aaa") ++ (D) ++ (D) ++ (D) ++ ("abb") ++ (D) ++ ("ab") ++ (B) ++ (D) ++ ("babbaa") ++ (D) ++ ("b"))
    =
    (("aaa") ++ (D) ++ ("b") ++ (D) ++ ("abbbaba") ++ (D) ++ (D) ++ (D) ++ ("babbaba") ++ (D) ++ (D) ++ (D) ++ ("aaa") ++ (D) ++ (D) ++ ("a") ++ (D) ++ ("baabb"));
constraint
  ((D) ++ (D) ++ (D) ++ ("aaaaaaaaa") ++ (D) ++ (D) ++ ("b") ++ (D) ++ ("aabaaaaaa") ++ (D) ++ (D) ++ ("b") ++ (D) ++ ("aa") ++ (D) ++ ("ababa") ++ (D))
    =
    ((D) ++ (D) ++ ("baaaaaaaaa") ++ (D) ++ ("b") ++ (D) ++ (D) ++ ("aabaaaaaa") ++ (D) ++ (D) ++ ("b") ++ (D) ++ ("aababa") ++ (D) ++ ("ab"));
constraint
  (("aaa") ++ (D) ++ ("aa") ++ (D) ++ ("abb") ++ (D) ++ ("aa") ++ (D) ++ (D) ++ ("aa") ++ (D) ++ (D) ++ (D) ++ ("a") ++ (D) ++ ("baa") ++ (D) ++ ("baa") ++ (D) ++ ("a") ++ (D) ++ ("aaabab"))
    =
    (("aaa") ++ (D) ++ ("aa") ++ (D) ++ ("a") ++ (D) ++ (D) ++ (D) ++ ("aa") ++ (D) ++ (D) ++ ("aa") ++ (D) ++ ("bba") ++ (D) ++ (D) ++ ("aabbaababaaabab"));
constraint
  (("aa") ++ (D) ++ (D) ++ ("a") ++ (D) ++ (D) ++ (D) ++ ("a") ++ (D) ++ (D) ++ ("aaaaa") ++ (D) ++ ("baab") ++ (D) ++ (D) ++ ("babbb") ++ (D) ++ ("aaa") ++ (D) ++ ("b") ++ (D) ++ ("baa"))
    =
    (("aab") ++ (D) ++ ("ab") ++ (D) ++ ("babbaaaaa") ++ (D) ++ (D) ++ ("aabb") ++ (D) ++ (D) ++ ("a") ++ (D) ++ ("b") ++ (D) ++ ("baaa") ++ (D) ++ (D) ++ ("b") ++ (D) ++ ("aa"));
constraint
  (("baaa") ++ (D) ++ ("abababba") ++ (D) ++ ("aaab") ++ (D) ++ ("baa") ++ (D) ++ ("bba") ++ (D) ++ ("aa") ++ (D) ++ ("aaa") ++ (D) ++ ("ab") ++ (D) ++ ("b"))
    =
    (("baaa") ++ (D) ++ ("aba") ++ (D) ++ ("ab") ++ (D) ++ ("abaaa") ++ (D) ++ ("bbaabbbabaabaaababb") ++ (D));
constraint
  ((D) ++ ("aabaabaaba") ++ (D) ++ ("aabab") ++ (D) ++ (D) ++ ("abab") ++ (D) ++ (D) ++ ("baaba") ++ (D) ++ ("a") ++ (D) ++ ("bba") ++ (D) ++ ("b"))
    =
    (("baa") ++ (D) ++ ("aa") ++ (D) ++ ("aa") ++ (D) ++ ("a") ++ (D) ++ ("aa") ++ (D) ++ ("a") ++ (D) ++ ("bba") ++ (D) ++ ("abbb") ++ (D) ++ ("aababa") ++ (D) ++ (D) ++ ("ba") ++ (D) ++ (D));
constraint
  (("a") ++ (D) ++ ("abab") ++ (D) ++ (D) ++ ("aabbabbbb") ++ (D) ++ ("baa") ++ (D) ++ ("bbbaaaaa") ++ (D) ++ ("a") ++ (D) ++ (D) ++ ("aaba"))
    =
    (("abababbbaabbabbb") ++ (D) ++ (D) ++ ("baa") ++ (D) ++ ("b") ++ (D) ++ (D) ++ ("aaaaaba") ++ (D) ++ (D) ++ ("aa") ++ (D) ++ ("a"));
constraint
  ((D) ++ ("aabab") ++ (D) ++ (D) ++ ("aaaabbbaaaaab") ++ (D) ++ (D) ++ ("abab") ++ (D) ++ ("a") ++ (D) ++ ("aab") ++ (D) ++ ("ba") ++ (D) ++ ("b"))
    =
    ((D) ++ ("aaba") ++ (D) ++ (D) ++ (D) ++ ("aaaabbbaaaaabb") ++ (D) ++ ("abab") ++ (D) ++ ("abaa") ++ (D) ++ (D) ++ ("ba") ++ (D) ++ (D));
constraint
  (("baaaa") ++ (D) ++ (D) ++ ("aaaa") ++ (D) ++ ("a") ++ (D) ++ ("aaab") ++ (D) ++ (D) ++ ("babaa") ++ (D) ++ ("b") ++ (D) ++ ("aba") ++ (D) ++ ("aaaaa") ++ (D))
    =
    ((D) ++ ("aaaa") ++ (D) ++ ("baaaaba") ++ (D) ++ ("aaab") ++ (D) ++ ("bbabaa") ++ (D) ++ ("b") ++ (D) ++ ("ababaaaaa") ++ (D));
constraint
  (("bbaaba") ++ (D) ++ ("bbaa") ++ (D) ++ ("abaaaa") ++ (D) ++ ("aaaababbaa") ++ (D) ++ ("a") ++ (D) ++ ("a") ++ (D) ++ (D) ++ ("aab"))
    =
    ((D) ++ ("baababb") ++ (D) ++ ("aababaaaa") ++ (D) ++ ("aaaabab") ++ (D) ++ ("aa") ++ (D) ++ ("aba") ++ (D) ++ ("baa") ++ (D));
solve satisfy;
