var string: F;
var string: E;
constraint
  (("acbb") ++ (F) ++ ("ba") ++ (F) ++ (E) ++ ("cbcba") ++ (F)) =
    (("acbbcbacaabcb") ++ (F) ++ ("abcabbc") ++ (F) ++ (F) ++ (F) ++ ("baa") ++ (F) ++ (F) ++ ("cabb") ++ (F) ++ ("ababbaaa") ++ (F) ++ ("ca") ++ (F) ++ ("ab") ++ (F) ++ ("b") ++ (F) ++ ("ba") ++ (F));
solve satisfy;
