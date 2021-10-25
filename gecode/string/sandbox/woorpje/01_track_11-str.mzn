var string: C;
var string: G;
constraint
  (("aacadcacdabddadbdab") ++ (G)) =
    (("aa") ++ (C) ++ ("abddcaccbccaacaadbccababbacc"));
solve satisfy;
