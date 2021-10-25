var string: B;
var string: A;
var string: C;
constraint
  (("efdcffab") ++ (A) ++ ("fdcb")) =
    (("efdcffabae") ++ (C) ++ ("bcecfd") ++ (B) ++ ("ddac") ++ (B) ++ ("ffdcb"));
solve satisfy;
