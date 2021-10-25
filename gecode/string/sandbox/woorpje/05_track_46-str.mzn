var string: C;
var string: D;
var string: F;
constraint
  (("beaadedbdaeccb") ++ (D) ++ ("c")) =
    ((F) ++ ("cdddcfafbcaadbabfc") ++ (""));
constraint
  (("aeafafbcf") ++ (C) ++ ("bbcbeeceeddfcebcdebdbfadae") ++ (C) ++ ("e")) =
    (("aeafafbcf") ++ (C) ++ ("bbcbeeceeddfcebcdebdbfadaeaae"));
constraint
  (("cafdcefdaaaefcefcbbcfedebebbdcececcabbec") ++ ("")) =
    (("cafdcefd") ++ (C) ++ ("aefcefcbbcfedebebbdcececcabbec"));
constraint ((str_len (F)) * (15)) <= (555);
constraint ((str_len (D)) * (5)) >= (70);
constraint ((str_len (D)) * (1)) <= (101);
solve satisfy;
