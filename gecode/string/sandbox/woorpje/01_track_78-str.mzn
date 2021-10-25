var string: A;
constraint
  (("caaabbbbbcbb") ++ (A) ++ ("c")) =
    (("caaabbbbbcbbbbcacacabcbbbabababbaaabaabc") ++ (""));
solve satisfy;
