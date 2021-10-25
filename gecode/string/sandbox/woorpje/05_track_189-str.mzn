var string: I;
var string: G;
var string: D;
constraint
  (("ffeefbfaabcebfdaf") ++ (D) ++ ("cac")) =
    (("ffeefbfaabcebfda") ++ (I) ++ ("cac"));
constraint
  (("ffceafedfeaffafde") ++ (G) ++ ("cefdcccebceeadfaceaee")) =
    (("ffceafedfeaffafde") ++ (G) ++ ("cefdcccebceeadfaceaee"));
constraint
  (("fecbbcadfbfbecaeafbcefacebaeafcedaacfcef") ++ ("")) =
    (("fec") ++ (G) ++ ("cadfbfbecaeafbcefacebaeafcedaacfcef"));
constraint ((str_len (G)) * (15)) <= (300);
solve satisfy;
