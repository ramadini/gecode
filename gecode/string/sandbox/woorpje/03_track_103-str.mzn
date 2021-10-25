var string: E;
constraint
  ((E) ++ ("aa") ++ (E) ++ ("ab") ++ (E)) =
    (("a") ++ (E) ++ (E) ++ ("aabaa"));
solve satisfy;
