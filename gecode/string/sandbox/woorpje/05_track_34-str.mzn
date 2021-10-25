var string: H;
var string: I;
var string: G;
var string: A;
var string: C;
constraint
  (("bbebfc") ++ (C) ++ ("ccee") ++ (G) ++ (A) ++ ("ead") ++ (A) ++ ("bbd") ++ (A) ++ ("e") ++ (A) ++ ("e") ++ (A) ++ ("eddbe"))
    =
    (("bbebfc") ++ (C) ++ (H) ++ ("e") ++ (A) ++ ("e") ++ (I) ++ ("ae") ++ (A) ++ ("d") ++ (A) ++ ("bbdae") ++ (A) ++ ("eaeddbe"));
constraint
  (("bfecbe") ++ (A) ++ ("cdfbec") ++ (A) ++ ("fdfbdcf") ++ (A) ++ ("cccbb") ++ (A) ++ ("fdceaedbfaec"))
    = (("bfecbeacdfbecafdfbdcf") ++ (A) ++ ("cccbbafdceaedbfaec"));
constraint
  (("ba") ++ (A) ++ ("dfbd") ++ (A) ++ ("fadeffbefdcebd") ++ (A) ++ ("eb") ++ (C) ++ (A) ++ ("ddbedaf") ++ (A) ++ ("deca"))
    =
    (("b") ++ (A) ++ ("adfbd") ++ (A) ++ ("f") ++ (A) ++ ("deffbefdcebd") ++ (A) ++ ("ebb") ++ (A) ++ (A) ++ ("ddbed") ++ (A) ++ ("f") ++ (A) ++ ("deca"));
constraint ((str_len (I)) * (1)) >= (6);
constraint ((str_len (I)) * (1)) <= (35);
constraint ((str_len (H)) * (6)) >= (6);
constraint ((str_len (G)) * (14)) <= (518);
constraint ((str_len (A)) * (5)) >= (5);
constraint ((str_len (C)) * (15)) <= (150);
solve satisfy;
