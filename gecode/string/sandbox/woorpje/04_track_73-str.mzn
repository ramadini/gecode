var string: B;
var string: D;
var string: A;
constraint
  (("babcacbbccbaabbbaabb") ++ (D) ++ ("abaab")) =
    (("babcacbbccba") ++ (A) ++ ("bcbcacaaaaccbaabaaabbabaab"));
constraint
  (("babaababaabaaacaaacacbbbb") ++ (B) ++ ("cabbaacbcabbccacaccaab")) =
    (("babaababaabaaacaaacacbbbb") ++ (B) ++ ("cabbaacbcabbccacaccaab"));
constraint
  (("aaccbcbababcbabaaacbbbbcbacccbcacabbaaabcccccabbaabcba") ++ ("")) =
    (("aaccbcbababcbabaaacbbbbcb") ++ (B) ++ ("cabbaaabcccccabbaabcba"));
constraint
  (("abbccaabccacbcaabbbcabaaaacbcccaacccbcaabbbabbbccbcbba") ++ ("")) =
    (("abbccaabccacbcaabbbcabaaaacbccca") ++ (B) ++ ("abbbabbbccbcbba"));
constraint
  (("aabbccbaabcaabb") ++ (B) ++ ("cbbcbabcbbcbbcbcbabbccbabbbbbcbc")) =
    (("aabbccbaabcaabbacccbcacbbcbabcbbcbbcbcbabbccbabbbbbcbc") ++ (""));
constraint
  (("cb") ++ (B) ++ ("bacabcbacabccbcabbbbabcaaabcabaaaacbaabaabaca")) =
    (("cb") ++ (B) ++ ("bacabcbacabccbcabbbbabcaaabcabaaaacbaabaabaca"));
constraint
  (("bcbcbb") ++ (B) ++ ("acacbcbaabcbcbbabababbcbabbbccabccabbcbab")) =
    (("bcbcbb") ++ (B) ++ ("acacbcbaabcbcbbabababbcbabbbccabccabbcbab"));
constraint
  (("ccbcacbbacccbcaaabbacbaacaacbbcabbacaaaaaababcabbccbac") ++ ("")) =
    (("ccbcacbb") ++ (B) ++ ("aabbacbaacaacbbcabbacaaaaaababcabbccbac"));
constraint
  (("bbbaccaacbacabcaaabacaaaaacacaabccaacabbcaacccbcabbccc") ++ ("")) =
    (("bbbaccaacbacabcaaabacaaaaacacaabccaacabbca") ++ (B) ++ ("bbccc"));
constraint
  (("bccacccbcaabccacaaacabaacaccaaacbbaabbccabaccccabbbbac") ++ ("")) =
    (("bcc") ++ (B) ++ ("abccacaaacabaacaccaaacbbaabbccabaccccabbbbac"));
solve satisfy;
