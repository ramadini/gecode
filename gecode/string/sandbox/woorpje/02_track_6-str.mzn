var string: B;
var string: E;
var string: A;
var string: C;
var string: D;
constraint
  ((A) ++ ("a") ++ (A) ++ ("b") ++ (B) ++ ("b") ++ (C) ++ ("b") ++ (D) ++ ("b") ++ (E))
    =
    (("a") ++ (A) ++ (B) ++ (B) ++ ("b") ++ (C) ++ (C) ++ ("b") ++ (D) ++ (D) ++ ("b") ++ (E) ++ (E) ++ ("baa"));
solve satisfy;
