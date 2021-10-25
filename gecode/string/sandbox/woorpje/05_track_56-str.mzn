var string: A;
var string: J;
constraint
  (("bfe") ++ (J) ++ ("c") ++ (J) ++ ("caedcfde") ++ (J) ++ (J) ++ (J) ++ ("d") ++ (J) ++ ("adcdcad") ++ (J) ++ ("e") ++ (J) ++ ("ad") ++ (J) ++ ("ad") ++ (J) ++ ("dcdbd"))
    =
    (("bfe") ++ (J) ++ ("c") ++ (J) ++ ("cae") ++ (A) ++ ("adcdcadfefad") ++ (J) ++ ("adfdcdbd"));
constraint
  ((J) ++ ("eadaabedcecdafdeeedaeccdb") ++ (J) ++ ("fddaada") ++ (J) ++ ("df") ++ (J) ++ ("ef"))
    =
    ((J) ++ ("eadaabedcecdafdeeedaeccdb") ++ (J) ++ (J) ++ ("ddaada") ++ (J) ++ ("dffe") ++ (J));
constraint
  (("ceddbbebcd") ++ (J) ++ (J) ++ ("bccfcde") ++ (J) ++ ("e") ++ (J) ++ ("cadaaeccddbca") ++ (J) ++ ("aade"))
    =
    (("ceddbbebcd") ++ (J) ++ (J) ++ ("bccfcde") ++ (J) ++ ("e") ++ (J) ++ ("cadaaeccddbcafaade"));
constraint ((str_len (J)) * (18)) <= (18);
solve satisfy;
