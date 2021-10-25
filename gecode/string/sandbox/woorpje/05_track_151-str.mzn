var string: C;
var string: F;
var string: A;
constraint
  (("eecddaddeffbfdd") ++ (F) ++ (C) ++ ("bfaadeaefeeaed")) =
    (("ee") ++ (C) ++ ("dd") ++ (A) ++ ("aacbfaadeaefeeaed"));
constraint
  (("bf") ++ (C) ++ ("d") ++ (C) ++ ("e") ++ (C) ++ ("adbbf") ++ (C) ++ ("aaaebbfb") ++ (C) ++ ("bfbacfd") ++ (C) ++ ("deebbeefeb"))
    =
    (("bfcdce") ++ (C) ++ ("adbbfcaaaebbfbcbfba") ++ (C) ++ ("fd") ++ (C) ++ ("deebbeefeb"));
constraint
  (("ebcfcfbadedabeddfabbaafebe") ++ (C) ++ ("cebdade") ++ (C) ++ ("ebdcb"))
    =
    (("ebcfcfbadedabeddfabbaafebeccebdade") ++ (C) ++ ("ebd") ++ (C) ++ ("b"));
constraint ((str_len (A)) * (10)) >= (130);
constraint ((str_len (F)) * (8)) >= (32);
constraint ((str_len (F)) * (5)) <= (330);
constraint ((str_len (C)) * (18)) >= (18);
constraint ((str_len (C)) * (4)) <= (4);
solve satisfy;
