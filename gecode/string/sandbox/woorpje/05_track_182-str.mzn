var string: I;
var string: H;
var string: F;
var string: J;
constraint
  (("fdccdb") ++ (J) ++ ("f") ++ (I) ++ ("eacbfcdee")) =
    (("fdccdbbbdcffa") ++ (F) ++ ("e"));
constraint
  (("abacddbfeeefcecffbfbaaabfabcbcfbbcfcfcfb") ++ ("")) =
    (("abacddbfeeefc") ++ (H) ++ ("fbfbaaabfabcbcfbbcfcfcfb"));
constraint
  (("dfdaebacaeefbcfeeafbceccbaecfeabbecbcdbb") ++ ("")) =
    (("dfdaebacaeefbcfeeafbceccba") ++ (H) ++ ("eabbecbcdbb"));
constraint ((str_len (I)) * (14)) >= (140);
constraint ((str_len (I)) * (5)) <= (900);
constraint ((str_len (H)) * (16)) >= (16);
constraint ((str_len (H)) * (19)) <= (114);
constraint ((str_len (J)) * (2)) >= (2);
constraint ((str_len (F)) * (3)) <= (279);
solve satisfy;
