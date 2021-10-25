var string: B;
var string: F;
var string: E;
var string: G;
var string: A;
var string: C;
var string: D;
constraint
  ((A) ++ ("a") ++ (A) ++ ("b") ++ (B) ++ ("b") ++ (C) ++ ("b") ++ (D) ++ ("b") ++ (E) ++ ("b") ++ (F) ++ ("b") ++ (G))
    =
    (("a") ++ (A) ++ (B) ++ (B) ++ ("b") ++ (C) ++ (C) ++ ("b") ++ (D) ++ (D) ++ ("b") ++ (E) ++ (E) ++ ("b") ++ (F) ++ (F) ++ ("b") ++ (G) ++ (G) ++ ("baa"));
solve satisfy;
