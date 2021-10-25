var string: H;
var string: B;
var string: F;
constraint
  (("fe") ++ (B) ++ (B) ++ ("eccecac") ++ (B) ++ (H) ++ ("fbcdba") ++ (B) ++ ("b") ++ (H) ++ ("ea") ++ (H) ++ ("ebecbf") ++ (B) ++ ("fdbfbcdd"))
    =
    (("fe") ++ (B) ++ ("decce") ++ (F) ++ ("ea") ++ (H) ++ ("ebecbf") ++ (B) ++ ("fdbfbc") ++ (B) ++ ("d"));
constraint
  ((H) ++ (B) ++ ("ba") ++ (H) ++ ("fcc") ++ (H) ++ ("fc") ++ (H) ++ ("ffeebefaeafad") ++ (H) ++ ("ee") ++ (B) ++ ("f") ++ (H) ++ ("fb") ++ (B) ++ (B) ++ ("dadac"))
    =
    (("a") ++ (B) ++ ("ba") ++ (H) ++ ("fcc") ++ (H) ++ ("fc") ++ (H) ++ ("ffeebef") ++ (H) ++ ("e") ++ (H) ++ ("f") ++ (H) ++ (B) ++ ("aeedf") ++ (H) ++ ("fb") ++ (B) ++ (B) ++ (B) ++ ("a") ++ (B) ++ ("ac"));
constraint
  (("fbeccaff") ++ (H) ++ ("cfcf") ++ (H) ++ ("fc") ++ (B) ++ ("abaafe") ++ (B) ++ ("a") ++ (H) ++ ("ffeeabcf") ++ (B) ++ (H) ++ ("ecc") ++ (H))
    =
    (("fbeccaff") ++ (H) ++ ("cfcf") ++ (H) ++ ("fcd") ++ (H) ++ ("baafe") ++ (B) ++ (H) ++ ("affee") ++ (H) ++ ("bcf") ++ (B) ++ ("aecc") ++ (H));
constraint ((str_len (F)) * (9)) >= (27);
constraint ((str_len (F)) * (13)) <= (728);
constraint ((str_len (B)) * (19)) >= (19);
constraint ((str_len (H)) * (8)) >= (8);
constraint ((str_len (H)) * (19)) <= (114);
solve satisfy;
