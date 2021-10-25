var string: I;
var string: C;
var string: G;
constraint
  (("c") ++ (I) ++ ("fcdfdaed") ++ (G) ++ ("daadee") ++ (I) ++ ("dcaddccdccc") ++ (I) ++ ("dadbebfa"))
    = (("cbfc") ++ (C) ++ ("addccdcccbdadbebfa"));
constraint
  (("acdedcceebbadfcdbcafbbdfaeefdcaeccbd") ++ (I) ++ ("cdc")) =
    (("acdedccee") ++ (I) ++ ("badfcdbcafbbdfaeefdcaeccbdbcdc"));
constraint
  (("deceabcffafeeb") ++ (I) ++ ("afecabdafcd") ++ (I) ++ ("dcdd") ++ (I) ++ ("ccadbfaa"))
    =
    (("deceabcffafeebbafecabdafcd") ++ (I) ++ ("dcdd") ++ (I) ++ ("ccad") ++ (I) ++ ("faa"));
constraint ((str_len (C)) * (5)) <= (220);
solve satisfy;
