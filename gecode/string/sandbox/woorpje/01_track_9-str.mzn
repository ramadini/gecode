var string: B;
var string: A;
constraint
  (("aabcbcbabaaabbbcacbbcccbcbbcaabbabcacccb") ++ (A) ++ ("aacccaaaacbbabccc"))
    = (("aabcbcbab") ++ (B) ++ ("aabcacccaaaaacccaaaacbbabccc"));
solve satisfy;
