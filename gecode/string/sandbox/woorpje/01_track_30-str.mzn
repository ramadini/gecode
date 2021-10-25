var string: H;
var string: F;
var string: A;
constraint
  ((A) ++ ("d") ++ (A) ++ ("ee") ++ (H) ++ ("c") ++ (A)) =
    (("bd") ++ (A) ++ (F) ++ ("c") ++ (A));
solve satisfy;
