var string: G;
var string: E;
var string: A;
var string: J;
var string: D;
constraint
  ((E) ++ ("cdf") ++ (G) ++ ("baaabdebdaadffb")) =
    (("efb") ++ (A) ++ (D) ++ ("bdaadffb"));
constraint
  (("cbafcbedbdfeebeccbdefdebffaadee") ++ (J) ++ ("ceaec")) =
    (("cbafcbedbdfeebeccbdefdebffaadeecadcceaec") ++ (""));
constraint
  (("eaedcbfebbadbbddbfafcddebcaadbcaefbdfeae") ++ ("")) =
    (("eaedcbfebbadbbddbfafcddebcaadbca") ++ (E) ++ ("ae"));
constraint ((str_len (D)) * (15)) >= (165);
constraint ((str_len (D)) * (12)) <= (1452);
constraint ((str_len (A)) * (9)) <= (459);
constraint ((str_len (J)) * (5)) >= (5);
constraint ((str_len (J)) * (5)) <= (20);
solve satisfy;
