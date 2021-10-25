var string: I;
var string: B;
var string: C;
constraint
  (("c") ++ (C) ++ ("bdbbc") ++ (I) ++ ("cabbd") ++ (C) ++ ("eaccefcfcbafbeba"))
    =
    (("c") ++ (C) ++ ("bdbbc") ++ (I) ++ ("cabb") ++ (C) ++ ("deaccefcfcbafbeba"));
constraint
  (("b") ++ (C) ++ ("cfcbbffcdcebcfa") ++ (B) ++ ("eceacaaba") ++ (C) ++ ("babe") ++ (C) ++ ("f") ++ (B) ++ ("b") ++ (C) ++ ("d"))
    =
    (("b") ++ (C) ++ ("cf") ++ (B) ++ ("bffc") ++ (C) ++ ("cebcfa") ++ (B) ++ ("eceacaaba") ++ (C) ++ ("babedfcbbd") ++ (C));
constraint
  (("dbfcec") ++ (B) ++ (C) ++ ("ddefc") ++ (C) ++ ("ffddecabceafafcad") ++ (C) ++ ("adcdcce"))
    =
    ((C) ++ ("bfcec") ++ (B) ++ ("dddefcdffd") ++ (C) ++ ("ecabceafafca") ++ (C) ++ (C) ++ ("adc") ++ (C) ++ ("cce"));
constraint ((str_len (I)) * (17)) >= (153);
constraint ((str_len (I)) * (14)) <= (462);
constraint ((str_len (B)) * (11)) >= (22);
constraint ((str_len (B)) * (15)) <= (420);
solve satisfy;
