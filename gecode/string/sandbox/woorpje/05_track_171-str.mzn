var string: F;
var string: J;
var string: C;
var string: D;
constraint
  (("fbdaffaab") ++ (F) ++ ("dea") ++ (C) ++ (J) ++ ("acaeeeedacffd")) =
    (("fbdaffaabceb") ++ (D) ++ ("dacffd"));
constraint
  (("ffccbcecdabefadfddecedecdadacdccdfeeebaf") ++ ("")) =
    (("ffccbcecdabefadf") ++ (C) ++ ("cedecdadacdccdfeeebaf"));
constraint
  (("cacdcdfcddbea") ++ (C) ++ ("edbceddcdceaedbcecabfbeb")) =
    (("cacdcdfcddbea") ++ (C) ++ ("edbceddcdceaedbcecabfbeb"));
constraint ((str_len (D)) * (4)) <= (1260);
constraint ((str_len (J)) * (4)) <= (160);
constraint ((str_len (F)) * (1)) >= (3);
solve satisfy;
