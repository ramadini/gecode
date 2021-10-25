var string: E;
var string: A;
var string: C;
constraint
  (("ccd") ++ (C) ++ (E) ++ ("aa")) =
    (("ccd") ++ (C) ++ ("baccc") ++ (C) ++ ("dbaa"));
constraint
  (("baccabbbca") ++ (C) ++ ("d") ++ (C) ++ ("c")) =
    (("baccabbbca") ++ (C) ++ (C) ++ (C) ++ ("c"));
constraint
  (("cc") ++ (C) ++ ("cbbabacda") ++ (C) ++ ("a")) =
    (("ccdcbbabacdada") ++ (""));
constraint
  (("bcdabb") ++ (C) ++ ("badcc") ++ (C) ++ (C)) =
    (("bc") ++ (C) ++ ("abb") ++ (C) ++ ("badccd") ++ (C));
constraint (("bacbbaabadbcbc") ++ ("")) = (("bacbbaaba") ++ (C) ++ ("bcbc"));
constraint
  (("bbbccba") ++ (C) ++ (C) ++ ("baddc")) =
    (("bbbccbad") ++ (C) ++ ("ba") ++ (C) ++ ("dc"));
constraint
  (("bacbbaba") ++ (C) ++ ("bbcdb")) = (("bacbbaba") ++ (C) ++ ("bbcdb"));
constraint
  (("daccc") ++ (C) ++ ("cbabbba") ++ (C)) =
    ((C) ++ ("accc") ++ (C) ++ ("cbabbba") ++ (C));
constraint
  (("d") ++ (C) ++ ("caaabbc") ++ (C) ++ ("cbcc")) =
    ((C) ++ ("dcaaabbcdcbcc") ++ (""));
constraint
  (("c") ++ (C) ++ ("cdbac") ++ (C) ++ ("aaaabc")) =
    (("c") ++ (C) ++ ("c") ++ (C) ++ ("bac") ++ (C) ++ ("aaaabc"));
constraint
  (("a") ++ (C) ++ ("cabaaaacbaa") ++ (C)) =
    (("a") ++ (C) ++ ("cabaaaacbaad"));
constraint
  ((C) ++ ("bc") ++ (C) ++ ("caacccb") ++ (C) ++ ("cb")) =
    (("dbcdcaacccbdcb") ++ (""));
constraint
  (("c") ++ (C) ++ ("c") ++ (C) ++ ("cdcb") ++ (C) ++ ("dab") ++ (C) ++ ("b"))
    =
    (("cdc") ++ (C) ++ ("c") ++ (C) ++ ("cb") ++ (C) ++ (C) ++ ("ab") ++ (C) ++ ("b"));
constraint
  (("ccbaa") ++ (C) ++ ("cba") ++ (C) ++ ("cbca")) =
    (("ccbaadcba") ++ (C) ++ ("cbca"));
constraint
  (("abbc") ++ (C) ++ ("ccdbaabc") ++ (C)) =
    (("abbcdcc") ++ (C) ++ ("baabc") ++ (C));
constraint
  (("acccd") ++ (C) ++ ("a") ++ (C) ++ ("caca") ++ (C) ++ ("c")) =
    (("acccd") ++ (C) ++ ("adcaca") ++ (C) ++ ("c"));
constraint
  (("bcdabcbb") ++ (C) ++ ("abdbb")) = (("bcdabcbbdab") ++ (C) ++ ("bb"));
constraint
  ((C) ++ ("c") ++ (C) ++ ("caacdcdcbbb")) =
    (("dcdcaac") ++ (C) ++ ("cdcbbb"));
constraint
  (("cac") ++ (C) ++ (C) ++ (C) ++ ("bcb") ++ (C) ++ ("ccbb")) =
    (("cacdddbcb") ++ (C) ++ ("ccbb"));
constraint (("aa") ++ (C) ++ ("dcbccbbcccd")) = (("aaddcbccbbccc") ++ (C));
constraint
  (("cbdb") ++ (C) ++ ("a") ++ (C) ++ ("c") ++ (C) ++ ("cdbab")) =
    (("cbdb") ++ (C) ++ ("adc") ++ (C) ++ ("c") ++ (C) ++ ("bab"));
constraint
  (("ccddcabbbabc") ++ (C) ++ ("a")) =
    (("cc") ++ (C) ++ (C) ++ ("cabbbabc") ++ (C) ++ ("a"));
constraint
  (("aaaaba") ++ (C) ++ ("ab") ++ (C) ++ ("bada")) =
    (("aaaaba") ++ (C) ++ ("abdbada"));
constraint
  (("aa") ++ (C) ++ ("cdabbdbabdc")) =
    (("aa") ++ (C) ++ ("cdabbdbab") ++ (C) ++ ("c"));
constraint
  (("bd") ++ (C) ++ ("baddadbabb") ++ (C)) =
    (("b") ++ (C) ++ (C) ++ ("badda") ++ (C) ++ ("babb") ++ (C));
constraint (("cababbcdaaadbb") ++ ("")) = (("cababbcdaaa") ++ (C) ++ ("bb"));
constraint
  ((C) ++ ("bcac") ++ (C) ++ ("a") ++ (C) ++ (C) ++ ("cac") ++ (C) ++ ("b"))
    = (("dbcacdad") ++ (C) ++ ("cac") ++ (C) ++ ("b"));
constraint
  (("abbbba") ++ (C) ++ ("cbbdbb") ++ (C)) =
    (("abbbbadcbb") ++ (C) ++ ("bbd"));
constraint ((C) ++ ("aaabccacadab") ++ (C)) = (("daaabccacadab") ++ (C));
constraint
  (("abcc") ++ (C) ++ ("aabaaaaac")) = (("abcc") ++ (C) ++ ("aabaaaaac"));
constraint
  (("dc") ++ (C) ++ ("ab") ++ (C) ++ ("caadcbba")) =
    (("dcdab") ++ (C) ++ ("caadcbba"));
constraint
  (("cd") ++ (C) ++ ("aa") ++ (C) ++ (C) ++ (C) ++ ("cbb") ++ (C) ++ ("aa"))
    = (("cddaad") ++ (C) ++ (C) ++ ("cbb") ++ (C) ++ ("aa"));
constraint
  (("adcb") ++ (C) ++ ("bc") ++ (C) ++ ("b") ++ (C) ++ (C) ++ ("bda")) =
    (("adcbdbcdb") ++ (C) ++ (C) ++ ("bda"));
constraint
  (("abc") ++ (C) ++ ("bbabcbcdba")) = (("abcdbbabcbc") ++ (C) ++ ("ba"));
constraint
  (("cbca") ++ (C) ++ ("bbbbba") ++ (C) ++ ("aa")) =
    (("cbcadbbbbbadaa") ++ (""));
constraint
  ((C) ++ (C) ++ (C) ++ ("dcbabdaabcc")) =
    ((C) ++ (C) ++ ("d") ++ (C) ++ ("cbab") ++ (C) ++ ("aabcc"));
constraint
  (("bdbbacada") ++ (C) ++ ("bccc")) = (("bdbbacada") ++ (C) ++ ("bccc"));
constraint
  ((C) ++ ("add") ++ (C) ++ ("ccac") ++ (C) ++ ("daca")) =
    (("dadddccacd") ++ (C) ++ ("aca"));
constraint (("bbccbaabcdacbb") ++ ("")) = (("bbccbaabc") ++ (C) ++ ("acbb"));
constraint
  (("cdc") ++ (C) ++ ("cdad") ++ (C) ++ ("bacdc")) =
    (("cdcdcdaddbac") ++ (C) ++ ("c"));
constraint
  (("daac") ++ (C) ++ ("adbab") ++ (C) ++ ("b") ++ (C) ++ ("a")) =
    (("daacdadbabdbda") ++ (""));
constraint
  (("cdba") ++ (C) ++ ("cddbabbac")) =
    (("cdba") ++ (C) ++ ("c") ++ (C) ++ (C) ++ ("babbac"));
constraint
  (("bcdacaaaa") ++ (C) ++ ("cbbd")) =
    (("bc") ++ (C) ++ ("acaaaa") ++ (C) ++ ("cbb") ++ (C));
constraint
  (("a") ++ (C) ++ ("dcadabad") ++ (C) ++ (C) ++ ("bb")) =
    (("ad") ++ (C) ++ ("cadaba") ++ (C) ++ (C) ++ ("dbb"));
constraint
  (("bcddaabbc") ++ (C) ++ ("ca") ++ (C) ++ ("c")) =
    (("bc") ++ (C) ++ ("daabbc") ++ (C) ++ ("cadc"));
constraint
  (("ccabbcbdbaadad") ++ ("")) = (("ccabbcb") ++ (C) ++ ("baada") ++ (C));
constraint
  (("b") ++ (C) ++ (C) ++ (C) ++ ("daacadacda")) =
    (("b") ++ (C) ++ (C) ++ ("ddaacadac") ++ (C) ++ ("a"));
constraint
  (("bddaadaca") ++ (C) ++ ("b") ++ (C) ++ ("da")) =
    (("bddaadacadb") ++ (C) ++ ("da"));
constraint
  (("d") ++ (C) ++ ("a") ++ (C) ++ (C) ++ ("aaacbaaac")) =
    ((C) ++ ("da") ++ (C) ++ ("daaacbaaac"));
constraint
  (("baab") ++ (C) ++ ("bacdbcc") ++ (C) ++ ("b")) =
    (("baab") ++ (C) ++ ("bac") ++ (C) ++ ("bccdb"));
constraint
  (("b") ++ (C) ++ ("dcbcaaacb") ++ (C) ++ ("aa")) =
    (("b") ++ (C) ++ ("dcbcaaacb") ++ (C) ++ ("aa"));
constraint (("aadaabcbcbbdac") ++ ("")) = (("aadaabcbcbb") ++ (C) ++ ("ac"));
constraint (("ccacaacbdbcdbd") ++ ("")) = (("ccacaacbdbc") ++ (C) ++ ("bd"));
constraint
  (("bccbcbbaba") ++ (C) ++ (C) ++ ("da")) =
    (("bccbcbbaba") ++ (C) ++ (C) ++ ("da"));
constraint
  (("d") ++ (C) ++ ("dcdaccac") ++ (C) ++ ("acb")) =
    ((C) ++ ("ddc") ++ (C) ++ ("accac") ++ (C) ++ ("acb"));
constraint
  (("cbadaadccccaab") ++ ("")) =
    (("cba") ++ (C) ++ ("aa") ++ (C) ++ ("ccccaab"));
constraint
  (("adadaba") ++ (C) ++ ("bcacaa")) = (("a") ++ (C) ++ ("adabadbcacaa"));
constraint
  (("a") ++ (C) ++ ("dbcaca") ++ (C) ++ ("cbbcd")) =
    (("a") ++ (C) ++ (C) ++ ("bcacadcbbc") ++ (C));
constraint
  (("add") ++ (C) ++ ("cbaaabccac")) = (("a") ++ (C) ++ ("ddcbaaabccac"));
constraint
  (("a") ++ (C) ++ ("dbbccbbdadaa")) = (("addbbccbbda") ++ (C) ++ ("aa"));
constraint
  ((C) ++ ("acbbacbbacada") ++ ("")) =
    ((C) ++ ("acbbacbbaca") ++ (C) ++ ("a"));
constraint
  (("dabbdbddcbbbcb") ++ ("")) =
    (("dabb") ++ (C) ++ ("b") ++ (C) ++ (C) ++ ("cbbbcb"));
constraint (("aabacdaabaccc") ++ (C)) = (("aabacdaabaccc") ++ (C));
constraint
  (("accbba") ++ (C) ++ ("cdcacbc")) =
    (("accbba") ++ (C) ++ ("c") ++ (C) ++ ("cacbc"));
constraint
  (("aaddbbdc") ++ (C) ++ ("aa") ++ (C) ++ ("bb")) =
    (("aa") ++ (C) ++ ("dbbdcdaa") ++ (C) ++ ("bb"));
constraint
  (("b") ++ (C) ++ ("bdaaabbabc") ++ (C) ++ ("c")) =
    (("b") ++ (C) ++ ("bdaaabbabc") ++ (C) ++ ("c"));
constraint
  (("bcbcb") ++ (C) ++ ("c") ++ (C) ++ ("cbaaac")) =
    (("bcbcbdc") ++ (C) ++ ("cbaaac"));
constraint
  (("db") ++ (C) ++ ("aab") ++ (C) ++ ("bcbaca") ++ (C)) =
    (("db") ++ (C) ++ ("aab") ++ (C) ++ ("bcbaca") ++ (C));
constraint
  (("caccc") ++ (C) ++ ("babaad") ++ (C) ++ (C)) =
    (("cacccdbabaa") ++ (C) ++ ("dd"));
constraint
  (("d") ++ (C) ++ (C) ++ (C) ++ ("a") ++ (C) ++ ("bbbabbca")) =
    (("ddddadbbbabbca") ++ (""));
constraint
  (("bad") ++ (C) ++ ("ccdcabadcc")) =
    (("ba") ++ (C) ++ (C) ++ ("cc") ++ (C) ++ ("cabadcc"));
constraint
  (("baccadca") ++ (C) ++ ("ddcab")) =
    (("bacca") ++ (C) ++ ("ca") ++ (C) ++ ("ddcab"));
constraint (("cbdbcbcabaadab") ++ ("")) = (("cb") ++ (C) ++ ("bcbcabaadab"));
constraint (("babcbaadabbdcb") ++ ("")) = (("babcbaa") ++ (C) ++ ("abbdcb"));
constraint
  (("abcc") ++ (C) ++ ("cbb") ++ (C) ++ (C) ++ ("cba") ++ (C)) =
    (("abcc") ++ (C) ++ ("cbb") ++ (C) ++ (C) ++ ("cbad"));
constraint
  (("bcdaabbddacbaa") ++ ("")) = (("bcdaabb") ++ (C) ++ (C) ++ ("acbaa"));
constraint
  (("badcb") ++ (C) ++ ("dababa") ++ (C) ++ ("a")) =
    (("badcbd") ++ (C) ++ ("ababada"));
constraint
  (("adcadc") ++ (C) ++ ("ddacbaa")) =
    (("a") ++ (C) ++ ("ca") ++ (C) ++ ("c") ++ (C) ++ (C) ++ (C) ++ ("acbaa"));
constraint (("d") ++ (C) ++ ("abbabcccbbca")) = (("ddabbabcccbbca") ++ (""));
constraint ((C) ++ ("bcacaaaacabaa") ++ ("")) = (("dbcacaaaacabaa") ++ (""));
constraint
  (("acbcdacc") ++ (C) ++ ("c") ++ (C) ++ ("a") ++ (C) ++ ("a")) =
    (("acbcdaccdc") ++ (C) ++ ("a") ++ (C) ++ ("a"));
constraint
  (("acccdb") ++ (C) ++ ("caaac") ++ (C) ++ ("a")) =
    (("accc") ++ (C) ++ ("b") ++ (C) ++ ("caaac") ++ (C) ++ ("a"));
constraint
  ((C) ++ ("aba") ++ (C) ++ ("bbd") ++ (C) ++ ("dddac")) =
    ((C) ++ ("aba") ++ (C) ++ ("bbdddddac"));
constraint
  (("bbb") ++ (C) ++ ("cac") ++ (C) ++ ("cc") ++ (C) ++ ("dac")) =
    (("bbb") ++ (C) ++ ("cacdccd") ++ (C) ++ ("ac"));
constraint (("bcadcadaaaabbd") ++ ("")) = (("bcadcadaaaabb") ++ (C));
constraint
  (("abaacbb") ++ (C) ++ ("b") ++ (C) ++ ("bbda")) =
    (("abaacbbdb") ++ (C) ++ ("bbda"));
constraint
  (("caacdbadbabc") ++ (C) ++ (C)) =
    (("caac") ++ (C) ++ ("ba") ++ (C) ++ ("babc") ++ (C) ++ (C));
constraint
  (("aaababcdb") ++ (C) ++ ("bcd") ++ (C)) =
    (("aaababc") ++ (C) ++ ("b") ++ (C) ++ ("bc") ++ (C) ++ ("d"));
constraint (("bbbcac") ++ (C) ++ ("dbbbabb")) = (("bbbcacddbbbabb") ++ (""));
constraint
  (("b") ++ (C) ++ ("dcccbc") ++ (C) ++ ("accc") ++ (C)) =
    (("b") ++ (C) ++ ("dcccbc") ++ (C) ++ ("accc") ++ (C));
constraint
  (("abcdacbbddcddb") ++ ("")) =
    (("abcdacbbd") ++ (C) ++ ("c") ++ (C) ++ (C) ++ ("b"));
constraint
  (("bc") ++ (C) ++ (C) ++ ("badcad") ++ (C) ++ ("cb") ++ (C)) =
    (("b") ++ (A) ++ ("ba") ++ (C) ++ ("ca") ++ (C) ++ (C) ++ ("cb") ++ (C));
constraint
  (("ccbada") ++ (C) ++ ("ab") ++ (C) ++ ("bbab")) =
    (("ccbada") ++ (C) ++ ("abdbbab"));
constraint (("dcdbaaababbbbc") ++ ("")) = ((C) ++ ("cdbaaababbbbc") ++ (""));
constraint (("ddbaaabbacabaa") ++ ("")) = ((C) ++ (C) ++ ("baaabbacabaa"));
constraint
  ((C) ++ ("bbdbccadaaadb") ++ ("")) = (("dbb") ++ (C) ++ ("bccadaaadb"));
constraint
  (("a") ++ (C) ++ ("caaacdaa") ++ (C) ++ ("cbd")) =
    (("a") ++ (C) ++ ("caaacdaa") ++ (C) ++ ("cb") ++ (C));
constraint
  (("cdacbbacd") ++ (C) ++ ("bbbb")) = (("cdacbbacd") ++ (C) ++ ("bbbb"));
constraint (("bbccbbbacbaba") ++ (C)) = (("bbccbbbacbaba") ++ (C));
constraint
  (("ba") ++ (C) ++ ("ddbccddaacb")) =
    (("bad") ++ (C) ++ (C) ++ ("bccddaacb"));
constraint
  (("aad") ++ (C) ++ ("bbbaccbbca")) = (("aad") ++ (C) ++ ("bbbaccbbca"));
constraint (("cabbccabac") ++ (C) ++ ("bbc")) = (("cabbccabacdbbc") ++ (""));
constraint
  (("cbccabbc") ++ (C) ++ ("bcbda")) = (("cbccabbcdbcb") ++ (C) ++ ("a"));
constraint
  (("cacccbd") ++ (C) ++ ("ccccbc")) =
    (("cacccb") ++ (C) ++ (C) ++ ("ccccbc"));
constraint
  (("bbdcdcabdabc") ++ (C) ++ ("b")) =
    (("bbdcdcab") ++ (C) ++ ("abc") ++ (C) ++ ("b"));
constraint
  (("caccdbc") ++ (C) ++ ("daccac")) =
    (("cacc") ++ (C) ++ ("bc") ++ (C) ++ ("daccac"));
constraint
  (("dcdbcdcbbdc") ++ (C) ++ ("dc")) =
    ((C) ++ ("cdbc") ++ (C) ++ ("cbbdc") ++ (C) ++ ("dc"));
constraint
  ((A) ++ ("dacabcdbcac") ++ ("")) = (("cd") ++ (C) ++ ("dacabcdbcac"));
constraint
  ((C) ++ ("daa") ++ (C) ++ ("ab") ++ (C) ++ ("aaabbb")) =
    ((C) ++ ("daadabdaaabbb") ++ (""));
constraint
  (("bdaad") ++ (C) ++ ("bcccccbc")) = (("b") ++ (C) ++ ("aaddbcccccbc"));
constraint (("ccdabbabdbbcad") ++ ("")) = (("ccdabbabdbbca") ++ (C));
solve satisfy;
