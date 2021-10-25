var string: N;
var string: E;
var string: J;
var string: H;
var string: K;
var string: F;
var string: L;
var string: Q;
constraint
  ((E) ++ (F) ++ ("aa") ++ (E) ++ (F) ++ ("ab") ++ (L) ++ (K) ++ ("b") ++ (N))
    =
    (("a") ++ (H) ++ (E) ++ (K) ++ (J) ++ (K) ++ (J) ++ ("b") ++ (Q) ++ (Q) ++ ("baa"));
solve satisfy;
