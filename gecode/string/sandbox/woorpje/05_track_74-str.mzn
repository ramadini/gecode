var string: G;
var string: F;
var string: A;
constraint
  (("cdfbeebfebaeabc") ++ (F) ++ ("dddadbbcca")) =
    (("cdfbeebfebaeab") ++ (G) ++ ("c") ++ (A) ++ ("ddadbbcca"));
constraint
  (("aabffeacc") ++ (G) ++ ("cbbdccadfdaaceabdefbddcebccda")) =
    (("aabffeacccfcbbdccadfdaaceabdefbddcebccda") ++ (""));
constraint
  (("cda") ++ (G) ++ ("b") ++ (G) ++ ("abdacbdfccc") ++ (G) ++ ("adacdbaddeeebfbfbfa"))
    = (("cdacfbcfabdacbdfccccfadacdbaddeeebfbfbfa") ++ (""));
constraint ((str_len (G)) * (2)) <= (18);
constraint ((str_len (A)) * (1)) >= (2);
constraint ((str_len (A)) * (10)) <= (620);
constraint ((str_len (F)) * (8)) >= (16);
constraint ((str_len (F)) * (18)) <= (1872);
solve satisfy;
