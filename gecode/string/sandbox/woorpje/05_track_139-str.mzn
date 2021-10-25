var string: B;
var string: E;
var string: I;
var string: F;
var string: A;
var string: C;
var string: D;
constraint
  (("ecaeabccaeacfbeab") ++ (D) ++ ("cbaefcfafeb")) =
    (("ecae") ++ (F) ++ ("ccbaefcfafeb"));
constraint
  (("ccde") ++ (E) ++ ("c") ++ (F) ++ (B) ++ ("f") ++ (C) ++ ("a") ++ (B) ++ ("abb"))
    =
    ((I) ++ ("de") ++ (E) ++ ("cd") ++ (A) ++ ("a") ++ (F) ++ ("c") ++ (B) ++ ("cdd") ++ (E) ++ ("ddadabcbecaca") ++ (E) ++ ("abb"));
constraint
  (("dbaeddddf") ++ (E) ++ ("eba") ++ (E) ++ ("dcaaffdad") ++ (B) ++ ("aaaeaa") ++ (E) ++ ("c") ++ (B) ++ ("abf") ++ (B) ++ ("cab"))
    =
    (("dbaeddd") ++ (F) ++ ("febafdcaaffdadfaaaeaa") ++ (E) ++ ("c") ++ (E) ++ ("ab") ++ (E) ++ (B) ++ ("cab"));
constraint
  ((B) ++ ("ddbebfbbca") ++ (B) ++ ("acd") ++ (B) ++ ("ef") ++ (F) ++ ("fdb") ++ (E) ++ ("daca") ++ (B) ++ ("b") ++ (B) ++ ("bcccdbcdb"))
    =
    ((E) ++ ("ddbeb") ++ (E) ++ ("bbca") ++ (B) ++ ("ac") ++ (F) ++ ("e") ++ (B) ++ ("d") ++ (B) ++ (B) ++ ("db") ++ (E) ++ ("daca") ++ (E) ++ ("b") ++ (E) ++ ("bcccdbcdb"));
constraint ((str_len (A)) * (5)) <= (155);
constraint ((str_len (E)) * (11)) >= (11);
constraint ((str_len (E)) * (19)) <= (76);
constraint ((str_len (D)) * (10)) >= (30);
constraint ((str_len (D)) * (5)) <= (520);
constraint ((str_len (I)) * (4)) >= (8);
solve satisfy;
