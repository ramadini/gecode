var string: E;
var string: J;
constraint
  (("ebdbbbfd") ++ (E) ++ ("cacebbeebbeeeefcdee")) =
    (("ebdbbbfdadebeceaad") ++ (J) ++ ("cebbeebbeeeefcdee"));
constraint
  (("ceffeeebcafbbfbeefabfeeeeaddddeb") ++ (J) ++ ("dfe")) =
    (("ceffeeebcafbbfbeefabfeeeeaddddeb") ++ (J) ++ ("dfe"));
constraint
  (("f") ++ (J) ++ ("dacbfabdafacfddecbaffeaeeffedacacc")) =
    (("fbbecadacbfabdafacfddecbaffeaeeffedacacc") ++ (""));
constraint ((str_len (J)) * (9)) >= (18);
solve satisfy;
