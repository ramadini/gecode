var string: D;
var string: J;
constraint
  (("bbaabbbacb") ++ (J) ++ ("ccbccccbabaacbabaabbaacbbacaaaaabcbabccccbccaabacbbccbcbaa") ++ (D))
    =
    (("bbaabbbacbbbbbcbcbcacbbab") ++ (D) ++ ("aabaaabac") ++ (D) ++ ("bbcbbcacccbb") ++ (D) ++ ("accabbaabcccc") ++ (D) ++ ("accbccccbabaacbabaabbaacbbacaaaaabcbabccccbc") ++ (D) ++ ("abacbbccbcbaaca"));
solve satisfy;
