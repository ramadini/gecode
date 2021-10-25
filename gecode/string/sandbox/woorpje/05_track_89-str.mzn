var string: B;
var string: G;
var string: E;
var string: A;
var string: D;
constraint
  (("daa") ++ (G) ++ ("aeccd") ++ (B) ++ ("ce") ++ (G) ++ ("ebddebcda") ++ (G) ++ ("fa"))
    =
    (("daa") ++ (G) ++ ("aeccd") ++ (E) ++ (D) ++ ("e") ++ (G) ++ ("ebddebcda") ++ (G) ++ (G) ++ ("a"));
constraint
  ((G) ++ ("cbfbf") ++ (G) ++ ("fdddf") ++ (G) ++ ("dcca") ++ (G) ++ (G) ++ ("bfcdbbdfda") ++ (G) ++ (G) ++ ("cfcbd") ++ (G) ++ ("d") ++ (G) ++ ("c"))
    =
    ((G) ++ ("cb") ++ (G) ++ ("b") ++ (G) ++ (G) ++ ("fdddffdccaffbfc") ++ (A) ++ ("bd") ++ (G) ++ ("da") ++ (G) ++ (G) ++ ("c") ++ (G) ++ ("cbd") ++ (G) ++ ("d") ++ (G) ++ ("c"));
constraint
  (("eaedeafecfebfcfbd") ++ (G) ++ ("beadbdaeeccbcecabea") ++ (G) ++ ("cf"))
    =
    (("eaedea") ++ (G) ++ ("ecfebfc") ++ (G) ++ ("bdfbea") ++ (A) ++ ("daeeccbcecabeafc") ++ (G));
constraint ((str_len (E)) * (17)) >= (102);
constraint ((str_len (E)) * (7)) <= (504);
constraint ((str_len (D)) * (20)) >= (80);
constraint ((str_len (A)) * (10)) <= (20);
solve satisfy;
