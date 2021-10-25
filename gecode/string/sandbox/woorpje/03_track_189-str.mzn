var string: S;
var string: J;
var string: T;
var string: I;
var string: H;
var string: V;
var string: N;
var string: O;
var string: M;
var string: Q;
constraint
  (("aa") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("aaaa") ++ (I) ++ ("aaa") ++ (I) ++ ("aa") ++ (I) ++ (I) ++ ("aaaa") ++ (I) ++ ("b") ++ (J) ++ ("aaa") ++ (J) ++ (J) ++ (J) ++ ("aba") ++ (M) ++ (O) ++ ("ab") ++ (Q) ++ (S) ++ ("b") ++ (V))
    =
    (("a") ++ (J) ++ (H) ++ (I) ++ (I) ++ ("aa") ++ (I) ++ ("aa") ++ (J) ++ ("aaa") ++ (M) ++ (J) ++ ("aaa") ++ (M) ++ ("b") ++ (N) ++ (N) ++ (M) ++ (N) ++ (N) ++ (N) ++ (M) ++ (N) ++ ("b") ++ (S) ++ (S) ++ (S) ++ (S) ++ ("b") ++ (T) ++ (T) ++ ("baa"));
solve satisfy;
