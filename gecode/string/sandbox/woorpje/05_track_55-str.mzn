var string: B;
var string: G;
var string: D;
constraint
  (("bfceeaceeff") ++ (G) ++ ("ff") ++ (B) ++ ("febbdfdcddfd") ++ (B) ++ ("fdf") ++ (G) ++ ("fdffcc") ++ (G) ++ ("a"))
    =
    ((G) ++ ("fceeacee") ++ (D) ++ ("ddfd") ++ (B) ++ ("fdfbfdffcc") ++ (G) ++ ("a"));
constraint
  (("fcfedcdcaace") ++ (B) ++ (G) ++ ("efc") ++ (G) ++ ("dedefdfefb") ++ (G) ++ ("abe") ++ (B) ++ ("fe") ++ (G) ++ ("a") ++ (B) ++ (B) ++ (G))
    =
    (("fcfedcdca") ++ (B) ++ ("ce") ++ (B) ++ (G) ++ ("efcbdedefdfefbb") ++ (B) ++ ("be") ++ (B) ++ ("feba") ++ (B) ++ (B) ++ (G));
constraint
  (("afeddb") ++ (G) ++ ("bebcfed") ++ (G) ++ ("ccec") ++ (G) ++ ("fca") ++ (B) ++ ("f") ++ (G) ++ ("ed") ++ (G) ++ ("ffdeed") ++ (B) ++ ("bdd") ++ (B))
    =
    (("afeddbb") ++ (G) ++ ("e") ++ (G) ++ ("cfedbccec") ++ (G) ++ ("fcaaf") ++ (G) ++ ("ed") ++ (G) ++ ("ffdeed") ++ (B) ++ (G) ++ ("dd") ++ (B));
constraint ((str_len (B)) * (17)) >= (17);
constraint ((str_len (B)) * (10)) <= (50);
constraint ((str_len (D)) * (10)) >= (40);
constraint ((str_len (D)) * (3)) <= (309);
constraint ((str_len (G)) * (2)) >= (2);
solve satisfy;
