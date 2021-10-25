var string: C;
var string: F;
constraint
  (("fc") ++ (F) ++ ("f") ++ (C) ++ ("ccd")) =
    (("fc") ++ (F) ++ ("fghehahcaaaccd"));
solve satisfy;
