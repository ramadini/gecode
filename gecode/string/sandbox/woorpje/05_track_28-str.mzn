var string: H;
var string: B;
var string: F;
var string: A;
var string: J;
constraint
  ((B) ++ ("abb") ++ (H) ++ ("eccdacffc")) =
    (("aedadacbfabbcaee") ++ (J) ++ (F) ++ ("cdacffc"));
constraint
  (("fdbebbddaab") ++ (F) ++ ("ebaccfcdcebadeeadeedcbdfaed")) =
    (("fdbebbddaab") ++ (F) ++ ("ebaccfcdcebadeeadeedcbdfaed"));
constraint
  (("dafafbbfbfbfedaffcedaabf") ++ (A) ++ ("cebbdeeffbfea")) =
    (("dafafbbfbfbfedaffcedaabf") ++ (A) ++ ("cebbdeeffbfea"));
constraint ((str_len (F)) * (20)) <= (320);
constraint ((str_len (H)) * (19)) <= (1577);
constraint ((str_len (B)) * (17)) <= (680);
solve satisfy;
