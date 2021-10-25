var string: I;
var string: F;
var string: E;
var string: A;
var string: J;
var string: D;
constraint
  (("e") ++ (F) ++ ("bdefdffd") ++ (A) ++ ("edc") ++ (I) ++ ("cdfdb")) =
    (("eabdefd") ++ (J) ++ ("ed") ++ (D) ++ ("dc") ++ (I) ++ ("cdfdb"));
constraint
  (("edc") ++ (F) ++ ("fdedbcbdeeccfbbebbdbfecfeebfba") ++ (E) ++ ("dee")) =
    (("edc") ++ (F) ++ ("fdedbcbdeeccfbbebbdbfecfeebfba") ++ (D) ++ ("dee"));
constraint
  (("cbeadacf") ++ (F) ++ ("cfacccddceebaefcbccc") ++ (F) ++ ("abbbaebcfd"))
    =
    (("cbe") ++ (F) ++ ("dacf") ++ (F) ++ ("cf") ++ (F) ++ ("cccddceebaefcbcccaabbbaebcfd"));
constraint ((str_len (J)) * (13)) >= (52);
constraint ((str_len (D)) * (19)) <= (551);
constraint ((str_len (E)) * (2)) <= (18);
constraint ((str_len (F)) * (11)) >= (11);
solve satisfy;
