var string: G;
var string: D;
var string: A;
constraint (("eadfd") ++ (A) ++ ("fbfbb")) = (("eadfdeeadcbbcfe") ++ (D));
constraint
  (("bcafba") ++ (G) ++ ("fdcfeaeddccbbdccefedcffdfeddbb")) =
    (("bcafba") ++ (G) ++ ("fdcfeaeddccbbdccefedcffdfeddbb"));
constraint
  (("debfefdeaccadebedeafcdabbabfaeeadfbcacfc") ++ ("")) =
    (("debfefdeaccadebedeafcdabbabfa") ++ (G) ++ ("fbcacfc"));
constraint ((str_len (G)) * (12)) >= (12);
constraint ((str_len (G)) * (11)) <= (132);
solve satisfy;
