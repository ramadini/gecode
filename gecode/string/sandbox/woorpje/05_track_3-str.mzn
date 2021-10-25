var string: F;
var string: E;
constraint
  (("aacfcbebeabbeec") ++ (F) ++ ("bfceaccdeaeddbeefacddaa")) =
    (("aacfcbebeabbeecbbb") ++ (E) ++ ("daa"));
constraint
  (("ecffdbaddfbbbffaaf") ++ (F) ++ ("eddfebaadaacafeabcae")) =
    (("ecffdbaddfbbbffaafbbeddfebaadaacafeabcae") ++ (""));
constraint
  (("fdbcacfa") ++ (F) ++ ("dfcddbefafedefcffeebfecdecdbfd")) =
    (("fdbcacfa") ++ (F) ++ ("dfcddbefafedefcffeebfecdecdbfd"));
solve satisfy;
