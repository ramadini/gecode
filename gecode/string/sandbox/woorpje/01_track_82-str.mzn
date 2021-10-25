var string: B;
var string: G;
var string: A;
constraint
  (("cabe") ++ (B) ++ ("dccb") ++ (A) ++ ("ba") ++ (B) ++ (B) ++ ("edcafd") ++ (B) ++ ("edeefeaaeb"))
    =
    (("cabefd") ++ (G) ++ ("c") ++ (B) ++ ("abeacbccedbcdbbdddbedba") ++ (B) ++ ("fedca") ++ (B) ++ ("dfedeefeaaeb"));
solve satisfy;
