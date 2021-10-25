var string: S;
var string: J;
var string: T;
var string: R;
var string: K;
var string: G;
var string: N;
var string: M;
var string: Z;
var string: P;
var string: W;
constraint
  ((J) ++ (G) ++ (J) ++ (J) ++ ("aaa") ++ (K) ++ (J) ++ (J) ++ (J) ++ (J) ++ ("aa") ++ (J) ++ (G) ++ (J) ++ (J) ++ ("aaa") ++ (K) ++ (J) ++ (J) ++ (J) ++ (J) ++ ("ab") ++ (K) ++ (N) ++ (M) ++ ("aba") ++ (P) ++ (S) ++ ("b") ++ (W) ++ ("ab"))
    =
    (("a") ++ (J) ++ (J) ++ (G) ++ (G) ++ (G) ++ (G) ++ ("a") ++ (J) ++ (J) ++ ("a") ++ (J) ++ ("a") ++ (M) ++ (N) ++ (N) ++ (N) ++ (M) ++ (N) ++ (N) ++ (N) ++ ("b") ++ (R) ++ (R) ++ (R) ++ (R) ++ ("b") ++ (T) ++ (T) ++ (T) ++ (T) ++ ("b") ++ (Z) ++ (Z) ++ ("baa"));
solve satisfy;
