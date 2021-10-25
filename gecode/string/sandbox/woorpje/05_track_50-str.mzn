var string: H;
var string: F;
var string: E;
var string: C;
constraint
  (("bef") ++ (E) ++ ("dfcfbfe") ++ (H) ++ ("ccf")) =
    (("befba") ++ (C) ++ ("ad") ++ (F) ++ ("dfeaeccf"));
constraint
  (("ddacecaaedffedebdecdefadfcfeabacd") ++ (E) ++ ("fac")) =
    (("ddacecaaedffedebdecdefadfcfeabacdbabefac") ++ (""));
constraint
  (("cceaaefdffbbacdfbcbb") ++ (E) ++ ("dafcdebdffdaaaef")) =
    (("cceaaefdffbbacdfbcbbbabedafcdebdffdaaaef") ++ (""));
constraint ((str_len (F)) * (5)) >= (55);
constraint ((str_len (F)) * (19)) <= (1425);
constraint ((str_len (E)) * (1)) >= (1);
constraint ((str_len (C)) * (9)) >= (9);
constraint ((str_len (H)) * (10)) <= (270);
solve satisfy;
