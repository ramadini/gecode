var string: H;
var string: B;
var string: F;
var string: J;
constraint
  ((F) ++ ("aefcceccd") ++ (B) ++ ("beefbdfdeaaafecfdf")) =
    (("ac") ++ (J) ++ ("fdeaaafecfdf"));
constraint
  (("fa") ++ (H) ++ ("bdaace") ++ (F) ++ ("be") ++ (F) ++ ("dcdcbeebdffcdcbedfdefdb"))
    = (("fa") ++ (H) ++ ("bdaaceacbeacdcdcbeebdffcdcbedfdefdb"));
constraint
  (("aedaabebeabbbcfacdbbbfebfcfaebbdfeeacdff") ++ ("")) =
    (("aedaabebeabbbcf") ++ (F) ++ ("dbbbfebfcfaebbdfeeacdff"));
constraint ((str_len (J)) * (18)) <= (756);
constraint ((str_len (H)) * (15)) >= (30);
constraint ((str_len (H)) * (1)) <= (7);
constraint ((str_len (F)) * (13)) >= (26);
solve satisfy;
