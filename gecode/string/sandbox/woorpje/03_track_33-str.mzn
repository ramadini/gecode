var string: S;
var string: N;
var string: J;
var string: T;
var string: X;
var string: I;
var string: H;
var string: R;
var string: L;
var string: M;
var string: P;
var string: W;
constraint
  ((J) ++ ("iaaa") ++ (J) ++ ("aa") ++ (J) ++ (I) ++ ("aaa") ++ (J) ++ ("ab") ++ (L) ++ (L) ++ ("b") ++ (P) ++ (S) ++ (S) ++ ("b") ++ (S) ++ (W) ++ ("b"))
    =
    (("a") ++ (H) ++ (J) ++ ("aa") ++ (M) ++ (N) ++ (M) ++ ("aaa") ++ (M) ++ (N) ++ (M) ++ ("aab") ++ (R) ++ (R) ++ (R) ++ (R) ++ ("b") ++ (S) ++ (T) ++ (S) ++ (T) ++ ("b") ++ (X) ++ (X) ++ ("baa"));
solve satisfy;
