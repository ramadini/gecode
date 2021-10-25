var string: F;
var string: E;
constraint
  (("cbab") ++ (F) ++ ("abaacbcacb")) =
    (("cbabacccc") ++ (E) ++ ("bbbaabaacbcacb"));
solve satisfy;
