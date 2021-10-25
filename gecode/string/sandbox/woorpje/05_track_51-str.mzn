var string: F;
var string: E;
var string: A;
constraint
  (("b") ++ (E) ++ ("ffcebddcfededfd")) = (("ba") ++ (F) ++ ("dfd"));
constraint
  (("e") ++ (E) ++ ("fdec") ++ (A) ++ (E) ++ ("b") ++ (F) ++ ("d") ++ (A) ++ ("d") ++ (A) ++ ("df") ++ (A) ++ (A) ++ ("cddae") ++ (E) ++ ("ecdedaca") ++ (E) ++ ("ea") ++ (A) ++ ("dc"))
    =
    (("e") ++ (E) ++ ("fdecc") ++ (E) ++ ("be") ++ (E) ++ ("edcdcdf") ++ (A) ++ ("c") ++ (A) ++ ("dda") ++ (F) ++ ("cdeda") ++ (A) ++ ("a") ++ (E) ++ ("ea") ++ (A) ++ ("d") ++ (A));
constraint
  (("e") ++ (A) ++ ("ebdb") ++ (E) ++ ("ead") ++ (A) ++ (E) ++ ("d") ++ (A) ++ ("cfaaeafcac") ++ (A) ++ ("beabcdaaeeb") ++ (A) ++ ("ed") ++ (E))
    =
    (("ecebdb") ++ (E) ++ ("ead") ++ (A) ++ ("fdcc") ++ (E) ++ ("aaeaf") ++ (A) ++ ("a") ++ (A) ++ ("cbeabcdaaeeb") ++ (A) ++ ("ed") ++ (E));
constraint
  ((A) ++ ("cacddb") ++ (E) ++ (E) ++ ("cdeda") ++ (E) ++ (A) ++ ("be") ++ (A) ++ ("cadaeaecfbedddeecbfcb"))
    =
    (("cca") ++ (A) ++ ("ddbffcdeda") ++ (E) ++ (A) ++ ("beccadaeae") ++ (A) ++ ("fbedddee") ++ (A) ++ ("b") ++ (E) ++ ("cb"));
constraint ((str_len (A)) * (7)) >= (7);
solve satisfy;
