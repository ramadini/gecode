var string: H;
var string: I;
var string: G;
var string: E;
var string: C;
constraint
  (("cffdcef") ++ (G) ++ ("acc") ++ (C) ++ ("eca") ++ (E) ++ (H) ++ ("aabecaee"))
    =
    (("cffdce") ++ (I) ++ ("d") ++ (C) ++ ("cc") ++ (C) ++ ("ec") ++ (C) ++ (E) ++ (H) ++ (C) ++ ("abecaee"));
constraint
  (("fdaed") ++ (C) ++ ("fc") ++ (C) ++ ("ebbdb") ++ (C) ++ ("aacccaefddf") ++ (C) ++ ("ae") ++ (C) ++ ("ebebaffddc"))
    =
    (("fdaedafcaebbdb") ++ (C) ++ ("a") ++ (C) ++ ("ccc") ++ (C) ++ ("efddf") ++ (C) ++ ("aeaebeb") ++ (C) ++ ("ffddc"));
constraint
  (("ede") ++ (C) ++ ("cfcbfbdabddccccc") ++ (C) ++ (C) ++ ("a") ++ (C) ++ ("ebcbdefc") ++ (C) ++ ("dfe") ++ (C) ++ ("eae"))
    =
    (("edeacfcbfbd") ++ (C) ++ ("bddccccca") ++ (C) ++ ("aaebcbdefcadfe") ++ (C) ++ ("e") ++ (C) ++ ("e"));
constraint ((str_len (I)) * (16)) >= (16);
constraint ((str_len (E)) * (13)) <= (65);
solve satisfy;
