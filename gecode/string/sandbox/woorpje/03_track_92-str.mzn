var string: D;
var string: E;
var string: F;
constraint
  ((D) ++ ("aa") ++ (D) ++ ("ab") ++ (F)) = (("a") ++ (E) ++ ("aaabaa"));
solve satisfy;
