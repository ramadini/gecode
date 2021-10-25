var string: H;
var string: I;
var string: E;
var string: A;
var string: J;
constraint
  (("a") ++ (J) ++ ("bacfcdbfda") ++ (H) ++ ("e") ++ (J) ++ ("fd") ++ (E)) =
    (("afb") ++ (A) ++ ("fcdbfd") ++ (A) ++ ("aedc") ++ (I) ++ (A) ++ ("ffeedecee") ++ (J) ++ (J) ++ ("da") ++ (J));
constraint
  (("ebebaadaebded") ++ (A) ++ ("ffbaaaddedaf") ++ (J) ++ ("eadf") ++ (J) ++ (J) ++ ("dcdbbd"))
    =
    (("ebebaadaebded") ++ (A) ++ (J) ++ ("fbaaadded") ++ (E) ++ (J) ++ ("eadf") ++ (J) ++ (J) ++ ("dcdbbd"));
constraint
  ((A) ++ ("bdddedffcd") ++ (J) ++ ("eda") ++ (J) ++ ("bbdcaddbcdbdbeefb") ++ (J) ++ ("c") ++ (E) ++ (E))
    =
    ((A) ++ ("bdddedf") ++ (J) ++ ("cd") ++ (J) ++ ("eda") ++ (J) ++ ("bbdcaddbcdbdbeefbfcaf") ++ (E));
constraint ((str_len (I)) * (20)) >= (20);
constraint ((str_len (I)) * (16)) <= (1328);
constraint ((str_len (J)) * (11)) >= (11);
constraint ((str_len (J)) * (16)) <= (80);
constraint ((str_len (E)) * (16)) <= (96);
solve satisfy;
