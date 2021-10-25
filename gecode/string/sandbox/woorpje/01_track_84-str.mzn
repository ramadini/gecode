var string: C;
var string: E;
constraint
  (("caccabbcc") ++ (E) ++ ("abbbcbcababbcbbbbbcabcbaa")) =
    (("caccabbccabacbbc") ++ (C) ++ ("bcbbbbbcabcbaa"));
solve satisfy;
