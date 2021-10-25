var string: C;
var string: E;
constraint
  (("cddcebeddcbbbaee") ++ (C) ++ ("aaafbcbabffbbbdefff")) =
    (("cddcebeddcbbba") ++ (E) ++ ("bbdefff"));
constraint
  (("ebfceeafbffdfebffacebeccfbfcccfaff") ++ (C) ++ ("a")) =
    (("ebfceeafbffdfebffacebeccfbfcccfaffebecaa") ++ (""));
constraint
  (("adbebecaebabeebcadfaccddbdcbcbaedeaecfee") ++ ("")) =
    (("adb") ++ (C) ++ ("ebabeebcadfaccddbdcbcbaedeaecfee"));
constraint ((str_len (E)) * (19)) >= (114);
constraint ((str_len (E)) * (6)) <= (852);
constraint ((str_len (C)) * (14)) >= (42);
constraint ((str_len (C)) * (17)) <= (221);
solve satisfy;
