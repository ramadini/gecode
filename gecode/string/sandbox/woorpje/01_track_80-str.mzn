var string: C;
var string: G;
var string: E;
var string: A;
constraint
  ((C) ++ (A) ++ ("aa") ++ (E) ++ (E) ++ ("aaaaaaa") ++ (A) ++ (A)) =
    ((G) ++ (E) ++ ("aaaaa"));
solve satisfy;
