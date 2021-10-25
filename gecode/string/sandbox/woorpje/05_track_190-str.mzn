var string: I;
var string: G;
var string: J;
constraint
  (("ffee") ++ (J) ++ ("cfb") ++ (J) ++ ("d") ++ (I) ++ ("dd")) =
    (("ffeedcfb") ++ (J) ++ (J) ++ ("cdcdc") ++ (J) ++ (G));
constraint
  (("e") ++ (J) ++ ("ade") ++ (J) ++ ("bd") ++ (J) ++ (J) ++ ("bfcaebdeecbab") ++ (J) ++ ("ceacffeda") ++ (J) ++ ("cfeeee"))
    =
    (("eda") ++ (J) ++ ("edbdd") ++ (J) ++ ("bfcaebdeecbab") ++ (J) ++ ("ceacffedadcfeeee"));
constraint
  (("feeecedfeafea") ++ (J) ++ ("eeabfb") ++ (J) ++ ("aedcaaaeba") ++ (J) ++ ("cbc") ++ (J) ++ ("afaa"))
    =
    (("feeece") ++ (J) ++ ("feafea") ++ (J) ++ ("eeabfb") ++ (J) ++ ("ae") ++ (J) ++ ("caaaeba") ++ (J) ++ ("cbc") ++ (J) ++ ("afaa"));
constraint ((str_len (I)) * (9)) >= (162);
constraint ((str_len (J)) * (19)) >= (19);
constraint ((str_len (J)) * (11)) <= (33);
solve satisfy;
