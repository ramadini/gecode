var string: A;
constraint
  (("bcbab") ++ (A) ++ ("badedcd")) = (("bcbab") ++ (A) ++ ("badedcd"));
constraint
  ((A) ++ (A) ++ ("eceeceabab")) = (("ac") ++ (A) ++ ("eceeceabab"));
constraint
  (("bcac") ++ (A) ++ ("e") ++ (A) ++ ("eddad")) =
    (("bc") ++ (A) ++ (A) ++ ("e") ++ (A) ++ ("eddad"));
constraint (("bbbcdaeabaaaca") ++ ("")) = (("bbbcdaeabaa") ++ (A) ++ ("a"));
solve satisfy;
