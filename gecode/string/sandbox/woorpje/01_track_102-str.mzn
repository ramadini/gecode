var string: C;
var string: G;
var string: F;
constraint
  ((C) ++ (G) ++ (G) ++ ("aaaaaaaaaaa")) = ((F) ++ (G) ++ ("aaaaaaaaaaaaaa"));
solve satisfy;
