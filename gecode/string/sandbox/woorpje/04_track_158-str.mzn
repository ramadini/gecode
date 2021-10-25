var string: B;
var string: D;
var string: E;
var string: A;
constraint
  ((D) ++ (D) ++ (D) ++ ("a") ++ (A) ++ (A) ++ (A) ++ (E) ++ (B) ++ (D) ++ (D) ++ (A) ++ ("aa"))
    =
    ((D) ++ (A) ++ (A) ++ ("aaaaa") ++ (A) ++ ("a") ++ (A) ++ (A) ++ ("aaaaa") ++ (A) ++ (A) ++ ("a") ++ (A) ++ ("aaa") ++ (A) ++ (D) ++ (D) ++ ("a") ++ (A) ++ (A) ++ (A) ++ ("aa") ++ (A) ++ ("a") ++ (D) ++ ("aa") ++ (A) ++ (A) ++ (D) ++ ("a") ++ (A) ++ ("aaa"));
solve satisfy;
