var string: C;
var string: G;
var string: A;
constraint
  (("a") ++ (A) ++ (A) ++ ("aaa") ++ (C) ++ ("b") ++ (A) ++ ("a") ++ (A) ++ ("bbabbcbaacbacaabbabb") ++ (A) ++ (A) ++ ("bbba") ++ (A) ++ ("ba") ++ (A) ++ ("b"))
    =
    (("a") ++ (A) ++ (A) ++ ("aaababccabb") ++ (G) ++ ("cbaa") ++ (A) ++ ("ba") ++ (A) ++ ("aabbabbc") ++ (A) ++ ("bbbacbacb"));
constraint
  ((A) ++ (A) ++ ("ca") ++ (A) ++ ("bab") ++ (A) ++ ("abaa") ++ (A) ++ ("aaaba") ++ (A) ++ ("bccaab") ++ (A) ++ ("bcccacacaccca") ++ (A) ++ ("bcbabaabaaaacabbb") ++ (A) ++ ("bbccbabac") ++ (A))
    =
    (("ccca") ++ (A) ++ ("babcabaacaaabacbccaab") ++ (A) ++ ("b") ++ (A) ++ (A) ++ ("caca") ++ (A) ++ ("a") ++ (A) ++ ("c") ++ (A) ++ ("a") ++ (A) ++ ("b") ++ (A) ++ ("babaabaaaacabbbcbbccbaba") ++ (A) ++ (A));
constraint
  (("baa") ++ (A) ++ ("bcbbaaa") ++ (A) ++ ("bbbacaabc") ++ (A) ++ ("baaaaba") ++ (A) ++ ("aaba") ++ (A) ++ ("bbbbbbbbb") ++ (A) ++ ("cccbaacc") ++ (A) ++ ("cb") ++ (A) ++ ("babb") ++ (A) ++ ("bbabb") ++ (A) ++ ("b"))
    =
    (("baa") ++ (A) ++ ("bcbbaaacbbbacaabccbaaaaba") ++ (A) ++ ("aaba") ++ (A) ++ ("bbbbbbbbbc") ++ (A) ++ ("ccbaaccc") ++ (A) ++ ("bcbabbcbbabbcb"));
constraint
  (("ab") ++ (A) ++ ("cbcab") ++ (A) ++ (A) ++ (A) ++ ("cbbbabaacbaaacac") ++ (A) ++ (A) ++ ("abbb") ++ (A) ++ ("bacabbcb") ++ (A) ++ ("abbbbacbc") ++ (A) ++ ("bbbbcbcccac") ++ (A) ++ ("b") ++ (A) ++ ("ab"))
    =
    (("ab") ++ (A) ++ ("cbcab") ++ (A) ++ (A) ++ ("ccbbbabaa") ++ (A) ++ ("baaacac") ++ (A) ++ (A) ++ ("abbbcba") ++ (A) ++ ("abb") ++ (A) ++ ("b") ++ (A) ++ ("abbbba") ++ (A) ++ ("bccbbbbcb") ++ (A) ++ ("cca") ++ (A) ++ (A) ++ ("bcab"));
constraint
  ((A) ++ ("aaa") ++ (A) ++ ("bcbcca") ++ (A) ++ ("bb") ++ (A) ++ ("acaaccacabbaa") ++ (A) ++ ("a") ++ (A) ++ ("a") ++ (A) ++ ("baa") ++ (A) ++ ("babbbbbbbabbaabbaacabc") ++ (A) ++ ("aacaa") ++ (A) ++ ("c") ++ (A) ++ (A))
    =
    (("caaacbcb") ++ (A) ++ ("cacbb") ++ (A) ++ ("acaaccacabbaaca") ++ (A) ++ ("acbaacbabbbbbbbabbaabbaacab") ++ (A) ++ ("caa") ++ (A) ++ ("aacc") ++ (A) ++ ("c"));
constraint
  (("abccaabbabcbbba") ++ (A) ++ ("bbcbaa") ++ (A) ++ (A) ++ ("aabba") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("bababcbccaabbcbbbabaabba") ++ (A) ++ ("cbaac") ++ (A) ++ ("caaab"))
    =
    (("abc") ++ (A) ++ ("aabbab") ++ (A) ++ ("bbbacbb") ++ (A) ++ ("baa") ++ (A) ++ (A) ++ ("aabba") ++ (A) ++ ("accbabab") ++ (A) ++ ("bccaabb") ++ (A) ++ ("bbbabaabba") ++ (A) ++ ("cbaacccaaab"));
constraint
  (("bbaa") ++ (A) ++ ("ccbbb") ++ (A) ++ ("aaaabcab") ++ (A) ++ ("ccaca") ++ (A) ++ ("acaaabccbaaabbcaba") ++ (A) ++ ("bbbaba") ++ (A) ++ ("c") ++ (A) ++ (A) ++ ("aabbb") ++ (A) ++ ("abbaba") ++ (A) ++ ("a"))
    =
    (("bbaa") ++ (A) ++ (A) ++ ("cbbb") ++ (A) ++ ("aaaabcabc") ++ (A) ++ ("caca") ++ (A) ++ ("acaaab") ++ (A) ++ ("cbaaabbcaba") ++ (A) ++ ("bbbaba") ++ (A) ++ ("c") ++ (A) ++ (A) ++ ("aabbbcabbabaca"));
constraint
  (("b") ++ (A) ++ ("aba") ++ (A) ++ ("bcbbb") ++ (A) ++ ("bacb") ++ (A) ++ (A) ++ ("c") ++ (A) ++ ("bcabba") ++ (A) ++ ("ac") ++ (A) ++ ("abaa") ++ (A) ++ ("b") ++ (A) ++ ("aaba") ++ (A) ++ ("abb") ++ (A) ++ ("aab") ++ (A) ++ ("bb") ++ (A) ++ ("acbccb") ++ (A) ++ ("bbba") ++ (A) ++ ("baca"))
    =
    (("bcabacb") ++ (A) ++ ("bbbcba") ++ (A) ++ ("bccc") ++ (A) ++ ("bcabbacac") ++ (A) ++ ("abaa") ++ (A) ++ ("bcaabacabb") ++ (A) ++ ("aab") ++ (A) ++ ("bbca") ++ (A) ++ ("bccbcbbbacbaca"));
solve satisfy;
