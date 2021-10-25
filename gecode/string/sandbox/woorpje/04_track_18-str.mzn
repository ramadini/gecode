var string: B;
var string: A;
constraint
  (("aabb") ++ (B) ++ ("bb") ++ (B) ++ ("bbbbab") ++ (B) ++ (A) ++ ("baaaabbaaa"))
    =
    (("aabbb") ++ (A) ++ ("bbbbabb") ++ (B) ++ ("bb") ++ (A) ++ (A) ++ (A) ++ ("aaabbaaa"));
solve satisfy;
