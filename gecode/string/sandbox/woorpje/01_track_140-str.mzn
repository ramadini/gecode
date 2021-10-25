var string: F;
var string: E;
var string: D;
constraint
  ((E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (E) ++ (F) ++ (E) ++ (F) ++ ("aaaaaa"))
    =
    ((F) ++ (F) ++ (E) ++ (F) ++ ("aaaaaa") ++ (E) ++ (F) ++ (D) ++ ("a") ++ (F));
solve satisfy;
