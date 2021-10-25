var string: B;
var string: E;
var string: F;
var string: A;
var string: C;
var string: D;
constraint
  ((C) ++ (D) ++ ("aaaaaaaaaaaaaaaaaaa")) =
    ((B) ++ ("aaaaaaa") ++ (F) ++ (B) ++ ("a"));
constraint
  ((C) ++ (A) ++ ("aaaaaaaaaaaaaaaaa")) =
    ((D) ++ ("aaaaaa") ++ (F) ++ (B) ++ ("a"));
constraint
  ((D) ++ (D) ++ (D) ++ ("aaaaaaaaaaaa")) = ((F) ++ (E) ++ ("aaaaaaaaaaaaa"));
solve satisfy;
