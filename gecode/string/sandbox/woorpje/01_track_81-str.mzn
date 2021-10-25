var string: B;
var string: C;
var string: G;
var string: J;
constraint
  (("caacc") ++ (C) ++ ("caaccbc") ++ (B) ++ ("bacabbccbcaccababc")) =
    (("ca") ++ (G) ++ (J) ++ ("bccbcaccababc"));
solve satisfy;
