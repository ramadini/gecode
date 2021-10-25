var string: H;
var string: F;
var string: I;
constraint
  (("f") ++ (H) ++ ("afaa") ++ (I) ++ ("ebabdfea") ++ (I) ++ ("edddcceffccff") ++ (I) ++ ("cacfcd"))
    =
    (("ffac") ++ (I) ++ ("b") ++ (F) ++ (I) ++ ("dfeabedddcceffccff") ++ (I) ++ ("cacfcd"));
constraint
  (("faeb") ++ (I) ++ ("fddfedfdceaaabafbedca") ++ (I) ++ ("dfceccc") ++ (I) ++ ("cfacf"))
    =
    (("fae") ++ (I) ++ (I) ++ ("fddfedfdceaaa") ++ (I) ++ ("af") ++ (I) ++ ("edca") ++ (I) ++ ("dfcecccbcfacf"));
constraint
  (("b") ++ (I) ++ ("eefb") ++ (I) ++ ("afbbdfaecaaa") ++ (I) ++ (I) ++ ("edda") ++ (I) ++ ("bceefacfaddeb") ++ (I))
    =
    (("b") ++ (I) ++ ("eef") ++ (I) ++ ("bafb") ++ (I) ++ ("dfaecaaa") ++ (I) ++ ("bedda") ++ (I) ++ ("bceefacfaddebb"));
constraint ((str_len (H)) * (12)) >= (12);
constraint ((str_len (I)) * (2)) <= (18);
constraint ((str_len (F)) * (20)) <= (180);
solve satisfy;
