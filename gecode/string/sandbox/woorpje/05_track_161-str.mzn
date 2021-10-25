var string: I;
var string: H;
var string: F;
var string: J;
var string: C;
constraint
  (("e") ++ (I) ++ ("bab") ++ (F) ++ ("bececbacfcdede")) =
    (("e") ++ (C) ++ ("ddcfffbbcc") ++ (J) ++ ("fbaddbececbacfcdede"));
constraint
  (("abbf") ++ (C) ++ ("dcaaecceeabfeebdddaeabbaacabdcad")) =
    (("abbf") ++ (C) ++ ("dcaaecceeabfeebdddaeabbaacabdcad"));
constraint
  (("eaebdabffddbabededcebcebddb") ++ (H) ++ ("ecefdcbdf")) =
    (("eaebdabffddbabededcebcebddb") ++ (H) ++ ("ecefdcbdf"));
constraint ((str_len (C)) * (1)) <= (24);
constraint ((str_len (J)) * (14)) >= (56);
constraint ((str_len (J)) * (13)) <= (1105);
constraint ((str_len (I)) * (17)) <= (306);
constraint ((str_len (H)) * (4)) <= (236);
constraint ((str_len (F)) * (3)) <= (60);
solve satisfy;
