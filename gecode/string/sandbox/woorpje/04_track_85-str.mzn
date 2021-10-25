var string: B;
var string: A;
var string: C;
var string: D;
constraint
  (("bbaacac") ++ (A) ++ ("ccbcbbabbacbaaabba")) =
    (("bbaacac") ++ (A) ++ ("ccbcbbabbacbaaabba"));
constraint
  (("aa") ++ (A) ++ ("acbcababccbabbbabbccaacbcabbccbbcaaacccccccabcccaabbcbcaa"))
    =
    (("aacbbabc") ++ (B) ++ (C) ++ ("babccbabbbabbccaacbcabbccbbcaaacccccccabcccaabbcbcaa"));
constraint
  (("c") ++ (D) ++ ("accccacaaacba") ++ (B) ++ ("bbaabcabcaacacaccbaaccbbaacaa"))
    =
    (("c") ++ (D) ++ ("accccacaaacbacbbbabacbaababbbabbbacccbccaacacbacabacbbaabcabcaacacaccbaaccbbaacaa"));
constraint
  (("bbababcabcabcaccacccaabaaacbc") ++ (B) ++ ("ababbbbcccbacbabcacbcaccbacbacaab"))
    =
    (("bbababcabcabcaccacccaabaaacbcccbc") ++ (C) ++ ("bcacbcaccbacbacaab"));
constraint
  (("cbbabcaaccacbccbabcccaaac") ++ (B) ++ ("accbabacbbbcaaaacccaccaacb")) =
    (("cbbabcaaccacbccbabcccaaaccccacbcabbbacbccbbbab") ++ (A) ++ ("caccbacbabcbcbaccbabacbbbcaaaacccaccaacb"));
constraint
  (("acbbccbacbaccacbccbbcabcabaabaacbabbcc") ++ (A) ++ ("bacccbaccabcbcaabbccbccbbacacbcbabaaacaccabaacab"))
    =
    (("acbbccbacbaccacbccbbcabcabaabaacbabbcc") ++ (A) ++ ("bacccbaccabcbcaabbccbccbbacacbcbabaaacaccabaacab"));
constraint
  (("bbbccb") ++ (A) ++ ("ccbabbccabbcaacaabcacbbcabbcaccaccbaaaacbbccbcaaaabcaacbcaccabbabaaabcbccbbacbcb"))
    =
    (("bbbccbcbaacbcbbaccbabbccabbcaacaabcacbbcabbcaccaccbaaaacbbccbcaaaabcaacbcaccabbabaaabcbccbbacbcb") ++ (""));
constraint
  (("bca") ++ (D) ++ ("aaca") ++ (D) ++ ("ccbcac") ++ (C) ++ ("bacacb") ++ (D) ++ ("acca") ++ (D) ++ ("caabcc") ++ (D) ++ ("bbcacbbbbabbbbcbacccacbacbcaccbcbabcb"))
    =
    (("bcabaaacabaccbcac") ++ (C) ++ (D) ++ ("cacbbaaccabacaabcc") ++ (D) ++ ("bbcacbbbbabbbbcbacccacbacbcaccbc") ++ (D) ++ ("bcb"));
constraint
  (("bacaacbbccbbcaacca") ++ (D) ++ ("cbbcabaccaaaacca") ++ (D) ++ ("abbcb") ++ (D) ++ ("baaacbcb") ++ (D) ++ ("bbbccaabbcacccaacbaacabbabcccbbacbaccbccb"))
    =
    ((D) ++ ("caacbbccbbcaacca") ++ (D) ++ ("cbbcabaccaaaacca") ++ (D) ++ ("abbcbba") ++ (D) ++ ("aacbcbbabbbccaabbcacccaac") ++ (D) ++ ("acabbabcccbbac") ++ (D) ++ ("ccbccb"));
constraint
  (("a") ++ (D) ++ ("b") ++ (D) ++ ("caaaaabbbacccbcacabbabcc") ++ (D) ++ ("bcacaccaaba") ++ (D) ++ ("aabbbcbacacab") ++ (D) ++ ("ac") ++ (D) ++ ("accabaccbcaabbaabbabaacbaaaacccc"))
    =
    (("a") ++ (D) ++ ("bbacaaaaabb") ++ (D) ++ ("cccbcacab") ++ (D) ++ ("bcc") ++ (D) ++ ("bcacaccaababaaabbbcbacacab") ++ (D) ++ ("ac") ++ (D) ++ ("acca") ++ (D) ++ ("ccbcaab") ++ (D) ++ ("ab") ++ (D) ++ ("baacbaaaacccc"));
constraint
  (("cbbcccabbacbcccaac") ++ (D) ++ ("ba") ++ (D) ++ ("acabbb") ++ (D) ++ ("acaab") ++ (D) ++ ("cacabccaaaccccaabbca") ++ (D) ++ ("cabb") ++ (D) ++ ("b") ++ (D) ++ ("abaccccaabccaaba") ++ (D) ++ ("bbbcaaab"))
    =
    (("cbbcccabbacbcccaac") ++ (D) ++ ("ba") ++ (D) ++ ("acabbbbaacaab") ++ (D) ++ ("cacabccaaaccccaabbca") ++ (D) ++ ("cabb") ++ (D) ++ ("bbaabaccccaabccaa") ++ (D) ++ ("babbbcaaab"));
constraint
  (("babbccabacaa") ++ (D) ++ ("cbba") ++ (D) ++ ("baabaaccbacaabacbbccabcacab") ++ (D) ++ ("ccabccacc") ++ (D) ++ ("acaabbcccacbcccaacabbcaca") ++ (D) ++ ("cbcbcccaa"))
    =
    (("babbcca") ++ (D) ++ ("caabacb") ++ (D) ++ (D) ++ ("baabaacc") ++ (D) ++ ("caabacbbccabcacab") ++ (D) ++ ("ccabccaccbaacaabbcccacbcccaacabbcaca") ++ (D) ++ ("cbcbcccaa"));
constraint
  (("bbabbccbcbcabcaabbbb") ++ (D) ++ ("bbabcac") ++ (D) ++ ("acbcabcabbcbaacac") ++ (D) ++ ("b") ++ (D) ++ ("acaaccbcacbacacabbccabcacbcbcccabaa") ++ (D) ++ ("cabcbc"))
    =
    (("bbabbccbcbcabcaabbbb") ++ (D) ++ ("b") ++ (D) ++ ("bcac") ++ (D) ++ ("acbcabcabbcbaacacbab") ++ (D) ++ ("acaaccbcac") ++ (D) ++ ("cacabbccabcacbcbccca") ++ (D) ++ ("abacabcbc"));
constraint
  (("accbbbbcbbcbcbbbcbbbbaacacbaaaabcccbaabb") ++ (D) ++ ("cccacbba") ++ (D) ++ ("ccac") ++ (D) ++ ("baabbbcaaaaccbbab") ++ (D) ++ ("cccaacbb") ++ (D) ++ ("acbcbacac"))
    =
    (("accbbbbcbbcbcbbbcbbbbaacacbaaaabccc") ++ (D) ++ ("abbbacccacb") ++ (D) ++ ("baccacbabaabbbcaaaaccbbabbacccaacbbbaacbc") ++ (D) ++ ("cac"));
constraint
  (("cbccbcbb") ++ (D) ++ ("aacbbbcbbaaccccabcaabcaabcbbcbbc") ++ (D) ++ ("bccbacc") ++ (D) ++ ("cbaacaacacaa") ++ (D) ++ ("ccaaaabbcbbcacba") ++ (D) ++ ("cbccbbcabaa"))
    =
    (("cbccbcbbbaaacbbbcbbaaccccabcaabcaabcbbcbbc") ++ (D) ++ ("bcc") ++ (D) ++ ("ccbac") ++ (D) ++ ("acaacacaa") ++ (D) ++ ("ccaaaabbcbbcacba") ++ (D) ++ ("cbccbbca") ++ (D) ++ ("a"));
constraint
  ((D) ++ ("caabcaabbaa") ++ (D) ++ ("ccbb") ++ (D) ++ ("aaccbba") ++ (D) ++ ("bbcccbbacab") ++ (D) ++ ("ccbcaabbbcbcaabbcacabcbbabbcbacaaabca") ++ (D) ++ ("cbbbcb") ++ (D) ++ ("abbbbb"))
    =
    ((D) ++ ("caabcaabbaabaccbb") ++ (D) ++ ("aaccb") ++ (D) ++ ("babbcccbbacab") ++ (D) ++ ("ccbcaabbbcbcaabbcacabcbbabbcbacaaabca") ++ (D) ++ ("cbbbcb") ++ (D) ++ ("abbbbb"));
constraint
  (("abcbcbbcabcaa") ++ (D) ++ ("caaaccbacbbccccccabbccaccabba") ++ (D) ++ ("bbc") ++ (D) ++ ("acaccbcabcbac") ++ (D) ++ ("cbcbb") ++ (D) ++ ("cacacbccc") ++ (D) ++ ("abccb") ++ (D) ++ ("bcc") ++ (D))
    =
    (("abcbcbbcabcaabacaaacc") ++ (D) ++ ("cbbccccccabbccaccabba") ++ (D) ++ ("bbcbaacaccbcabc") ++ (D) ++ ("c") ++ (D) ++ ("cbcbbbacacacbccc") ++ (D) ++ ("abccb") ++ (D) ++ ("bccba"));
constraint
  (("abaaaacba") ++ (D) ++ ("baabccacbcaccbbbcbbaacaaacabbcaabbaccabbabcb") ++ (D) ++ ("abbbb") ++ (D) ++ ("abcac") ++ (D) ++ ("aabababbaacabcabbbcabacbb"))
    =
    (("abaaaacbaba") ++ (D) ++ ("abccacbcaccbbbcb") ++ (D) ++ ("acaaacabbcaabbaccab") ++ (D) ++ ("bcbbaabbbb") ++ (D) ++ ("abcacbaaababab") ++ (D) ++ ("acabcabbbca") ++ (D) ++ ("cbb"));
constraint
  (("baacaaa") ++ (D) ++ ("cab") ++ (D) ++ ("baacbbbccbccabccacaca") ++ (D) ++ ("bbbcaabbcaaa") ++ (D) ++ ("bbcccabbbb") ++ (D) ++ ("cababccabccccccacbbcbccbcbcbcabcb"))
    =
    ((D) ++ ("acaaa") ++ (D) ++ ("cab") ++ (D) ++ (D) ++ ("acbbbccbccabccacaca") ++ (D) ++ ("bbbcaabbcaaababbcccabbbbbaca") ++ (D) ++ ("bccabccccccacbbcbccbcbcbcabcb"));
constraint
  (("baabca") ++ (D) ++ ("acbccacccbbbcbbbbbbca") ++ (D) ++ ("ac") ++ (D) ++ ("bacaacbaaaacab") ++ (D) ++ ("abc") ++ (D) ++ ("ccaacccbbcbbccc") ++ (D) ++ ("bbcbcc") ++ (D) ++ ("cabccccccbbbbbc"))
    =
    (("baabca") ++ (D) ++ ("acbccacccbbbcbbbbbbcabaacbabacaacbaaaacab") ++ (D) ++ ("abc") ++ (D) ++ ("ccaacccbbcbbcccbabbcbccbacabccccccbbbbbc"));
constraint
  (("acbb") ++ (D) ++ ("ccbcbabbcacbaacb") ++ (D) ++ (D) ++ (D) ++ ("bcbbcaacacccabbbacbbbabcbbaacb") ++ (D) ++ ("aaccaaaacab") ++ (D) ++ ("abcabbbabcabbbaabccbbbc"))
    =
    (("acbb") ++ (D) ++ ("ccbcbabbcacbaacbbaba") ++ (D) ++ ("bcbbcaacacccabbbacbbbabcb") ++ (D) ++ ("acbbaaaccaaaacabbaabcabbbabcabb") ++ (D) ++ ("abccbbbc"));
constraint
  (("abcaabcbc") ++ (D) ++ ("abcacabcbbbabbabacabc") ++ (D) ++ ("bbcbac") ++ (D) ++ ("ccbabcca") ++ (D) ++ ("bbbbabbcbabbccbacccabaacbccaaccbaacabcccabab"))
    =
    (("abcaabcbc") ++ (D) ++ ("abcacabcbbbabbabacabc") ++ (D) ++ ("bbcbac") ++ (D) ++ ("ccbabccababbbbabbc") ++ (D) ++ ("bbcc") ++ (D) ++ ("ccca") ++ (D) ++ ("acbccaacc") ++ (D) ++ ("acabccca") ++ (D) ++ ("b"));
constraint
  (("baaac") ++ (D) ++ ("cbbaaccbbbcaabacbccacaacacab") ++ (D) ++ ("aaacbc") ++ (D) ++ ("acbcabcabaaac") ++ (D) ++ ("acaaaccabba") ++ (D) ++ ("cabbbcbbbbbbacbc") ++ (D) ++ ("acaaa"))
    =
    ((D) ++ ("aac") ++ (D) ++ ("cbbaaccbbbcaa") ++ (D) ++ ("cbccacaacacab") ++ (D) ++ ("aaacbcbaacbcabca") ++ (D) ++ ("aacbaacaaaccab") ++ (D) ++ ("bacabbbcbbbbbbacbc") ++ (D) ++ ("acaaa"));
constraint
  (("ccaaaccc") ++ (D) ++ ("baaaccabcbbbcc") ++ (D) ++ ("ccabbbcaabbbccc") ++ (D) ++ ("abcbcaaccb") ++ (D) ++ ("abca") ++ (D) ++ ("abbcaccbbac") ++ (D) ++ ("bbcacb") ++ (D) ++ ("cabaababccbaaa"))
    =
    (("ccaaacccbabaaaccabcbbbccbaccabbbcaabbbccc") ++ (D) ++ ("abcbcaaccbbaabca") ++ (D) ++ ("abbcaccb") ++ (D) ++ ("cbabbcacb") ++ (D) ++ ("cabaa") ++ (D) ++ ("bcc") ++ (D) ++ ("aa"));
constraint
  (("cbabacbaaacabbcaacbbaabc") ++ (D) ++ ("cccaaacac") ++ (D) ++ ("acaa") ++ (D) ++ ("bcaacbccc") ++ (D) ++ ("bbacaacacbbbcccabbc") ++ (D) ++ (D) ++ ("c") ++ (D) ++ ("bbacabbcccaba") ++ (D) ++ ("c"))
    =
    (("cba") ++ (D) ++ ("c") ++ (D) ++ ("aacabbcaacbbaabcbacccaaacacbaacaababcaacbcccbab") ++ (D) ++ ("caacacbbbcccabbcbabac") ++ (D) ++ ("b") ++ (D) ++ ("cabbcccababac"));
constraint
  ((D) ++ (D) ++ ("cbacbbbaccbccbc") ++ (D) ++ ("ccabcaabcbbcccbcccacbcbababccbabcccccbcbabbcaaa") ++ (D) ++ ("cbcaabcaaaccaccbb") ++ (D) ++ ("acbbacb"))
    =
    ((D) ++ (D) ++ ("cbacbbbaccbccbc") ++ (D) ++ ("ccabcaabcbbcccbcccacbc") ++ (D) ++ ("babcc") ++ (D) ++ ("bcccccbcbabbcaaabacbcaabcaaaccaccbbbaacbbacb"));
constraint
  (("accbbcaacbccbbcccbc") ++ (D) ++ ("aabbcaabbbbaabcbacabbccaaaaacaccbacaccababccbbacbcccabbccacacbbbacccaaacaaa"))
    =
    (("accbbcaacbccbbcccbcbaaabbcaabbb") ++ (D) ++ ("abc") ++ (D) ++ ("cabbccaaaaacaccbacacca") ++ (D) ++ ("bccb") ++ (D) ++ ("cbcccabbccacacbb") ++ (D) ++ ("cccaaacaaa"));
constraint
  (("ccacacaaca") ++ (D) ++ ("bbbcccbc") ++ (D) ++ (D) ++ (D) ++ ("b") ++ (D) ++ ("caccbbabacbbccbcccc") ++ (D) ++ ("caaacbbccbaacbbbbccbccacbccabbc") ++ (D) ++ (D) ++ ("baccbcababb"))
    =
    (("ccacacaacababbbcccbc") ++ (D) ++ ("ba") ++ (D) ++ ("b") ++ (D) ++ ("caccbba") ++ (D) ++ ("cbbccbccccbacaaacbbccbaacbbbbccbccacbccabbc") ++ (D) ++ (D) ++ (D) ++ ("ccbcababb"));
constraint
  (("bcccaccbcbcbbbcacbaca") ++ (D) ++ ("babbaaabbbbcacccbccacab") ++ (D) ++ ("bc") ++ (D) ++ ("ccccaacaaacbbcbbbbcaccba") ++ (D) ++ ("bccccca") ++ (D) ++ ("aa") ++ (D) ++ ("baccb"))
    =
    (("bcccaccbcbcbbbcac") ++ (D) ++ ("cabababbaaabbbbcacccbccacabbabcbaccccaacaaacbbcbbbbcaccba") ++ (D) ++ ("bcccccabaaaba") ++ (D) ++ ("ccb"));
constraint
  (("bbccacba") ++ (D) ++ ("aaabcbbcc") ++ (D) ++ ("ccabbbb") ++ (D) ++ ("cbccabcbccccacabacaac") ++ (D) ++ ("accccbbbbbccccacbbbcabccaaccbccbccaacccaaca"))
    =
    (("bbccacbabaaaabcbbccbaccabbbb") ++ (D) ++ ("cbccabcbccccaca") ++ (D) ++ ("caacbaaccccbbbbbccccacbbbcabccaaccbccbccaacccaaca"));
constraint
  (("aba") ++ (D) ++ ("bbcccaaabbccacabbaabbccaabcbbc") ++ (D) ++ ("bbbc") ++ (D) ++ ("ccabbaaccba") ++ (D) ++ ("caacbccc") ++ (D) ++ ("acccabbcbacaaaaccacabbbbcccaba"))
    =
    (("a") ++ (D) ++ (D) ++ ("bbcccaaabbccacabbaabbccaabcbbc") ++ (D) ++ ("bbbcbaccab") ++ (D) ++ ("accba") ++ (D) ++ ("caacbccc") ++ (D) ++ ("acccabbcbacaaaaccacabbbbcccaba"));
constraint
  (("cabacaabbbcbaaccab") ++ (D) ++ ("bcac") ++ (D) ++ ("ccaaababcb") ++ (D) ++ (D) ++ ("aaaa") ++ (D) ++ ("acbccabcbabac") ++ (D) ++ ("c") ++ (D) ++ ("a") ++ (D) ++ ("caaccacbabbcba") ++ (D) ++ ("bacbcccbc") ++ (D) ++ ("aa"))
    =
    (("ca") ++ (D) ++ ("caabbbcbaaccab") ++ (D) ++ ("bcacbaccaaababcbbabaaaaabaacbccabcba") ++ (D) ++ ("cbac") ++ (D) ++ ("a") ++ (D) ++ ("caaccacbabbc") ++ (D) ++ ("babacbcccbcbaaa"));
constraint
  (("bcbcccaccccbbbccbbbacac") ++ (D) ++ ("accbcab") ++ (D) ++ ("acbccbcabcbcacabaaccbaabcbb") ++ (D) ++ ("cabb") ++ (D) ++ ("caacaacabbccbcabbac") ++ (D) ++ ("babacc"))
    =
    (("bcbcccaccccbbbccbbbacacbaaccbcab") ++ (D) ++ ("acbccbcabcbcacabaaccbaabcbb") ++ (D) ++ ("cabb") ++ (D) ++ ("caacaacabbccbcab") ++ (D) ++ ("c") ++ (D) ++ (D) ++ (D) ++ ("cc"));
constraint
  (("bbcccaacc") ++ (D) ++ ("bacaaa") ++ (D) ++ ("cbc") ++ (D) ++ ("bcbcbca") ++ (D) ++ ("aaabc") ++ (D) ++ ("abbc") ++ (D) ++ ("ccaacaaccbbcbbabaabbca") ++ (D) ++ ("abbbcbccccbcacc") ++ (D) ++ ("bcabcaa") ++ (D))
    =
    (("bbcccaaccba") ++ (D) ++ ("caaa") ++ (D) ++ ("cbc") ++ (D) ++ ("bcbcbcabaaaabc") ++ (D) ++ ("abbcbaccaacaaccbbcbbabaabbca") ++ (D) ++ ("abbbcbccccbcacc") ++ (D) ++ ("bcabcaaba"));
constraint
  (("baaabcbcacab") ++ (D) ++ ("cacbbbbaaaccacaaccbcbcacacc") ++ (D) ++ ("bbcbbbcaaabbacbbcccaababaaabacacbbb") ++ (D) ++ ("bcacbbbc") ++ (D) ++ ("caacaa"))
    =
    ((D) ++ ("aabcbcacabbacacbbb") ++ (D) ++ ("aaccacaaccbcbcacaccbabbcbbbcaaab") ++ (D) ++ ("cbbcccaababaaa") ++ (D) ++ ("cacbbbbabcacbbbcbacaacaa"));
constraint
  (("ac") ++ (D) ++ ("bcaababcbbaacbcaaaacabaacba") ++ (D) ++ ("abbccbca") ++ (D) ++ ("bcbaccccacaba") ++ (D) ++ ("ababcbbcbbcacabaabbbcccccbacc") ++ (D) ++ ("ccccc") ++ (D))
    =
    (("acbabcaababcb") ++ (D) ++ ("acbcaaaacabaacbabaabbccbcababc") ++ (D) ++ ("ccccacababaa") ++ (D) ++ ("bcbbcbbcaca") ++ (D) ++ ("abbbcccccbaccbacccccba"));
constraint
  (("ab") ++ (D) ++ ("b") ++ (D) ++ ("ccbacccaaaaccbacbbabcbbbbcacaaacbca") ++ (D) ++ ("ccaa") ++ (D) ++ ("ccacbcaaabcbcbbbaacc") ++ (D) ++ ("abcacacacbbcca") ++ (D) ++ ("cccaac") ++ (D))
    =
    (("abbab") ++ (D) ++ ("cc") ++ (D) ++ ("cccaaaaccbacb") ++ (D) ++ ("bcbbbbcacaaacbcabaccaa") ++ (D) ++ ("ccacbcaaabcbcbbbaaccbaabcacacacbbccabacccaacba"));
constraint
  (("b") ++ (D) ++ ("cbbc") ++ (D) ++ ("cb") ++ (D) ++ ("bccaaccbbca") ++ (D) ++ ("caccabc") ++ (D) ++ ("bcb") ++ (D) ++ ("aacbabb") ++ (D) ++ ("abbacbbcccc") ++ (D) ++ ("abcbcab") ++ (D) ++ (D) ++ ("cabcabcacaaaccc") ++ (D) ++ ("acabbb"))
    =
    (("bbacbbc") ++ (D) ++ ("cb") ++ (D) ++ ("bccaaccbbca") ++ (D) ++ ("caccabcbabcb") ++ (D) ++ ("aac") ++ (D) ++ ("bb") ++ (D) ++ ("ab") ++ (D) ++ ("cbbcccc") ++ (D) ++ ("abcbcabba") ++ (D) ++ ("cabcabcacaaacccbaacabbb"));
constraint
  (("caacabccbaab") ++ (D) ++ ("bcbaabcacca") ++ (D) ++ ("cabccaaabb") ++ (D) ++ ("ccaaacbaabcbacaacab") ++ (D) ++ ("cac") ++ (D) ++ ("cccacbccccaaacab") ++ (D) ++ ("acacaa") ++ (D) ++ ("aacab"))
    =
    (("caacabcc") ++ (D) ++ ("abbabc") ++ (D) ++ ("abcaccabacabccaaabb") ++ (D) ++ ("ccaaacbaabc") ++ (D) ++ ("caacab") ++ (D) ++ ("cac") ++ (D) ++ ("cccacbccccaaacab") ++ (D) ++ ("acacaa") ++ (D) ++ ("aacab"));
constraint
  (("ababcabaacbaabc") ++ (D) ++ ("cccaccaaaaaaaabacbccc") ++ (D) ++ ("cccc") ++ (D) ++ ("accbabbbcacbaaccabcbaabccbbcbbbcbbaca") ++ (D) ++ ("caaac") ++ (D) ++ ("bacb"))
    =
    (("ababca") ++ (D) ++ ("ac") ++ (D) ++ ("abcbacccaccaaaaaaaabacbcccbaccccbaacc") ++ (D) ++ ("bbbcac") ++ (D) ++ ("accabc") ++ (D) ++ ("abccbbcbbbcbbaca") ++ (D) ++ ("caaac") ++ (D) ++ ("bacb"));
constraint
  (("cbbaaccacaacaaaabacabbcbaacabbacabcbbcabbbacabb") ++ (D) ++ ("ab") ++ (D) ++ ("accccbb") ++ (D) ++ ("cacbccaaacbcbbbacca") ++ (D) ++ ("aaaacbbccaabb"))
    =
    (("cbbaaccacaacaaaa") ++ (D) ++ ("cabbcbaacab") ++ (D) ++ ("cabcbbcabbbacabbbaab") ++ (D) ++ ("accccbbbacacbccaaacbcbb") ++ (D) ++ ("cca") ++ (D) ++ ("aaaacbbccaabb"));
constraint
  ((D) ++ ("acaaabcbabcbcabaaccbbaccacaa") ++ (D) ++ ("bcabaaacacaacacbbbcbccbbcaccaabccbabacabbccacabc") ++ (D) ++ ("a") ++ (D) ++ ("bbcbccaccbc"))
    =
    ((D) ++ ("acaaabc") ++ (D) ++ ("bcbcabaaccbbaccacaa") ++ (D) ++ ("bcabaaacacaacacbbbcbccbbcaccaabccba") ++ (D) ++ ("cabbccacabc") ++ (D) ++ ("a") ++ (D) ++ ("bbcbccaccbc"));
constraint
  (("aa") ++ (D) ++ ("ccccccaaccaccabbbbcbbcbab") ++ (D) ++ ("accababbaaabaaabc") ++ (D) ++ ("acbbbccccaaccbcac") ++ (D) ++ ("babcbca") ++ (D) ++ ("bcbbbcabccabccabac"))
    =
    (("aabaccccccaaccaccabbbbcbbcbab") ++ (D) ++ ("accabab") ++ (D) ++ ("aa") ++ (D) ++ ("aabc") ++ (D) ++ ("acbbbccccaaccbcac") ++ (D) ++ ("babcbca") ++ (D) ++ ("bcbbbcabccabcca") ++ (D) ++ ("c"));
constraint
  (("cacaccbbcbacaac") ++ (D) ++ ("aacabcaaacb") ++ (D) ++ ("ccbabbcbacaabbbaaabb") ++ (D) ++ ("acaccaaacccacccaaccaa") ++ (D) ++ ("aacacaabccccbbbbc") ++ (D) ++ ("cc"))
    =
    (("cacaccbbc") ++ (D) ++ ("caacbaaacabcaaacbbaccbabbc") ++ (D) ++ ("caabb") ++ (D) ++ ("aabbbaacaccaaacccacccaaccaabaaacacaabccccbbbbc") ++ (D) ++ ("cc"));
constraint
  (("aaccbbaacaaaaacbacbbcaccbccbbb") ++ (D) ++ ("aaccaa") ++ (D) ++ ("abb") ++ (D) ++ ("cca") ++ (D) ++ ("ccaab") ++ (D) ++ ("aaaaccccbccabcacaabbcbbccc") ++ (D) ++ ("caaccabcbcb"))
    =
    (("aaccb") ++ (D) ++ ("acaaaaacbacbbcaccbccbbb") ++ (D) ++ ("aaccaa") ++ (D) ++ ("abbbacca") ++ (D) ++ ("ccaab") ++ (D) ++ ("aaaaccccbccabcacaabbcbbccc") ++ (D) ++ ("caaccabcbcb"));
constraint
  (("caabb") ++ (D) ++ ("cccabbcbcacaaabc") ++ (D) ++ ("accccccccabcbbabccbccaaacbcabbbcbbccbbbca") ++ (D) ++ ("cbcbbcaabb") ++ (D) ++ ("ccabbcbcbacaaccc"))
    =
    (("caabbbacccabbcbcacaaabc") ++ (D) ++ ("accccccccabcb") ++ (D) ++ ("bccbccaaacbcabbbcbbccbbbca") ++ (D) ++ ("cbcbbcaabb") ++ (D) ++ ("ccabbcbcbacaaccc"));
constraint
  (("cccbccaabaa") ++ (D) ++ ("caaacac") ++ (D) ++ ("bb") ++ (D) ++ ("bcaac") ++ (D) ++ ("a") ++ (D) ++ ("ac") ++ (D) ++ ("abcaacaacbaacacbccaacbbbcbbaacc") ++ (D) ++ ("accbbbbcbbcbabccbccabca"))
    =
    (("cccbccaabaa") ++ (D) ++ ("caaacacbabb") ++ (D) ++ ("bcaac") ++ (D) ++ ("abaac") ++ (D) ++ ("abcaacaacbaacacbccaacbbbcb") ++ (D) ++ ("acc") ++ (D) ++ ("accbbbbcbbc") ++ (D) ++ ("bccbccabca"));
constraint
  (("bb") ++ (D) ++ ("aaccacbbbb") ++ (D) ++ ("cb") ++ (D) ++ ("ccabbb") ++ (D) ++ ("abcabcac") ++ (D) ++ ("ccca") ++ (D) ++ ("caca") ++ (D) ++ ("bcbbcaaacc") ++ (D) ++ ("cbcaabcacbbcac") ++ (D) ++ ("abccbbabccabcbbacb"))
    =
    (("bbbaaaccacbbbb") ++ (D) ++ ("cb") ++ (D) ++ ("ccabbb") ++ (D) ++ ("abcabcacbacccabacaca") ++ (D) ++ ("bcbbcaaacc") ++ (D) ++ ("cbcaabcacbbcacbaabccbbabccabcbbacb"));
constraint
  (("caaacc") ++ (D) ++ ("bacaaa") ++ (D) ++ ("cababcbcbbaacaacaabaaabaaaac") ++ (D) ++ ("bcbaaaacbacaabcbacaaab") ++ (D) ++ ("aacaaccacbbcbccbbcccbabcab"))
    =
    (("caaaccbabacaaa") ++ (D) ++ ("cababcbcbbaacaacaa") ++ (D) ++ ("aabaaaac") ++ (D) ++ ("bcbaaaacbacaabc") ++ (D) ++ ("caaabbaaacaaccacbbcbccbbccc") ++ (D) ++ ("bcab"));
constraint
  (("acbaaccabaab") ++ (D) ++ ("aababbacaacaabbbcccbabccabbbabaaabcaabcaccabc") ++ (D) ++ (D) ++ ("bc") ++ (D) ++ ("caaaccbbbcaaaaabcacac") ++ (D) ++ (D) ++ ("ccca"))
    =
    (("acbaacca") ++ (D) ++ ("abbaaababbacaacaabbbccc") ++ (D) ++ ("bccabbbabaaabcaabcaccabc") ++ (D) ++ (D) ++ ("bc") ++ (D) ++ ("caaaccbbbcaaaaabcacac") ++ (D) ++ ("baccca"));
constraint
  (("cbbbccbbcccccaa") ++ (D) ++ ("bbbbacacaaacbbbccbccaacc") ++ (D) ++ ("bcbcc") ++ (D) ++ ("cacbbbc") ++ (D) ++ ("caabbbcac") ++ (D) ++ ("ac") ++ (D) ++ ("abbbbbccacbbcbccaaa") ++ (D) ++ ("a"))
    =
    (("cbbbccbbcccccaababbb") ++ (D) ++ ("cacaaacbbbccbccaacc") ++ (D) ++ ("bcbccbacacbbbc") ++ (D) ++ ("caabbbcacbaac") ++ (D) ++ ("abbbbbccacbbcbccaaa") ++ (D) ++ ("a"));
constraint
  (("abaabcbaacaccbbbcc") ++ (D) ++ ("cbabcccacac") ++ (D) ++ ("cbccaabaaaca") ++ (D) ++ ("cabcccbbcccb") ++ (D) ++ ("bbcaaaa") ++ (D) ++ ("ab") ++ (D) ++ ("caccabccccbbacacbabbba"))
    =
    (("a") ++ (D) ++ ("abcbaacaccbbbcc") ++ (D) ++ ("c") ++ (D) ++ ("bcccacac") ++ (D) ++ ("cbccaabaaacabacabcccbbcccb") ++ (D) ++ ("bbcaaaa") ++ (D) ++ ("abbacaccabccccb") ++ (D) ++ ("cacbabb") ++ (D));
constraint
  (("aabbcbbcabccaccaaaaaaacbbabbbcbbcabccccaabccbaaacabcbbacbaacbbcbbccacccbccbccc") ++ (D) ++ ("abcbba") ++ (D) ++ ("ac") ++ (D) ++ ("bbcb"))
    =
    (("aabbcbbcabccaccaaaaaaacbbabbbcbbcabccccaabccbaaacabcb") ++ (D) ++ ("cbaacbbcbbccacccbccbcccbaabcb") ++ (D) ++ ("baac") ++ (D) ++ ("bbcb"));
constraint
  (("bca") ++ (D) ++ ("bbccaccbcbccccaa") ++ (D) ++ ("acacaa") ++ (D) ++ ("abbbacbccb") ++ (D) ++ ("aaabaabbcabccbc") ++ (D) ++ ("aacbaabbbcbc") ++ (D) ++ ("abcaacbc") ++ (D) ++ ("cbbbcbbbcbab"))
    =
    (("bca") ++ (D) ++ ("bbccaccbcbccccaa") ++ (D) ++ ("acacaa") ++ (D) ++ ("abb") ++ (D) ++ ("cbccb") ++ (D) ++ ("aaabaabbcabccbc") ++ (D) ++ ("aacbaabbbcbc") ++ (D) ++ ("abcaacbc") ++ (D) ++ ("cbbbcbbbc") ++ (D) ++ ("b"));
constraint
  (("aaccbcbaaaabcccacbcccababccccbccb") ++ (D) ++ (D) ++ ("aaaccbac") ++ (D) ++ ("caa") ++ (D) ++ ("bccccbbaa") ++ (D) ++ ("bcbbccbaabbccacbcaaacbaabccaaaacc"))
    =
    (("aaccbc") ++ (D) ++ ("aaabcccacbcccababccccbccbba") ++ (D) ++ ("aaaccbacbacaababccccbbaababcbbcc") ++ (D) ++ ("abbccacbcaaacbaabccaaaacc"));
constraint
  (("ccbbbccbccbacacbabbccacacbccccbcabccbacbcccaacacabcaa") ++ (D) ++ ("acbcccaacacabaaacabbcacaa") ++ (D) ++ ("acbacccccbacca"))
    =
    (("ccbbbccbcc") ++ (D) ++ ("cac") ++ (D) ++ ("bbccacacbccccbcabccbacbcccaacacabcaa") ++ (D) ++ ("acbcccaacacabaaacabbcacaa") ++ (D) ++ ("acbacccccbacca"));
constraint
  (("c") ++ (D) ++ ("cacabb") ++ (D) ++ ("bbacbbbacbbcccaaccb") ++ (D) ++ ("abcccc") ++ (D) ++ ("bbbcccbcbaaacabcaaabcbbbbbaa") ++ (D) ++ (D) ++ ("babbccacbcbb") ++ (D) ++ ("aca") ++ (D) ++ ("bbbcb"))
    =
    (("c") ++ (D) ++ ("cacabb") ++ (D) ++ ("b") ++ (D) ++ ("cbb") ++ (D) ++ ("cbbcccaaccbbaabcccc") ++ (D) ++ ("bbbcccbc") ++ (D) ++ ("aacabcaaabcbbbbbaa") ++ (D) ++ ("ba") ++ (D) ++ ("bbccacbcbbbaacababbbcb"));
constraint
  (("c") ++ (D) ++ ("accacababbbccbcabc") ++ (D) ++ ("acaabaa") ++ (D) ++ (D) ++ ("bbaacaccaaaaaacaacccbcbaaaac") ++ (D) ++ ("cbacaaacccbbacabaaacaaccabbcacbc"))
    =
    (("cbaaccacababbbccbcabc") ++ (D) ++ ("acaabaaba") ++ (D) ++ ("b") ++ (D) ++ ("acaccaaaaaacaacccbcbaaaac") ++ (D) ++ ("cbacaaacccb") ++ (D) ++ ("cabaaacaaccabbcacbc"));
constraint
  (("baacaaabcaabbccbbbbcbcc") ++ (D) ++ ("bcbcaacbcabbccacab") ++ (D) ++ ("bbccbabbbcccabccbacbcccbc") ++ (D) ++ ("cbbc") ++ (D) ++ (D) ++ ("c") ++ (D) ++ ("cbcacaacbcaac"))
    =
    ((D) ++ ("acaaabcaabbccbbbbcbccbabcbcaacbcabbccacab") ++ (D) ++ ("bbcc") ++ (D) ++ ("bbbcccabcc") ++ (D) ++ ("cbcccbc") ++ (D) ++ ("cbbcba") ++ (D) ++ ("c") ++ (D) ++ ("cbcacaacbcaac"));
constraint
  (("bccabc") ++ (D) ++ ("aac") ++ (D) ++ ("bcbcacaaaaccbacccbacbbbcaabcacb") ++ (D) ++ ("acccbb") ++ (D) ++ ("cbcacbcaabca") ++ (D) ++ ("baabcaacaabbbbcbcbcccaccaabc"))
    =
    (("bccabc") ++ (D) ++ ("aacbabcbcacaaaacc") ++ (D) ++ ("ccc") ++ (D) ++ ("cbbbcaabcacbbaacccbb") ++ (D) ++ ("cbcacbcaabca") ++ (D) ++ (D) ++ ("abcaacaabbbbcbcbcccaccaabc"));
solve satisfy;
