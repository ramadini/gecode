var string: H;
var string: G;
var string: F;
var string: J;
constraint
  (("bb") ++ (F) ++ ("abeeffbabadcbbeddabdfff")) =
    (("bbfbcaf") ++ (G) ++ ("deabe") ++ (J) ++ ("bbeddabdfff"));
constraint
  (("fcbafdcbdecdbbaefdabaeceacaffceeffcbffab") ++ ("")) =
    (("fcbafd") ++ (H) ++ ("cdbbaefdabaeceacaffceeffcbffab"));
constraint
  (("afeaeabfecaedafcb") ++ (H) ++ ("aafcdfbbcddbcabbabc")) =
    (("afeaeabfecaedafcb") ++ (H) ++ ("aafcdfbbcddbcabbabc"));
constraint ((str_len (H)) * (19)) >= (38);
constraint ((str_len (H)) * (14)) <= (168);
constraint ((str_len (G)) * (9)) >= (36);
constraint ((str_len (J)) * (4)) <= (188);
solve satisfy;
