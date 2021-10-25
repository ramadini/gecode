var string: H;
var string: J;
var string: N;
var string: M;
var string: D;
var string: L;
constraint
  (("aabccbc") ++ (N) ++ ("c") ++ (J)) =
    (("aabccbcccbcaa") ++ (D) ++ ("ac") ++ (M) ++ ("abbacccbbb") ++ (H) ++ ("baab") ++ (L) ++ ("ccacbbaa"));
solve satisfy;
