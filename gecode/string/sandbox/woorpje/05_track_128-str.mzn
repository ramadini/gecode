var string: I;
var string: A;
var string: C;
constraint
  (("eadbeafabbfc") ++ (A) ++ ("fefbcd") ++ (A) ++ ("dedadfacdceeaedeeace"))
    = (("eadbeaf") ++ (C) ++ ("cd") ++ (I) ++ ("eeaedeea") ++ (I) ++ ("e"));
constraint
  (("bdebfefafedfdd") ++ (A) ++ ("dda") ++ (I) ++ ("bfbb") ++ (A) ++ ("bdadfedffdbbbe") ++ (I) ++ ("d"))
    =
    (("bdebfefafedfddcddacbfbb") ++ (A) ++ ("bdadfedffdbbbe") ++ (I) ++ ("d"));
constraint
  (("cf") ++ (I) ++ ("aedaaebaba") ++ (A) ++ ("dbfedbdaeaadddfffbefaafdfa"))
    =
    ((I) ++ ("f") ++ (A) ++ ("aedaaebaba") ++ (I) ++ ("dbfedbdaeaadddfffbefaafdfa"));
constraint ((str_len (A)) * (14)) <= (70);
constraint ((str_len (C)) * (18)) <= (1206);
solve satisfy;
