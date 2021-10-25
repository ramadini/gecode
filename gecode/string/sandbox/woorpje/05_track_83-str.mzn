var string: I;
var string: H;
var string: B;
var string: G;
var string: F;
var string: C;
var string: D;
constraint
  (("cf") ++ (D) ++ ("facfd") ++ (B) ++ ("ecefad") ++ (C) ++ ("edaa")) =
    (("cf") ++ (G) ++ ("cdcf") ++ (I) ++ (H) ++ ("cffdecefad") ++ (C) ++ ("edaa"));
constraint
  (("eacfeaaad") ++ (C) ++ ("deefffecef") ++ (C) ++ ("bcccdafeaddeec") ++ (C) ++ ("abfe"))
    =
    (("eacfeaaadbdeefffecefb") ++ (C) ++ ("cccdafeaddeecba") ++ (C) ++ ("fe"));
constraint
  (("e") ++ (C) ++ ("badefca") ++ (C) ++ ("ededfecaacf") ++ (F) ++ ("eeadad") ++ (C) ++ ("cffcacbede"))
    =
    (("eb") ++ (C) ++ ("adefca") ++ (C) ++ ("ededfecaacfaeeeadad") ++ (C) ++ ("cffcacbede"));
constraint ((str_len (B)) * (7)) <= (315);
constraint ((str_len (H)) * (10)) <= (260);
constraint ((str_len (F)) * (15)) >= (30);
constraint ((str_len (D)) * (16)) >= (48);
constraint ((str_len (G)) * (20)) <= (80);
solve satisfy;
