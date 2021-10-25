var string: C;
var string: G;
var string: F;
constraint
  (("edfbdefaaacb") ++ (G) ++ ("fdcacbaeedbdbbab")) =
    (("edfbdefaaacb") ++ (C) ++ ("ab"));
constraint
  (("edefbcdac") ++ (G) ++ ("acfcbbedddddcdafaed")) =
    (("edefbcdac") ++ (C) ++ ("cbbedddddcdafaed"));
constraint
  (("ccddce") ++ (F) ++ ("c")) = (("ccdd") ++ (C) ++ ("adabddeeeaddbcfc"));
constraint
  (("cddadbfbeaccbcccbeecfdaeebffedcffdc") ++ (G) ++ ("abd")) =
    (("cddadbfbeaccbcccbeecfdaeebffedcffdc") ++ (G) ++ ("abd"));
constraint
  (("dcdbfdecbcabcdaeffcfbbebebbfceacddcbbaac") ++ ("")) =
    ((G) ++ ("dbfdecbcabcdaeffcfbbebebbfceacddcbbaac") ++ (""));
constraint ((str_len (C)) * (16)) >= (48);
constraint ((str_len (F)) * (20)) >= (520);
constraint ((str_len (G)) * (12)) <= (144);
solve satisfy;
