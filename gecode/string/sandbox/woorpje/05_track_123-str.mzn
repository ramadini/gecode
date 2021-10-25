var string: E;
var string: J;
constraint
  (("dfdbfe") ++ (J) ++ ("eabdbdeadcfb") ++ (E) ++ ("aafbbffdde") ++ (J) ++ ("dadb") ++ (J) ++ ("d"))
    =
    (("dfdbfe") ++ (J) ++ ("eabdbdead") ++ (J) ++ ("fbe") ++ (J) ++ ("bcaafbbffddecdadbcd"));
constraint
  (("ddefbee") ++ (J) ++ (J) ++ ("acdeb") ++ (J) ++ ("ada") ++ (J) ++ (E) ++ ("ea") ++ (J) ++ ("adadfdfdbfada") ++ (J))
    =
    (("ddefbee") ++ (J) ++ (J) ++ ("acdebcadace") ++ (J) ++ ("bcea") ++ (J) ++ ("adadfdfdbfada") ++ (J));
constraint
  (("cacebfdcedbcce") ++ (J) ++ ("bbabc") ++ (J) ++ ("eebabbbfaaceaebfade"))
    =
    ((J) ++ ("acebfd") ++ (J) ++ ("edbc") ++ (J) ++ ("ecbbabcceebabbbfaa") ++ (J) ++ ("eaebfade"));
constraint ((str_len (J)) * (1)) >= (1);
constraint ((str_len (J)) * (20)) <= (20);
constraint ((str_len (E)) * (7)) >= (21);
solve satisfy;
