var string: I;
var string: H;
var string: B;
var string: F;
var string: J;
constraint
  (("dcecff") ++ (I) ++ ("eefd")) = (("dcecffbff") ++ (B) ++ ("aeefd"));
constraint
  ((J) ++ ("a") ++ (J) ++ (J) ++ ("be") ++ (J) ++ ("eabba") ++ (H) ++ ("aaebaeb") ++ (J) ++ ("acae") ++ (J) ++ ("abfadceb"))
    =
    ((J) ++ ("a") ++ (J) ++ (J) ++ ("be") ++ (J) ++ ("eabbadbb") ++ (F) ++ ("edabfa") ++ (J) ++ ("ceb"));
constraint
  (("aabebbdeeccadfecabceffc") ++ (J) ++ ("e") ++ (J) ++ ("befb") ++ (J) ++ ("fcddcbaf") ++ (J))
    =
    (("aabebbdeecca") ++ (J) ++ ("fecabceffcde") ++ (J) ++ ("befbdfc") ++ (J) ++ (J) ++ ("cbaf") ++ (J));
constraint
  (("eaffdbbe") ++ (J) ++ ("acaebcbbd") ++ (J) ++ ("aacfcaa") ++ (J) ++ ("b") ++ (J) ++ ("affbbfca") ++ (J) ++ ("e") ++ (J))
    =
    (("eaff") ++ (J) ++ ("bbe") ++ (J) ++ ("acaebcbb") ++ (J) ++ (J) ++ ("aacfcaadb") ++ (J) ++ ("affbbfcaded"));
constraint ((str_len (B)) * (14)) >= (252);
constraint ((str_len (H)) * (5)) >= (15);
constraint ((str_len (H)) * (9)) <= (63);
constraint ((str_len (J)) * (20)) >= (20);
solve satisfy;
