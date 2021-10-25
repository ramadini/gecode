var string: B;
var string: A;
constraint
  (("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") ++ (B) ++ (B) ++ ("aaaaaaaa"))
    = ((A) ++ (B) ++ ("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
solve satisfy;
