var string: I;
var string: B;
var string: G;
var string: F;
constraint
  (("dd") ++ (F) ++ ("ebacadcdcef") ++ (G) ++ ("abfb")) =
    (("ddfccbcfebaca") ++ (I) ++ ("cdbabbabfb"));
constraint
  (("cfddd") ++ (B) ++ ("acbdbdeaabbabbaaedec") ++ (B) ++ ("dbaddbcbfbe")) =
    (("cfddddfacbdbdeaabbabbaaedecdfdbaddbcbfbe") ++ (""));
constraint
  (("dbbcfaabacbbfeaffcbfdbaeeefbeefdeb") ++ (B) ++ ("ccdb")) =
    (("dbbcfaabacbbfeaffcbfdbaeeefbeefdeb") ++ (B) ++ ("ccdb"));
constraint ((str_len (I)) * (16)) >= (80);
constraint ((str_len (I)) * (7)) <= (574);
constraint ((str_len (B)) * (14)) >= (28);
constraint ((str_len (B)) * (12)) <= (72);
constraint ((str_len (F)) * (7)) >= (7);
constraint ((str_len (F)) * (19)) <= (665);
constraint ((str_len (G)) * (11)) <= (1463);
solve satisfy;
