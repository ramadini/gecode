var string: C;
var string: F;
constraint
  (("bcabbbcbccbcccccca") ++ (C) ++ ("cbaabccabbcaccbaaaaba")) =
    (("bcab") ++ (F) ++ ("cbcccccca") ++ (C) ++ ("cbaabccabbcaccbaaaaba"));
solve satisfy;
