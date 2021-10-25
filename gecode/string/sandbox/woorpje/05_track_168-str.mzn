var string: G;
var string: F;
var string: A;
var string: J;
var string: D;
constraint
  (("fa") ++ (J) ++ (G) ++ ("cb") ++ (A) ++ ("addeddeba")) =
    (("fad") ++ (D) ++ ("ecbcb") ++ (A) ++ ("addeddeba"));
constraint
  (("dcebbdccdedbccdddddc") ++ (F) ++ ("cebcedfedeebdce")) =
    (("dcebbdccdedbccdddddc") ++ (F) ++ ("cebcedfedeebdce"));
constraint
  (("fbecaebfaefadecdfdcab") ++ (A) ++ ("aedecdbafaaaccab")) =
    (("fbecaebfaefadecdfdcabcbaaedecdbafaaaccab") ++ (""));
constraint ((str_len (D)) * (2)) <= (392);
constraint ((str_len (G)) * (6)) >= (78);
constraint ((str_len (J)) * (7)) <= (343);
constraint ((str_len (A)) * (20)) >= (60);
constraint ((str_len (F)) * (16)) >= (16);
constraint ((str_len (F)) * (15)) <= (75);
solve satisfy;
