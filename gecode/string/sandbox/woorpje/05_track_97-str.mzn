var string: B;
var string: A;
var string: J;
var string: C;
constraint
  (("dbefbfbae") ++ (B) ++ ("cc") ++ (J) ++ ("dcbcfbe")) =
    (("dbefbfbaeb") ++ (C) ++ (A) ++ ("cbfbafcfdfcadcbcfbe"));
constraint
  (("fbfedbbcd") ++ (A) ++ ("adeabed") ++ (A) ++ ("dfafccdcfdddabcbfaff")) =
    (("fbfedbbcdeeadeabedeedfafccdcfdddabcbfaff") ++ (""));
constraint
  (("cddaadaaddfbccdaafacaeadbcfadbfeedfcceca") ++ ("")) =
    (("cddaadaaddfbccdaafacaeadbcfadbf") ++ (A) ++ ("dfcceca"));
constraint ((str_len (A)) * (1)) <= (4);
constraint ((str_len (B)) * (12)) <= (468);
constraint ((str_len (J)) * (8)) >= (88);
constraint ((str_len (J)) * (3)) <= (69);
constraint ((str_len (C)) * (19)) >= (95);
solve satisfy;
