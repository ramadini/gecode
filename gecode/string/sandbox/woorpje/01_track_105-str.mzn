var string: I;
var string: G;
var string: J;
constraint
  (("c") ++ (J) ++ ("cbgagcdddc") ++ (G) ++ ("bgebe") ++ (I) ++ ("cdgdafegbccedfaeefa"))
    =
    (("c") ++ (J) ++ ("cbgagc") ++ (I) ++ ("dc") ++ (G) ++ ("bgebe") ++ (I) ++ ("cdgdafegbccedfaeefa"));
solve satisfy;
