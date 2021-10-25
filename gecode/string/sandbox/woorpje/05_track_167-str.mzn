var string: C;
var string: G;
var string: A;
constraint
  (("fafc") ++ (A) ++ ("acbc") ++ (A) ++ ("af") ++ (C) ++ ("aeffefef") ++ (A) ++ ("fccfafcbeb") ++ (A) ++ ("d"))
    =
    (("fafc") ++ (A) ++ ("acbcda") ++ (G) ++ ("daeffefef") ++ (A) ++ ("fccfafcbebdd"));
constraint
  (("cfedaabcc") ++ (A) ++ ("facfce") ++ (A) ++ ("cbbfeacbcafaeaaeee") ++ (A) ++ ("acbf"))
    =
    (("cfedaabccdfacfce") ++ (A) ++ ("cbbfeacbcafaeaaeee") ++ (A) ++ ("acbf"));
constraint
  (("acbaa") ++ (A) ++ ("dcdfcfd") ++ (A) ++ ("fcc") ++ (A) ++ ("ea") ++ (A) ++ ("ffaf") ++ (A) ++ ("cffefeadba") ++ (A) ++ ("f") ++ (A) ++ ("a"))
    =
    (("acbaaddcdfcf") ++ (A) ++ (A) ++ ("fccdea") ++ (A) ++ ("ffaf") ++ (A) ++ ("cffefea") ++ (A) ++ ("ba") ++ (A) ++ ("f") ++ (A) ++ ("a"));
constraint ((str_len (G)) * (1)) >= (4);
constraint ((str_len (C)) * (7)) >= (35);
solve satisfy;
