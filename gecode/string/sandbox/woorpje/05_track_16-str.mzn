var string: I;
var string: E;
var string: A;
constraint
  (("aecafdb") ++ (E) ++ (I) ++ (A) ++ ("deffaded") ++ (E) ++ ("acece")) =
    (("aecafdbbefbec") ++ (A) ++ ("deffaded") ++ (E) ++ ("acece"));
constraint
  (("bdafbbcd") ++ (E) ++ (E) ++ ("ffcffffcdc") ++ (E) ++ ("dcacdadabdd") ++ (E) ++ ("acfaedc"))
    =
    (("bdaf") ++ (E) ++ (E) ++ ("cd") ++ (E) ++ ("bffcffffcdc") ++ (E) ++ ("dcacdadabddbacfaedc"));
constraint
  (("dccebddcedadbabf") ++ (E) ++ ("eff") ++ (E) ++ ("eecead") ++ (E) ++ ("fcefdeada") ++ (E) ++ ("eb"))
    =
    (("dcce") ++ (E) ++ ("ddcedad") ++ (E) ++ ("abfbeff") ++ (E) ++ ("eecead") ++ (E) ++ ("fcefdeadabe") ++ (E));
constraint ((str_len (E)) * (7)) >= (7);
constraint ((str_len (E)) * (5)) <= (25);
constraint ((str_len (A)) * (3)) >= (12);
constraint ((str_len (A)) * (6)) <= (606);
constraint ((str_len (I)) * (9)) >= (45);
constraint ((str_len (I)) * (2)) <= (28);
solve satisfy;
