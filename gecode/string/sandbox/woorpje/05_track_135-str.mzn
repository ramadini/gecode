var string: H;
var string: F;
var string: E;
var string: C;
var string: D;
constraint
  (("ebd") ++ (C) ++ (H) ++ ("cdacfacff") ++ (E) ++ ("fdeebc") ++ (C) ++ ("edeefffc"))
    = (("ebd") ++ (C) ++ ("ccdcdccdacfacf") ++ (D) ++ ("edeefffc"));
constraint
  (("dbadebebfbdb") ++ (C) ++ ("dbddcbceebcefaabfbaacabada")) =
    (("dbadebe") ++ (C) ++ ("bdb") ++ (C) ++ ("dbddcbceebcefaa") ++ (C) ++ ("baacabada"));
constraint
  (("aedbbddabbd") ++ (F) ++ ("deafcbbcecababefceaefcebc")) =
    (("aedbbddabbd") ++ (F) ++ ("deafcbbcecababefceaefcebc"));
constraint ((str_len (H)) * (9)) >= (36);
constraint ((str_len (H)) * (17)) <= (1734);
constraint ((str_len (E)) * (3)) >= (6);
constraint ((str_len (C)) * (16)) <= (96);
constraint ((str_len (F)) * (14)) <= (602);
solve satisfy;
