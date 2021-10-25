var string: B;
var string: C;
var string: G;
constraint
  (("baaab") ++ (C) ++ ("abba")) =
    (("b") ++ (B) ++ ("abb") ++ (G) ++ (B) ++ ("b") ++ (B) ++ ("b") ++ (B) ++ ("baabba"));
solve satisfy;
