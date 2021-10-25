var string: I;
var string: G;
var string: J;
var string: C;
constraint
  (("ba") ++ (G) ++ ("ceeabd") ++ (G) ++ (G) ++ ("ba") ++ (J) ++ ("bedbaeaabeafed"))
    =
    (("bacc") ++ (C) ++ ("ff") ++ (G) ++ (G) ++ ("da") ++ (I) ++ ("dbaeaabeafed"));
constraint
  (("ebcfddafeaefbdbd") ++ (G) ++ ("eecdffddfafdaefffa") ++ (G) ++ ("bcde"))
    = (("ebcfddafeaefbdbdcee") ++ (G) ++ ("dffddfafdaefffacbcde"));
constraint
  (("cafefeefbdbddbfecccfaebbbeeccddfdaab") ++ (G) ++ ("dfe")) =
    (("cafefeefbdbddbfe") ++ (G) ++ ("ccfaebbbee") ++ (G) ++ ("cddfdaab") ++ (G) ++ ("dfe"));
constraint ((str_len (J)) * (8)) >= (40);
constraint ((str_len (G)) * (12)) >= (12);
constraint ((str_len (G)) * (16)) <= (192);
solve satisfy;
