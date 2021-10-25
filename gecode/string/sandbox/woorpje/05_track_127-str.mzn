var string: G;
var string: E;
var string: A;
var string: C;
constraint
  (("eabcaeb") ++ (C) ++ (E) ++ ("ebacba")) =
    (("eabcaeb") ++ (A) ++ ("bcbc") ++ (G) ++ (C) ++ ("ebacba"));
constraint
  (("fbfafecfbbdaf") ++ (C) ++ ("fcbbffeafbbfbe") ++ (C) ++ ("bbbbffaaa")) =
    (("fbfafecfbbdaf") ++ (C) ++ ("fcbbffeafbbfbe") ++ (C) ++ ("bbbbffaaa"));
constraint
  (("afacedcafe") ++ (C) ++ ("fdafbeacaafedabbedccaf") ++ (C) ++ ("ffcb")) =
    (("afacedcafe") ++ (C) ++ ("fdafbeacaafedabbedccaf") ++ (C) ++ ("ffcb"));
constraint ((str_len (A)) * (18)) >= (162);
constraint ((str_len (C)) * (6)) >= (6);
constraint ((str_len (C)) * (15)) <= (90);
constraint ((str_len (E)) * (10)) >= (130);
solve satisfy;
