var string: H;
var string: J;
constraint
  (("dbffebee") ++ (H) ++ (H) ++ ("dbbceffb") ++ (H) ++ ("beee") ++ (H) ++ ("dbaabbbcea") ++ (H) ++ (H) ++ ("bbcb"))
    = (("dbffebeec") ++ (H) ++ ("dbb") ++ (H) ++ ("eff") ++ (J));
constraint
  (("afebadcfffaaddaabfcdfaddb") ++ (H) ++ (H) ++ ("adaafdfe") ++ (H) ++ ("effc"))
    =
    (("afebadcfffaaddaabf") ++ (H) ++ ("dfaddbc") ++ (H) ++ ("adaafdfeceff") ++ (H));
constraint
  (("ceeeffaaaa") ++ (H) ++ ("fbbbbca") ++ (H) ++ ("aeecdeff") ++ (H) ++ ("fcb") ++ (H) ++ ("eea") ++ (H) ++ ("ebaa"))
    =
    ((H) ++ ("eeeffaaaacfbbbbca") ++ (H) ++ ("aee") ++ (H) ++ ("deff") ++ (H) ++ ("f") ++ (H) ++ ("b") ++ (H) ++ ("eea") ++ (H) ++ ("ebaa"));
constraint ((str_len (J)) * (8)) >= (48);
constraint ((str_len (J)) * (1)) <= (188);
constraint ((str_len (H)) * (20)) >= (20);
solve satisfy;
