var string: B;
var string: G;
var string: J;
constraint
  (("bdbcaffcdafdcedacc") ++ (G)) =
    (("bdbcaffcda") ++ (B) ++ ("edacccaacde") ++ (B) ++ ("dddfdeaaefeaa"));
constraint
  (("faebaaabbbccfecbfacfedafdaabbeaaffdcddca") ++ ("")) =
    (("faebaaabbb") ++ (J) ++ ("fecbfacfedafdaabbeaaf") ++ (B) ++ ("ddca"));
constraint
  (("fafacddcdbdbcddccfb") ++ (B) ++ ("dfebecfebdadabdfbd")) =
    (("fafacddcdbdbcdd") ++ (J) ++ ("fb") ++ (B) ++ ("dfebecfebdadabdfbd"));
constraint ((str_len (B)) * (8)) <= (72);
constraint ((str_len (J)) * (16)) >= (32);
constraint ((str_len (G)) * (2)) >= (22);
constraint ((str_len (G)) * (13)) <= (2652);
solve satisfy;
