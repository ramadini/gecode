var string: H;
var string: I;
var string: B;
var string: A;
var string: C;
var string: D;
constraint
  (("ebd") ++ (B) ++ ("ecaa") ++ (H) ++ (C) ++ (I) ++ ("ccdfabcf")) =
    (("e") ++ (A) ++ ("caa") ++ (H) ++ (C) ++ (D));
constraint
  (("cbb") ++ (B) ++ ("caadbedbdbcffaeedeebadecbfddabbcc")) =
    (("cbbeffecaadbedbdbcffaeedeebadecbfddabbcc") ++ (""));
constraint
  (("eebffbabbecbbdbaaedcceddcdcfdcefcbefdece") ++ ("")) =
    (("eebffbabbecbbdbaaedcceddcdcfdcefcbe") ++ (C) ++ ("ce"));
constraint ((str_len (D)) * (5)) <= (820);
constraint ((str_len (C)) * (1)) >= (3);
constraint ((str_len (C)) * (17)) <= (85);
constraint ((str_len (I)) * (6)) >= (36);
solve satisfy;
