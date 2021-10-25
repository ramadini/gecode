var string: I;
var string: E;
var string: J;
var string: C;
var string: D;
constraint
  (("cfca") ++ (J) ++ ("dffcab")) =
    (("cfcae") ++ (C) ++ ("cb") ++ (D) ++ ("cab"));
constraint
  ((E) ++ ("e") ++ (E) ++ ("b") ++ (J) ++ ("d") ++ (E) ++ ("baeecfefadb") ++ (E) ++ ("db") ++ (E) ++ ("fde") ++ (E) ++ ("dab") ++ (E) ++ ("edd"))
    = (("ce") ++ (E) ++ ("bea") ++ (I) ++ (E) ++ ("fdecdabcedd"));
constraint
  (("dddbde") ++ (E) ++ ("cabfedfb") ++ (E) ++ ("eadbdfcaf") ++ (E) ++ ("bf") ++ (E) ++ ("faaaaaca") ++ (E) ++ ("fe"))
    =
    (("dddbde") ++ (E) ++ (E) ++ ("abfedfb") ++ (E) ++ ("eadbdfcafcbfcfaaaaacacfe"));
constraint
  (("eacfb") ++ (E) ++ (E) ++ ("deabeafdbdeafccdfdbabaeddde") ++ (E) ++ ("ffaaa"))
    =
    (("ea") ++ (E) ++ ("fbc") ++ (E) ++ ("deabeafdbdeafccdfdbabaeddde") ++ (E) ++ ("ffaaa"));
constraint ((str_len (I)) * (11)) >= (33);
constraint ((str_len (E)) * (8)) <= (40);
constraint ((str_len (C)) * (8)) >= (32);
constraint ((str_len (J)) * (7)) >= (21);
constraint ((str_len (J)) * (16)) <= (400);
solve satisfy;
