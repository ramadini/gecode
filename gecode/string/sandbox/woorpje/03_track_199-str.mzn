var string: J;
var string: T;
var string: Q;
var string: I;
var string: K;
var string: R;
var string: U;
var string: L;
var string: N;
var string: O;
var string: M;
var string: Z;
var string: P;
var string: W;
constraint
  ((J) ++ (K) ++ (K) ++ ("aaaa") ++ (J) ++ (K) ++ (K) ++ ("aaab") ++ (N) ++ (L) ++ ("b") ++ (O) ++ (P) ++ ("aba") ++ (T) ++ (T) ++ ("ab") ++ (T) ++ ("b") ++ (Z))
    =
    (("a") ++ (I) ++ ("aaaaaaa") ++ (K) ++ (N) ++ ("aaaa") ++ (M) ++ (M) ++ (N) ++ ("aaaa") ++ (M) ++ (M) ++ ("b") ++ (Q) ++ (Q) ++ (Q) ++ (Q) ++ ("b") ++ (R) ++ (R) ++ ("b") ++ (U) ++ ("a") ++ (U) ++ ("ab") ++ (W) ++ (W) ++ ("baa"));
solve satisfy;
