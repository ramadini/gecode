var string: I;
var string: F;
var string: J;
var string: C;
constraint
  (("ccef") ++ (I) ++ ("cbfafcefaceef")) =
    (("cce") ++ (J) ++ ("ecbecbeeeffa") ++ (F) ++ ("ceef"));
constraint
  (("cccfedbacfbebfacdebafddcceecffacfddc") ++ (C) ++ ("d")) =
    (("cccfedbacfbebfacdebafddcceecffacfddcfaed") ++ (""));
constraint
  (("acdeceacae") ++ (C) ++ ("eabfbcffbdfbcafedddbbedaacf")) =
    (("acdeceacaefaeeabfbcffbdfbcafedddbbedaacf") ++ (""));
constraint ((str_len (J)) * (9)) >= (18);
constraint ((str_len (J)) * (16)) <= (336);
constraint ((str_len (I)) * (20)) >= (200);
constraint ((str_len (I)) * (2)) <= (136);
constraint ((str_len (C)) * (13)) >= (26);
solve satisfy;
