var string: B;
var string: F;
constraint
  (("aaaccbabaaccccaabb") ++ (B) ++ ("accaaaacbbbccccbccacacb")) =
    (("aaacc") ++ (F) ++ ("caaaacbbbccccbccacacb"));
solve satisfy;
