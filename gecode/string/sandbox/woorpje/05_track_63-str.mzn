var string: H;
var string: B;
var string: F;
var string: A;
var string: C;
var string: D;
constraint
  (("ccdfcfecbada") ++ (B) ++ ("dacbdcecadafaadfdb")) =
    (("ccdfcfecbadafeebbb") ++ (F) ++ ("cecadafaadfdb"));
constraint
  (("edd") ++ (H) ++ (A) ++ ("faabfceeff") ++ (F) ++ ("dafcebbdfbe")) =
    (("eddb") ++ (D) ++ ("f") ++ (C) ++ ("e"));
constraint
  (("d") ++ (D) ++ (D) ++ ("eeabdbaeebefffea") ++ (D) ++ ("eabd") ++ (D) ++ ("afc") ++ (D) ++ ("affcc") ++ (D) ++ ("cefad"))
    =
    (("dc") ++ (D) ++ ("eeabdbaeebefffea") ++ (D) ++ ("eabdcafc") ++ (D) ++ ("affccc") ++ (D) ++ ("efad"));
constraint
  (("defaebaebaedceebdabfafe") ++ (D) ++ ("eafebae") ++ (D) ++ (D) ++ ("cdfafff"))
    =
    (("defaebaebaed") ++ (D) ++ ("eebdabfafeceafebae") ++ (D) ++ ("ccdfafff"));
constraint ((str_len (C)) * (7)) >= (84);
constraint ((str_len (D)) * (17)) <= (17);
constraint ((str_len (F)) * (4)) >= (28);
constraint ((str_len (F)) * (15)) <= (750);
constraint ((str_len (H)) * (20)) >= (80);
constraint ((str_len (A)) * (3)) >= (3);
constraint ((str_len (A)) * (9)) <= (423);
solve satisfy;
