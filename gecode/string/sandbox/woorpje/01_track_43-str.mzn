var string: I;
var string: K;
var string: A;
var string: M;
var string: D;
constraint
  (("a") ++ (K) ++ ("aa") ++ (A) ++ ("fad") ++ (M) ++ ("dbbe") ++ (M) ++ ("ebebfdddac") ++ (M) ++ (M) ++ ("adceeecbcbfee"))
    =
    (("afedbba") ++ (M) ++ ("baaeecd") ++ (M) ++ ("e") ++ (M) ++ ("d") ++ (M) ++ ("cafddbb") ++ (I) ++ ("fdbbe") ++ (M) ++ ("ebeb") ++ (M) ++ (D) ++ ("dacf") ++ (M) ++ ("adceeecbcb") ++ (M) ++ ("ee"));
solve satisfy;
