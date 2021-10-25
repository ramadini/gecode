var string: I;
var string: H;
var string: G;
var string: D;
constraint
  (("ebbd") ++ (I) ++ ("d") ++ (D) ++ ("dedbbbef")) =
    (("eb") ++ (I) ++ (I) ++ ("ddcdbecddcdfb") ++ (G) ++ ("afbfdedbbbef"));
constraint
  (("fbbbbc") ++ (I) ++ ("dadbbcaffaecbfdddbadaeaceccbcfcd")) =
    (("fbbbbcbddadbbcaffaecbfdddbadaeaceccbcfcd") ++ (""));
constraint
  (("cbfbeacba") ++ (I) ++ ("aaffaebaffccdbbf") ++ (I) ++ ("aafffdeeeee")) =
    (("cbfbeacba") ++ (I) ++ ("aaffaebaffccdbbf") ++ (I) ++ ("aafff") ++ (H) ++ ("eee"));
constraint ((str_len (G)) * (10)) <= (440);
constraint ((str_len (D)) * (9)) >= (162);
constraint ((str_len (I)) * (1)) <= (6);
constraint ((str_len (H)) * (15)) >= (45);
solve satisfy;
