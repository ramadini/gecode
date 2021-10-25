var string: E;
var string: A;
var string: C;
var string: D;
constraint
  (("ff") ++ (C) ++ ("cbbffefdbccffabbcfc") ++ (E) ++ ("f") ++ (E) ++ (E)) =
    (("ffecbe") ++ (E) ++ ("eabaee") ++ (A) ++ ("cbbffefdbccff") ++ (E) ++ ("bbcfcafa") ++ (E));
constraint
  (("bcbbfaecec") ++ (D) ++ ("ceaaefffeacdfeaebadfdafa") ++ (E) ++ ("dbb")) =
    (("bcbbfaececbecea") ++ (E) ++ ("efffe") ++ (E) ++ ("cdfeaeb") ++ (E) ++ ("dfd") ++ (E) ++ ("f") ++ (E) ++ ("adbb"));
constraint
  (("eedfdbdcccec") ++ (E) ++ ("dedbdbd") ++ (E) ++ ("cdfddfdaeddccbaeebd"))
    =
    (("eedfdbdcccecadedbdbd") ++ (E) ++ ("cdfddfd") ++ (E) ++ ("eddccb") ++ (E) ++ ("eebd"));
constraint ((str_len (A)) * (10)) <= (190);
constraint ((str_len (D)) * (11)) <= (385);
constraint ((str_len (E)) * (13)) >= (13);
solve satisfy;
