var string: I;
var string: E;
var string: A;
var string: J;
constraint
  (("fccfdcc") ++ (I) ++ ("bfcecb") ++ (J) ++ ("f") ++ (I) ++ ("ede")) =
    (("fccfdccadbfcecbd") ++ (A) ++ ("dede"));
constraint
  (("accacafeabcadcffeabeeafbbbbafddafdcfdb") ++ (E)) =
    (("accacafeabcadcffeabeeafbbbbafddafdcfdbae") ++ (""));
constraint
  (("fbdecadeed") ++ (I) ++ ("fecffcabeedafbeedfebbccddaaa")) =
    (("fbdecadeedadfecffcabeedafbeedfebbccddaaa") ++ (""));
constraint ((str_len (J)) * (8)) >= (96);
constraint ((str_len (A)) * (5)) >= (20);
constraint ((str_len (I)) * (7)) <= (70);
solve satisfy;
