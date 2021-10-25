var string: I;
var string: H;
var string: F;
var string: E;
constraint
  (("efadc") ++ (E) ++ ("ebcecedeaabfcbfadfdcf")) =
    (("efadcfdcafd") ++ (I) ++ ("ce") ++ (F) ++ ("aabfcbfadfdcf"));
constraint
  (("fbcccbdefcfdaaffbeafbdfaf") ++ (H) ++ ("abababdeef")) =
    (("fbcccbdefcfdaaffbeafbdfaf") ++ (H) ++ ("abababdeef"));
constraint
  (("aebcefcffadceeabecadeeaae") ++ (I) ++ ("ccbfadcbcf")) =
    (("aebcefcffadceeabecadeeaaebcddcccbfadcbcf") ++ (""));
constraint ((str_len (I)) * (13)) >= (13);
constraint ((str_len (E)) * (17)) >= (204);
constraint ((str_len (E)) * (14)) <= (1834);
constraint ((str_len (H)) * (14)) <= (126);
constraint ((str_len (F)) * (9)) <= (1539);
solve satisfy;
