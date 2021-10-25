var string: C;
var string: D;
var string: F;
constraint
  (("abadcaaebeaeaeebad") ++ (D) ++ ("e") ++ (C) ++ ("eeeeeebbbcbddeabdcec"))
    =
    (("abadcaaebeaeaeebad") ++ (D) ++ ("eabb") ++ (F) ++ ("ebbbcbddeabdcec"));
solve satisfy;
