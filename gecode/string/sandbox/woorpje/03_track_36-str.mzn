var string: S;
var string: J;
var string: T;
var string: H;
var string: K;
var string: R;
var string: G;
var string: L;
var string: N;
var string: O;
var string: M;
var string: Z;
constraint
  ((K) ++ ("kaa") ++ (K) ++ (K) ++ (J) ++ ("aa") ++ (K) ++ (K) ++ ("aa") ++ (K) ++ (K) ++ (J) ++ ("ab") ++ (N) ++ ("a") ++ (L) ++ (K) ++ ("b") ++ (S) ++ (O) ++ ("b") ++ (S) ++ ("b"))
    =
    (("aaaa") ++ (H) ++ (G) ++ (K) ++ (K) ++ (H) ++ ("a") ++ (M) ++ (N) ++ (M) ++ ("a") ++ (N) ++ (L) ++ ("a") ++ (M) ++ (N) ++ (M) ++ ("a") ++ (N) ++ (L) ++ ("ab") ++ (R) ++ ("a") ++ (R) ++ ("a") ++ (R) ++ ("a") ++ (R) ++ ("ab") ++ (T) ++ (T) ++ ("b") ++ (Z) ++ (Z) ++ ("baa"));
solve satisfy;
