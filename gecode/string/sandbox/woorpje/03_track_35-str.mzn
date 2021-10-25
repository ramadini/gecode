var string: S;
var string: J;
var string: T;
var string: R;
var string: K;
var string: H;
var string: L;
var string: M;
var string: Z;
var string: P;
constraint
  ((K) ++ ("aaa") ++ (J) ++ ("a") ++ (K) ++ ("aaa") ++ (J) ++ ("b") ++ (L) ++ (M) ++ ("ab") ++ (R) ++ (P) ++ (R) ++ ("ab") ++ (S) ++ (S) ++ ("b"))
    =
    (("a") ++ (H) ++ (K) ++ ("a") ++ (M) ++ ("aa") ++ (M) ++ (M) ++ (M) ++ ("aa") ++ (M) ++ (M) ++ ("b") ++ (P) ++ (R) ++ (R) ++ ("a") ++ (P) ++ (R) ++ (R) ++ ("ab") ++ (S) ++ (T) ++ (S) ++ (T) ++ ("b") ++ (Z) ++ (Z) ++ ("baa"));
solve satisfy;
