var string: I;
var string: B;
var string: F;
var string: E;
var string: C;
constraint
  (("aebeecd") ++ (F) ++ ("def") ++ (I) ++ ("de")) =
    (("aebeecd") ++ (F) ++ ("defcdad") ++ (C) ++ ("cfcaadfacab") ++ (C) ++ ("cd") ++ (C) ++ ("de"));
constraint
  (("eae") ++ (E) ++ ("ebbffefdbefcfeabdbdcbdbbdafcfebacf")) =
    ((C) ++ ("e") ++ (E) ++ ("ebbffefdbefcf") ++ (C) ++ ("bdbdcbdbbdafcfebacf"));
constraint
  (("cdaa") ++ (B) ++ ("ddcc") ++ (C) ++ ("eedfeeadaceeeedcdbeffdbcbc")) =
    (("cdaadadeddcceaeedfeeadaceeeedcdbeffdbcbc") ++ (""));
constraint ((str_len (I)) * (10)) >= (180);
constraint ((str_len (I)) * (4)) <= (1252);
constraint ((str_len (F)) * (6)) >= (30);
constraint ((str_len (E)) * (7)) >= (21);
constraint ((str_len (E)) * (20)) <= (240);
solve satisfy;
