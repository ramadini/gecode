var string: C;
var string: A;
constraint
  (("aaaaaaaaaaaa") ++ (C) ++ ("a")) = ((A) ++ ("aaaaaaaaaaaa") ++ (""));
solve satisfy;
