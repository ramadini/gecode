var string: C;
var string: A;
constraint
  (("cddabcbbdbbbaaabbdaaaabadadbabbddabd") ++ (A) ++ (C) ++ ("bacbdcb")) =
    (("cddabcbbdbbbaaabbdaaaabadadbabbddabdcbd") ++ (C) ++ ("ba") ++ (A) ++ ("cb"));
solve satisfy;
