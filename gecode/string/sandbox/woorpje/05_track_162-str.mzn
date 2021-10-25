var string: H;
var string: I;
var string: G;
var string: A;
var string: C;
var string: D;
constraint
  (("fabfdfa") ++ (H) ++ ("ceeae") ++ (I) ++ ("cecd")) =
    (("fabfdfabedf") ++ (G) ++ ("dcdacecd"));
constraint
  (("adbecaabdbe") ++ (I) ++ ("cfbefbfedcaaef")) =
    (("adbecaabdbeeadcaaaa") ++ (D) ++ ("aef"));
constraint
  (("efcbfbe") ++ (H) ++ ("edbbaad") ++ (D)) =
    (("efc") ++ (G) ++ ("dbbaad") ++ (D));
constraint
  (("bedc") ++ (I) ++ (G) ++ ("aaa") ++ (C) ++ ("dfcefeabeacfcc")) =
    (("bedc") ++ (C) ++ ("bdbaea") ++ (A) ++ ("cc"));
constraint
  (("fabaaacadbceeaaadaddca") ++ (C) ++ ("efabaadbbaabef") ++ (C) ++ ("bf"))
    =
    ((C) ++ ("abaaacadbceeaaadaddcafe") ++ (C) ++ ("abaadbbaabe") ++ (C) ++ (C) ++ ("b") ++ (C));
constraint
  (("faf") ++ (C) ++ ("cc") ++ (C) ++ ("fdadcfbbcbbbafadcdbcacc") ++ (C) ++ ("dbadcedfd"))
    =
    ((C) ++ ("affcc") ++ (C) ++ ("fdadc") ++ (C) ++ ("bbcbbba") ++ (C) ++ ("adcdbcaccfdbadced") ++ (C) ++ ("d"));
constraint ((str_len (G)) * (9)) >= (36);
constraint ((str_len (G)) * (7)) <= (189);
constraint ((str_len (H)) * (12)) <= (1128);
constraint ((str_len (C)) * (6)) >= (6);
constraint ((str_len (C)) * (10)) <= (20);
solve satisfy;
