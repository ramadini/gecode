var string: H;
var string: C;
var string: F;
constraint
  (("ccf") ++ (C) ++ ("bbcaad") ++ (C) ++ (H) ++ ("c") ++ (F) ++ ("cccfabaecd") ++ (C) ++ ("bebbd") ++ (C) ++ ("cadcaa"))
    =
    (("ccf") ++ (C) ++ ("bbcaad") ++ (C) ++ (H) ++ ("c") ++ (F) ++ ("cccfabaecd") ++ (C) ++ ("bebbdecadcaa"));
constraint
  (("dad") ++ (C) ++ ("b") ++ (C) ++ ("acaaecbbffafffbfafadddbaccbfdacbcd"))
    =
    (("dad") ++ (C) ++ ("beacaa") ++ (C) ++ ("cbbffafffbfafadddbaccbfdacbcd"));
constraint
  ((C) ++ ("aaf") ++ (C) ++ ("cfcdaacaadadacfa") ++ (C) ++ ("ffcdbfbababbbbdcde"))
    =
    ((C) ++ ("aaf") ++ (C) ++ ("cfcdaacaadadacfa") ++ (C) ++ ("ffcdbfbababbbbdcd") ++ (C));
constraint ((str_len (F)) * (16)) >= (32);
constraint ((str_len (F)) * (14)) <= (140);
constraint ((str_len (C)) * (8)) <= (40);
solve satisfy;
