var string: B;
var string: D;
var string: E;
constraint
  (("adbceabccebacceedbbbcdcaeddaaabbcdceba") ++ (B) ++ ("ebdeecabbbadabddabecaeddbad"))
    =
    (("adbceabccebacceedbbbcdcaeddaaabbcdc") ++ (E) ++ ("dabddabecaeddbad"));
constraint
  (("cdbecdbabbceaababadeeddbbacddbbcbcbabdacdaebaadccebaceabbcaccdeeecbcbceceaaaeeecd") ++ (""))
    = ((E) ++ ("d") ++ (""));
constraint
  (("babcddebebaccbbddbddc") ++ (D) ++ ("bcbdceeacdaddeecddeecbaebce") ++ (D) ++ ("caaddccbbbddeeaaabcebcbbbba"))
    =
    (("babcddebebaccbbddbddceaabcbdceeacdaddeecddeecbaebceeaacaaddccbbbddeeaaabcebcbbbba") ++ (""));
constraint
  (("ceeccbddcacdeacbcaeabacebdeceacdeaddabcbadccabc") ++ (D) ++ ("adbbeecdaabecbaeeeddecbbbbacede"))
    =
    (("ceeccbddcacdeacbcaeabacebdeceacdeaddabcbadccabc") ++ (D) ++ ("adbbeecdaabecbaeeeddecbbbbacede"));
constraint
  (("cabceebacdbbbccbccabedcddabeacaedcbcadbabecadedbbacbcccaebcbcccbcacddceceaabdeada") ++ (""))
    =
    (("cabceebacdbbbccbccabedcddabeacaedcbcadbabecadedbbacbcccaebcbcccbcacddcec") ++ (D) ++ ("bdeada"));
constraint
  (("ababbccababaeeedcbdccddbcacddeebbdecacdbbbbbeebbdcaacaaeaeddcbeeddedbbbbde") ++ (D) ++ ("dbee"))
    =
    (("ababbccababaeeedcbdccddbcacddeebbdecacdbbbbbeebbdcaacaaeaeddcbeeddedbbbbdeeaadbee") ++ (""));
constraint
  (("dcbbeccaaeeccedbedbdadccecaceeca") ++ (D) ++ ("acaabbbcdeaeadceabddadbdcacbbcedaabecbacceddeb"))
    =
    (("dcbbeccaaeeccedbedbdadccecaceecaeaaacaabbbcdeaeadceabddadbdcacbbcedaabecbacceddeb") ++ (""));
solve satisfy;
