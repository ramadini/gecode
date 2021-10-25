var string: F;
var string: E;
constraint ((F) ++ ("a") ++ (F)) = (("a") ++ (E) ++ ("aa"));
solve satisfy;
