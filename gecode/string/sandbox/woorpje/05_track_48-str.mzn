var string: H;
var string: F;
var string: A;
constraint
  (("bedeedc") ++ (A) ++ ("fccfadcafaac") ++ (A) ++ ("abcfceadebdbc") ++ (A) ++ (A) ++ ("bdba"))
    =
    (("bedee") ++ (A) ++ ("cdfccfadc") ++ (H) ++ ("debdbc") ++ (A) ++ (A) ++ ("b") ++ (A) ++ ("ba"));
constraint
  (("fefeb") ++ (A) ++ (A) ++ ("fedfcf") ++ (A) ++ ("dc") ++ (A) ++ ("afdeafdbebebcefcabaeaaa"))
    =
    (("fefeb") ++ (A) ++ ("dfe") ++ (A) ++ ("fcf") ++ (A) ++ (A) ++ (F) ++ ("fdeaf") ++ (A) ++ ("bebebcefcabaeaaa"));
constraint
  (("bcaeaab") ++ (A) ++ ("da") ++ (A) ++ ("abd") ++ (A) ++ ("e") ++ (A) ++ ("caeeeefcecfadacbceaebbb"))
    =
    (("bcaeaab") ++ (A) ++ ("da") ++ (A) ++ ("abd") ++ (A) ++ ("e") ++ (A) ++ ("caeeeefcecfadacbceaebbb"));
constraint ((str_len (H)) * (7)) >= (63);
constraint ((str_len (H)) * (12)) <= (228);
solve satisfy;
