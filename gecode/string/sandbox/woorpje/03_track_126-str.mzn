var string: C;
var string: F;
constraint ((F) ++ ("a") ++ (F)) = (("a") ++ (C) ++ ("aa"));
solve satisfy;
