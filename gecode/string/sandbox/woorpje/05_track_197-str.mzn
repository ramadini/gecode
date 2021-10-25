var string: H;
var string: G;
var string: E;
var string: F;
constraint
  ((G) ++ ("fbcbfbc") ++ ("")) = (("edeedfacfbbccf") ++ (E) ++ ("fbc"));
constraint
  (("adcaadacaff") ++ (F) ++ ("bc") ++ (H) ++ ("f") ++ (H) ++ ("dbedcfebcaeecacffcdbd") ++ (H))
    =
    (("ad") ++ (H) ++ ("aadacaff") ++ (F) ++ ("b") ++ (H) ++ (H) ++ ("f") ++ (H) ++ ("dbedcfeb") ++ (H) ++ ("aeecacff") ++ (H) ++ ("dbdc"));
constraint
  (("ddaedcb") ++ (H) ++ ("afadbbf") ++ (H) ++ ("afeeda") ++ (H) ++ ("fd") ++ (H) ++ ("edbdebfbdd") ++ (H) ++ ("ade"))
    =
    (("ddaed") ++ (H) ++ ("b") ++ (H) ++ ("afadbbfcafeedacfdcedbdebfbddcade"));
constraint ((str_len (G)) * (5)) >= (45);
constraint ((str_len (G)) * (1)) <= (458);
constraint ((str_len (H)) * (6)) <= (12);
constraint ((str_len (F)) * (2)) <= (4);
constraint ((str_len (E)) * (15)) >= (60);
constraint ((str_len (E)) * (15)) <= (390);
solve satisfy;
