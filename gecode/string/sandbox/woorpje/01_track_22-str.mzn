var string: H;
var string: M;
var string: C;
var string: D;
constraint
  (("abe") ++ (D) ++ (H) ++ ("afbde")) =
    (("ab") ++ (C) ++ (M) ++ ("heecafbde"));
solve satisfy;
