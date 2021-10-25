var string: C;
var string: B;
var string: G;
var string: J;
constraint
  ((G) ++ ("aaaaaaaa") ++ (G) ++ ("aaaa") ++ (G) ++ (G) ++ ("aaaaaa") ++ (J) ++ ("a"))
    = ((C) ++ (B) ++ ("aaaaaaaaaaaa") ++ (G) ++ ("aa"));
solve satisfy;
