var string: H;
var string: G;
var string: E;
var string: A;
constraint
  ((E) ++ (H) ++ (A) ++ (E) ++ (E) ++ (H) ++ ("a")) =
    ((G) ++ (G) ++ (H) ++ (A));
solve satisfy;
