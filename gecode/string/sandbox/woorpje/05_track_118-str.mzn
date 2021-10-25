var string: I;
var string: F;
var string: A;
var string: C;
var string: D;
constraint
  ((D) ++ ("eeef") ++ (I) ++ ("aaeebdcdbff")) =
    ((F) ++ ("edbbeeeeef") ++ (A) ++ ("aaeebdcdbff"));
constraint
  (("eeddcceabbffaacbfbbcadcffcbbbebdbdbeabfb") ++ ("")) =
    (("eeddcceabbffaacbfbbcadcffcb") ++ (C) ++ ("bdbdbeabfb"));
constraint
  (("fcfbaeacecdcafeccffcffbeeffcbdebbad") ++ (C) ++ ("fb")) =
    (("fcfbaeacecdcafeccffcffbeeffcbdebbad") ++ (C) ++ ("fb"));
constraint ((str_len (A)) * (1)) >= (7);
constraint ((str_len (A)) * (9)) <= (1296);
constraint ((str_len (F)) * (14)) >= (70);
constraint ((str_len (F)) * (8)) <= (152);
constraint ((str_len (C)) * (7)) >= (14);
constraint ((str_len (I)) * (18)) >= (72);
constraint ((str_len (I)) * (7)) <= (714);
solve satisfy;
