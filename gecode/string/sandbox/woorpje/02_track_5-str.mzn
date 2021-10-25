var string: B;
var string: A;
var string: C;
constraint
  ((A) ++ ("a") ++ (A) ++ ("b") ++ (B) ++ ("b") ++ (C)) =
    (("a") ++ (A) ++ (B) ++ (B) ++ ("b") ++ (C) ++ (C) ++ ("baa"));
solve satisfy;
