var string: I;
var string: B;
var string: F;
var string: G;
var string: C;
constraint
  (("daef") ++ (F) ++ ("fee") ++ (I) ++ ("e") ++ (C) ++ ("e")) =
    (("daef") ++ (B) ++ (G) ++ ("e") ++ (C) ++ ("ede"));
constraint
  (("effcfba") ++ (C) ++ ("bcbaa") ++ (B) ++ ("acfeabffabaddcedbdbcdefd")) =
    (("effcfbadbcbaaccfacfeabffabaddcedb") ++ (C) ++ ("bcdefd"));
constraint
  (("cbaecfbbeaeebdbea") ++ (C) ++ ("dbda") ++ (C) ++ ("acba") ++ (C) ++ ("eebffbccffac"))
    =
    (("cbaecfbbeaeebdbea") ++ (C) ++ ("db") ++ (C) ++ ("adacbadeebffb") ++ (B) ++ ("fac"));
constraint ((str_len (B)) * (14)) <= (336);
constraint ((str_len (C)) * (15)) >= (15);
constraint ((str_len (G)) * (20)) >= (340);
constraint ((str_len (G)) * (7)) <= (1344);
constraint ((str_len (I)) * (19)) <= (2413);
constraint ((str_len (F)) * (2)) <= (74);
solve satisfy;
