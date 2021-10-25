var string: H;
var string: B;
var string: F;
var string: J;
var string: D;
constraint
  (("beefcbdedcdeffddfc") ++ (J) ++ ("ebfcebbdcefecafe")) =
    (("beefcbdedc") ++ (B) ++ ("cefecafe"));
constraint
  (("cdfcbfaecea") ++ (J) ++ ("dcedccddaedeaeacddfffca")) =
    (("cdfcbfaeceadefdfedcedccddaedeaeacddfffca") ++ (""));
constraint
  (("ef") ++ (H) ++ ("ccc") ++ (D) ++ ("eeafdaa") ++ (J) ++ ("ceabbefbaebbfaf"))
    =
    (("ef") ++ (H) ++ ("ccc") ++ (D) ++ ("eea") ++ (F) ++ ("aceedaceabbefbaebbfaf"));
constraint
  (("efaaccededaddbadbcdecdb") ++ (D) ++ ("bddbadbcdfccfb")) =
    (("efaaccededaddbadbcdecdbdcbbddbadbcdfccfb") ++ (""));
constraint
  (("fcfaaedfccabaeaddcdbcedbfdddabdfaaaffdae") ++ ("")) =
    (("fcfaaedfccabaeaddcdbcedbfdddabdfaaaf") ++ (F) ++ ("e"));
constraint ((str_len (B)) * (4)) >= (88);
constraint ((str_len (D)) * (12)) <= (204);
constraint ((str_len (F)) * (7)) <= (63);
constraint ((str_len (J)) * (9)) <= (576);
constraint ((str_len (H)) * (3)) >= (9);
constraint ((str_len (H)) * (19)) <= (114);
solve satisfy;
