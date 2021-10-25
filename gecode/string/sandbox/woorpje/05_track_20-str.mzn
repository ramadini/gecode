var string: I;
var string: F;
var string: A;
var string: D;
constraint
  (("afbdecfcee") ++ (F) ++ ("fa") ++ (A) ++ ("beaddeaafddee")) =
    (("afbdecf") ++ (I) ++ ("a") ++ (A) ++ ("beaddeaafddee"));
constraint
  (("daedbcdee") ++ (F) ++ ("befbceebdcfcfcaee") ++ (D) ++ ("deacfea")) =
    (("daedbcdeefdfbefbceebdcfcfcaeefbdedeacfea") ++ (""));
constraint
  (("ebf") ++ (D) ++ ("edeeddebcccfefabebaabccabddadbfcd")) =
    (("ebf") ++ (D) ++ ("edeeddebcccfefabebaabccabddadbfcd"));
constraint ((str_len (D)) * (11)) <= (297);
constraint ((str_len (F)) * (18)) >= (18);
constraint ((str_len (A)) * (13)) >= (156);
constraint ((str_len (I)) * (13)) >= (91);
constraint ((str_len (I)) * (12)) <= (396);
solve satisfy;
