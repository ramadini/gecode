var string: I;
var string: H;
var string: G;
var string: D;
constraint
  (("abacd") ++ (I) ++ ("cbaacddddcdeda")) =
    (("aba") ++ (D) ++ ("aaa") ++ (H) ++ ("edcbaacddddcdeda"));
constraint
  (("a") ++ (G) ++ ("df") ++ (G) ++ ("dfdcefedcfdcbddffbb") ++ (G) ++ ("dcbfceacceafcde"))
    =
    (("a") ++ (G) ++ ("d") ++ (G) ++ ("fd") ++ (G) ++ ("dcefedcfdcbddf") ++ (G) ++ ("bb") ++ (G) ++ ("dcbfceaccea") ++ (G) ++ ("cde"));
constraint
  (("eacacdcfd") ++ (G) ++ ("cbbacfeebeebcacadbfaad") ++ (G) ++ ("cdcfdbd"))
    =
    (("eacacdc") ++ (G) ++ ("d") ++ (G) ++ ("cbbac") ++ (G) ++ ("eebeebcacadb") ++ (G) ++ ("aadfcdcfdbd"));
constraint ((str_len (D)) * (2)) <= (26);
constraint ((str_len (I)) * (15)) >= (120);
constraint ((str_len (I)) * (10)) <= (1860);
constraint ((str_len (G)) * (17)) >= (17);
constraint ((str_len (G)) * (9)) <= (9);
solve satisfy;
