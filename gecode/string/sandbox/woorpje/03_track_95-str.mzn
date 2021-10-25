var string: D;
var string: E;
constraint ((D) ++ ("a") ++ (D)) = (("a") ++ (E) ++ ("aa"));
solve satisfy;
