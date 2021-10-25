var string: B;
var string: G;
var string: F;
var string: E;
var string: C;
var string: D;
constraint
  (("a") ++ (D) ++ ("bcce") ++ (B) ++ ("acfabcdf")) =
    (("addcdfcdcd") ++ (G) ++ ("cdcaafacfabcdf"));
constraint
  (("ba") ++ (F) ++ ("fdebdaefcbfeacbdecb")) =
    (("baebdadb") ++ (E) ++ ("decb"));
constraint
  (("ccddacdbcdcdbabdfdddffebafaeedccf") ++ ("")) =
    (("ccddacdbcdcdba") ++ (C) ++ ("ddffebafaeedccf"));
constraint
  (("bcedcadebebbdcbcef") ++ (C) ++ ("bbcffaccebb")) =
    (("bcedcadebebbdcbcef") ++ (C) ++ ("bbcffaccebb"));
constraint
  (("aa") ++ (C) ++ ("beddbcfbbfdfaabbeeccdfadbbb")) =
    (("aa") ++ (C) ++ ("beddbcfbbfdfaabbeeccdfadbbb"));
constraint
  (("afeaffaaeaffbecfcddfebdf") ++ (C) ++ ("dabaf")) =
    (("afeaffaaeaffbecfcddfebdf") ++ (C) ++ ("dabaf"));
constraint
  (("ddaabccdaac") ++ (C) ++ ("afdacafcbfefaafafd")) =
    (("ddaabccdaac") ++ (C) ++ ("afdacafcbfefaafafd"));
constraint
  (("bbdbbefbbeeebdabbaceacaae") ++ (C) ++ ("ebed")) =
    (("bbdbbefbbeeebdabbaceacaaebdfdebed") ++ (""));
constraint
  (("bcacdadffbecdeaddadbdfbe") ++ (B) ++ ("ddfcb")) =
    (("bcacdadffbecdeaddadbdfbeebdaddfcb") ++ (""));
constraint
  (("defbfdacacaafafcbcdeadc") ++ (B) ++ ("ccedcf")) =
    (("defbfdacacaafafcbcdeadcebdaccedcf") ++ (""));
constraint
  (("acceafcadfbfbadaf") ++ (B) ++ ("cccdcaadabdb")) =
    (("acceafcadfbfbadafebdacccdcaadabdb") ++ (""));
constraint
  (("bdabdcdfeadafbdfdcdbbfddbafcefabe") ++ ("")) =
    (("bdabdcdfeadaf") ++ (C) ++ ("cdbbfddbafcefabe"));
constraint
  (("fbfdfbaeb") ++ (B) ++ ("dbaaebfdbaedfbbfcbff")) =
    (("fbfdfbaebebdadbaaebfdbaedfbbfcbff") ++ (""));
constraint
  (("ddebafaedfbaedaaedabcbafab") ++ (C) ++ ("adc")) =
    (("ddebafaedfbaedaaedabcbafabbdfdadc") ++ (""));
constraint
  (("beccfeafbfbbceacaacaccabbea") ++ (C) ++ ("bf")) =
    (("beccfeafbfbbceacaacaccabbeabdfdbf") ++ (""));
constraint
  ((B) ++ ("dafcebbdebaebcbdbecebfeeaddbc") ++ ("")) =
    ((B) ++ ("dafcebbdebaebcbdbecebfeeaddbc") ++ (""));
constraint
  (("afddbeeefaaecbbbabdebfdbdfddeeddf") ++ ("")) =
    (("afddbeeefaaecbbbabdebfd") ++ (C) ++ ("deeddf"));
constraint
  (("e") ++ (B) ++ ("abefbafaafacadbfecdbecefeeae")) =
    (("eebdaabefbafaafacadbfecdbecefeeae") ++ (""));
constraint
  (("aeebdafaffbfbbdbfbccaacbbbfffebee") ++ ("")) =
    (("ae") ++ (B) ++ ("faffbfbbdbfbccaacbbbfffebee"));
constraint
  (("aedefabedadfaffaab") ++ (C) ++ ("adcbabfefae")) =
    (("aedefabedadfaffaabbdfdadcbabfefae") ++ (""));
constraint
  (("e") ++ (B) ++ ("dbdacdebefbbcbdedeffacffbbab")) =
    (("e") ++ (B) ++ ("dbdacdebefbbcbdedeffacffbbab"));
constraint
  (("babeaeadfdcdadcaeacbcd") ++ (B) ++ ("eccaacf")) =
    (("babeaeadfdcdadcaeacbcd") ++ (B) ++ ("eccaacf"));
constraint
  ((C) ++ ("ececdffecabeafeeddfdbffabcdec") ++ ("")) =
    ((C) ++ ("ececdffecabeafeeddfdbffabcdec") ++ (""));
constraint
  (("bfbadbefabafcec") ++ (C) ++ ("bacecaaffacaeb")) =
    (("bfbadbefabafcec") ++ (C) ++ ("bacecaaffacaeb"));
constraint
  (("cbcfaffecfffbeacbfdacbbbfeddc") ++ (B)) =
    (("cbcfaffecfffbeacbfdacbbbfeddcebda") ++ (""));
constraint
  (("decbcfbecbeaafedfdbcaa") ++ (C) ++ ("eabacfc")) =
    (("decbcfbecbeaafedfdbcaa") ++ (C) ++ ("eabacfc"));
constraint
  (("bafdaedfaefefafdaecdeabeaeb") ++ (C) ++ ("cf")) =
    (("bafdaedfaefefafdaecdeabeaeb") ++ (C) ++ ("cf"));
solve satisfy;
