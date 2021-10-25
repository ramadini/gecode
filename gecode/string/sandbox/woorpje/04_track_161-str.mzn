var string: B;
var string: G;
var string: A;
var string: D;
constraint
  (("ddeddabdeccbdedaedaadadcecaacdcdebdddedada") ++ (G) ++ ("ceecceebaebaaedcaacadbabbdbdb"))
    = (("ddeddabdeccbdedae") ++ (D) ++ ("dbdb"));
constraint
  (("ebbdaebaacbdcedcdd") ++ (D) ++ ("bbbcebacbbcadee") ++ (G) ++ ("cdddccbdca"))
    =
    (("ebbdaebaacb") ++ (B) ++ ("dcd") ++ (B) ++ ("cecbdbbbcebac") ++ (A) ++ ("bebdbdabacdeddcacdaadbebdbbeeedaededcdddccbdca"));
constraint
  (("bbddd") ++ (B) ++ ("ebddecbaceadcccdeabdbdcbdcadaecddcbecbbecaaebcacaddecbadddccdebdbcccacadeceaadeedbacdbaddba"))
    =
    (("bbddddceebddecbaceadcccdeabdbdcbdcadaecddcbecbbecaaebcacaddecbadddccdebdbcccacadeceaadeedbacdbaddba") ++ (""));
constraint
  (("dbcbeabbbadeaaaeddaaeddcabbbcedaebaadbddeaacaabaeddbacdbaabbbcceecdeabaebcedbaeccceeacccaadbbac") ++ (B) ++ ("a"))
    =
    (("dbcbeabbbadeaaaeddaaeddcabbbcedaebaadbddeaacaabaeddbacdbaabbbcceecdeabaebcedbaeccceeacccaadbbacdcea") ++ (""));
constraint
  (("ccbeedbdbcacccdebadbddbedbdeeebdabeeaacaacaccedacccae") ++ (B) ++ ("ccdceeabddaececdbcedecadeebdcbbddecaccacdaa"))
    =
    (("ccbeedbdbcacccdebadbddbedbdeeebdabeeaacaacaccedacccaedcecc") ++ (B) ++ ("eabddaececdbcedecadeebdcbbddecaccacdaa"));
constraint
  (("bcdddeeacabcbdacdbbeddadbecccabdbbdda") ++ (B) ++ ("cdcddcdbbcbaabccaacdbcecaeeaccaaeccbbadbcaccadbcecaaaacdbec"))
    =
    (("bcdddeeacabcbdacdbbeddadbecccabdbbdda") ++ (B) ++ ("cdcddcdbbcbaabccaacdbcecaeeaccaaeccbbadbcaccadbcecaaaacdbec"));
constraint
  (("cdecdeccbcddaceeedecbaadebaacddeecbeec") ++ (B) ++ ("baeaedabddbbeeaaebbcbccbdabaaaabbadaceebeadcbdeecbebde") ++ (B) ++ ("e"))
    =
    (("cdecdeccbcddaceeedecbaadebaacddeecbeec") ++ (B) ++ ("baeaedabddbbeeaaebbcbccbdabaaaabbadaceebeadcbdeecbebde") ++ (B) ++ ("e"));
constraint
  (("abedbbcecbcecabbacaadebcbabaeeccd") ++ (B) ++ ("cbededdecaccbedacacbdbba") ++ (B) ++ ("b") ++ (B) ++ ("eeabbbdacdecedcdbcbdceeabeecbcad"))
    =
    (("abedbbcecbcecabbacaadebcbabaeeccddcecbededdecaccbedacacbdbba") ++ (B) ++ ("bdceeeabbbdacdecedcdbcb") ++ (B) ++ ("eabeecbcad"));
constraint
  (("daeceacdddddbbbeaadbcbcaaaccddaecbbbdeecdaca") ++ (B) ++ ("acbdccbcabcbdadebdabeddade") ++ (B) ++ ("ddccedeaeaddbcdbcdaeaae"))
    =
    (("daeceacdddddbbbeaadbcbcaaaccddaecbbbdeecdaca") ++ (B) ++ ("acbdccbcabcbdadebdabeddadedceddccedeaeaddbcdbcdaeaae"));
constraint
  (("caadaebcdddaac") ++ (B) ++ ("eedbccdccaeeeebdecbbebceeabadcbebabdccdacccdeabdebbdbddeebeedddcaddadedaaddbeaacaa"))
    =
    (("caadaebcdddaacdceeedbccdccaeeeebdecbbebceeabadcbebabdccdacccdeabdebbdbddeebeedddcaddadedaaddbeaacaa") ++ (""));
constraint
  (("acccbbaaeececacecaadcbeeadcbdbbdeaebabcbeaedcdabccadded") ++ (B) ++ ("cacaaecdebedbbaedbdbcbdcaeeabababadbcadbd"))
    =
    (("acccbbaaeececacecaadcbeeadcbdbbdeaebabcbeaedcdabccaddeddcecacaaecdebedbbaedbdbcbdcaeeabababadbcadbd") ++ (""));
constraint
  (("cbcacccbcdaaddeabdbccaeceacaacbccbdbdbcabcddbcecaeeebab") ++ (B) ++ ("acbbaebacddccdabbdaaacebaaceccccadcaebcec"))
    =
    (("cbcacccbcdaaddeabdbccaeceacaacbccbdbdbcabcddbcecaeeebabdceacbbaebacddccdabbdaaacebaaceccccadcaebcec") ++ (""));
constraint
  (("dddaadccaeddeeaabb") ++ (B) ++ ("acbebbddcbcbeeaabbeeebcabdaeaadaddecdcacaadbdecacaabbadcaedcacaedaaddcdaabeeba"))
    =
    (("dddaadccaeddeeaabb") ++ (B) ++ ("acbebbddcbcbeeaabbeeebcabdaeaadaddecdcacaadbdecacaabbadcaedcacaedaaddcdaabeeba"));
solve satisfy;
