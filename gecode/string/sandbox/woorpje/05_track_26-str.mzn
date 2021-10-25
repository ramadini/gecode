var string: F;
var string: G;
var string: A;
var string: J;
var string: C;
constraint
  (("a") ++ (J) ++ ("fdccdbeececfdcefdd")) =
    (("aadcdeeaab") ++ (A) ++ ("dcefdd"));
constraint
  (("aaabdaaffdaafebecdcfebbcabdadbdcacbdfced") ++ ("")) =
    (("aaabdaaffdaafebecdcfebbcabdadbdc") ++ (F) ++ ("ed"));
constraint
  (("e") ++ (C) ++ ("eecaccbbebccddd")) = (("efbea") ++ (G) ++ (A) ++ ("d"));
constraint
  (("aafdfdeadbeccceceaaadfefcaf") ++ (G) ++ ("caddddffd")) =
    (("aafdfdeadbeccceceaaadfefcaf") ++ (G) ++ ("caddddffd"));
constraint
  (("adabfafdbcccfdcbefeefedbaabbabdaeecdfafa") ++ ("")) =
    (("adabfa") ++ (G) ++ ("ccfdcbefeefedbaabbabdaeecdfafa"));
constraint ((str_len (A)) * (3)) <= (120);
constraint ((str_len (F)) * (7)) >= (112);
constraint ((str_len (F)) * (13)) <= (7904);
constraint ((str_len (C)) * (8)) <= (560);
solve satisfy;
