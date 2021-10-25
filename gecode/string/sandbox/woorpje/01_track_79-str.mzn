var string: G;
var string: E;
var string: J;
constraint
  (("caaac") ++ (J) ++ ("bababcbabaaccaabbabaaacabcababcaaaabaacc") ++ (E) ++ ("cccabbbabccacabccaacbbabcbbcbacbccbccbaaacba"))
    =
    (("caaac") ++ (J) ++ ("bababcbabaaccaabbabaaa") ++ (G) ++ ("cbbcbacbccbccbaaacba"));
solve satisfy;
