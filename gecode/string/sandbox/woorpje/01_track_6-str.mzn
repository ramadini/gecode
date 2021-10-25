var string: C;
var string: A;
constraint
  (("aaadbcb") ++ (A) ++ ("ccdcccdebbcffcbbad") ++ (A) ++ ("fafcbccb") ++ (A) ++ ("edfa") ++ (A) ++ ("ccaacffaa") ++ (A) ++ ("feafacdccadabbdb") ++ (A) ++ ("eccbcbcdedecaaddaebfddbaabcabffadcae") ++ (A) ++ ("aababfaeed"))
    =
    (("aaadbcbfccdcccdebbc") ++ (A) ++ (A) ++ ("cbbad") ++ (A) ++ ("fa") ++ (A) ++ ("cbcc") ++ (C) ++ ("dabbdb") ++ (A) ++ ("eccbcbcdedecaaddaeb") ++ (A) ++ ("ddbaabcab") ++ (A) ++ (A) ++ ("adcaefaababfaeed"));
solve satisfy;
