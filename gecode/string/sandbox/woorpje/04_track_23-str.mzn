var string: B;
var string: A;
var string: C;
constraint
  (("aadaed") ++ (B) ++ ("eaebcb") ++ (C) ++ ("eabeedebcdca")) =
    (("aadaedabaeeacdea") ++ (A) ++ ("beedebcdca"));
constraint
  (("becadca") ++ (C) ++ ("ebba")) =
    (("becadcabcaecccdbbecda") ++ (B) ++ ("dcdbdeccececebba"));
constraint
  (("cbcdeedaaccdcbcacddadbaaedceeddacabcaecaecb") ++ ("")) =
    (("cbcdeedaaccdcbcacddadbaaedceeddac") ++ (A) ++ ("ecaecb"));
constraint
  (("dbbebaaadcbbbedbdeecebdadecebccaeeeabcaccba") ++ ("")) =
    (("dbbebaaadcbbbedbdeecebdadecebccaeee") ++ (A) ++ ("ccba"));
constraint
  (("abaabebcddbcbbeeddceadcddbccabcabdeeeedaaed") ++ ("")) =
    (("abaabebcddbcbbeeddceadcddbcc") ++ (A) ++ ("bdeeeedaaed"));
constraint
  (("aabeeecbcedcaddcbacadcecbeaababcaabaecaabbc") ++ ("")) =
    (("aabeeecbcedcaddcbacadcecbeaab") ++ (A) ++ ("abaecaabbc"));
constraint
  (("deadabeeaceeccadebceedbddd") ++ (A) ++ ("daabcedecdecb")) =
    (("deadabeeaceeccadebceedbddd") ++ (A) ++ ("daabcedecdecb"));
constraint
  (("caddbadbbeacaddbdbecbbedab") ++ (A) ++ ("ebbcbbaecddbb")) =
    (("caddbadbbeacaddbdbecbbedab") ++ (A) ++ ("ebbcbbaecddbb"));
constraint
  (("babaeadadadbeecdeddabcabadeaae") ++ (A) ++ ("cddbaebeb")) =
    (("babaeadadadbeecdedd") ++ (A) ++ ("badeaae") ++ (A) ++ ("cddbaebeb"));
constraint
  (("bcbddbcadceaabcadacbaccddaaaeedbcecbdbdccbe") ++ ("")) =
    (("bcbddbcadcea") ++ (A) ++ ("dacbaccddaaaeedbcecbdbdccbe"));
constraint
  (("bbcbbdbabcaeeddcdedacddddebcbaadebeadbcdaae") ++ ("")) =
    (("bbcbbdb") ++ (A) ++ ("eeddcdedacddddebcbaadebeadbcdaae"));
constraint
  (("dbbeaacbbdecdaeabcaddaabcaebcaccabbeddbebbb") ++ ("")) =
    (("dbbeaacbbdecdae") ++ (A) ++ ("dda") ++ (A) ++ ("ebcaccabbeddbebbb"));
constraint
  (("cdadebaaaebecdecdbcadbdbaecbdeddbaadce") ++ (A) ++ ("c")) =
    (("cdadebaaaebecdecdbcadbdbaecbdeddbaadce") ++ (A) ++ ("c"));
constraint
  (("adcabcaceaeeacdcbbebddacccbbebdbbaebeeccbbb") ++ ("")) =
    (("adc") ++ (A) ++ ("ceaeeacdcbbebddacccbbebdbbaebeeccbbb"));
constraint
  (("bdcdddccdceabccaedbaabcaaeacacdeacececdebce") ++ ("")) =
    (("bdcdddccdceabccaedba") ++ (A) ++ ("aeacacdeacececdebce"));
constraint
  (("baaecabbebededdbceeebdbdedac") ++ (A) ++ ("eaceecbcaeb")) =
    (("baaecabbebededdbceeebdbdedac") ++ (A) ++ ("eaceecbcaeb"));
constraint
  (("aadeaebaaadded") ++ (A) ++ ("ecedacebcecdbbbbeeddcdece")) =
    (("aadeaebaaaddedabcaecedacebcecdbbbbeeddcdece") ++ (""));
constraint
  (("cdbeddeceeaaaeeabcaaaabaaedaeccbdaaecbaebec") ++ ("")) =
    (("cdbeddeceeaaaee") ++ (A) ++ ("aaabaaedaeccbdaaecbaebec"));
constraint
  ((A) ++ ("edddedcddeecddbdecdebdaebeaaacaaadaeeab") ++ ("")) =
    ((A) ++ ("edddedcddeecddbdecdebdaebeaaacaaadaeeab") ++ (""));
solve satisfy;
