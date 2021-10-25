var string: H;
var string: C;
var string: B;
constraint
  (("ccaedb") ++ (H) ++ ("aaa")) =
    (("ccaedbcda") ++ (C) ++ ("adafccbfdbecebbaaa"));
constraint
  (("bebccabccdbeaaa") ++ (B) ++ ("dffdabdeddeaefffebbdbaa")) =
    (("bebccabccdbeaaa") ++ (B) ++ ("dffdabdeddeaefffebbdbaa"));
constraint
  (("becbddfefdaebcfaffbbfbddbeecd") ++ (B) ++ ("dacbfbadd")) =
    (("becbddfefdaebcfaffbbfbddbeecd") ++ (B) ++ ("dacbfbadd"));
constraint ((str_len (C)) * (8)) >= (72);
constraint ((str_len (B)) * (11)) >= (11);
constraint ((str_len (H)) * (7)) <= (658);
solve satisfy;
