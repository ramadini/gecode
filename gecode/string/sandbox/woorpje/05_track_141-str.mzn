var string: H;
var string: F;
var string: E;
var string: J;
constraint
  (("ecbfdeeaf") ++ (E) ++ ("cf") ++ (H) ++ ("abacdddbbbcaaaddbbbe")) =
    (("ecbfdee") ++ (F) ++ ("cfe") ++ (J) ++ ("ddbbbcaaaddbbbe"));
constraint
  (("f") ++ (H) ++ ("cdcdcdeadaceadeddcfbaaaeeafaaeafcbaff")) =
    (("fefcdcdcdeadaceadeddcfbaaaeeafaaeafcbaff") ++ (""));
constraint
  (("fcaeeeb") ++ (J) ++ ("aedeecbffcdada") ++ (H) ++ ("abfdccaaddf")) =
    (("fcaeeebfabacdaedeecbffcdadaefabfdccaaddf") ++ (""));
constraint ((str_len (J)) * (19)) >= (95);
constraint ((str_len (E)) * (2)) >= (10);
constraint ((str_len (E)) * (13)) <= (962);
constraint ((str_len (H)) * (17)) <= (34);
solve satisfy;
