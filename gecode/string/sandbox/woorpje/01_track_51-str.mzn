var string: G;
var string: E;
constraint
  (("babbbaabaaababb") ++ (E) ++ ("aaaababbabb")) =
    (("babbbaabaaababbbabababaaabababbaaababbbabbaaababaaaabb") ++ (G) ++ ("abbbbbabbbbbaaaaaababbabb"));
solve satisfy;
