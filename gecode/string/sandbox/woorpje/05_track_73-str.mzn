var string: H;
var string: C;
var string: G;
constraint
  (("cdfaefabdfa") ++ (C) ++ ("ba")) =
    (("cdfaefabdfabcaafa") ++ (G) ++ ("ba"));
constraint
  (("bcedddafafccaffdbfbbeaadebfc") ++ (H) ++ ("febcab")) =
    (("bcedddafafccaffdbfbbeaadebfcabdfabfebcab") ++ (""));
constraint
  (("dbdafdfbebafbdfcd") ++ (H) ++ ("bfecfebeebeaebdef")) =
    (("dbdafdfbebafbdfcdabdfabbfecfebeebeaebdef") ++ (""));
constraint ((str_len (C)) * (10)) >= (130);
constraint ((str_len (H)) * (5)) >= (5);
constraint ((str_len (G)) * (1)) >= (11);
solve satisfy;
