var string: I;
var string: H;
var string: B;
var string: F;
var string: C;
constraint
  (("e") ++ (B) ++ (I) ++ ("f") ++ (F) ++ ("ddcbabfcfeeec")) =
    (("ebe") ++ (C) ++ ("abdc") ++ (I) ++ (H) ++ ("abfcfeeec"));
constraint
  (("bafadabafaaabcabcdfcadcdfeaaefbbfafbebbb") ++ ("")) =
    (("bafadabafaaabcabcdfcadcdfea") ++ (I) ++ ("fbbfafbebbb"));
constraint
  (("eaebdfbfabeabcbeedcabbfaefbafcadbcdbdfab") ++ ("")) =
    (("e") ++ (I) ++ ("bdfbfabeabcbeedcabbf") ++ (I) ++ ("fbafcadbcdbdfab"));
constraint ((str_len (C)) * (11)) <= (352);
constraint ((str_len (H)) * (10)) <= (130);
constraint ((str_len (B)) * (12)) >= (24);
constraint ((str_len (I)) * (15)) >= (15);
constraint ((str_len (F)) * (12)) <= (960);
solve satisfy;
