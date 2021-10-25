var string: S;
var string: J;
var string: R;
var string: V;
var string: F;
var string: L;
var string: M;
var string: P;
var string: Q;
constraint
  ((J) ++ (F) ++ ("aa") ++ (J) ++ (F) ++ ("ab") ++ (L) ++ (M) ++ ("b") ++ (P) ++ (P) ++ ("b") ++ (V))
    =
    (("aa") ++ (F) ++ (F) ++ ("a") ++ (F) ++ (F) ++ ("aa") ++ (J) ++ (L) ++ (J) ++ (L) ++ ("b") ++ (R) ++ (Q) ++ (R) ++ (Q) ++ ("b") ++ (S) ++ (S) ++ ("baa"));
solve satisfy;
