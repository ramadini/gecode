var string: C;
var string: A;
constraint
  (("bc") ++ (A) ++ ("aab") ++ (A) ++ ("aaaac") ++ (A) ++ ("babbbc") ++ (A) ++ ("abbb") ++ (A) ++ ("abc") ++ (A))
    =
    (("b") ++ (A) ++ (A) ++ ("aab") ++ (A) ++ ("a") ++ (C) ++ ("bbbcab") ++ (A) ++ ("c"));
solve satisfy;
