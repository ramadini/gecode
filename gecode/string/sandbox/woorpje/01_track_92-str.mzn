var string: G;
var string: A;
constraint
  (("cccab") ++ (G) ++ ("baacb") ++ (G) ++ ("bb") ++ (A) ++ ("acbcbbccbbaaacabcacab"))
    =
    (("cccab") ++ (G) ++ ("baacb") ++ (G) ++ ("bbca") ++ (G) ++ ("acbcbbccbbaaacabcacab"));
solve satisfy;
