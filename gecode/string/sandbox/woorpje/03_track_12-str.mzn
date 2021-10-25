var string: D;
var string: E;
constraint
  ((D) ++ ("aa") ++ (D) ++ ("aba")) =
    (("a") ++ (D) ++ (D) ++ (E) ++ (E) ++ ("baa"));
solve satisfy;
