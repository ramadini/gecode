var string: I;
var string: A;
var string: J;
var string: D;
constraint
  (("cfcabffc") ++ (A) ++ ("cecc") ++ (J) ++ ("feebeb") ++ (D) ++ ("f") ++ (D) ++ ("bb"))
    =
    (("cfcabff") ++ (I) ++ ("eccfeef") ++ (D) ++ ("fad") ++ (D) ++ ("feebeb") ++ (D) ++ ("fabb"));
constraint
  (("fbbcebefae") ++ (D) ++ ("f") ++ (D) ++ ("fedfecfefddcefedaffbdedecef"))
    =
    (("fbbcebef") ++ (D) ++ ("e") ++ (D) ++ ("f") ++ (D) ++ ("fedfecfefddcefed") ++ (D) ++ ("ffbdedecef"));
constraint
  (("ebdbedbffafacbbcc") ++ (D) ++ (D) ++ ("efeef") ++ (D) ++ ("ecfbcbeeababdc") ++ (D))
    =
    (("ebdbedbff") ++ (D) ++ ("facbbcc") ++ (D) ++ (D) ++ ("efeef") ++ (D) ++ ("ecfbcbeeab") ++ (D) ++ ("bdc") ++ (D));
constraint ((str_len (I)) * (18)) <= (486);
constraint ((str_len (D)) * (4)) >= (4);
solve satisfy;
