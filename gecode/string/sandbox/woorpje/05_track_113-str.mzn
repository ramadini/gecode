var string: G;
var string: J;
var string: C;
var string: D;
constraint
  (("deaedccdfbeab") ++ (J) ++ ("cbebdedc")) =
    ((D) ++ ("c") ++ (C) ++ ("fafcbebdedc"));
constraint
  (("eafdbfaafdadaabfeedacfafffdfebabe") ++ (G) ++ ("cfdd")) =
    (("eafdbfaafdadaabfeedacfafffdfebabefbfcfdd") ++ (""));
constraint
  (("eaefbffdbacdeaeedefdaeecafdbbecfabafbbda") ++ ("")) =
    (("eae") ++ (G) ++ ("fdbacdeaeedefdaeecafdbbecfabafbbda"));
constraint ((str_len (C)) * (10)) >= (70);
constraint ((str_len (G)) * (1)) <= (21);
constraint ((str_len (D)) * (13)) <= (1846);
constraint ((str_len (J)) * (15)) >= (120);
constraint ((str_len (J)) * (3)) <= (129);
solve satisfy;
