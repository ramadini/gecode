var string: J;
var string: H;
var string: I;
var string: L;
var string: N;
var string: M;
var string: P;
var string: Q;
constraint
  ((H) ++ ("aaaaaaaa") ++ (H) ++ ("aa") ++ (H) ++ ("a") ++ (H) ++ ("aaaaaaaa") ++ (H) ++ ("aa") ++ (H) ++ ("baaaa") ++ (I) ++ ("b") ++ (L) ++ ("aa") ++ (L) ++ ("b") ++ (N) ++ (M) ++ ("b") ++ (P))
    =
    (("a") ++ (H) ++ ("aa") ++ (H) ++ (H) ++ ("aaaa") ++ (H) ++ ("aa") ++ (J) ++ ("aaaa") ++ (J) ++ ("aaaab") ++ (L) ++ ("a") ++ (L) ++ ("a") ++ (L) ++ ("a") ++ (L) ++ ("ab") ++ (M) ++ (N) ++ (M) ++ (N) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
