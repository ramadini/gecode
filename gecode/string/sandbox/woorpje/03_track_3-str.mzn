var string: S;
var string: N;
var string: J;
var string: X;
var string: H;
var string: K;
var string: R;
var string: L;
var string: O;
constraint
  (("aaaaaaa") ++ (J) ++ ("aaaaaaaaaa") ++ (J) ++ ("aab") ++ (K) ++ ("aa") ++ (K) ++ ("a") ++ (K) ++ (K) ++ (K) ++ ("b") ++ (O) ++ (S) ++ ("ab") ++ (S) ++ (S) ++ ("b"))
    =
    (("a") ++ (H) ++ (H) ++ ("aaaa") ++ (K) ++ (K) ++ (N) ++ (N) ++ (L) ++ (K) ++ (K) ++ (N) ++ (N) ++ (L) ++ ("b") ++ (S) ++ ("a") ++ (R) ++ (S) ++ ("a") ++ (R) ++ ("b") ++ (S) ++ ("a") ++ (S) ++ ("ab") ++ (X) ++ (X) ++ ("baa"));
solve satisfy;
