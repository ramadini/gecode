var string: C;
var string: F;
var string: J;
constraint
  ((F) ++ ("acd") ++ (C) ++ ("fcea") ++ (F) ++ ("d") ++ (F) ++ ("bbeeeeeebb"))
    = (("d") ++ (J) ++ ("b"));
constraint
  (("ffabdceadaec") ++ (F) ++ ("fdeb") ++ (F) ++ ("fa") ++ (F) ++ ("ae") ++ (F) ++ ("eb") ++ (F) ++ (F) ++ ("aaef") ++ (F) ++ ("bcdbfac"))
    =
    (("ffab") ++ (F) ++ ("cea") ++ (F) ++ ("aec") ++ (F) ++ ("fdeb") ++ (F) ++ ("fa") ++ (F) ++ ("aedeb") ++ (F) ++ ("daaef") ++ (F) ++ ("bc") ++ (F) ++ ("bfac"));
constraint
  (("eaacebefcbeecaeccecaaeaa") ++ (F) ++ ("beaccaabcadcdfa")) =
    (("eaacebefcbeecaeccecaaeaa") ++ (F) ++ ("beaccaabca") ++ (F) ++ ("cdfa"));
constraint ((str_len (J)) * (8)) >= (64);
constraint ((str_len (J)) * (4)) <= (988);
constraint ((str_len (C)) * (8)) >= (8);
constraint ((str_len (C)) * (11)) <= (2871);
solve satisfy;
