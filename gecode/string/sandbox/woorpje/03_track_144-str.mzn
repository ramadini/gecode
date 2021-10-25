var string: C;
var string: G;
constraint ((G) ++ ("a") ++ (G)) = (("a") ++ (C) ++ ("aa"));
solve satisfy;
