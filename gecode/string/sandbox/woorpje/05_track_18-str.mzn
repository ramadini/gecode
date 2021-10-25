var string: I;
var string: H;
var string: G;
var string: E;
var string: F;
constraint
  (("afacd") ++ (I) ++ ("cebbbedaafcaf") ++ (G)) =
    (("afacd") ++ (I) ++ ("cebbbeda") ++ (H) ++ ("f") ++ (E) ++ ("a"));
constraint
  (("badcadfcedddbeaddbfcbca") ++ (I) ++ ("bafefecabbbfaed")) =
    (("badcadfcedddbeaddbfcb") ++ (F) ++ ("afefecabbbfaed"));
constraint
  (("decdedebbeffcbbddbcffacababeabaaaecebbee") ++ ("")) =
    (("decdedebbe") ++ (I) ++ ("cbbddbc") ++ (I) ++ ("acababeabaaaecebbee"));
constraint ((str_len (F)) * (17)) >= (68);
constraint ((str_len (F)) * (14)) <= (350);
constraint ((str_len (H)) * (15)) >= (60);
constraint ((str_len (H)) * (10)) <= (350);
constraint ((str_len (G)) * (6)) <= (450);
constraint ((str_len (I)) * (1)) <= (6);
solve satisfy;
