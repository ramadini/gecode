var string: C;
var string: D;
var string: A;
constraint
  (("eefcdaccecdfcc") ++ (D) ++ ("bfccc") ++ (C) ++ ("e")) =
    (("e") ++ (A) ++ ("bbaeebbbbafcbdaee"));
constraint
  (("abdbbbeb") ++ (D) ++ ("edbacceaaffbecbbdcfeaffbefcafaa")) =
    (("abdbbbebdedbacceaaffbecbb") ++ (D) ++ ("cfeaffbefcafaa"));
constraint
  (("dfeb") ++ (D) ++ ("adfebeacfbeceaea") ++ (D) ++ ("aefdefaceebbedfffe"))
    =
    (("dfebda") ++ (D) ++ ("febeacfbeceaea") ++ (D) ++ ("aefdefaceebbe") ++ (D) ++ ("fffe"));
constraint ((str_len (C)) * (3)) >= (18);
constraint ((str_len (A)) * (20)) >= (400);
solve satisfy;
