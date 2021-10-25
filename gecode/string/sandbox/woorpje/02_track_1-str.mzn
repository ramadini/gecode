var string: B;
var string: D;
var string: A;
var string: C;
constraint
  ((A) ++ ("a") ++ (A) ++ ("b") ++ (B) ++ ("b") ++ (C) ++ ("b") ++ (D)) =
    (("a") ++ (A) ++ (B) ++ (B) ++ ("b") ++ (C) ++ (C) ++ ("b") ++ (D) ++ (D) ++ ("baa"));
solve satisfy;
