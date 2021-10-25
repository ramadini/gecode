var string: B;
var string: G;
var string: C;
var string: D;
constraint
  (("efcd") ++ (B) ++ ("fccdbfbc")) =
    (("efcdb") ++ (C) ++ ("cece") ++ (D) ++ ("faddebbfccdbfbc"));
constraint
  (("dcffdcadaadcaddbbabebfbcccdae") ++ (G) ++ ("eddfafbee")) =
    (("dcffdcadaadcaddbbabebfbcccdaeaceddfafbee") ++ (""));
constraint
  (("cefc") ++ (G) ++ ("afbdefcbadbebefdecdefaeffeccacfadf")) =
    (("cefcacafbdefcbadbebefdecdefaeffeccacfadf") ++ (""));
constraint ((str_len (B)) * (17)) >= (272);
constraint ((str_len (B)) * (16)) <= (3136);
constraint ((str_len (D)) * (13)) >= (78);
constraint ((str_len (G)) * (19)) >= (19);
constraint ((str_len (G)) * (14)) <= (168);
constraint ((str_len (C)) * (1)) >= (7);
solve satisfy;
