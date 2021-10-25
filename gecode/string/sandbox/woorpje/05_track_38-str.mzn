var string: H;
var string: B;
var string: F;
var string: J;
var string: C;
constraint
  (("bcadeacfffdfbeefd") ++ (J) ++ ("dfdbabcadda")) =
    (("bcadeacfffdf") ++ (H) ++ ("fdbabcadda"));
constraint
  (("cccdbeafcaecbadff") ++ (B) ++ ("cceeebfafbdfdceba")) =
    (("cccdbea") ++ (C) ++ ("a") ++ (F) ++ ("ebfafbdfdceba"));
constraint
  (("fafcfefcdcecafffdebcfdcabedbdebadbccfebf") ++ ("")) =
    (("fafcfefcdce") ++ (J) ++ ("debcfdcabedbdebadbccfebf"));
constraint
  (("cafffffebcacfcafcfbfbcfefcabcffaadcebcbe") ++ ("")) =
    ((J) ++ ("ffebcacfcafcfbfbcfefcabcffaadcebcbe") ++ (""));
constraint ((str_len (H)) * (11)) <= (1705);
constraint ((str_len (C)) * (17)) <= (2057);
constraint ((str_len (B)) * (8)) >= (24);
constraint ((str_len (B)) * (9)) <= (81);
solve satisfy;
