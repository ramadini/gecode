var string: I;
var string: H;
var string: B;
var string: A;
var string: J;
constraint
  ((B) ++ ("bc") ++ (I) ++ ("a") ++ (A) ++ ("b") ++ (J) ++ ("addbeadbcbfdaeddbe"))
    = (("bcacbccaafdfab") ++ (H) ++ ("adbcbfdaeddbe"));
constraint
  (("efabaeccfcbdcbcdbaacba") ++ (J) ++ ("cabdcccbbdbeebba")) =
    (("efabaeccfcbdcbcdbaacba") ++ (J) ++ (I) ++ ("bdcccbbdbeebba"));
constraint
  (("ecdddebeeddedcedcdfbeceeaecdeaceacecd") ++ (I) ++ ("e")) =
    (("ecdddebeeddedcedcdfbeceeaecdeaceacecd") ++ (I) ++ ("e"));
constraint ((str_len (B)) * (11)) <= (671);
constraint ((str_len (A)) * (14)) <= (140);
constraint ((str_len (I)) * (3)) >= (6);
constraint ((str_len (I)) * (7)) <= (175);
solve satisfy;
