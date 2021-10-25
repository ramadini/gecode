var string: I;
var string: G;
var string: J;
var string: D;
constraint
  (("cedebfe") ++ (G) ++ ("aecbaab")) =
    (("ce") ++ (J) ++ ("fbef") ++ (I) ++ ("eeaafaecbaab"));
constraint
  (("dcbffabcdefecebfbeafcdeadbdaf") ++ (D) ++ ("fbaffbcfe")) =
    (("dcbffabcdefecebfbeafcdeadbdaf") ++ (D) ++ ("fbaffbcfe"));
constraint
  (("dceccfccbdacadedcfdafdbcbaaeaedffbaaeafc") ++ ("")) =
    (("dceccfccbda") ++ (D) ++ ("dedcfdafdbcbaaeaedffbaaeafc"));
constraint ((str_len (J)) * (6)) >= (18);
constraint ((str_len (G)) * (12)) >= (204);
constraint ((str_len (I)) * (12)) <= (780);
solve satisfy;
