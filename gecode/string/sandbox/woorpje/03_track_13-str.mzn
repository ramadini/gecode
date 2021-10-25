var string: D;
var string: E;
constraint
  ((D) ++ ("aa") ++ (D) ++ ("aba")) =
    (("a") ++ (E) ++ (D) ++ (E) ++ (E) ++ ("baa"));
solve satisfy;
