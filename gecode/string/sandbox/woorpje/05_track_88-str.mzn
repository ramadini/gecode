var string: G;
var string: A;
var string: J;
constraint
  (("c") ++ (J) ++ ("ecbbeae")) =
    (("cec") ++ (G) ++ ("e") ++ (A) ++ (G) ++ ("b") ++ (G) ++ (G) ++ ("bdecbbeae"));
constraint
  (("ebabbcacaade") ++ (G) ++ ("dadbdebceeacba") ++ (G) ++ ("cbbbdfabbfbc"))
    = (("ebabbcacaadefdadbdebceeacbafcbbbd") ++ (G) ++ ("abbfbc"));
constraint
  (("ecead") ++ (G) ++ ("d") ++ (G) ++ ("cbecd") ++ (G) ++ ("eaea") ++ (G) ++ ("e") ++ (G) ++ ("cdddcbcacdbcdfccafc"))
    =
    (("ecead") ++ (G) ++ ("d") ++ (G) ++ ("cbecdfeaeafefcdddcbcacdbcd") ++ (G) ++ ("ccafc"));
constraint ((str_len (A)) * (4)) <= (368);
solve satisfy;
