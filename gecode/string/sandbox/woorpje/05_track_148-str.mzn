var string: I;
var string: B;
var string: F;
var string: J;
constraint
  (("eeaefdaf") ++ (I) ++ ("c") ++ (B) ++ ("adaccddecdc")) =
    (("eeaefdaffffecdcc") ++ (B) ++ ("adaccddecdc"));
constraint
  (("bcbdc") ++ (I) ++ (F) ++ ("faffdcdfafcacfbb")) =
    (("bcbdc") ++ (F) ++ ("eddba") ++ (F) ++ ("eaee") ++ (J));
constraint
  (("dbcccccfcdebdea") ++ (F) ++ (F) ++ ("bdfdbfdcbaffbfdfadcaddf")) =
    (("dbcccccfcdebdea") ++ (F) ++ (F) ++ ("bdfdbfdcbaffbfdfadcaddf"));
constraint
  (("dbafaccdbadd") ++ (F) ++ ("fbca") ++ (F) ++ ("bbdd") ++ (F) ++ ("fda") ++ (F) ++ ("d") ++ (F) ++ ("bdacacfbf") ++ (F) ++ (F))
    =
    (("dbafaccdbadd") ++ (F) ++ ("fbcaebbdd") ++ (F) ++ ("fda") ++ (F) ++ ("d") ++ (F) ++ ("bdacacfbf") ++ (F) ++ (F));
constraint ((str_len (J)) * (16)) <= (1168);
constraint ((str_len (B)) * (12)) <= (588);
constraint ((str_len (I)) * (7)) >= (49);
constraint ((str_len (F)) * (15)) >= (15);
solve satisfy;
