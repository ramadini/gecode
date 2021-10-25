var string: D;
var string: A;
constraint
  (("acab") ++ (A) ++ ("bccbccabacbcccbabbbbaaccaccbacbcacabcbaaabcabacacaaccbcccacaaabaaa"))
    =
    (("acabccbacbbbbbcbbcccbbaacabb") ++ (D) ++ ("ccaccbacbcacabcbaaabcabacacaaccbcccacaaabaaa"));
solve satisfy;
