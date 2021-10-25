var string: C;
var string: G;
var string: E;
constraint
  (("aabbh") ++ (C) ++ ("h") ++ (C) ++ ("fcbcd") ++ (C) ++ (E) ++ ("ibbeaeibih"))
    =
    (("aabbh") ++ (C) ++ ("hf") ++ (C) ++ ("cbcdfegeb") ++ (C) ++ ("bbchei") ++ (G) ++ ("bbeaeibih"));
solve satisfy;
