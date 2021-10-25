var string: I;
var string: H;
var string: G;
var string: E;
var string: A;
var string: C;
var string: D;
constraint
  (("fffedbaffdaaca") ++ (I) ++ ("eeabcadaabaceafddba")) =
    (("fffedbaffdaa") ++ (C) ++ ("ddba"));
constraint
  (("deed") ++ (D) ++ ("ebdeb") ++ (E) ++ ("e") ++ (C) ++ ("cab")) =
    (("d") ++ (H) ++ (G) ++ ("edeeecacab"));
constraint
  (("efdeacabfcbfcafbbbeec") ++ (E) ++ ("cccbfaeddebbeac")) =
    (("efdeacabfcbfcafbbbeec") ++ (E) ++ ("cccbfaeddebbeac"));
constraint
  (("ccabeeadcfccaccedeececacbfba") ++ (A) ++ ("dcddecfa")) =
    (("ccabeeadcfccaccedeececacbfba") ++ (A) ++ ("dcddecfa"));
constraint ((str_len (G)) * (14)) >= (84);
constraint ((str_len (H)) * (13)) >= (52);
constraint ((str_len (C)) * (11)) >= (110);
constraint ((str_len (C)) * (19)) <= (1368);
constraint ((str_len (D)) * (15)) >= (45);
constraint ((str_len (I)) * (1)) >= (3);
constraint ((str_len (I)) * (4)) <= (172);
constraint ((str_len (A)) * (9)) >= (18);
solve satisfy;
