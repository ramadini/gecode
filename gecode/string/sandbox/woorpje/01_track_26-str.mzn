var string: B;
var string: D;
var string: A;
var string: C;
constraint
  ((A) ++ ("cc") ++ (A) ++ ("bcc") ++ (B)) =
    ((C) ++ ("cc") ++ (A) ++ ("b") ++ (D) ++ ("bc") ++ (C) ++ ("c") ++ (A));
solve satisfy;
