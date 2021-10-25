var string: I;
var string: F;
var string: A;
constraint
  (("fdaefcdafedbba") ++ (I) ++ ("fddcae")) =
    (("fdae") ++ (F) ++ ("dafedbbadafeeb") ++ (A) ++ ("bfcbcfddcae"));
constraint
  (("afeffdb") ++ (F) ++ ("eaccdfbccabafddfbbcedbbedceaeae")) =
    (("afeffdb") ++ (F) ++ ("eaccdfbccabafddfbbcedbbedceaeae"));
constraint
  (("ecfcfdeeddadffebdceadddacbbeeabdedeeabcc") ++ ("")) =
    (("ec") ++ (F) ++ ("fdeeddadffebdceadddacbbeeabdedeeabcc"));
constraint ((str_len (I)) * (1)) <= (21);
constraint ((str_len (A)) * (9)) <= (1134);
solve satisfy;
