var string: B;
var string: A;
var string: C;
constraint
  (("bdbccabdddadadbccdac") ++ (A) ++ ("bcbdab") ++ (B) ++ ("aadaddbabac")) =
    (("bdbccabdddadadbccdacabdacd") ++ (C) ++ ("b") ++ (B) ++ ("aadaddbabac"));
solve satisfy;
