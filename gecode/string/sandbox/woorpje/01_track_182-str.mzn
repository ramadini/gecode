var string: I;
var string: C;
var string: D;
var string: J;
constraint
  (("h") ++ (D) ++ ("eg") ++ (C) ++ ("j") ++ (J) ++ ("edfc")) =
    (("hje") ++ (I) ++ ("cedfc"));
solve satisfy;
