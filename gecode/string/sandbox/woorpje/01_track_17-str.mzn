var string: H;
var string: F;
var string: L;
constraint
  (("abbbcaabbabccbcabaacbcbacab") ++ (H) ++ ("ccabbcacbbbcaa") ++ (L) ++ ("aacc"))
    =
    (("abbbcaabbabccbcabaacbc") ++ (L) ++ (F) ++ ("baabcaabacabbcacbccaaccbabbbccbccabbcacbbbcaabacaacc"));
solve satisfy;
