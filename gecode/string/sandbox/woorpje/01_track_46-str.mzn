var string: I;
var string: B;
var string: G;
var string: F;
var string: A;
var string: J;
constraint
  ((B) ++ ("aaaa") ++ (J) ++ (A) ++ (A) ++ ("aa")) =
    ((F) ++ (A) ++ ("aaaaaa") ++ (G) ++ ("aaaaaaaaaaaaaaa") ++ (G) ++ (I) ++ (A) ++ (A) ++ ("a"));
solve satisfy;
