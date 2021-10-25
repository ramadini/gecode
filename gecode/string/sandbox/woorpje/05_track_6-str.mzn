var string: H;
var string: E;
var string: A;
constraint
  (("aba") ++ (A) ++ ("abb") ++ (E) ++ ("afbec")) =
    (("aba") ++ (A) ++ ("abbe") ++ (H) ++ ("dedabcdeeeafbec"));
constraint
  ((H) ++ ("dfacacbaafafdcdeafbfddefcbedccbbfeb") ++ ("")) =
    (("bcaafdfacacbaafafdcdeafbfddefcbedccbbfeb") ++ (""));
constraint
  (("bbdbbfdefdccedbddfaeafbfcaa") ++ (H) ++ ("fddbddab")) =
    (("bbdbbfdefdccedbddfaeafbfcaabcaaffddbddab") ++ (""));
constraint ((str_len (A)) * (20)) >= (100);
constraint ((str_len (E)) * (20)) >= (160);
constraint ((str_len (E)) * (9)) <= (882);
constraint ((str_len (H)) * (1)) >= (4);
constraint ((str_len (H)) * (7)) <= (161);
solve satisfy;
