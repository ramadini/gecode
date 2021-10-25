var string: H;
var string: E;
var string: A;
var string: C;
constraint
  (("bce") ++ (A) ++ ("aacee") ++ (C) ++ ("ba") ++ (A) ++ (A) ++ (A) ++ (A) ++ ("ebcf") ++ (A) ++ ("df"))
    = (("bcedaac") ++ (H) ++ (A) ++ ("edebe") ++ (E) ++ ("ddf"));
constraint
  (("ef") ++ (A) ++ ("cbecfcefac") ++ (A) ++ ("ceaa") ++ (A) ++ ("faebccbbf") ++ (A) ++ ("bffb") ++ (A) ++ ("fafecf"))
    =
    (("ef") ++ (A) ++ ("cbecfcefacdceaadfaebccbbf") ++ (A) ++ ("bffb") ++ (A) ++ ("fafecf"));
constraint
  (("dacafefecfbca") ++ (A) ++ ("fccea") ++ (A) ++ ("ec") ++ (A) ++ (A) ++ ("fdecfdeceaeffbbd"))
    =
    ((A) ++ ("acafefecfbcadfccea") ++ (A) ++ ("ecd") ++ (A) ++ ("fdecf") ++ (A) ++ ("eceaeffbb") ++ (A));
constraint ((str_len (E)) * (6)) >= (102);
constraint ((str_len (H)) * (17)) >= (17);
solve satisfy;
