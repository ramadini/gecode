var string: I;
var string: D;
var string: H;
constraint
  ((D) ++ ("acbddbcfaeddcdea") ++ (I) ++ ("cebfddeccffeede")) =
    (("bfaacbddbcfae") ++ (H) ++ ("cebfddeccffeede"));
constraint
  (("eecafabeacefbdefbafdfeabaeaecebafdcebfab") ++ ("")) =
    (("eecafabeacefbdefbafdfeabaeaecebafdce") ++ (D) ++ ("b"));
constraint
  (("ebceabbddaadbfcaef") ++ (D) ++ ("fcfeeaccabfafeaaeaf")) =
    (("ebceabbddaadbfcaef") ++ (D) ++ ("fcfeeacca") ++ (D) ++ ("feaaeaf"));
constraint ((str_len (D)) * (4)) >= (4);
constraint ((str_len (D)) * (20)) <= (400);
solve satisfy;
