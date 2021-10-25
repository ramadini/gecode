var string: H;
var string: F;
var string: E;
var string: C;
var string: D;
constraint
  (("eecbfdecafbaaaaa") ++ (H) ++ ("bdfbbc")) =
    (("eecbfdecafbaaaaadaef") ++ (C) ++ ("baccbbebdfbbc"));
constraint
  ((E) ++ ("cdabb") ++ (F) ++ ("acdadcebfcddb")) =
    ((E) ++ ("cdabb") ++ (F) ++ ("acdadcebfcddb"));
constraint
  (("dbfabbdedbdada") ++ (F) ++ (H) ++ ("cf") ++ (H) ++ ("dd") ++ (H) ++ (H) ++ (H) ++ ("cba"))
    =
    (("dbf") ++ (H) ++ ("bbdedbdad") ++ (H) ++ ("fd") ++ (D) ++ ("aa") ++ (H) ++ ("cb") ++ (H));
constraint
  (("eabd") ++ (H) ++ ("dbeafceedccc") ++ (H) ++ ("fb") ++ (H) ++ ("adfffbbdbcbec") ++ (H) ++ ("fb") ++ (H) ++ ("da"))
    =
    (("eabdadbeafceedccc") ++ (H) ++ ("fbaadfffbbdbcbecafb") ++ (H) ++ ("da"));
constraint
  (("e") ++ (H) ++ ("dbd") ++ (H) ++ ("dddbe") ++ (H) ++ ("cdcdfcecdeedffabafccc") ++ (H) ++ ("cee") ++ (H) ++ ("fb"))
    =
    (("e") ++ (H) ++ ("dbdadddbeacdcdfcecdeedffabafccc") ++ (H) ++ ("cee") ++ (H) ++ ("fb"));
constraint ((str_len (E)) * (5)) <= (250);
constraint ((str_len (D)) * (10)) >= (30);
constraint ((str_len (D)) * (11)) <= (407);
constraint ((str_len (C)) * (7)) >= (7);
constraint ((str_len (F)) * (15)) >= (180);
constraint ((str_len (F)) * (5)) <= (180);
constraint ((str_len (H)) * (15)) <= (30);
solve satisfy;
