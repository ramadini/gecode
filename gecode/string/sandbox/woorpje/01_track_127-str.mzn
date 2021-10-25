var string: I;
var string: F;
var string: H;
constraint
  (("d") ++ (I) ++ ("dedacdcccbecaeaddbcbccdebababbbbecadedd") ++ (H) ++ ("ec"))
    =
    (("d") ++ (I) ++ ("dedacdcccbecaeaddbcbccdebababbbbecadeddbaea") ++ (F) ++ ("abbecabcbec"));
solve satisfy;
