var string: B;
var string: F;
var string: A;
var string: C;
constraint
  (("a") ++ (C) ++ ("baabac") ++ (A) ++ ("add")) =
    (("a") ++ (C) ++ ("baa") ++ (C) ++ ("acaac") ++ (C) ++ ("cc") ++ (C) ++ ("bc") ++ (B) ++ ("aaa") ++ (C) ++ ("dad") ++ (C) ++ ("dc") ++ (F) ++ ("ccaad") ++ (C) ++ ("caaadac") ++ (C) ++ ("dddbdcbadd"));
constraint
  (("ddac") ++ (C) ++ (C) ++ ("abb") ++ (C) ++ ("cab") ++ (C) ++ ("dccaada") ++ (C) ++ ("ac") ++ (C) ++ ("aaada") ++ (C) ++ ("ccada") ++ (C) ++ ("ccdddcbac") ++ (C) ++ ("addda") ++ (C) ++ ("acda") ++ (C) ++ ("dbb") ++ (C) ++ ("bbcada"))
    =
    (("ddac") ++ (C) ++ (C) ++ ("ab") ++ (C) ++ (C) ++ ("cab") ++ (C) ++ ("dccaadabac") ++ (C) ++ ("aaadabccadabccdddc") ++ (C) ++ ("acbaddda") ++ (C) ++ ("acdabd") ++ (C) ++ ("b") ++ (C) ++ ("bbcada"));
constraint
  (("b") ++ (C) ++ ("accddcd") ++ (C) ++ ("daac") ++ (C) ++ ("dacac") ++ (C) ++ ("daaac") ++ (C) ++ ("dbd") ++ (C) ++ ("d") ++ (C) ++ ("accadaaaaadd") ++ (C) ++ ("bc") ++ (C) ++ (C) ++ ("bad") ++ (C) ++ ("dcccdc") ++ (C) ++ ("dacd") ++ (C) ++ ("c") ++ (C))
    =
    ((C) ++ ("baccddcd") ++ (C) ++ ("daacbdacac") ++ (C) ++ ("daaacbd") ++ (C) ++ ("d") ++ (C) ++ ("dbaccadaaaaaddbbc") ++ (C) ++ ("bbadbdcccdc") ++ (C) ++ ("dacd") ++ (C) ++ ("c") ++ (C));
constraint
  ((C) ++ ("cdcdcadabaaabcaaddaccdcacdaccdabbdb") ++ (C) ++ (C) ++ ("abcbccdadbbdddddccaadbbccaddca"))
    =
    (("bcdcdcada") ++ (C) ++ ("aaa") ++ (C) ++ ("caaddaccdcacdaccda") ++ (C) ++ (C) ++ ("db") ++ (C) ++ ("babcbccdad") ++ (C) ++ (C) ++ ("dddddccaad") ++ (C) ++ (C) ++ ("ccaddca"));
constraint
  (("ca") ++ (C) ++ ("b") ++ (C) ++ ("baaddcadbcbddc") ++ (C) ++ ("b") ++ (C) ++ ("dda") ++ (C) ++ ("dacbdbac") ++ (C) ++ ("daaaabdddba") ++ (C) ++ ("bddc") ++ (C) ++ ("cac") ++ (C) ++ ("abddcba") ++ (C) ++ ("ba") ++ (C) ++ ("b"))
    =
    (("cab") ++ (C) ++ ("bbaaddcad") ++ (C) ++ ("c") ++ (C) ++ ("ddcb") ++ (C) ++ (C) ++ ("ddabdacbd") ++ (C) ++ ("acbdaaaabdddba") ++ (C) ++ (C) ++ ("ddc") ++ (C) ++ ("cacbabddc") ++ (C) ++ ("a") ++ (C) ++ (C) ++ ("ab") ++ (C));
constraint
  (("aaad") ++ (C) ++ ("ad") ++ (C) ++ ("d") ++ (C) ++ ("dadbcba") ++ (C) ++ ("ddcaba") ++ (C) ++ ("abdc") ++ (C) ++ ("dc") ++ (C) ++ ("adacada") ++ (C) ++ (C) ++ ("daccc") ++ (C) ++ ("aad") ++ (C) ++ ("c") ++ (C) ++ ("d") ++ (C) ++ ("ccdbacbacaaa"))
    =
    (("aaad") ++ (C) ++ ("ad") ++ (C) ++ ("dbdad") ++ (C) ++ ("cbabddca") ++ (C) ++ ("aba") ++ (C) ++ ("dcbdcbadacadab") ++ (C) ++ ("daccc") ++ (C) ++ ("aadbcbd") ++ (C) ++ ("ccdbac") ++ (C) ++ ("acaaa"));
solve satisfy;
