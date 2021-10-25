var string: I;
var string: G;
var string: F;
var string: J;
var string: C;
constraint
  ((F) ++ ("bfaeabac") ++ (J) ++ ("c") ++ (G) ++ ("eef") ++ (F) ++ ("dcdb") ++ (I) ++ ("c") ++ (F) ++ ("afbb"))
    =
    ((F) ++ ("bfaeaba") ++ (F) ++ (C) ++ ("ceb") ++ (F) ++ (I) ++ ("cedaebeef") ++ (F) ++ ("d") ++ (F) ++ ("dbfcca") ++ (I) ++ ("bb"));
constraint
  ((I) ++ ("eb") ++ (F) ++ ("eedded") ++ (F) ++ ("dd") ++ (F) ++ ("bededa") ++ (I) ++ ("dadfbab") ++ (F) ++ (F) ++ (I) ++ ("bdcccece") ++ (I))
    =
    (("feb") ++ (F) ++ ("eedded") ++ (F) ++ ("dd") ++ (F) ++ ("bededafdadfbabc") ++ (F) ++ ("fbdc") ++ (F) ++ (F) ++ ("ecef"));
constraint
  (("dafcbfde") ++ (I) ++ ("dceddfea") ++ (F) ++ ("d") ++ (I) ++ ("b") ++ (F) ++ ("ebeabda") ++ (F) ++ (F) ++ ("afb") ++ (F) ++ (I) ++ (F) ++ ("acd"))
    =
    (("daf") ++ (F) ++ ("bfdefdceddfea") ++ (F) ++ ("d") ++ (I) ++ ("b") ++ (F) ++ ("ebeabdaccafb") ++ (F) ++ ("f") ++ (F) ++ ("acd"));
constraint ((str_len (C)) * (2)) >= (10);
constraint ((str_len (G)) * (3)) >= (3);
constraint ((str_len (G)) * (6)) <= (42);
constraint ((str_len (F)) * (2)) <= (2);
constraint ((str_len (I)) * (1)) >= (1);
constraint ((str_len (I)) * (13)) <= (78);
constraint ((str_len (J)) * (5)) <= (490);
solve satisfy;
