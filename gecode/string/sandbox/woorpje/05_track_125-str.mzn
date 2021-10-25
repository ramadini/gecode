var string: B;
var string: F;
var string: G;
var string: J;
var string: C;
constraint
  (("eabaacfcdeaefcaa") ++ (F) ++ ("ebc")) =
    (("eab") ++ (B) ++ (J) ++ ("ccdaecbebc"));
constraint
  (("dfbfdfee") ++ (F) ++ ("bbccba") ++ (C) ++ ("b")) =
    (("dfbf") ++ (G) ++ (J) ++ ("ffecceb"));
constraint
  (("bfabcdbafbfdcafdbdbafcfaacde") ++ (F) ++ ("eecaeeac")) =
    (("bfabcdbafbfdcafdbdbafcfaacde") ++ (F) ++ ("eecaeeac"));
constraint
  (("bdfaccdadafcfeacbadfcbceaeedebfdfbafabdc") ++ ("")) =
    (("bdfaccdadaf") ++ (F) ++ ("cbadfcbceaeedebfdfbafabdc"));
constraint ((str_len (G)) * (10)) >= (110);
constraint ((str_len (B)) * (15)) <= (3720);
constraint ((str_len (J)) * (7)) >= (112);
constraint ((str_len (F)) * (13)) <= (156);
constraint ((str_len (C)) * (17)) >= (187);
constraint ((str_len (C)) * (20)) <= (3420);
solve satisfy;
