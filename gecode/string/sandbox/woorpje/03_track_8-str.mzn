var string: E;
constraint
  (("a") ++ (E) ++ ("aa") ++ (E) ++ ("ba")) =
    (("a") ++ (E) ++ (E) ++ (E) ++ (E) ++ ("baa"));
solve satisfy;
