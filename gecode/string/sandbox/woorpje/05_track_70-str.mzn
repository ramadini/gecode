var string: H;
var string: C;
var string: G;
constraint
  (("cdcafcabff") ++ (C) ++ ("acdeebccbecabe")) =
    (("cdcafcabffdbddcfef") ++ (G) ++ ("e"));
constraint
  (("dbdcbfdbebeebebfccfedccba") ++ (H) ++ ("bebdcabebce")) =
    (("dbdcbfdbebeebebfccfedccbacafcbebdcabebce") ++ (""));
constraint
  (("dadfbeefeedffdfcefeebfaabcdc") ++ (H) ++ ("dbdbaabf")) =
    (("dadfbeefeedffdfcefeebfaabcdccafcdbdbaabf") ++ (""));
constraint ((str_len (H)) * (19)) >= (76);
constraint ((str_len (H)) * (3)) <= (159);
constraint ((str_len (G)) * (16)) >= (240);
constraint ((str_len (G)) * (1)) <= (351);
constraint ((str_len (C)) * (16)) >= (192);
constraint ((str_len (C)) * (7)) <= (441);
solve satisfy;
