var string: B;
var string: E;
var string: J;
var string: H;
var string: I;
var string: F;
var string: G;
var string: A;
var string: C;
var string: D;
constraint
  ((A) ++ ("a") ++ (A) ++ ("b") ++ (B) ++ ("b") ++ (C) ++ ("b") ++ (D) ++ ("b") ++ (E) ++ ("b") ++ (F) ++ ("b") ++ (G) ++ ("b") ++ (H) ++ ("b") ++ (I) ++ ("b") ++ (J))
    =
    (("a") ++ (A) ++ (B) ++ (B) ++ ("b") ++ (C) ++ (C) ++ ("b") ++ (D) ++ (D) ++ ("b") ++ (E) ++ (E) ++ ("b") ++ (F) ++ (F) ++ ("b") ++ (G) ++ (G) ++ ("b") ++ (H) ++ (H) ++ ("b") ++ (I) ++ (I) ++ ("b") ++ (J) ++ (J) ++ ("baa"));
solve satisfy;
