var string: I;
var string: E;
var string: A;
constraint
  (("aebcdaecfbcdcb") ++ (A) ++ (I) ++ (A) ++ ("daefcc") ++ (A) ++ ("faaedfdd"))
    = (("a") ++ (E));
constraint
  (("cccbede") ++ (A) ++ ("ed") ++ (A) ++ ("eacbfc") ++ (A) ++ ("dadeeda") ++ (A) ++ ("cfaeaaccfca") ++ (A) ++ ("d") ++ (A))
    =
    (("cccbedebedbeac") ++ (A) ++ ("fcbdadeeda") ++ (A) ++ ("cfaeaaccfcabdb"));
constraint
  (("bfffffec") ++ (A) ++ ("adfdea") ++ (A) ++ ("ebbcebe") ++ (A) ++ ("caccceaecafeef") ++ (A) ++ ("c"))
    =
    (("bfffffecbadfdeabe") ++ (A) ++ (A) ++ ("ce") ++ (A) ++ ("e") ++ (A) ++ ("caccceaecafeefbc"));
solve satisfy;
