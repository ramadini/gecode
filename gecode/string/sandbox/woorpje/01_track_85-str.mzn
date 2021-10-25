var string: B;
var string: C;
constraint
  (("c") ++ (B) ++ ("abbab") ++ (B) ++ ("ab") ++ (B) ++ ("b") ++ (B) ++ ("cbabcbcbb"))
    = (("c") ++ (C) ++ (B) ++ ("cbabcbcbb"));
solve satisfy;
