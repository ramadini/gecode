var string: H;
var string: B;
var string: C;
var string: J;
constraint
  (("dd") ++ (B) ++ ("efcdedbaafcdbcbbbaf")) =
    (("dddceebcbf") ++ (H) ++ ("aafcdbcbbbaf"));
constraint
  (("fddeadddbeeeabdaeadffbbfcafeadefceeceffe") ++ ("")) =
    (("fddeadddbeeeabdaeadffbbfca") ++ (J) ++ ("efceeceffe"));
constraint
  (("b") ++ (C) ++ ("bccdffaeddcddecfcddebcbcfcabaabeff")) =
    (("bfecefbccdffaeddcddecfcddebcbcfcabaabeff") ++ (""));
constraint ((str_len (H)) * (1)) >= (7);
constraint ((str_len (H)) * (20)) <= (1380);
solve satisfy;
