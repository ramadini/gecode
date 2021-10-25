var string: D;
var string: G;
var string: A;
var string: J;
constraint
  (("aeca") ++ (D) ++ ("acdffebcdcbdddfabcecfaacaeebabca") ++ (D) ++ ("de") ++ (A) ++ ("afcaddfbcaecbbcfeadddacdbceca"))
    =
    (("aeca") ++ (D) ++ ("acdffe") ++ (J) ++ ("acaeebabca") ++ (D) ++ ("defdbdaeeafe") ++ (G) ++ ("ddfbcaecbbcfeadddacdbceca"));
solve satisfy;
