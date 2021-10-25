var string: I;
var string: J;
constraint
  (("cdc") ++ (J) ++ (J) ++ ("d") ++ (J) ++ ("cf") ++ (J) ++ ("c") ++ (J) ++ ("fa") ++ (I) ++ ("fcddbdbbdddcff"))
    =
    (("cdc") ++ (J) ++ (J) ++ ("d") ++ (J) ++ ("cfac") ++ (J) ++ ("f") ++ (J) ++ (I) ++ ("fcddbdbbdddcff"));
constraint
  ((J) ++ ("edaed") ++ (J) ++ (J) ++ ("fcdedbeeddbcdcfddaeedfcdecceedcd")) =
    (("aed") ++ (J) ++ ("eda") ++ (J) ++ ("fcdedbeeddbcdcfdd") ++ (J) ++ ("eedfcdecceedcd"));
constraint
  (("fbafe") ++ (J) ++ ("cdccbcbbcaeaffecdabdd") ++ (J) ++ ("deb") ++ (J) ++ ("fecffdee"))
    =
    (("fbafeacdccbcbbc") ++ (J) ++ ("e") ++ (J) ++ ("ffecdabddadebafecffdee"));
constraint ((str_len (J)) * (7)) <= (14);
solve satisfy;
