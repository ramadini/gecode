var string: I;
var string: F;
var string: A;
var string: J;
var string: C;
constraint
  (("dedfecea") ++ (I) ++ ("baaacdfebdefafdca")) =
    (("dedfeceabcefcfbadbe") ++ (A) ++ ("dfebdefafdca"));
constraint
  (("ddabac") ++ (C) ++ ("ebcacecddbba") ++ (C) ++ ("bbaceaebafabc") ++ (C) ++ ("efcccc"))
    = (("d") ++ (J) ++ ("cecddbba") ++ (F) ++ ("cccc"));
constraint
  (("d") ++ (C) ++ ("d") ++ (C) ++ ("ebbcdcecf") ++ (C) ++ ("cbeffda") ++ (C) ++ ("aebaadcbfbccaabecd"))
    =
    (("d") ++ (C) ++ ("d") ++ (C) ++ ("ebbcdcecf") ++ (C) ++ ("cbe") ++ (C) ++ ("fdafaebaadcbfbccaabecd"));
constraint
  (("cacabfdbd") ++ (C) ++ ("aadedcfdeefbdaccfcdcdbeed") ++ (C) ++ ("daae"))
    =
    (("cacabfdbd") ++ (C) ++ ("aadedc") ++ (C) ++ ("deefbdaccfcdcdbeedfdaae"));
constraint ((str_len (A)) * (18)) >= (36);
constraint ((str_len (A)) * (6)) <= (90);
constraint ((str_len (J)) * (11)) <= (1045);
constraint ((str_len (I)) * (12)) >= (168);
constraint ((str_len (C)) * (12)) >= (12);
constraint ((str_len (C)) * (17)) <= (51);
constraint ((str_len (F)) * (5)) >= (75);
constraint ((str_len (F)) * (7)) <= (392);
solve satisfy;
