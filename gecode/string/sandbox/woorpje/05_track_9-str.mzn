var string: H;
var string: B;
var string: C;
constraint
  (("a") ++ (B) ++ ("eccbdcbcbaad")) = (("abefffaccf") ++ (C) ++ ("cbcbaad"));
constraint
  (("eeecabecfbaeffdadecbdd") ++ (H) ++ ("fbbdafbbcbfce")) =
    (("eeecabecfbaeffdadecbddbaeedfbbdafbbcbfce") ++ (""));
constraint
  (("edec") ++ (H) ++ ("edfbafeedcecbceeedbeaadddccfccb")) =
    (("edecbaeededfbafeedcecbceeedbeaadddccfccb") ++ (""));
constraint ((str_len (B)) * (9)) >= (243);
constraint ((str_len (C)) * (11)) >= (198);
constraint ((str_len (C)) * (17)) <= (680);
constraint ((str_len (H)) * (2)) >= (4);
solve satisfy;
