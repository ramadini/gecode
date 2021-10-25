var string: M;
var string: F;
var string: A;
var string: J;
constraint
  (("igfb") ++ (F) ++ (A) ++ (J) ++ ("b") ++ (F) ++ ("hd")) =
    (("i") ++ (M) ++ ("d"));
solve satisfy;
