var string: S;
var string: W;
var string: N;
var string: J;
var string: T;
var string: I;
var string: R;
var string: K;
var string: V;
var string: U;
var string: L;
var string: Y;
var string: Q;
constraint
  (("aaa") ++ (K) ++ (J) ++ (K) ++ ("a") ++ (J) ++ ("aa") ++ (J) ++ ("aa") ++ (J) ++ (J) ++ (J) ++ ("aa") ++ (J) ++ ("aaaaaa") ++ (K) ++ (J) ++ (K) ++ ("a") ++ (J) ++ ("aa") ++ (J) ++ ("aa") ++ (J) ++ (J) ++ (J) ++ ("aa") ++ (J) ++ ("aab") ++ (L) ++ ("aaaaaaaaaa") ++ (L) ++ ("aab") ++ (Q) ++ ("ab") ++ (T) ++ (T) ++ ("b") ++ (W) ++ (T) ++ ("b") ++ (Y))
    =
    (("a") ++ (I) ++ ("aaaaaa") ++ (J) ++ (N) ++ ("aa") ++ (L) ++ ("a") ++ (N) ++ ("aa") ++ (L) ++ ("ab") ++ (N) ++ ("aaaa") ++ (N) ++ ("aaaab") ++ (S) ++ ("a") ++ (R) ++ ("a") ++ (S) ++ ("a") ++ (R) ++ ("ab") ++ (V) ++ (U) ++ (V) ++ (U) ++ ("b") ++ (W) ++ (W) ++ ("baa"));
solve satisfy;
