var string: H;
var string: K;
var string: N;
var string: E;
var string: L;
var string: O;
var string: M;
constraint
  ((E) ++ (E) ++ ("a") ++ (E) ++ (E) ++ ("b") ++ (K) ++ (L) ++ ("b") ++ (N))
    =
    (("a") ++ (H) ++ (H) ++ (M) ++ (M) ++ (M) ++ (M) ++ ("b") ++ (O) ++ (O) ++ ("baa"));
solve satisfy;
