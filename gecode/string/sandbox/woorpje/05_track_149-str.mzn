var string: I;
var string: H;
var string: J;
var string: D;
constraint
  (("eebffefe") ++ (J) ++ ("ffb") ++ (D) ++ ("ecbcf")) =
    (("eebf") ++ (I) ++ ("ddeebdceccecbcf"));
constraint
  (("ddfcefdfdaaaadfbfbffeeadcdbecbdfcadbaeba") ++ ("")) =
    (("ddfcefdfdaaaadfbf") ++ (H) ++ ("eadcdbecbdfcadbaeba"));
constraint
  (("afeffafcbcbe") ++ (H) ++ ("ffbdcacebadadbabcadcbdec")) =
    (("afeffafcbcbebffeffbdcacebadadbabcadcbdec") ++ (""));
constraint ((str_len (I)) * (20)) >= (320);
constraint ((str_len (I)) * (20)) <= (1260);
constraint ((str_len (H)) * (19)) >= (57);
solve satisfy;
