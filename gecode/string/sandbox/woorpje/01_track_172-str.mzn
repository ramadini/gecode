var string: B;
var string: F;
var string: E;
constraint
  (("g") ++ (E) ++ ("gdbdccbcfcga") ++ (F) ++ ("ab")) =
    (("gdde") ++ (B) ++ ("cfcga") ++ (F) ++ ("ab"));
solve satisfy;
