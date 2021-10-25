var string: B;
var string: G;
var string: C;
constraint
  (("bbba") ++ (G) ++ ("baabaaaa") ++ (B) ++ ("ababbbbababbaaabb")) =
    (("bbba") ++ (G) ++ (C) ++ ("abbaaabb"));
solve satisfy;
