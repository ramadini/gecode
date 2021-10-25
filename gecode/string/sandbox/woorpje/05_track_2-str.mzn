var string: H;
var string: G;
var string: J;
constraint
  (("aacfb") ++ (G) ++ ("abdeddaaa")) =
    (("aacfbdffebaaaaac") ++ (H) ++ ("aaa"));
constraint
  (("efbaeecedaaecfceffaffaedfcebcf") ++ (J) ++ ("aeaadcbe")) =
    (("e") ++ (J) ++ ("aeecedaaecfceffaffaedfcebcf") ++ (J) ++ ("aeaadcbe"));
constraint
  (("bdceafbededddcfcacffdeaefcfa") ++ (J) ++ ("dbabcdebee")) =
    (("bdceafbededddcfcacffdeaefcfa") ++ (J) ++ ("dbabcdebee"));
constraint ((str_len (J)) * (3)) >= (6);
constraint ((str_len (H)) * (5)) <= (255);
solve satisfy;
