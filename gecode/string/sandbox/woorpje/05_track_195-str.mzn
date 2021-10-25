var string: H;
var string: B;
var string: G;
var string: F;
var string: J;
constraint
  ((G) ++ ("b") ++ (F) ++ ("dffcebfeaafac")) =
    ((G) ++ (H) ++ ("afeede") ++ (J) ++ ("bf") ++ (G) ++ ("b") ++ (B) ++ ("fac"));
constraint
  (("ffddddcb") ++ (G) ++ ("cfaefeffefbccfd") ++ (J) ++ ("debbbfafabfee")) =
    (("ffddddcbcecfaefeffefbccfdecdebbbfafabfee") ++ (""));
constraint
  (("dcfbfbdedfdcbbbfabaadacfebcdbaebf") ++ (H) ++ ("db") ++ (G) ++ ("c")) =
    (("dcfbfbdedfdcbbbfa") ++ (H) ++ ("adacfebcdbaebf") ++ (H) ++ ("dbcec"));
constraint ((str_len (B)) * (15)) <= (315);
constraint ((str_len (J)) * (16)) >= (32);
constraint ((str_len (J)) * (1)) <= (8);
constraint ((str_len (F)) * (7)) >= (14);
constraint ((str_len (F)) * (4)) <= (528);
constraint ((str_len (G)) * (7)) >= (7);
solve satisfy;
