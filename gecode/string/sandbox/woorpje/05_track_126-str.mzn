var string: I;
var string: G;
constraint
  (("eabbdfefaafdfbedccff") ++ (G) ++ ("ccaeaabebdcbcaedb")) =
    (("eabbdfefaa") ++ (I) ++ ("caeaabebdcbcaedb"));
constraint
  (("deacfba") ++ (G) ++ ("abaafaafaccdbdfedeeebbeddfdabf")) =
    (("deacfba") ++ (G) ++ ("abaafaafaccdbdfedeeebbeddfdabf"));
constraint
  (("ffbfefcabcfbcfefbaeaaefabffeadcbbecfcebe") ++ ("")) =
    ((G) ++ ("fefcabcfbcfefbaeaaefabffeadcbbecfcebe") ++ (""));
constraint ((str_len (I)) * (14)) <= (266);
constraint ((str_len (G)) * (4)) <= (136);
solve satisfy;
