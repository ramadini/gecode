var string: I;
var string: B;
var string: J;
var string: D;
constraint
  (("beceddbdcbfeabce") ++ (D) ++ ("fbaad")) =
    (("be") ++ (I) ++ ("fadbbcbfbaad"));
constraint
  ((D) ++ ("fbbc") ++ ("")) = (("cabe") ++ (J) ++ ("cdbbddbdbaeddfbbc"));
constraint
  (("dafadddaabaafebaceacdfcabedfcfccdeecbbdf") ++ ("")) =
    (("dafadddaabaafebaceacdfcabedfcfcc") ++ (B) ++ ("ecbbdf"));
constraint
  (("cbddfafaafceeebfbfbefbbeaeffbfedfceac") ++ (B) ++ ("c")) =
    (("cbddfafaafceeebfbfbefbbeaeffbfedfceacdec") ++ (""));
constraint ((str_len (J)) * (17)) >= (238);
constraint ((str_len (J)) * (6)) <= (246);
constraint ((str_len (I)) * (6)) >= (6);
constraint ((str_len (I)) * (18)) <= (2610);
solve satisfy;
