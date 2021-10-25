var string: I;
var string: E;
constraint
  (("ae") ++ (E) ++ ("bbbccbaefabdb") ++ (I) ++ ("babececadaef")) =
    (("ae") ++ (E) ++ ("bbbccbaefabdb") ++ (I) ++ ("babececadaef"));
constraint
  (("dbafaafebdafbaffadabcfbcbdbebcffafaffabb") ++ ("")) =
    (("dbafaafebdafbaffadabcfbcbd") ++ (E) ++ ("ffafaffabb"));
constraint
  (("cdbadcaefbfafdbafbcecfdaccbeb") ++ (E) ++ ("ceddfad")) =
    (("cdbadcaefbfafdbafbcecfdaccbeb") ++ (E) ++ ("ceddfad"));
constraint ((str_len (E)) * (4)) >= (8);
constraint ((str_len (I)) * (15)) <= (135);
solve satisfy;
