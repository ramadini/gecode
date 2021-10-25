var string: C;
var string: A;
constraint
  (("bbadbd") ++ (A) ++ ("caccbcbaaacbdcdaad") ++ (C) ++ ("cdcd")) =
    (("bbadbdccdaccaccbcbaaacbdcdaadcaacbbcaaadbdadcaccbdbbdadcdcd") ++ (""));
constraint
  (("aaaddccacbcadcaabaaacdcaacbdddccaccabca") ++ (A) ++ ("dbdcbcdaadbdccc"))
    =
    (("aaaddccacbcadcaabaaacdcaacbdddccaccabcaccdacdbdcbcdaadbdccc") ++ (""));
constraint
  (("dcabaaccdbaddccbdcbccaacadadadccbdcaaacadbaabaccdaccdaaadcd") ++ ("")) =
    (("dcabaaccdbaddccbdcbccaacadadadccbdcaaacadbaaba") ++ (A) ++ ("cdaaadcd"));
constraint
  (("abaaddabdbbbadbcddcbb") ++ (A) ++ ("ddaddddbcdddbccabadbdcadcbbdbbaaa"))
    =
    (("abaaddabdbbbadbcddcbb") ++ (A) ++ ("ddaddddbcdddbccabadbdcadcbbdbbaaa"));
constraint
  (("bdbab") ++ (A) ++ ("dcacdcbcdacaabbbdadaaaabadcddcbbadbacabbddbbcdcba"))
    =
    (("bdbabccdacdcacdcbcdacaabbbdadaaaabadcddcbbadbacabbddbbcdcba") ++ (""));
constraint
  (("acbddbbaabdddbabdcdadacacbaadbdbbccaaccdacdbaaaaccdbdddbaaa") ++ ("")) =
    (("acbddbbaabdddbabdcdadacacbaadbdbbccaa") ++ (A) ++ ("dbaaaaccdbdddbaaa"));
constraint
  (("acdcaddbdcabba") ++ (A) ++ ("bcdbcbcbcdbbcccacaabbacddabbdbddcdbddcbc"))
    =
    (("acdcaddbdcabbaccdacbcdbcbcbcdbbcccacaabbacddabbdbddcdbddcbc") ++ (""));
constraint
  (("dbddcadacdbbbdbacaabccdbcb") ++ (A) ++ ("ccaaddbaccdcaabcbcddacbadcbc"))
    =
    (("dbddcadacdbbbdbacaabccdbcbccdacccaaddbaccdcaabcbcddacbadcbc") ++ (""));
constraint
  (("cbacdaaaddbaaabdaddcbbdcdcdbbaccddbbaccdacbbccbddabacabaaad") ++ ("")) =
    (("cbacdaaaddbaaabdaddcbbdcdcdbbaccddbba") ++ (A) ++ ("bbccbddabacabaaad"));
constraint
  (("badab") ++ (A) ++ ("baaabbcccacdccdadddabddbaabcdbcaaccccabbdccbabdda"))
    =
    (("badabccdacbaaabbcccacdccdadddabddbaabcdbcaaccccabbdccbabdda") ++ (""));
constraint
  (("cbbdbbbbddaacdd") ++ (A) ++ ("bccdadcdccaccdbbdbaccaddcabcbbbcbbcabcc"))
    =
    (("cbbdbbbbddaacddccdacbccdadcdccaccdbbdbaccaddcabcbbbcbbcabcc") ++ (""));
constraint
  (("acaacdcbadbbccdaadbabcccbdadaccdacaabbcbbcabdcbbbadbacadaca") ++ ("")) =
    (("acaacdcbadbbccdaadbabcccbdada") ++ (A) ++ ("aabbcbbcabdcbbbadbacadaca"));
constraint
  (("dddcaabdaabdccccbcdddbbdbabdccbacddacbdbbabdaa") ++ (A) ++ ("adcdadcb"))
    =
    (("dddcaabdaabdccccbcdddbbdbabdccbacddacbdbbabdaa") ++ (A) ++ ("adcdadcb"));
constraint
  (("bcdcadbbbaaacdddbcccbbbdadabcdacdaa") ++ (A) ++ ("abadbacbabacbacadab"))
    =
    (("bcdcadbbbaaacdddbcccbbbdadabcdacdaa") ++ (A) ++ ("abadbacbabacbacadab"));
constraint
  (("cdaababacdbcdcdccabaaadabadaddbacbbdbbbbc") ++ (A) ++ ("dbbbaabccaabb"))
    =
    (("cdaababacdbcdcdccabaaadabadaddbacbbdbbbbcccdacdbbbaabccaabb") ++ (""));
constraint
  (("acbddcaabdbddbbbaaabdaabbacabaabbaccadbddaccbdacccbcab") ++ (A)) =
    (("acbddcaabdbddbbbaaabdaabbacabaabbaccadbddaccbdacccbcab") ++ (A));
constraint
  (("cbcadcadaacadaddbcccaaabcbdbdabbddaccdcabcdddbdb") ++ (A) ++ ("acacdd"))
    =
    (("cbcadcadaacadaddbcccaaabcbdbdabbddaccdcabcdddbdbccdacacacdd") ++ (""));
constraint
  (("adbbccbacdccbabcbbadcccabcbcccbbdcbccdacacbcadadbdacdcaaacd") ++ ("")) =
    (("adbbccbacdccbabcbbadcccabcbcccbbdcb") ++ (A) ++ ("acbcadadbdacdcaaacd"));
constraint
  (("abbccdaccdbdadcbcccbadbbdbdccabcbbbaaccddbacaaaadabdbdbdccb") ++ ("")) =
    (("abb") ++ (A) ++ ("cdbdadcbcccbadbbdbdccabcbbbaaccddbacaaaadabdbdbdccb"));
constraint
  (("cddddcdabadabccccdbcbdacdcbbaaabcadacbad") ++ (A) ++ ("bdabdacdaaccdc"))
    =
    (("cddddcdabadabccccdbcbdacdcbbaaabcadacbad") ++ (A) ++ ("bdabdacdaaccdc"));
solve satisfy;
