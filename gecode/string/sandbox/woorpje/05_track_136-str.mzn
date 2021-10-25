var string: G;
var string: E;
var string: A;
var string: J;
var string: F;
constraint
  (("eb") ++ (E) ++ ("cfebbeedc") ++ (A) ++ ("d")) =
    (("ebabddcf") ++ (J) ++ ("dafeafaaeded"));
constraint
  (("aeecaeee") ++ (E) ++ ("fbdbbaeeefceeaaaccccaacbec")) =
    (("aeecaeee") ++ (E) ++ ("fbdbbaeeefceeaaaccccaacbec"));
constraint
  (("bfdbca") ++ (F) ++ (G) ++ ("ffbaaacdfa")) =
    (("bfdbca") ++ (J) ++ ("ffdbb") ++ (A) ++ ("cdfa"));
constraint
  ((J) ++ ("bdcafedbeeaecacadbebcbbaacabbcbddbdcf") ++ ("")) =
    (("aabbdcafedbeeaecacadbebcbbaacabbcbddbdcf") ++ (""));
constraint
  (("ebaabbcacbbabecddbddbdefabcfeccbecabacbf") ++ ("")) =
    (("eb") ++ (J) ++ ("bcacbbabecddbddbdefabcfeccbecabacbf"));
constraint ((str_len (J)) * (3)) >= (9);
constraint ((str_len (J)) * (14)) <= (560);
constraint ((str_len (G)) * (6)) >= (60);
constraint ((str_len (F)) * (8)) >= (24);
constraint ((str_len (F)) * (9)) <= (414);
solve satisfy;
