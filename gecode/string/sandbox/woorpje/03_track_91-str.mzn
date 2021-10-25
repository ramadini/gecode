var string: C;
var string: D;
constraint ((C) ++ ("a") ++ (C)) = (("a") ++ (D) ++ ("aa"));
solve satisfy;
