var string: I;
var string: E;
var string: A;
constraint
  ((I) ++ ("bddeeffdab") ++ (A) ++ ("b") ++ (I) ++ ("abcfbbbaa")) =
    ((I) ++ ("bddeeffda") ++ (E) ++ ("fbe") ++ (I) ++ ("defebcab") ++ (I) ++ ("fbbbaa"));
constraint
  (("addadaebaeeaeffdbacfeafbf") ++ (I) ++ ("f") ++ (I) ++ ("fbaafdeabbea"))
    = (("addadaebaeeaeffdbacfeafbf") ++ (I) ++ ("fcfbaafdeabbea"));
constraint
  (("fcafbfafeffeca") ++ (I) ++ (I) ++ ("fbfcfad") ++ (I) ++ ("af") ++ (I) ++ (I) ++ ("d") ++ (I) ++ (I) ++ ("bbecfa") ++ (I) ++ ("af"))
    =
    (("f") ++ (I) ++ ("afbfafeffe") ++ (I) ++ ("a") ++ (I) ++ ("cfbf") ++ (I) ++ ("fadcafc") ++ (I) ++ ("dccbbecfa") ++ (I) ++ ("af"));
constraint ((str_len (E)) * (19)) >= (209);
constraint ((str_len (E)) * (1)) <= (47);
solve satisfy;
