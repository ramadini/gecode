var string: I;
var string: B;
var string: A;
var string: C;
constraint
  (("feacebbddffac") ++ (C) ++ ("fcafbadf")) =
    (("feacebbddffacebb") ++ (A) ++ ("afbadf"));
constraint
  (("c") ++ (C) ++ ("dbeee") ++ (C) ++ ("eaaebc") ++ (C) ++ ("ca") ++ (C) ++ ("cb") ++ (A) ++ ("effabdaebeafaf"))
    =
    (("cbd") ++ (C) ++ ("ee") ++ (B) ++ ("aaebc") ++ (C) ++ ("cabc") ++ (C) ++ (I) ++ ("afaf"));
constraint
  (("ecabedccefbfedfefc") ++ (C) ++ ("bdeceeddd") ++ (C) ++ ("cdbdeeda") ++ (C) ++ ("a") ++ (C))
    =
    (("ecabedccef") ++ (C) ++ ("fedfefcb") ++ (C) ++ ("deceeddd") ++ (C) ++ ("cd") ++ (C) ++ ("deeda") ++ (C) ++ ("ab"));
constraint
  (("dddedaedd") ++ (C) ++ ("cadff") ++ (C) ++ ("aadefea") ++ (C) ++ ("eeacd") ++ (C) ++ ("edbdf") ++ (C) ++ (C) ++ ("eff"))
    =
    (("dddedaedd") ++ (C) ++ ("cadffbaadefeabeeacd") ++ (C) ++ ("ed") ++ (C) ++ ("df") ++ (C) ++ (C) ++ ("eff"));
constraint ((str_len (C)) * (6)) <= (108);
constraint ((str_len (B)) * (14)) >= (42);
constraint ((str_len (I)) * (3)) >= (9);
constraint ((str_len (I)) * (11)) <= (2948);
solve satisfy;
