var string: H;
var string: B;
var string: F;
var string: E;
var string: A;
constraint
  ((B) ++ ("abbabbadeead") ++ (A) ++ ("acbaca") ++ (H) ++ ("aeb") ++ (H) ++ ("edb") ++ (A) ++ ("c") ++ (A) ++ ("c") ++ (H) ++ ("ebabcc") ++ (E) ++ ("cbc") ++ (H) ++ (H))
    =
    ((A) ++ ("bb") ++ (H) ++ ("abb") ++ (A) ++ ("bba") ++ (H) ++ ("e") ++ (F) ++ ("cad") ++ (E) ++ ("bde") ++ (H) ++ ("b") ++ (A) ++ ("cacdebabcc") ++ (A) ++ ("ecbcd") ++ (H));
solve satisfy;
