var string: C;
var string: A;
constraint
  ((A) ++ (A) ++ (A) ++ ("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")) =
    ((A) ++ ("aaaaaaaaaaaaaaaaaaa") ++ (A) ++ ("aaaaaa") ++ (C) ++ ("aaa"));
solve satisfy;
