var string: B;
var string: D;
var string: A;
constraint
  (("cdb") ++ (D) ++ ("bbdbccabbb") ++ (D) ++ ("cb") ++ (D) ++ ("aacccaacbbdbbabccadcccab") ++ (D) ++ ("cadb") ++ (B) ++ (D) ++ ("bd") ++ (D))
    =
    (("c") ++ (D) ++ ("b") ++ (A) ++ ("acccaacbbdbbabccadcccab") ++ (D) ++ ("cadbcaacb") ++ (D) ++ ("aaababcbcbcbdbbbc") ++ (D) ++ ("bcabacdbcdaacbadbb") ++ (D) ++ ("a") ++ (D) ++ ("b") ++ (D) ++ ("d"));
constraint
  (("bca") ++ (D) ++ ("dbcba") ++ (D) ++ (D) ++ ("bbd") ++ (D) ++ ("d") ++ (D) ++ ("cccddbaacdbccb") ++ (D) ++ ("bacdacbbbcabbcbbcdcc") ++ (D) ++ ("babcabccd") ++ (D) ++ (D) ++ ("cb") ++ (D) ++ ("adaaabbcbbcdacaba") ++ (D) ++ ("bcdcbaabcb"))
    =
    (("bcad") ++ (D) ++ ("bcbaddbb") ++ (D) ++ ("dddcccd") ++ (D) ++ ("baac") ++ (D) ++ ("bccb") ++ (D) ++ ("bacdacbbbcabbcbbcdccdbabcabccd") ++ (D) ++ (D) ++ ("cbdadaaabbcbbcdacabadbcdcbaabcb"));
constraint
  (("caaaccb") ++ (D) ++ ("bacbac") ++ (D) ++ ("bc") ++ (D) ++ ("aababcca") ++ (D) ++ (D) ++ (D) ++ ("ca") ++ (D) ++ ("c") ++ (D) ++ ("abdcaad") ++ (D) ++ ("ccbabcbcaabcdaaccbccbcbcbc") ++ (D) ++ ("ccabcb") ++ (D) ++ ("cb") ++ (D) ++ (D) ++ ("aca") ++ (D) ++ (D) ++ ("bcaacdcb") ++ (D) ++ ("a"))
    =
    (("caaaccbdbacbacdbcdaababcca") ++ (D) ++ (D) ++ ("dcadc") ++ (D) ++ ("abdcaa") ++ (D) ++ ("dccbabcbcaabc") ++ (D) ++ ("aaccbccbcbcbc") ++ (D) ++ ("ccabcb") ++ (D) ++ ("cb") ++ (D) ++ (D) ++ ("acad") ++ (D) ++ ("bcaac") ++ (D) ++ ("cbda"));
constraint
  ((D) ++ ("dccc") ++ (D) ++ (D) ++ ("aa") ++ (D) ++ ("c") ++ (D) ++ (D) ++ (D) ++ ("d") ++ (D) ++ ("baabbbdab") ++ (D) ++ ("dbda") ++ (D) ++ ("d") ++ (D) ++ ("cbaaa") ++ (D) ++ ("cb") ++ (D) ++ ("dbbcbbbcaacccabcbabdbaccbbbdbcabdccadcbdcc") ++ (D) ++ (D) ++ ("bbbdbccbb"))
    =
    ((D) ++ ("dcccddaa") ++ (D) ++ ("cdddddbaabbb") ++ (D) ++ ("abd") ++ (D) ++ ("bdadddcbaaadcb") ++ (D) ++ (D) ++ ("bbcbbbcaacccabcbabdbaccbbb") ++ (D) ++ ("bcabdcca") ++ (D) ++ ("cbdcc") ++ (D) ++ ("dbbbdbccbb"));
constraint
  (("bc") ++ (D) ++ ("b") ++ (D) ++ (D) ++ ("adacbbaca") ++ (D) ++ ("bdcbaaddcbadcbcc") ++ (D) ++ ("cbbcacdb") ++ (D) ++ ("cacbbcbbcbbbcadabaa") ++ (D) ++ ("ba") ++ (D) ++ ("abb") ++ (D) ++ ("aababccba") ++ (D) ++ ("acbbb") ++ (D) ++ ("ad") ++ (D) ++ ("aabaabd"))
    =
    (("bc") ++ (D) ++ ("bddadacbbacadbdcbaad") ++ (D) ++ ("cba") ++ (D) ++ ("cbcc") ++ (D) ++ ("cbbcacdbdcacbbcbbcbbbca") ++ (D) ++ ("abaadba") ++ (D) ++ ("abb") ++ (D) ++ ("aababccba") ++ (D) ++ ("acbbb") ++ (D) ++ ("ad") ++ (D) ++ ("aabaab") ++ (D));
constraint
  ((D) ++ ("a") ++ (D) ++ ("b") ++ (D) ++ (D) ++ ("cc") ++ (D) ++ ("baaab") ++ (D) ++ ("d") ++ (D) ++ (D) ++ ("aabcbcddbacdbbbaaccbaacbcb") ++ (D) ++ ("aaaabdba") ++ (D) ++ ("cdbbccbcc") ++ (D) ++ ("cbcadc") ++ (D) ++ ("c") ++ (D) ++ ("abaaabc") ++ (D) ++ ("cbb") ++ (D) ++ ("ccdcaddadb"))
    =
    ((D) ++ ("adb") ++ (D) ++ ("dccdbaaabddd") ++ (D) ++ ("aabcbc") ++ (D) ++ (D) ++ ("bac") ++ (D) ++ ("bbbaaccbaacbcb") ++ (D) ++ ("aaaab") ++ (D) ++ ("ba") ++ (D) ++ ("cdbbccbcc") ++ (D) ++ ("cbcadcdc") ++ (D) ++ ("abaaabcdcbb") ++ (D) ++ ("cc") ++ (D) ++ ("ca") ++ (D) ++ ("dadb"));
constraint
  (("ad") ++ (D) ++ ("bcbb") ++ (D) ++ ("aacbdccacca") ++ (D) ++ ("accb") ++ (D) ++ ("bdcac") ++ (D) ++ ("cbbbbcbcbbacddcc") ++ (D) ++ ("b") ++ (D) ++ ("daaacaccacbbbaad") ++ (D) ++ ("bababdcdd") ++ (D) ++ ("ddaaaddcbcaabcbcac"))
    =
    (("addbcbbdaacbdccaccadaccbdb") ++ (D) ++ ("cac") ++ (D) ++ ("cbbbbcbcbbac") ++ (D) ++ ("dccdbddaaacaccacbbbaaddbababdcd") ++ (D) ++ (D) ++ ("ddaaaddcbcaabcbcac"));
solve satisfy;
