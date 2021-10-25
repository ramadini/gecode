var string: G;
var string: E;
var string: C;
var string: D;
constraint
  ((D) ++ ("cacd") ++ (G) ++ ("adfdccacfedcabac")) =
    ((D) ++ ("cac") ++ (C) ++ ("fceffe") ++ (D) ++ ("ded") ++ (E) ++ ("adbaadfdccacf") ++ (D) ++ ("dcabac"));
constraint
  (("cbdeaccacfadebdcd") ++ (D) ++ ("bacffbacf") ++ (D) ++ ("ceedbadbfbed"))
    =
    (("cbdeaccacfadebdcdebacffbacf") ++ (D) ++ ("c") ++ (D) ++ (D) ++ ("dbadbfbed"));
constraint
  (("bcadefcbabdafbebdbccbcecbe") ++ (C) ++ ("baefed") ++ (D) ++ ("fdbdb")) =
    (("bcadefcbabdafbebdbccbcecb") ++ (D) ++ (C) ++ ("ba") ++ (D) ++ ("f") ++ (D) ++ ("defdbdb"));
constraint ((str_len (C)) * (20)) >= (20);
constraint ((str_len (C)) * (5)) <= (50);
constraint ((str_len (D)) * (16)) <= (96);
constraint ((str_len (G)) * (2)) >= (20);
constraint ((str_len (E)) * (20)) <= (520);
solve satisfy;
