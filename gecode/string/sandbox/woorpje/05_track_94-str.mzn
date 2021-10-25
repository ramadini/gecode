var string: B;
var string: F;
var string: G;
var string: A;
constraint
  (("dba") ++ (G) ++ (B) ++ ("cebfd")) =
    (("dbadfdb") ++ (A) ++ ("eeeccbeebacebfd"));
constraint
  (("eecbadbfaeffca") ++ (F) ++ ("fedaebcefebacbaaeeccab")) =
    (("eecbadbfaeffcadfeafedaebcefebacbaaeeccab") ++ (""));
constraint
  ((F) ++ ("bcdabbcadfdccefdcfebbeaeebfeffebdcda") ++ ("")) =
    (("dfeabcdabbcadfdccefdcfebbeaeebfeffebdcda") ++ (""));
constraint ((str_len (A)) * (17)) >= (119);
constraint ((str_len (B)) * (18)) >= (270);
constraint ((str_len (G)) * (14)) >= (56);
solve satisfy;
