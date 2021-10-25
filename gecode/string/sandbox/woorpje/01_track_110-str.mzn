var string: I;
var string: C;
var string: G;
var string: J;
constraint
  (("dbde") ++ (I) ++ ("bbb") ++ (J) ++ ("edbbedaaaecdeaabedeab") ++ (I) ++ ("abcbccbcdd"))
    =
    (("dbde") ++ (I) ++ ("bbbdadbcceec") ++ (G) ++ ("bbebb") ++ (C) ++ ("cbccbcdd"));
solve satisfy;
