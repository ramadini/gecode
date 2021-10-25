var string: I;
var string: E;
var string: A;
var string: C;
constraint
  (("bddddebafbfdb") ++ (E) ++ ("acaa")) = ((A) ++ ("ddacacaa") ++ (""));
constraint
  (("bec") ++ (C) ++ ("ffdafbebc") ++ (I) ++ ("ddddcea")) =
    (("bec") ++ (A) ++ ("facddddcea"));
constraint
  (("fcbdeaaddeddca") ++ (C) ++ ("dfecdbcbddbebdfdfbcd")) =
    (("fcbdeaaddeddcadbaafbdfecdbcbddbebdfdfbcd") ++ (""));
constraint
  (("ddbadbaafbdaebcdccdaacbfffbebccbcbcdbafe") ++ ("")) =
    (("ddba") ++ (C) ++ ("daebcdccdaacbfffbebccbcbcdbafe"));
constraint ((str_len (A)) * (11)) >= (33);
constraint ((str_len (E)) * (12)) <= (1656);
constraint ((str_len (C)) * (1)) >= (4);
constraint ((str_len (C)) * (11)) <= (231);
solve satisfy;
