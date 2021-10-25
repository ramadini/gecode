var string: G;
var string: A;
constraint
  (("ccffeffbcebbcfc") ++ (A) ++ (A) ++ ("cfc") ++ (G) ++ ("aefe")) =
    (("ccffeffbcebbcfc") ++ (A) ++ (A) ++ ("cfc") ++ (A) ++ ("fcaceb") ++ (A) ++ ("b") ++ (A) ++ ("eafce") ++ (A) ++ ("aefe"));
constraint
  (("ffbec") ++ (A) ++ ("dfcbfbbdeeccddeebfaec") ++ (A) ++ ("bafcc") ++ (A) ++ ("afbfda"))
    =
    (("ffbec") ++ (A) ++ (A) ++ ("fcbfbb") ++ (A) ++ ("eecc") ++ (A) ++ (A) ++ ("eebfaecdbafccdafbfda"));
constraint
  (("faa") ++ (A) ++ ("adc") ++ (A) ++ ("ddcecebfafba") ++ (A) ++ ("aafeedffccbccacfbea"))
    =
    (("faadadc") ++ (A) ++ (A) ++ (A) ++ ("cecebfafba") ++ (A) ++ ("aafee") ++ (A) ++ ("ffccbccacfbea"));
constraint ((str_len (A)) * (12)) >= (12);
constraint ((str_len (A)) * (12)) <= (84);
constraint ((str_len (G)) * (7)) >= (105);
constraint ((str_len (G)) * (8)) <= (1512);
solve satisfy;
