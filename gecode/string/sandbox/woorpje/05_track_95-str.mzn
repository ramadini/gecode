var string: H;
var string: B;
var string: A;
var string: J;
var string: C;
constraint
  (("dbb") ++ (A) ++ ("be") ++ (C) ++ ("fc")) =
    (("dbb") ++ (A) ++ ("beaf") ++ (B) ++ (J) ++ ("bcdccabbbdaeacdfc"));
constraint
  (("aaaefedabecefdc") ++ (H) ++ ("ffaefaeadfddecaadcda")) =
    (("aaaefedabecefdc") ++ (J) ++ ("ffaefaeadfddecaadcda"));
constraint
  (("cfabfccaacbcdbbebbbffccffecdccfcbffbeaed") ++ ("")) =
    (("cfabfccaacbcdbbebbbffccffecdccfcb") ++ (A) ++ ("d"));
constraint ((str_len (B)) * (18)) >= (18);
constraint ((str_len (J)) * (9)) >= (18);
constraint ((str_len (A)) * (6)) >= (18);
constraint ((str_len (H)) * (7)) <= (126);
solve satisfy;
