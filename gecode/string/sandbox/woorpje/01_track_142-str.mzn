var string: B;
var string: C;
constraint
  (("efacfebffaaddcbdffbedffcfdcdccffdecaaa") ++ (B) ++ ("dcaafebeefdfeacfcaabefaa"))
    =
    (("efacfebffaa") ++ (C) ++ ("cdccffdecaaa") ++ (B) ++ ("dcaafebeefdfeacfcaabefaa"));
solve satisfy;
