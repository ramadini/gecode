var string: E;
constraint
  ((E) ++ (E) ++ ("a") ++ (E) ++ (E) ++ ("ba")) =
    (("aa") ++ (E) ++ (E) ++ (E) ++ ("baa"));
solve satisfy;
