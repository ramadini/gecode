var string: G;
var string: E;
var string: A;
constraint
  (("cdbfaffbe") ++ (E) ++ ("bceae")) =
    (("cdbf") ++ (A) ++ ("ffbefcccbec") ++ (A) ++ ("bf") ++ (G) ++ ("daebce") ++ (A) ++ ("e"));
constraint
  (("af") ++ (A) ++ ("eceeebbdccd") ++ (A) ++ ("fcdfffcdeeeedccdd") ++ (A) ++ ("bfbbdcb"))
    = ((A) ++ ("faeceeebbdccdafcdfffcdeeeedccddabfbbdcb") ++ (""));
constraint
  ((A) ++ ("cdeccdfbbcacafac") ++ (A) ++ ("fefcf") ++ (A) ++ ("bbeabbebcbcf") ++ (A) ++ ("ccc"))
    =
    (("acdeccdfbbc") ++ (A) ++ ("caf") ++ (A) ++ ("cafefcf") ++ (A) ++ ("bbeabbebcbcf") ++ (A) ++ ("ccc"));
constraint ((str_len (G)) * (12)) >= (132);
constraint ((str_len (G)) * (7)) <= (322);
constraint ((str_len (E)) * (1)) >= (21);
solve satisfy;
