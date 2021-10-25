var string: B;
var string: G;
var string: J;
constraint
  ((J) ++ ("aaaaaa") ++ (B) ++ ("aa") ++ (B) ++ ("aaaaaa") ++ (B) ++ (B) ++ ("aaaa") ++ (B))
    =
    ((G) ++ ("aa") ++ (B) ++ ("aa") ++ (B) ++ ("aa") ++ (B) ++ ("a") ++ (G) ++ (B) ++ (B) ++ (B) ++ (B) ++ (B) ++ (B) ++ ("a") ++ (G));
solve satisfy;
