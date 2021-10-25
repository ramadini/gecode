var string: H;
var string: I;
var string: A;
var string: C;
var string: D;
constraint
  (("ef") ++ (H) ++ ("fcabccbc") ++ (I) ++ (I) ++ ("ffeeecfa") ++ (I) ++ ("cfca") ++ (A) ++ ("abebbcf"))
    = ((D) ++ ("fca") ++ (A) ++ ("abeb") ++ (I) ++ ("cf"));
constraint
  ((C) ++ ("fcfdf") ++ (I) ++ ("fddeedadfeeac") ++ (C) ++ ("cfdeccbaedec") ++ (C) ++ ("af") ++ (I) ++ ("acc"))
    =
    ((C) ++ ("fcfdfbfddee") ++ (C) ++ ("a") ++ (C) ++ ("feeacdcf") ++ (C) ++ ("ecc") ++ (I) ++ ("aedec") ++ (C) ++ ("af") ++ (I) ++ ("acc"));
constraint
  (("eeaaeaffcbb") ++ (I) ++ ("dfedfefabbefdecbeceacfafe") ++ (I) ++ (C) ++ ("e"))
    =
    (("eeaaeaffcbbb") ++ (C) ++ ("fedfefa") ++ (I) ++ ("bef") ++ (C) ++ ("ecbeceacfafebde"));
constraint ((str_len (D)) * (2)) >= (6);
constraint ((str_len (D)) * (14)) <= (462);
constraint ((str_len (A)) * (10)) >= (20);
constraint ((str_len (H)) * (12)) >= (24);
constraint ((str_len (C)) * (2)) <= (18);
solve satisfy;
