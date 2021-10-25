var string: S;
var string: N;
var string: J;
var string: T;
var string: X;
var string: H;
var string: I;
var string: R;
var string: G;
var string: Q;
var string: L;
var string: Y;
var string: M;
var string: W;
constraint
  (("aaaaa") ++ (J) ++ (J) ++ ("aaaaaaa") ++ (J) ++ (J) ++ ("abaaa") ++ (L) ++ (M) ++ ("ab") ++ (S) ++ (S) ++ (S) ++ (S) ++ ("b") ++ (S) ++ (T) ++ ("b") ++ (Y))
    =
    (("a") ++ (H) ++ (G) ++ (I) ++ ("a") ++ (M) ++ (M) ++ (M) ++ (M) ++ (N) ++ ("aa") ++ (M) ++ (M) ++ (M) ++ (M) ++ (N) ++ ("ab") ++ (S) ++ (R) ++ (Q) ++ ("a") ++ (S) ++ (R) ++ (Q) ++ ("ab") ++ (S) ++ (W) ++ (S) ++ (W) ++ ("b") ++ (X) ++ (X) ++ ("baa"));
solve satisfy;
