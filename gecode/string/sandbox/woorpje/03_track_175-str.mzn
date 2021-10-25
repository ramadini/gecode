var string: J;
var string: T;
var string: R;
var string: V;
var string: G;
var string: F;
var string: L;
var string: O;
var string: M;
constraint
  ((J) ++ ("a") ++ (J) ++ ("a") ++ (G) ++ ("a") ++ (G) ++ ("aa") ++ (J) ++ ("a") ++ (J) ++ ("a") ++ (G) ++ ("a") ++ (G) ++ ("ab") ++ (L) ++ (J) ++ (J) ++ ("b") ++ (O) ++ (R) ++ ("b") ++ (V))
    =
    (("a") ++ (F) ++ (F) ++ (M) ++ (M) ++ (J) ++ (M) ++ (M) ++ (M) ++ (J) ++ (M) ++ ("ba") ++ (R) ++ ("a") ++ (R) ++ ("b") ++ (T) ++ (T) ++ ("baa"));
solve satisfy;
