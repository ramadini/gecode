var string: B;
var string: A;
constraint
  (("bcbabcaabbabbcbcbcaabbcccbab") ++ (B) ++ ("aaaababba")) =
    (("bcbab") ++ (A) ++ ("caabbcccbabcabcabcbbcacbbccccbcabaaabbabbbaacccbbaaacbaaaccacbacccabcacabcbaaaaababba"));
solve satisfy;
