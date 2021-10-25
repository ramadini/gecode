var string: C;
var string: D;
constraint ((D) ++ ("a") ++ (D)) = (("a") ++ (C) ++ ("aa"));
solve satisfy;
