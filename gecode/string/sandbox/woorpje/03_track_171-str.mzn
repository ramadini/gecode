var string: J;
var string: T;
var string: I;
var string: K;
var string: R;
var string: V;
var string: H;
var string: L;
var string: O;
var string: M;
var string: P;
constraint
  ((J) ++ ("aaa") ++ (I) ++ ("aa") ++ (J) ++ ("aaa") ++ (I) ++ ("ab") ++ (J) ++ (L) ++ (K) ++ ("b") ++ (P) ++ (R) ++ ("b") ++ (V))
    =
    (("a") ++ (H) ++ (I) ++ (M) ++ (L) ++ (M) ++ (L) ++ ("b") ++ (O) ++ (R) ++ (O) ++ (R) ++ ("b") ++ (T) ++ (T) ++ ("baa"));
solve satisfy;
