var string: D;
var string: F;
var string: J;
constraint
  (("afdcbdecffdcedafc") ++ (D) ++ (F) ++ ("fda") ++ (D) ++ ("deaab") ++ (D) ++ (D) ++ ("aadcddd"))
    =
    (("afdcbd") ++ (D) ++ ("cffdc") ++ (J) ++ ("ed") ++ (D) ++ ("aabe") ++ (D) ++ ("aadcddd"));
constraint
  ((D) ++ ("aaadc") ++ (D) ++ ("fecbbceebbcaedceecabeabbdc") ++ (D) ++ ("cdaafd"))
    =
    (("eaaadc") ++ (D) ++ ("f") ++ (D) ++ ("cbbc") ++ (D) ++ ("ebbca") ++ (D) ++ ("dc") ++ (D) ++ ("ecabeabbdc") ++ (D) ++ ("cdaafd"));
constraint
  ((D) ++ ("debcfbaadcdddc") ++ (D) ++ ("ebfacacca") ++ (D) ++ ("dcdcacccdcebdc"))
    =
    ((D) ++ ("debcfbaadcdddc") ++ (D) ++ (D) ++ ("bfacaccaedcdcacccdc") ++ (D) ++ ("bdc"));
constraint ((str_len (F)) * (16)) >= (16);
constraint ((str_len (D)) * (9)) <= (72);
constraint ((str_len (J)) * (6)) >= (24);
solve satisfy;
