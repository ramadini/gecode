var string: C;
var string: G;
var string: E;
constraint
  (("eaa") ++ (G) ++ ("bfaacaebbccceceeedc")) =
    (("eaacffbebcba") ++ (C) ++ ("bbccceceeedc"));
constraint
  (("cdaaebabbccfecafecccdbedbddcefdcfcddabdc") ++ ("")) =
    (("cda") ++ (E) ++ ("abbccfecafecccdbedbddcefdcfcddabdc"));
constraint
  (("fecdecafcaffd") ++ (E) ++ ("ffcebacfcfadcefafefccebd")) =
    (("fecdecafcaffdaebffcebacfcfadcefafefccebd") ++ (""));
constraint ((str_len (C)) * (14)) <= (308);
constraint ((str_len (E)) * (6)) >= (12);
constraint ((str_len (E)) * (5)) <= (60);
constraint ((str_len (G)) * (17)) >= (272);
constraint ((str_len (G)) * (14)) <= (1274);
solve satisfy;
