var string: I;
var string: F;
var string: A;
var string: J;
constraint
  (("cabccabbaccbc") ++ (F) ++ ("cabbc")) =
    (("cabccabbaccbcca") ++ (A) ++ ("aabc") ++ (J) ++ ("aaabaacbaacabbc"));
constraint
  (("cbcbbacca") ++ (I) ++ ("cacbbbaacabacbbbbcabcaaaaaaabbbcabcbbabaabcabcbc"))
    =
    (("cbcbbacca") ++ (I) ++ ("cacbbbaacabacbbbbcabcaaaaaaabbbcabcbbabaabcabcbc"));
constraint
  (("cbb") ++ (I) ++ ("bcacccbaabccbacbbbbbacaccacaacacacbabcacccaacbbaacccaa"))
    =
    (("cbbbcbabbcacccbaabccbacbbbbbacaccacaacacacbabcacccaacbbaacccaa") ++ (""));
solve satisfy;
