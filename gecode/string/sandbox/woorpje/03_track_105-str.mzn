var string: D;
var string: E;
constraint ((E) ++ ("a") ++ (E)) = (("a") ++ (D) ++ ("aa"));
solve satisfy;
