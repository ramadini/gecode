var string: H;
var string: C;
var string: G;
constraint
  (("cbaafdfbb") ++ (H) ++ ("cd")) =
    (("cb") ++ (C) ++ ("facaf") ++ (G) ++ ("dbeabdecccfbcbcd"));
constraint
  (("bbeeceaadcaeaebbbcbfddefccbad") ++ (G) ++ ("fafbcca")) =
    (("bbeeceaadcaeaebbbcbfddefccbad") ++ (G) ++ ("fafbcca"));
constraint
  (("edfefbcedfefdceda") ++ (G) ++ ("afaadbeebaefcdebbeb")) =
    (("edfefbcedfefdceda") ++ (G) ++ ("afaadbeebaefcdebbeb"));
solve satisfy;
