var string: F;
var string: B;
var string: D;
var string: A;
constraint
  ((B) ++ (D) ++ (D) ++ ("aa") ++ (D) ++ ("aa") ++ (D) ++ (D) ++ ("aa") ++ (A) ++ (A) ++ (D) ++ ("aaaaaa") ++ (D) ++ (D) ++ ("aa") ++ (D) ++ (D) ++ (D) ++ ("aaaa") ++ (D) ++ ("aa") ++ (D) ++ (D) ++ ("aaaaa"))
    = ((F) ++ (F) ++ (F) ++ (F) ++ ("a"));
solve satisfy;
