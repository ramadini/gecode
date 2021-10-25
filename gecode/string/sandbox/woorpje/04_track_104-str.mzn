var string: B;
var string: A;
var string: C;
var string: D;
constraint
  ((B) ++ ("d") ++ (C) ++ ("cabcaadccccdbcbcb")) =
    (("bddcdbacddbdaaabdcaaccccbaadbacabcaadccccdbcbcb") ++ (""));
constraint
  (("abbaaabcdabbdbbcddb") ++ (B) ++ ("ddddaaa")) =
    (("abbaaabcdabbdbbc") ++ (D) ++ ("babadaddaaccabaabddddaaa"));
constraint
  (("bcaaaabbaadbcccaacbcbadadaacdaccdbdbdaccaaaccbc") ++ ("")) =
    (("bcaaaabbaadbcccaacbcbadadaacda") ++ (A) ++ ("bdbdaccaaaccbc"));
constraint
  (("dadaabaabacadabcdabbdcaacddcaddcda") ++ (A) ++ ("dabddccaca")) =
    (("dadaabaabacadabcdabbdcaacddcaddcda") ++ (A) ++ ("dabddccaca"));
constraint
  (("dabcaabbccdbddbddbbbbacaddbdcacabacddddcaadcabb") ++ ("")) =
    (("dabcaabb") ++ (A) ++ ("bddbddbbbbacaddbdcacabacddddcaadcabb"));
constraint
  (("bbdaadbcababdbaabbccaaabbaabbbaccbdaa") ++ (A) ++ ("bbcdadc")) =
    (("bbdaadbcababdbaabbccaaabbaabbbaccbdaa") ++ (A) ++ ("bbcdadc"));
constraint
  (("aacbbbaccccabcbaaabaacccaabcdacbadacdaadcbdc") ++ (A)) =
    (("aacbbbaccccabcbaaabaacccaabcdacbadacdaadcbdcccd") ++ (""));
constraint
  (("bcdcbcbdbaddabdbbcbbaaddaadbcdbacdbbaccdabcaaab") ++ ("")) =
    (("bcdcbcbdbaddabdbbcbbaaddaadbcdbacdbba") ++ (A) ++ ("abcaaab"));
constraint
  (("adccbcacbaaabbbbbcdbdaadcdccdcaacdaccbacabdbcdc") ++ ("")) =
    (("adccbcacbaaabbbbbcdbdaadcd") ++ (A) ++ ("caacdaccbacabdbcdc"));
solve satisfy;
