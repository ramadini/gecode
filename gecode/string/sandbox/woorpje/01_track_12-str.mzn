var string: G;
var string: E;
var string: A;
var string: D;
constraint
  (("aadbabccceabfffdcbdaadffe") ++ (G) ++ ("fdeccdddefededcdbeecfabc") ++ (A) ++ ("fbcb") ++ (E) ++ ("efcceeaadbadcfacddbffddcadeed"))
    =
    (("aadbabccceabfffdcbdaadffe") ++ (G) ++ ("fdeccdddefededcd") ++ (D) ++ ("dbffddcadeed"));
solve satisfy;
