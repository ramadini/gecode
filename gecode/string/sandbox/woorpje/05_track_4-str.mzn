var string: C;
var string: D;
var string: J;
constraint
  (("aadbfffb") ++ (J) ++ ("abffaefb") ++ (J) ++ (D) ++ ("fbaeaefd")) =
    (("aadbfffb") ++ (J) ++ ("a") ++ (J) ++ ("ffaef") ++ (C));
constraint
  (("bdbf") ++ (J) ++ ("fdbacfafaaeadaedcbccecebccaddbfb") ++ (J) ++ ("d") ++ (J))
    =
    ((J) ++ ("d") ++ (J) ++ ("f") ++ (J) ++ ("fdbacfafaaeadaedc") ++ (J) ++ ("ccece") ++ (J) ++ ("ccaddbfbbdb"));
constraint
  (("fffbddffdadec") ++ (J) ++ (J) ++ ("bbbcb") ++ (J) ++ ("aefaeecf") ++ (J) ++ ("aabbfabebd"))
    =
    (("fff") ++ (J) ++ ("ddffdadec") ++ (J) ++ (J) ++ ("b") ++ (J) ++ ("bcb") ++ (J) ++ ("aefaeecfbaa") ++ (J) ++ (J) ++ ("fa") ++ (J) ++ ("ebd"));
constraint ((str_len (J)) * (13)) <= (169);
constraint ((str_len (C)) * (8)) <= (504);
constraint ((str_len (D)) * (12)) >= (168);
solve satisfy;
