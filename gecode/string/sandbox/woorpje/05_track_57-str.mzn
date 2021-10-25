var string: C;
var string: E;
var string: A;
constraint
  (("bfff") ++ (C) ++ ("f")) =
    (("bfffffbcebffceff") ++ (A) ++ ("deabeaddbeabccbdacf"));
constraint
  (("cfdbdceefefcbbcdcdadfaaaffdccfdedcdfbcff") ++ ("")) =
    (("cfdbdceefefcbbcdcda") ++ (A) ++ ("ffdccfdedcdfbcff"));
constraint
  (("eeadceabbcafddcfe") ++ (E) ++ ("cbfbadbefffccdeeeb")) =
    (("eeadceabbcafddcfefffffcbfbadbefffccdeeeb") ++ (""));
constraint ((str_len (C)) * (17)) >= (544);
constraint ((str_len (C)) * (15)) <= (1395);
constraint ((str_len (A)) * (8)) <= (312);
constraint ((str_len (E)) * (16)) >= (32);
solve satisfy;
