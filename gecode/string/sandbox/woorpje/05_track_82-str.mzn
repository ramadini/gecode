var string: I;
var string: B;
var string: E;
var string: J;
constraint
  (("cfdbccbd") ++ (J) ++ (E) ++ ("ddeda")) =
    (("cfdbccbdfebde") ++ (B) ++ ("fdefeacebfcfddeda"));
constraint
  (("fddfeecdfcaafdccaeeefdfdefeeeaccfedcdeeb") ++ ("")) =
    (("fddfeecdfcaafdccaee") ++ (I) ++ ("efeeeaccfedcdeeb"));
constraint
  (("aaeabccdfefdfddafcdcfdbfbfeddfdbecfefeed") ++ ("")) =
    (("aaeabccdf") ++ (I) ++ ("dafcdcfdbfbfeddfdbecfefeed"));
constraint ((str_len (E)) * (9)) >= (171);
constraint ((str_len (I)) * (16)) >= (32);
constraint ((str_len (I)) * (11)) <= (176);
constraint ((str_len (J)) * (13)) >= (65);
solve satisfy;
