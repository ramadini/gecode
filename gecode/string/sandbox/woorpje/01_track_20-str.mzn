var string: A;
constraint
  (("abcd") ++ (A) ++ ("cebdc") ++ (A) ++ ("ccbfebdfaaeeaecfeea")) =
    ((A) ++ ("cd") ++ (A) ++ ("cebdcabccbfebdfaaeeaecfeea"));
solve satisfy;
