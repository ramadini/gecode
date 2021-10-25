var string: D;
var string: A;
var string: J;
constraint
  (("bceababfaceafb") ++ (A) ++ ("efdafbcf")) =
    (("bceabab") ++ (D) ++ ("feebdbacfacefdafbcf"));
constraint
  (("ec") ++ (J) ++ ("cccaffeeeeeaeaccaefbebaafbcdeeabfced")) =
    (("ecadcccaffeeeeeaeaccaefbebaafbcdeeabfced") ++ (""));
constraint
  (("fcacadbafecaccfaeafcadabeceefcedbaeadbee") ++ ("")) =
    (("fcacadbafecaccfaeafc") ++ (J) ++ ("abeceefcedbaeadbee"));
constraint ((str_len (J)) * (17)) >= (34);
constraint ((str_len (A)) * (2)) >= (12);
constraint ((str_len (D)) * (16)) >= (176);
constraint ((str_len (D)) * (10)) <= (370);
solve satisfy;
