var string: B;
var string: G;
var string: A;
var string: C;
constraint
  (("fcfdbbcc") ++ (B) ++ ("ecfb") ++ (G) ++ (A) ++ ("c") ++ (C) ++ ("bcbfb") ++ (G) ++ ("fff"))
    =
    (("fcfdbbccdfecfb") ++ (G) ++ ("abecccdeacffcbd") ++ (G) ++ ("bcbfbafff"));
constraint
  (("afcabd") ++ (G) ++ ("caefebdcbeedefdd") ++ (G) ++ ("fbcc") ++ (G) ++ ("abfcfadbfce"))
    =
    ((G) ++ ("fc") ++ (G) ++ ("bd") ++ (G) ++ ("caefebdcbeedefddafbcca") ++ (G) ++ ("bfcf") ++ (G) ++ ("dbfce"));
constraint
  (("bbcccfdebbdbbdbcfeaebfcdccd") ++ (G) ++ (G) ++ ("fd") ++ (G) ++ ("ea") ++ (G) ++ ("cecbd"))
    =
    (("bbcccfdebbdbbdbcfeaebfcdccd") ++ (G) ++ (G) ++ ("fdaea") ++ (G) ++ ("cecbd"));
constraint ((str_len (G)) * (16)) <= (16);
constraint ((str_len (A)) * (4)) >= (12);
constraint ((str_len (A)) * (15)) <= (720);
constraint ((str_len (C)) * (20)) >= (120);
solve satisfy;
