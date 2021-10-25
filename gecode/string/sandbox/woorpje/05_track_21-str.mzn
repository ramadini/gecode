var string: B;
var string: I;
var string: F;
var string: G;
var string: A;
var string: D;
constraint
  (("afccfcfcabcba") ++ (I) ++ (F) ++ ("afbecbecedcd")) =
    (("af") ++ (D) ++ ("a") ++ (A) ++ ("cd"));
constraint
  (("ceacab") ++ (I) ++ ("ec") ++ (B) ++ ("cf") ++ (F) ++ ("eddedfeaebacbddc") ++ (I) ++ ("ea"))
    =
    (("ceaca") ++ (I) ++ (I) ++ ("e") ++ (G) ++ ("eae") ++ (I) ++ ("acbddcbea"));
constraint
  (("fafefefacccedabeddfedc") ++ (I) ++ ("af") ++ (I) ++ ("bffafeaeeddded"))
    =
    (("fafefefaccceda") ++ (I) ++ ("eddfedcba") ++ (A) ++ ("bffafeaeeddded"));
constraint
  (("caece") ++ (I) ++ ("baddeeadbfccf") ++ (I) ++ ("efcceccafa") ++ (I) ++ ("eedfaa") ++ (I) ++ ("fe"))
    =
    (("caece") ++ (I) ++ (I) ++ ("addeeadbfcc") ++ (A) ++ ("efcceccafabeedfaabfe"));
constraint ((str_len (G)) * (20)) >= (160);
constraint ((str_len (D)) * (15)) <= (285);
constraint ((str_len (B)) * (1)) >= (1);
constraint ((str_len (B)) * (5)) <= (100);
solve satisfy;
