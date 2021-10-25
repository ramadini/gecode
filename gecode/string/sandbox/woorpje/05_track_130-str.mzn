var string: C;
var string: G;
var string: F;
constraint
  (("eafbedddccfdc") ++ (C) ++ ("fafdbeec")) =
    (("eafbed") ++ (G) ++ ("bcedcabbdfafdbeec"));
constraint
  (("eabbdcaf") ++ (F) ++ ("edabfaecfcebafacbfccfdbcccabd")) =
    (("eabbdcafeccedabfaecfcebafacbfccfdbcccabd") ++ (""));
constraint
  (("fdcfcdddeecfbbcbfcfaedeeddcbfddaceaeeccb") ++ ("")) =
    (("fdcfcdddeecfbbcbfcfaedeeddcbfddaceae") ++ (F) ++ ("b"));
constraint ((str_len (F)) * (1)) <= (12);
constraint ((str_len (C)) * (8)) >= (144);
constraint ((str_len (C)) * (11)) <= (2002);
solve satisfy;
