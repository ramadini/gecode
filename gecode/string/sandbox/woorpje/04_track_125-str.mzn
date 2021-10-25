var string: B;
var string: C;
constraint
  (("ccabcccaccb") ++ (C) ++ ("ccaabbacaacabacbacaccabbbbbccaccabbccbcccbcbaaccabccaa"))
    = (("c") ++ (B) ++ ("cbcccbcbaaccabccaa"));
solve satisfy;
