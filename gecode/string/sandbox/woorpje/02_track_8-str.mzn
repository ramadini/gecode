var string: B;
var string: A;
constraint
  ((A) ++ ("a") ++ (A) ++ ("b") ++ (B)) =
    (("a") ++ (A) ++ (B) ++ (B) ++ ("baa"));
solve satisfy;
