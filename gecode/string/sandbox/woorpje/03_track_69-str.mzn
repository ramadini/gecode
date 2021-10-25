var string: S;
var string: Y;
var string: T;
var string: H;
var string: I;
var string: R;
var string: V;
var string: G;
var string: O;
var string: M;
var string: Z;
var string: P;
var string: Q;
constraint
  ((H) ++ ("aa") ++ (H) ++ (I) ++ (G) ++ ("a") ++ (H) ++ ("aa") ++ (H) ++ (I) ++ (G) ++ ("b") ++ (O) ++ (M) ++ ("aa") ++ (M) ++ ("b") ++ (P) ++ (R) ++ (S) ++ ("b") ++ (S) ++ (S) ++ ("b") ++ (Y))
    =
    (("a") ++ (H) ++ (H) ++ (H) ++ (H) ++ (H) ++ ("aa") ++ (H) ++ ("aaaa") ++ (M) ++ (M) ++ ("aaaa") ++ (M) ++ (M) ++ ("aab") ++ (Q) ++ (R) ++ (R) ++ (R) ++ (Q) ++ (R) ++ (R) ++ (R) ++ ("b") ++ (V) ++ (T) ++ (V) ++ (T) ++ ("b") ++ (Z) ++ (Z) ++ ("baa"));
solve satisfy;
