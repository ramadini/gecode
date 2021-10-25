var string: F;
var string: E;
var string: C;
var string: D;
constraint
  (("bccac") ++ (F) ++ ("afbb") ++ (E) ++ ("fbfef") ++ (D) ++ ("dcdacedfebfcdcada"))
    = (("bccac") ++ (F) ++ ("afbbcceefbfef") ++ (D) ++ ("dcdacedfebfcdcada"));
constraint
  (("dcebaeacaad") ++ (D) ++ ("ffefcafeaabb") ++ (D) ++ ("dccaad") ++ (C) ++ ("baecb"))
    =
    (("dcebaeacaad") ++ (D) ++ ("ffefcafeaabb") ++ (D) ++ ("dccaadafbaecb"));
constraint
  (("a") ++ (F) ++ ("dbbbfeddfb") ++ (C) ++ ("cbdfe") ++ (C) ++ ("fafabd") ++ (D) ++ ("edeffbbdb"))
    =
    (("a") ++ (F) ++ ("dbbbfeddfb") ++ (C) ++ ("cbdfeaff") ++ (C) ++ ("abdbcedeffbbdb"));
constraint ((str_len (F)) * (16)) >= (48);
constraint ((str_len (C)) * (3)) <= (12);
solve satisfy;
