var string: B;
var string: F;
var string: E;
var string: D;
constraint
  (("bfabfb") ++ (D) ++ ("ddfca") ++ (B) ++ ("dd") ++ (F) ++ ("feecb") ++ (F) ++ ("afcbfeca") ++ (F))
    =
    (("bfabfbedbc") ++ (F) ++ (F) ++ ("fcaa") ++ (F) ++ ("e") ++ (E) ++ (F) ++ ("feecb") ++ (F) ++ ("afcbfeca") ++ (F));
constraint
  (("eeaaafbccdaafbfffeeaac") ++ (F) ++ ("fbbaffbf") ++ (F) ++ ("cdffce") ++ (F) ++ ("e"))
    =
    (("eeaaafbcc") ++ (F) ++ ("aafbfffeeaacdfbbaffbf") ++ (F) ++ ("c") ++ (F) ++ ("ffcede"));
constraint
  (("faaebab") ++ (F) ++ ("cafdc") ++ (F) ++ ("feacfeeacbcaafaecfeecbced") ++ (F))
    =
    (("faaebabdcaf") ++ (F) ++ ("c") ++ (F) ++ ("feacfeeacbcaafaecfeecbced") ++ (F));
constraint ((str_len (D)) * (19)) >= (57);
constraint ((str_len (D)) * (11)) <= (154);
constraint ((str_len (E)) * (20)) >= (120);
constraint ((str_len (F)) * (4)) <= (40);
constraint ((str_len (B)) * (14)) >= (56);
constraint ((str_len (B)) * (6)) <= (276);
solve satisfy;
