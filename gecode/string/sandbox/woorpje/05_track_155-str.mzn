var string: B;
var string: G;
var string: E;
constraint
  ((E) ++ ("fac") ++ (B) ++ ("dafbadadbcfcfbcabf")) =
    (("ddaeaeea") ++ (G) ++ ("dadbcfcfbcabf"));
constraint
  (("eedfaeffafbddfceebeabceeb") ++ (B) ++ ("bbacabaedc")) =
    (("eedfaeffafbddfceebeabceebdfcedbbacabaedc") ++ (""));
constraint
  (("cddbfadfacc") ++ (B) ++ ("ccdcbdfebcfcbfbcfffbaebe")) =
    (("cddbfadfacc") ++ (B) ++ ("ccdcbdfebcfcbfbcfffbaebe"));
constraint ((str_len (B)) * (4)) <= (36);
constraint ((str_len (G)) * (16)) >= (304);
constraint ((str_len (G)) * (13)) <= (1859);
solve satisfy;
