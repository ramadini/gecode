var string: G;
var string: E;
var string: F;
var string: J;
var string: C;
constraint
  ((G) ++ ("bec") ++ (C) ++ ("eeea") ++ (E) ++ ("da") ++ (C) ++ (C) ++ ("c") ++ (C) ++ ("da"))
    =
    (("d") ++ (F) ++ ("beaabec") ++ (C) ++ ("eeead") ++ (J) ++ ("badda") ++ (C) ++ ("cc") ++ (C) ++ ("da"));
constraint
  (("aedaeb") ++ (C) ++ ("a") ++ (C) ++ ("cffeaec") ++ (C) ++ ("bafebafbdadedaefeaccbdb"))
    =
    (("aedaeb") ++ (C) ++ ("a") ++ (C) ++ (C) ++ ("ffeaec") ++ (C) ++ ("bafebafbdadedaefea") ++ (C) ++ ("cbdb"));
constraint
  ((C) ++ ("fefafc") ++ (C) ++ ("adebaeeeaafaebb") ++ (C) ++ ("dafeceddaeaa") ++ (C) ++ ("abf"))
    =
    ((C) ++ ("fefaf") ++ (C) ++ ("cadebaeeeaafaebb") ++ (C) ++ ("dafeceddaeaa") ++ (C) ++ ("abf"));
constraint ((str_len (E)) * (12)) >= (12);
constraint ((str_len (F)) * (5)) >= (10);
constraint ((str_len (F)) * (19)) <= (836);
constraint ((str_len (J)) * (1)) >= (9);
constraint ((str_len (G)) * (12)) >= (60);
constraint ((str_len (C)) * (15)) >= (15);
constraint ((str_len (C)) * (17)) <= (119);
solve satisfy;
