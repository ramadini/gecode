var string: F;
var string: A;
constraint
  (("b") ++ (A) ++ (A) ++ (A) ++ ("cbc")) = (("b") ++ (F) ++ ("bc"));
solve satisfy;
