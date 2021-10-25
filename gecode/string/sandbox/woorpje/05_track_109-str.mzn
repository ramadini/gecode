var string: F;
var string: A;
var string: D;
constraint
  (("dddaccfab") ++ (F) ++ ("bacecdbaffabbcbdfdeea")) =
    (("dddaccfab") ++ (D) ++ ("fabbcbdfdeea"));
constraint
  (("debcbfaeafeafc") ++ (A) ++ ("bbaccbcbddcebbaefffccfb")) =
    (("debcbfaeafeafcbcdbbaccbcbddcebbaefffccfb") ++ (""));
constraint
  (("cedafadeaabdccddddcd") ++ (A) ++ ("efdbeeacefceafcdb")) =
    (("cedafadeaabdccddddcdbcdefdbeeacefceafcdb") ++ (""));
constraint ((str_len (D)) * (14)) >= (70);
constraint ((str_len (D)) * (7)) <= (301);
constraint ((str_len (A)) * (7)) >= (7);
solve satisfy;
