var string: B;
var string: E;
var string: J;
var string: H;
var string: I;
var string: G;
constraint
  (("dddfbdcabb") ++ (J) ++ ("faefbeecdce")) =
    (("dddfb") ++ (B) ++ ("dfffaefbeecdce"));
constraint
  (("feccacabadffefedfebcbdcfeaaeebab") ++ (E) ++ ("ea")) =
    (("feccacabadffefedfebcbdcfeaaeebabaefcbfea") ++ (""));
constraint
  (("accbafce") ++ (E) ++ ("a") ++ (H) ++ ("f") ++ (H) ++ (E) ++ (I) ++ ("dedcbe") ++ (H) ++ ("bbd") ++ (E) ++ ("ffa") ++ (I) ++ ("de") ++ (H) ++ ("da") ++ (E) ++ ("c") ++ (I) ++ ("e") ++ (E))
    =
    (("a") ++ (G) ++ ("a") ++ (H) ++ ("f") ++ (H) ++ (E) ++ (I) ++ (I) ++ ("e") ++ (B) ++ ("ffa") ++ (I) ++ (I) ++ ("ec") ++ (I) ++ ("abcde") ++ (E));
constraint
  (("aa") ++ (H) ++ (H) ++ (I) ++ ("ec") ++ (I) ++ ("fed") ++ (I) ++ ("ecefed") ++ (H) ++ ("e") ++ (I) ++ (H) ++ ("bc") ++ (I) ++ ("af") ++ (I) ++ ("ea") ++ (H) ++ ("adafbfe") ++ (I) ++ ("a"))
    =
    (("aac") ++ (H) ++ ("de") ++ (H) ++ (I) ++ ("fed") ++ (I) ++ ("ecefe") ++ (I) ++ ("cedc") ++ (E) ++ ("c") ++ (I) ++ ("af") ++ (I) ++ ("eaca") ++ (I) ++ ("af") ++ (E) ++ ("feda"));
constraint
  (("aeaafdfaafabfeaeaeafcaffc") ++ (H) ++ ("a") ++ (E) ++ ("afda") ++ (H) ++ ("eefe") ++ (H) ++ ("fa"))
    =
    (("aeaafdfaafa") ++ (E) ++ ("feaeaeafcaff") ++ (H) ++ (H) ++ ("a") ++ (E) ++ ("afdaceefe") ++ (H) ++ ("fa"));
constraint ((str_len (G)) * (20)) >= (160);
constraint ((str_len (G)) * (18)) <= (216);
constraint ((str_len (B)) * (7)) >= (42);
constraint ((str_len (B)) * (13)) <= (2119);
constraint ((str_len (I)) * (6)) >= (6);
constraint ((str_len (H)) * (17)) >= (17);
solve satisfy;
