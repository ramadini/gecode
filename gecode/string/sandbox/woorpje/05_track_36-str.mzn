var string: H;
var string: B;
var string: F;
var string: D;
constraint
  (("b") ++ (B) ++ (H) ++ (D) ++ ("edbbdeca")) =
    (("b") ++ (F) ++ ("fbacbfbfaedbbdeca"));
constraint
  (("eecfbaafadadeedaa") ++ (B) ++ ("bfcfadcfddbdbbacdfcba")) =
    (("eecfbaafadadeedaaffbfcfadcfddbdbbacdfcba") ++ (""));
constraint
  (("beacfedebffbebbafbeabeefdeacbaeacafcedca") ++ ("")) =
    (("beacfedeb") ++ (B) ++ ("bebbafbeabeefdeacbaeacafcedca"));
constraint ((str_len (D)) * (13)) >= (65);
constraint ((str_len (D)) * (14)) <= (2422);
constraint ((str_len (F)) * (13)) >= (260);
constraint ((str_len (H)) * (19)) >= (133);
constraint ((str_len (H)) * (6)) <= (126);
constraint ((str_len (B)) * (19)) <= (152);
solve satisfy;
