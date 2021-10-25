var string: J;
var string: T;
var string: R;
var string: H;
var string: G;
var string: L;
var string: M;
var string: P;
var string: Q;
constraint
  ((G) ++ (G) ++ ("aaaaa") ++ (G) ++ (G) ++ ("aaaab") ++ (L) ++ ("a") ++ (L) ++ ("a") ++ (L) ++ (M) ++ (M) ++ ("ab") ++ (P) ++ (P) ++ ("b") ++ (R) ++ (R) ++ ("ba"))
    =
    (("aaaaa") ++ (J) ++ (H) ++ (J) ++ (L) ++ (M) ++ (M) ++ (J) ++ (L) ++ (M) ++ (M) ++ ("b") ++ (M) ++ ("a") ++ (M) ++ ("ab") ++ (R) ++ (Q) ++ (R) ++ (Q) ++ ("b") ++ (T) ++ (T) ++ ("baa"));
solve satisfy;
