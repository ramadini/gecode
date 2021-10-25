var string: I;
var string: C;
var string: E;
constraint
  (("dc") ++ (C) ++ ("ffccddafbc")) = (("d") ++ (I) ++ ("dfdafbffccddafbc"));
constraint
  (("fdebffdfebbacdeeeecfbfdcdaefdcc") ++ (E) ++ ("dacfcf")) =
    (("fdebffdfebbacdeeeecfbfdcdaefdccfffdacfcf") ++ (""));
constraint
  (("dfeeafebfaabfecdcecfedbdcdebfaafc") ++ (E) ++ ("afbf")) =
    (("dfeeafebfaabfecdcecfedbdcdebfaafc") ++ (E) ++ ("afbf"));
constraint ((str_len (I)) * (9)) >= (153);
solve satisfy;
