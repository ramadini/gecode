var string: A;
constraint
  (("dcadcacbabda") ++ (A) ++ ("aa") ++ (A) ++ ("adb") ++ (A) ++ ("abcb") ++ (A) ++ ("adaa") ++ (A) ++ ("badaadd") ++ (A) ++ ("d") ++ (A) ++ ("cabdbccd"))
    =
    (("dcad") ++ (A) ++ ("acbabda") ++ (A) ++ ("aacadb") ++ (A) ++ ("abcb") ++ (A) ++ ("adaacbadaadd") ++ (A) ++ ("d") ++ (A) ++ ("cabdb") ++ (A) ++ (A) ++ ("d"));
constraint
  (("adbabddddacdaab") ++ (A) ++ ("adaacb") ++ (A) ++ ("a") ++ (A) ++ ("ba") ++ (A) ++ ("abd") ++ (A) ++ ("ddcbbdcddaabbcca"))
    =
    (("adbabdddda") ++ (A) ++ ("daabcadaa") ++ (A) ++ ("b") ++ (A) ++ ("a") ++ (A) ++ ("ba") ++ (A) ++ ("abd") ++ (A) ++ ("dd") ++ (A) ++ ("bbdcddaabbcca"));
constraint
  (("dddadc") ++ (A) ++ ("bdccbacacaacbdab") ++ (A) ++ ("bc") ++ (A) ++ ("baddcdcada") ++ (A) ++ ("ddabaadacd"))
    =
    (("dddad") ++ (A) ++ (A) ++ ("bdccbaca") ++ (A) ++ ("aa") ++ (A) ++ ("bdabcb") ++ (A) ++ ("cbaddcdcada") ++ (A) ++ ("ddabaada") ++ (A) ++ ("d"));
constraint
  ((A) ++ (A) ++ ("ddbcdbdbaa") ++ (A) ++ ("baccaddbbbcbac") ++ (A) ++ ("dcaaabaaadacdabcba") ++ (A) ++ ("d"))
    =
    ((A) ++ ("cddbcdbdbaacba") ++ (A) ++ (A) ++ ("addbbb") ++ (A) ++ ("ba") ++ (A) ++ ("cdcaaabaaada") ++ (A) ++ ("dab") ++ (A) ++ ("bacd"));
constraint
  (("bbc") ++ (A) ++ ("aadadbdcddc") ++ (A) ++ ("ad") ++ (A) ++ ("bbba") ++ (A) ++ ("bcabaadbdbabcbbdb") ++ (A) ++ (A) ++ ("cacab"))
    =
    (("bb") ++ (A) ++ ("caadadbdcdd") ++ (A) ++ ("cadcbbbacb") ++ (A) ++ ("abaadbdbabcbbdbcc") ++ (A) ++ ("acab"));
constraint
  (("dccba") ++ (A) ++ ("d") ++ (A) ++ ("bdddbaddabbdbd") ++ (A) ++ ("acabb") ++ (A) ++ ("aca") ++ (A) ++ ("dabaddacadad") ++ (A) ++ ("a") ++ (A))
    =
    (("d") ++ (A) ++ ("cbacdcbdddbaddabbdbdca") ++ (A) ++ ("abbca") ++ (A) ++ ("acdabadda") ++ (A) ++ ("adadca") ++ (A));
constraint
  (("abbacbcdbd") ++ (A) ++ (A) ++ (A) ++ (A) ++ ("cbbbdaacab") ++ (A) ++ ("aadcca") ++ (A) ++ (A) ++ ("ddc") ++ (A) ++ ("acddcddabdb"))
    =
    (("abba") ++ (A) ++ ("b") ++ (A) ++ ("dbd") ++ (A) ++ ("ccccbbbdaa") ++ (A) ++ ("abcaadc") ++ (A) ++ ("accddc") ++ (A) ++ ("acddcddabdb"));
constraint
  ((A) ++ ("abadabbad") ++ (A) ++ (A) ++ ("aadaadbbb") ++ (A) ++ ("adabcaa") ++ (A) ++ ("bb") ++ (A) ++ ("bbddaccdbcabbda"))
    =
    ((A) ++ ("abadabbadccaadaadbbb") ++ (A) ++ ("adabcaacbbcbbdda") ++ (A) ++ (A) ++ ("dbcabbda"));
solve satisfy;
