var string: I;
var string: G;
var string: D;
constraint
  (("cceacbccdecbfcffadbe") ++ (I) ++ ("c") ++ (G) ++ ("fbacadfcabd")) =
    (("cceacbccdecb") ++ (G) ++ ("c") ++ (G) ++ (D) ++ ("aabcecffbacad") ++ (G) ++ ("cabd"));
constraint
  (("eaa") ++ (G) ++ ("dbbdebcdbe") ++ (G) ++ ("acbdaccdb") ++ (G) ++ ("beadbbfbddeaeba"))
    =
    (("eaa") ++ (G) ++ ("dbbdebcdbefacbdaccdbfbeadbb") ++ (G) ++ ("bddeaeba"));
constraint
  (("dbdadcbeceadbab") ++ (G) ++ ("cedadbbdcacfccbaaebedace")) =
    (("dbdadcbeceadbab") ++ (G) ++ ("cedadbbdcac") ++ (G) ++ ("ccbaaebedace"));
constraint ((str_len (I)) * (12)) >= (24);
constraint ((str_len (D)) * (9)) <= (675);
constraint ((str_len (G)) * (2)) >= (2);
solve satisfy;
