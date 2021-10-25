var string: I;
var string: H;
var string: B;
var string: A;
var string: D;
constraint
  (("fadaefebfa") ++ (D) ++ ("d") ++ (B) ++ ("fcabcfdacebbbe")) =
    (("fadaefeb") ++ (I) ++ ("cb") ++ (A) ++ ("fabaabfcabcfdacebbbe"));
constraint
  (("dcbdbeedfefbdfeeeebacafaafccf") ++ (H) ++ ("bafadef")) =
    (("dcbdbeedfefbdfeeeebacafaafccf") ++ (H) ++ ("bafadef"));
constraint
  (("bafebdacececa") ++ (I) ++ ("fbdafcafcebaacaebeeacfd")) =
    (("bafebdacececafafdfbdafcafcebaacaebeeacfd") ++ (""));
constraint ((str_len (A)) * (17)) <= (340);
constraint ((str_len (I)) * (14)) >= (14);
constraint ((str_len (I)) * (18)) <= (306);
constraint ((str_len (D)) * (8)) <= (168);
solve satisfy;
