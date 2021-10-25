var string: H;
var string: G;
var string: E;
constraint
  (("abcaccccccab") ++ (E) ++ ("bc") ++ (E) ++ ("ccacbca") ++ (E) ++ ("caaaa") ++ (E) ++ ("cbacccacbcbccaaa") ++ (E) ++ ("abccccac") ++ (H))
    = (("abc") ++ (G));
solve satisfy;
