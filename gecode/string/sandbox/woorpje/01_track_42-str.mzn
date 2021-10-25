var string: I;
var string: G;
var string: L;
var string: J;
constraint
  (("a") ++ (J) ++ ("aaaa") ++ (L) ++ ("acbbbabb") ++ (J) ++ ("cc") ++ (G) ++ (J) ++ ("a") ++ (J) ++ ("aa") ++ (L) ++ ("aaa") ++ (J) ++ (L) ++ ("ba") ++ (L) ++ ("cabaa") ++ (L) ++ ("ab") ++ (L) ++ ("a"))
    =
    (("a") ++ (J) ++ ("aaaa") ++ (G) ++ ("a") ++ (G) ++ ("b") ++ (J) ++ (J) ++ ("a") ++ (J) ++ ("bb") ++ (L) ++ (L) ++ (G) ++ (I) ++ ("ab") ++ (G) ++ (J) ++ ("a") ++ (G) ++ (L) ++ ("a") ++ (J) ++ ("aa") ++ (G) ++ ("a") ++ (J) ++ (L) ++ ("a"));
solve satisfy;
