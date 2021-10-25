var string: H;
var string: A;
var string: J;
var string: C;
var string: D;
constraint
  (("dedcffef") ++ (C) ++ ("ee") ++ (J) ++ ("ff")) =
    (("dedcffefe") ++ (A) ++ (H) ++ ("eebee") ++ (D) ++ ("f"));
constraint
  (("bdcbbeecbbabefbdabfdf") ++ (A) ++ ("dbcbcb") ++ (A) ++ ("fceccdfdc") ++ (A) ++ ("f"))
    =
    (("bdcbbeecbb") ++ (A) ++ ("befbd") ++ (A) ++ ("bfdfadbcbcbafceccdfdcaf"));
constraint
  (("bbeeccddfbdccebdffbf") ++ (A) ++ ("fbddcdabdc") ++ (A) ++ ("babcafef"))
    =
    (("bbeeccddfbdccebdffbfafbddcdabdcab") ++ (A) ++ ("bc") ++ (A) ++ ("fef"));
constraint ((str_len (D)) * (13)) >= (104);
constraint ((str_len (A)) * (10)) >= (10);
constraint ((str_len (J)) * (13)) <= (3757);
constraint ((str_len (C)) * (1)) <= (103);
constraint ((str_len (H)) * (10)) >= (10);
constraint ((str_len (H)) * (5)) <= (140);
solve satisfy;
