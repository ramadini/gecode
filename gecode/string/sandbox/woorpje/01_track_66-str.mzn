var string: D;
var string: E;
constraint
  (("bcccaccbaaaabbcbc") ++ (E) ++ ("ccbbbccc")) =
    (("bcccaccbaaaabbcbcb") ++ (D) ++ ("bbccc"));
solve satisfy;
