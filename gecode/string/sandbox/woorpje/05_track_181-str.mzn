var string: C;
var string: D;
var string: A;
constraint
  (("fdca") ++ (C) ++ ("cea") ++ (A) ++ ("deebeeefebcdacacbedadfc")) =
    (("fdca") ++ (C) ++ ("c") ++ (D) ++ ("ebcdacacbedadfc"));
constraint
  (("feedeaebabfecedbcdbbddebab") ++ (A) ++ ("bfcaefcfdb")) =
    (("feedeaebabfecedbcdbbddebabddaebfcaefcfdb") ++ (""));
constraint
  (("eafdadadbceadf") ++ (A) ++ ("eeedffeefffbedfaabadba")) =
    (("eafdadadbceadf") ++ (A) ++ ("eeedffeefffbedfaabadba"));
constraint ((str_len (A)) * (13)) <= (468);
constraint ((str_len (C)) * (18)) <= (1188);
solve satisfy;
