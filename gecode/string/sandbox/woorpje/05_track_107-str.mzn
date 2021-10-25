var string: I;
var string: H;
var string: F;
var string: A;
var string: J;
var string: C;
constraint
  (("d") ++ (C) ++ (J) ++ ("db") ++ (I) ++ ("fdfb") ++ (H) ++ ("afdf") ++ (I) ++ ("bacfadbccf"))
    = (("d") ++ (I) ++ (I) ++ (A) ++ ("dbccf"));
constraint
  (("feca") ++ (I) ++ ("ccdc") ++ (J) ++ ("fadffeecaeacfecd") ++ (I) ++ ("e") ++ (I) ++ ("c") ++ (I) ++ ("bfc") ++ (I) ++ (I) ++ ("edc"))
    =
    (("fecabccdc") ++ (J) ++ ("fadffeecaeacfecdbe") ++ (I) ++ ("c") ++ (I) ++ ("bfcb") ++ (I) ++ ("edc"));
constraint
  (("bfddead") ++ (I) ++ ("eafc") ++ (F) ++ ("dbddfffeaefe") ++ (I) ++ ("afdfaabfecce"))
    =
    ((I) ++ ("fddead") ++ (I) ++ ("eafced") ++ (I) ++ ("d") ++ (I) ++ ("ddfffeaefe") ++ (I) ++ ("afdfaabfecce"));
constraint ((str_len (I)) * (1)) >= (1);
constraint ((str_len (I)) * (20)) <= (340);
constraint ((str_len (C)) * (13)) <= (221);
constraint ((str_len (J)) * (18)) >= (36);
solve satisfy;
