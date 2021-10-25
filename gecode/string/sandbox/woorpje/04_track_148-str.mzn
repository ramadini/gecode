var string: B;
var string: C;
constraint
  (("dbbcbe") ++ (B) ++ ("eeac") ++ (C) ++ ("c") ++ (B) ++ ("dd")) =
    (("dbbcbe") ++ (B) ++ ("eeac") ++ (C) ++ ("cadd"));
constraint
  (("becdbc") ++ (B) ++ (B) ++ ("bbddececcedc") ++ (B) ++ ("ceecdb")) =
    (("becdbc") ++ (B) ++ ("abbddececcedcaceecdb"));
constraint
  (("cbade") ++ (B) ++ ("ceebd") ++ (B) ++ ("a") ++ (B) ++ ("debeceebabcbb"))
    =
    (("cbade") ++ (B) ++ ("ceebd") ++ (B) ++ ("a") ++ (B) ++ ("debeceeb") ++ (B) ++ ("bcbb"));
constraint
  (("beb") ++ (B) ++ (B) ++ ("ecedbaaebdebaeedbceced")) =
    (("beb") ++ (B) ++ ("aecedb") ++ (B) ++ ("aebdebaeedbceced"));
constraint
  ((B) ++ ("bbbedd") ++ (B) ++ ("eaa") ++ (B) ++ ("ddd") ++ (B) ++ ("eddebdcaecb"))
    =
    ((B) ++ ("bbbedd") ++ (B) ++ ("e") ++ (B) ++ (B) ++ (B) ++ ("dddaeddebdcaecb"));
constraint
  (("bccceedebecd") ++ (B) ++ ("d") ++ (B) ++ ("c") ++ (B) ++ ("b") ++ (B) ++ ("beeddcc") ++ (B))
    = (("bccceedebecdadac") ++ (B) ++ ("b") ++ (B) ++ ("beeddcca"));
constraint
  ((B) ++ ("cbecbec") ++ (B) ++ ("a") ++ (B) ++ ("dcaccc") ++ (B) ++ ("ecebebdbb"))
    = (("acbecbecaaadcaccc") ++ (B) ++ ("ecebebdbb"));
constraint
  (("cbccedcb") ++ (B) ++ ("aebe") ++ (B) ++ ("de") ++ (B) ++ ("dbeecbcbbd"))
    = (("cbccedcb") ++ (B) ++ ("aebe") ++ (B) ++ ("deadbeecbcbbd"));
constraint
  (("bebbedbbaea") ++ (B) ++ ("aceebecaec") ++ (B) ++ ("dca") ++ (B)) =
    (("bebbedbbae") ++ (B) ++ (B) ++ (B) ++ ("ceebec") ++ (B) ++ ("ecadcaa"));
constraint
  (("eeddccecebcbeaccecebebccd") ++ (B) ++ (B)) =
    (("eeddccecebcbeaccecebebccd") ++ (B) ++ (B));
constraint
  (("b") ++ (B) ++ ("ecbbbcde") ++ (B) ++ ("eacdbccaad") ++ (B) ++ ("aa") ++ (B) ++ ("ba"))
    =
    (("baecbbbcde") ++ (B) ++ ("eacdbcca") ++ (B) ++ ("d") ++ (B) ++ (B) ++ ("aab") ++ (B));
constraint
  (("da") ++ (B) ++ ("caedec") ++ (B) ++ ("ecb") ++ (B) ++ ("cbcdd") ++ (B) ++ ("deeacae"))
    =
    (("d") ++ (B) ++ (B) ++ ("c") ++ (B) ++ ("edec") ++ (B) ++ ("ecbacbcdd") ++ (B) ++ ("deeacae"));
constraint
  (("bbcebeedeadc") ++ (B) ++ ("bcd") ++ (B) ++ ("bccdeddccd")) =
    (("bbcebeedeadcabcdabccdeddccd") ++ (""));
constraint
  (("ccdccccbdccdbdbceeacceecc") ++ (B) ++ ("c")) =
    (("ccdccccbdccdbdbcee") ++ (B) ++ ("cceecc") ++ (B) ++ ("c"));
constraint
  (("ebeeb") ++ (B) ++ ("ebebbbaaebccaeeb") ++ (B) ++ ("dbcb")) =
    (("ebeeb") ++ (B) ++ ("ebebbb") ++ (B) ++ (B) ++ ("ebccaeeb") ++ (B) ++ ("dbcb"));
constraint
  (("dddbbba") ++ (B) ++ ("cbeb") ++ (B) ++ ("cebaedcbddbbae")) =
    (("dddbbba") ++ (B) ++ ("cbebacebaedcbddbbae"));
constraint
  (("deec") ++ (B) ++ (B) ++ ("ceabadbeecbdcecabbebc")) =
    (("deec") ++ (B) ++ (B) ++ ("ce") ++ (B) ++ ("b") ++ (B) ++ ("dbeecbdcec") ++ (B) ++ ("bbebc"));
constraint
  (("c") ++ (B) ++ ("cedeccbcb") ++ (B) ++ ("bcdedcdcd") ++ (B) ++ ("b") ++ (B) ++ ("cd") ++ (B))
    = (("cacedeccbcbabcdedcdcdab") ++ (B) ++ ("cda"));
constraint
  (("eeabdeb") ++ (B) ++ ("dbaccdccadec") ++ (B) ++ ("dcd") ++ (B) ++ ("c") ++ (B))
    = (("eeabdebadbaccdccadec") ++ (B) ++ ("dcdac") ++ (B));
constraint
  (("e") ++ (B) ++ ("ceeeccddc") ++ (B) ++ ("dddeddedcecea") ++ (B) ++ ("d"))
    =
    (("e") ++ (B) ++ ("ceeeccddc") ++ (B) ++ ("dddeddedcece") ++ (B) ++ ("ad"));
solve satisfy;
