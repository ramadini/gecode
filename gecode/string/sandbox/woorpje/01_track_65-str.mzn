var string: C;
var string: A;
constraint
  (("bccbacccacacbbcbbaca") ++ (C) ++ ("aaacabccabcbbababcaaaaabcabccbbccbababbc"))
    =
    (("bccbaccca") ++ (A) ++ ("aacacbbaccababaaacabccabcbbababcaaaaabcabccbbccbababbc"));
solve satisfy;
