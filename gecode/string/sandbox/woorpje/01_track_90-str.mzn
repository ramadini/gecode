var string: B;
var string: F;
constraint
  (("ccacbbcbcaa") ++ (B) ++ ("bbcaa")) =
    (("ccacbbcbcaabaaacbabcccbcbbbccab") ++ (F) ++ ("ccbcbcbabbcaccbbaaccacacccaabcccaaaaaabcbbcaa"));
solve satisfy;
