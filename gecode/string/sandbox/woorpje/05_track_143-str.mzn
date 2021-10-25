var string: H;
var string: F;
var string: E;
var string: C;
constraint
  (("ecdfdcdcee") ++ (F) ++ ("efadcdd")) =
    (("ecdfdcdceeff") ++ (H) ++ ("bbeeeaeefadcdd"));
constraint
  (("aeaaaaeb") ++ (H) ++ ("b") ++ (E) ++ ("caaacfbbbfbdeabf")) =
    (("aeaaaaeb") ++ (H) ++ ("b") ++ (H) ++ ("f") ++ (C) ++ ("ccaaacfbbbfb") ++ (H) ++ ("eabf"));
constraint
  (("bcfdbacf") ++ (H) ++ ("cabc") ++ (H) ++ ("ffcbecadcaecceffcabfaacacd"))
    =
    (("bcfdbacf") ++ (H) ++ ("cabc") ++ (H) ++ ("ffcbecadcaecceffcabfaacac") ++ (H));
constraint
  (("bbaacabaf") ++ (H) ++ ("bbacfafaffbadadeedeebaecbccaca")) =
    (("bbaacabaf") ++ (H) ++ ("bbacfafaffba") ++ (H) ++ ("a") ++ (H) ++ ("ee") ++ (H) ++ ("eebaecbccaca"));
constraint ((str_len (E)) * (8)) >= (48);
constraint ((str_len (E)) * (13)) <= (312);
constraint ((str_len (H)) * (15)) <= (15);
constraint ((str_len (F)) * (2)) >= (18);
solve satisfy;
