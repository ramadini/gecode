var string: I;
var string: B;
var string: G;
var string: F;
var string: C;
constraint
  ((G) ++ (B) ++ ("ffefe") ++ (F) ++ ("f") ++ (C) ++ ("dffabbffd") ++ (C) ++ ("caaeaeacb"))
    = (("baf") ++ (I) ++ ("eacb"));
constraint
  (("ecaec") ++ (C) ++ ("efbbbbbdcfebfeeaade") ++ (C) ++ ("bccdfeafbebcfa"))
    =
    (("ecaecdefbbbbb") ++ (C) ++ ("cfebfeeaadedbcc") ++ (C) ++ ("feafbebcfa"));
constraint
  (("dcacca") ++ (C) ++ ("cefefeaaab") ++ (C) ++ ("eabefbcfeededccaeea") ++ (C) ++ ("fa"))
    = (("dcaccadcefefeaaabdeabefbcfeededccaeeadfa") ++ (""));
constraint ((str_len (I)) * (10)) >= (130);
constraint ((str_len (I)) * (2)) <= (402);
constraint ((str_len (G)) * (12)) >= (12);
constraint ((str_len (G)) * (15)) <= (435);
solve satisfy;
