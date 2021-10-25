var string: H;
var string: G;
var string: E;
var string: F;
constraint
  (("ec") ++ (H) ++ ("afdffc")) =
    (("ecfbce") ++ (E) ++ (F) ++ ("bcf") ++ (G) ++ ("afdffc"));
constraint
  (("caffabfeceffabacbba") ++ ("")) =
    (("caffabfeceffa") ++ (G) ++ ("acb") ++ (G) ++ ("a"));
constraint
  (("a") ++ (G) ++ ("aff") ++ (E) ++ ("acee") ++ (G) ++ ("df") ++ (E) ++ ("ffebe"))
    = (("abaffdaceeb") ++ (E) ++ ("f") ++ (E) ++ ("ffebe"));
constraint
  (("af") ++ (E) ++ ("befaffcceccadefd")) =
    (("afdbefaffccecca") ++ (E) ++ ("ef") ++ (E));
constraint
  (("ccb") ++ (F) ++ (G) ++ ("cdbbfd") ++ (E) ++ ("fede") ++ (E) ++ (E)) =
    (("ccbcfbcdb") ++ (G) ++ ("fddfededd"));
constraint
  (("e") ++ (G) ++ ("bfeceebf") ++ (G) ++ ("edcfccc") ++ (G)) =
    (("ebbfeceebfbed") ++ (F) ++ ("cccb"));
constraint
  ((G) ++ (E) ++ ("ff") ++ (G) ++ ("ecefceaa") ++ (G) ++ ("aecef")) =
    (("bdffbecefceaabaecef") ++ (""));
constraint
  (("beaeb") ++ (G) ++ (E) ++ ("caedcfdea") ++ (E) ++ ("fc")) =
    ((G) ++ ("eae") ++ (G) ++ (G) ++ (E) ++ ("caedcf") ++ (E) ++ ("eadfc"));
constraint
  (("daaafc") ++ (G) ++ ("bee") ++ (G) ++ ("d") ++ (G) ++ ("de") ++ (F) ++ ("aa"))
    = (("daaafc") ++ (G) ++ (G) ++ ("eeb") ++ (E) ++ ("bdecfaa"));
constraint
  (("da") ++ (E) ++ ("eadecaefecad") ++ (F) ++ ("f") ++ (E)) =
    (("da") ++ (E) ++ ("eadecaefecadcff") ++ (E));
constraint
  (("efcc") ++ (E) ++ ("adeaccaaba") ++ (E) ++ ("cfe")) =
    (("efcc") ++ (E) ++ ("a") ++ (E) ++ ("eaccaaba") ++ (E) ++ ("cfe"));
constraint
  (("eee") ++ (E) ++ ("caacdeae") ++ (G) ++ ("fef") ++ (G) ++ (G) ++ ("f")) =
    (("eee") ++ (E) ++ ("caacdeae") ++ (G) ++ ("fef") ++ (G) ++ (G) ++ ("f"));
solve satisfy;
