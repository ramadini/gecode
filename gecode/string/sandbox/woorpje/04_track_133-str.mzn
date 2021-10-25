var string: C;
var string: D;
var string: A;
constraint
  (("cdbbdbbbbac") ++ (D) ++ ("bcddccbbb")) =
    (("cdbbdbbbbacbbcbdabb") ++ (C) ++ ("bbb"));
constraint
  (("aadabbbdaadcbbdbdcdbcccdac") ++ (A) ++ ("bbcbccacccadbbc")) =
    (("aadabbbdaadcbbdbdcdbcccdac") ++ (A) ++ ("bbcbccacccadbbc"));
constraint
  (("ddbddccbddccbbddbabbaaaabaaacbbbcbbaddaa") ++ (A) ++ ("c")) =
    (("ddbddccbddccbbddbabbaaaabaaacbbbcbbaddaa") ++ (A) ++ ("c"));
constraint
  (("dccbacbbaadab") ++ (A) ++ ("dabdcbacadcbdcbacbbcacbdbadc")) =
    (("dccbacbbaadababbbbadabdcbacadcbdcbacbbcacbdbadc") ++ (""));
constraint
  (("dcaaabbbbabbdbcbdabaccbaabdaacdbddbcdddcbaabcaa") ++ ("")) =
    (("dcaa") ++ (A) ++ ("bbdbcbdabaccbaabdaacdbddbcdddcbaabcaa"));
constraint
  (("ababbbbaaabbdbbabacadcbbabcccbddbbddacccbbcddcc") ++ ("")) =
    (("ab") ++ (A) ++ ("aabbdbbabacadcbbabcccbddbbddacccbbcddcc"));
constraint
  (("aaaaddddbb") ++ (A) ++ ("abacabcdbcaaaacacddcdcdccdbaadd")) =
    (("aaaaddddbb") ++ (A) ++ ("abacabcdbcaaaacacddcdcdccdbaadd"));
constraint
  (("cbcbaaddbaaaacbdccddcaccdbcbcabccdac") ++ (A) ++ ("daada")) =
    (("cbcbaaddbaaaacbdccddcaccdbcbcabccdacabbbbadaada") ++ (""));
solve satisfy;
