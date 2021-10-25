var string: E;
var string: A;
var string: C;
var string: D;
constraint
  (("dabedccdaabbcbdbdacbbaebcbabbedcbbccacdecaacceccdadaccccbcbdceccc") ++ (""))
    = (("dabe") ++ (E) ++ ("bcbdceccc"));
constraint
  (("eedbaeaba") ++ (D) ++ ("beabdaecaaacbbecceceed")) =
    (("eed") ++ (C) ++ ("bcbabecbcabeabdaecaaacbbecceceed"));
constraint
  (("ebb") ++ (C) ++ ("ccbdeecddcaaeebd") ++ (D) ++ ("accddaecaebbaddbebddceebcbe"))
    =
    (("ebb") ++ (C) ++ ("ccbdeecdd") ++ (A) ++ ("bacabaccddaecaebbaddbebddceebcbe"));
constraint
  (("adcbcbbaeacadaaeaeabdecebbdebeaededbeaaeebeaabdedaddbde") ++ (C) ++ ("beabba"))
    =
    (("adcbcbbaeacadaaeaeabdecebbdebeaededbeaaeebeaabdedaddbde") ++ (C) ++ ("beabba"));
constraint
  (("debceaaddbddbaaedccddccaeadecdeaacbbcabcebddcecbcddabadbdcededdad") ++ (""))
    =
    (("debceaaddbddbaaedccddccaeadecdeaacbbc") ++ (C) ++ ("bddcecbcddabadbdcededdad"));
constraint
  (("edcaceaabbcbabedacdbabceaaaeadcbeeeadacbaecbbacaecbaeddbbccdabbcd") ++ (""))
    =
    (("edcaceaabbcbabedacdb") ++ (C) ++ ("aaaeadcbeeeadacbaecbbacaecbaeddbbccdabbcd"));
constraint
  (("bcedaccddebbaaebedecaaebeadcbaecddabaaacdaaabbaaadbcb") ++ (C) ++ ("ddaadcdc"))
    =
    (("bcedaccddebbaaebedecaaebeadcbaecddabaaacdaaabbaaadbcbabceddaadcdc") ++ (""));
constraint
  (("ceabceabababdaddbcbebeebebcececdeeeeeeaedcdbeddeaddadebbdaccdacec") ++ (""))
    =
    (("ce") ++ (C) ++ ("abababdaddbcbebeebebcececdeeeeeeaedcdbeddeaddadebbdaccdacec"));
constraint
  (("aabbcbebbaeaaeddebbdcceadaccdadeebaeedddcbebbdeabcbbeceddacbdabce") ++ (""))
    =
    (("aabbcbebbaeaaeddebbdcceadaccdadeebaeedddcbebbdeabcbbeceddacbd") ++ (C));
constraint
  (("cebdbbbbdcacddeecccbbd") ++ (C) ++ ("cacbccdbaebccaebaccebaebacbaeecccbacaee"))
    =
    (("cebdbbbbdcacddeecccbbdabcecacbccdbaebccaebaccebaebacbaeecccbacaee") ++ (""));
constraint
  (("dddedbcaaddadcdeeeeabeabaebbccadc") ++ (C) ++ ("badaccbaaecccdccccbeadbeebec"))
    =
    (("dddedbcaaddadcdeeeeabeabaebbccadc") ++ (C) ++ ("badaccbaaecccdccccbeadbeebec"));
constraint
  (("abcccbbbadbbbeedabba") ++ (C) ++ ("edecbceaedaecbaedecececcdeabebccccbacdcbb"))
    =
    (("abcccbbbadbbbeedabbaabceedecbceaedaecbaedecececcdeabebccccbacdcbb") ++ (""));
constraint
  (("ccabcbcdeaeccbccaadedbcccecee") ++ (C) ++ ("cbeebbaabdeadaedeaedaabacdcadbdc"))
    =
    (("ccabcbcdeaeccbccaadedbccceceeabcecbeebbaabdeadaedeaedaabacdcadbdc") ++ (""));
solve satisfy;
