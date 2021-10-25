var string: F;
var string: J;
var string: C;
var string: D;
constraint
  ((D) ++ ("ccde") ++ (F) ++ (C) ++ ("aacbeeeeedccd")) =
    (("acaccccde") ++ (C) ++ (J) ++ ("adaebbecafbb") ++ (C) ++ ("aacbeeeeedccd"));
constraint
  (("d") ++ (C) ++ (C) ++ ("ddeebdedb") ++ (C) ++ ("ed") ++ (C) ++ ("eecefdacc") ++ (C) ++ ("dbeafeddecacea"))
    =
    (("d") ++ (C) ++ ("fddeebdedbfed") ++ (C) ++ ("eece") ++ (C) ++ ("dacc") ++ (C) ++ ("dbeafeddecacea"));
constraint
  ((C) ++ ("aacecbddeabfccca") ++ (C) ++ ("dadcecdacd") ++ (C) ++ ("aedbdc") ++ (C) ++ (C) ++ ("ccc"))
    = (("faacecbddeabfcccafdadcecdacdfaedbdcffccc") ++ (""));
constraint ((str_len (C)) * (7)) >= (7);
constraint ((str_len (F)) * (20)) >= (120);
constraint ((str_len (F)) * (12)) <= (456);
constraint ((str_len (J)) * (17)) <= (544);
solve satisfy;
