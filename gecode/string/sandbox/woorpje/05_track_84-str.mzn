var string: I;
var string: F;
var string: J;
constraint
  (("cfe") ++ (I) ++ ("fbeefba") ++ (F) ++ ("dfcbdefdaafeacbdacba")) =
    (("cfe") ++ (I) ++ ("fbeefbaffbc") ++ (J) ++ ("ba"));
constraint
  (("bbebadfaffedcbdbdfacfe") ++ (I) ++ ("bcdafbbdeddcbcb")) =
    (("bbebadfaffedcbdbdfacfe") ++ (I) ++ ("bcdafbbdeddcbcb"));
constraint
  (("cdbfbfaedfdcaecece") ++ (I) ++ ("fafdfcfecbcfbecfeef")) =
    (("cdbfbfaedfdcaececebfefafdfcfecbcfbecfeef") ++ (""));
constraint ((str_len (F)) * (9)) >= (45);
constraint ((str_len (J)) * (2)) <= (270);
solve satisfy;
