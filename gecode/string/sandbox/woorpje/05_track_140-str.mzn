var string: B;
var string: C;
var string: I;
constraint
  (("e") ++ (C) ++ ("baaecb") ++ (B) ++ ("dcaaaf")) =
    (("ecaec") ++ (I) ++ ("fdcaaaf"));
constraint
  (("cbdcbbdfcecbffdedcaaeecaecfffbdbfdbbbaee") ++ ("")) =
    (("cbdcbbdfcecbffdedcaaee") ++ (C) ++ ("fbdbfdbbbaee"));
constraint
  (("ecebbfadcfbdaefaedaceeaacf") ++ (C) ++ ("abfcbead")) =
    (("ecebbfadcfbdaefaedaceeaacfcaecffabfcbead") ++ (""));
constraint ((str_len (C)) * (13)) >= (26);
constraint ((str_len (C)) * (6)) <= (72);
solve satisfy;
