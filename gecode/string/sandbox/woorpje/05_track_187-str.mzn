var string: E;
var string: A;
constraint
  (("f") ++ (E) ++ ("eeba")) =
    (("fbcefedcfebb") ++ (A) ++ ("abafe") ++ (A) ++ ("fdbcaedcffeeffefaeeba"));
constraint
  (("efefecaa") ++ (A) ++ ("abb") ++ (A) ++ ("ebe") ++ (A) ++ ("c") ++ (A) ++ ("aecfaeaefcfedcdedca") ++ (A) ++ ("f"))
    =
    (("efefecaadabbdebedcdaecfaeaefcfedc") ++ (A) ++ ("edca") ++ (A) ++ ("f"));
constraint
  ((A) ++ ("aabccfebcaac") ++ (A) ++ ("ffcf") ++ (A) ++ (A) ++ ("abcbaabbcedcaed") ++ (A) ++ ("affb"))
    =
    (("daabccfebcaac") ++ (A) ++ ("ffcf") ++ (A) ++ (A) ++ ("abcbaabbce") ++ (A) ++ ("cae") ++ (A) ++ (A) ++ ("affb"));
constraint ((str_len (E)) * (13)) >= (234);
constraint ((str_len (A)) * (20)) >= (20);
constraint ((str_len (A)) * (13)) <= (13);
solve satisfy;
