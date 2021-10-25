var string: I;
var string: E;
var string: A;
var string: C;
constraint
  (("bb") ++ (I) ++ ("c") ++ (C) ++ ("ccf")) =
    (("bb") ++ (I) ++ ("cc") ++ (A) ++ ("aafbdabcfcdfccf"));
constraint
  (("adeebececeafbcfcdbbdedfffacdacfbdfecbcbd") ++ ("")) =
    (("adeebececeafbcfc") ++ (E) ++ ("dfffacdacfbdfecbcbd"));
constraint
  (("bdaeeb") ++ (E) ++ ("deafcbbeaeddaecabedbcaabbdadc")) =
    (("bdaeeb") ++ (E) ++ ("deafcbbeaeddaecabedbcaabbdadc"));
constraint ((str_len (A)) * (1)) <= (80);
constraint ((str_len (I)) * (9)) <= (567);
solve satisfy;
