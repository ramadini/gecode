var string: I;
var string: H;
constraint
  (("aafdcfb") ++ (I) ++ ("e") ++ (I) ++ ("bb") ++ (I) ++ ("caddfcafceacefd") ++ (I) ++ ("aa") ++ (I) ++ ("eaddedbf"))
    =
    (("aafdcf") ++ (H) ++ ("ceacefd") ++ (I) ++ ("aa") ++ (I) ++ ("eadded") ++ (I) ++ ("f"));
constraint
  (("febdbceadbbdfeadafabfffeccceafaedaa") ++ (I) ++ ("eaee")) =
    (("fe") ++ (I) ++ ("d") ++ (I) ++ ("ceadb") ++ (I) ++ ("dfeadafa") ++ (I) ++ ("fffeccceafaedaa") ++ (I) ++ ("eaee"));
constraint
  (("daedefc") ++ (I) ++ ("afcbd") ++ (I) ++ ("fcdfcdfddfffaefb") ++ (I) ++ ("decbf") ++ (I) ++ ("dbe"))
    =
    (("daedefc") ++ (I) ++ ("afcbdbfcdfcdfddfffaefb") ++ (I) ++ ("dec") ++ (I) ++ ("f") ++ (I) ++ ("dbe"));
constraint ((str_len (H)) * (19)) <= (1197);
solve satisfy;
