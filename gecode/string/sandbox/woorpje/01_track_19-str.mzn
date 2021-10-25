var string: G;
var string: E;
constraint
  (("abcccacb") ++ (E) ++ ("abcabcca")) =
    (("abcccacba") ++ (G) ++ ("bcacbbbaaaacbbabcabcca"));
solve satisfy;
