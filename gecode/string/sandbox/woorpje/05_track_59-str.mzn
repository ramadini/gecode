var string: G;
var string: F;
var string: C;
var string: D;
constraint
  (("cbbfdb") ++ (G) ++ ("ccbcda") ++ (C) ++ ("abbaaaeeddae")) =
    (("cbb") ++ (D) ++ ("ccabbaaaeeddae"));
constraint
  (("d") ++ (F) ++ ("cabafcabfaedbadedacddcbabfcdabaefbb")) =
    (("ddbcccabafcabfaedbadedacddcbabfcdabaefbb") ++ (""));
constraint
  (("edebbfeedaadb") ++ (F) ++ ("afefbbaacbbaebceeabcbda")) =
    (("edebbfeedaadbdbccafefbbaacbbaebceeabcbda") ++ (""));
constraint ((str_len (F)) * (2)) <= (14);
constraint ((str_len (G)) * (19)) >= (38);
constraint ((str_len (G)) * (11)) <= (1342);
constraint ((str_len (C)) * (9)) <= (315);
solve satisfy;
