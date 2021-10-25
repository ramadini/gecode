var string: D;
var string: A;
constraint
  ((D) ++ ("abb") ++ (A) ++ ("ccbbcbcb")) =
    ((D) ++ ("abb") ++ (D) ++ ("acabababbccbaacc") ++ (D) ++ ("abcaaab") ++ (D) ++ ("cabcbbcbcbbaa") ++ (D) ++ ("cbbcbcb"));
solve satisfy;
