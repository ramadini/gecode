var string: H;
var string: D;
var string: J;
constraint
  (("baeffcbea") ++ (J) ++ ("ea") ++ (D) ++ ("aecdcccc")) =
    (("baeffcb") ++ (J) ++ ("a") ++ (H) ++ ("fa") ++ (J) ++ ("cdcccc"));
constraint
  (("aaabafbba") ++ (J) ++ ("ccb") ++ (J) ++ (J) ++ ("cedffaabddbddafcbefc") ++ (J) ++ ("ab") ++ (J) ++ (J))
    =
    (("aaabafbba") ++ (J) ++ ("ccb") ++ (J) ++ (J) ++ ("cedffaabddbddafcbefc") ++ (J) ++ ("ab") ++ (J) ++ (J));
constraint
  ((J) ++ ("cdbadedaadcdcacafcffd") ++ (J) ++ ("b") ++ (J) ++ ("aaa") ++ (J) ++ ("aafadadcdbe"))
    =
    ((J) ++ ("cdbad") ++ (J) ++ ("daadcdcacafcffd") ++ (J) ++ ("beaaaeaafadadcdbe"));
constraint ((str_len (D)) * (6)) >= (84);
constraint ((str_len (D)) * (12)) <= (2088);
constraint ((str_len (H)) * (13)) >= (260);
constraint ((str_len (H)) * (18)) <= (2970);
solve satisfy;
