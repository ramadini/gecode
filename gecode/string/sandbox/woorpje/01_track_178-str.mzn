var string: H;
var string: G;
var string: F;
var string: I;
constraint
  ((H) ++ ("acc") ++ (F) ++ ("cddcaebbaacabecdbbcecceacbacbcecbc") ++ (H) ++ ("abedceaaeeadebade") ++ (H) ++ ("eaeac"))
    = (("dbacc") ++ (G) ++ ("bbaacabe") ++ (I) ++ ("beaeac"));
solve satisfy;
