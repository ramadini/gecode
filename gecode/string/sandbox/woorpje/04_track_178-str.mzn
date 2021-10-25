var string: B;
var string: F;
var string: C;
constraint
  (("eebedfb") ++ (B) ++ ("acdceeffc")) =
    (("eebedfbdcaa") ++ (F) ++ ("effc"));
constraint
  (("eafabcebdfacddbfffafedbe") ++ ("")) =
    (("eafabce") ++ (C) ++ ("dbfffafedbe"));
constraint
  (("bbbdddbbbbeb") ++ (B) ++ ("eacebebfdc")) = (("b") ++ (C) ++ ("dc"));
constraint
  (("cfbbcdcdccadeba") ++ (F) ++ ("aeadbeb")) =
    (("cfbbcdcdccadeba") ++ (F) ++ ("aead") ++ (F) ++ ("b"));
constraint
  (("accbfba") ++ (F) ++ ("cccbdcfabadcfcc")) =
    (("accbfba") ++ (F) ++ ("cccbdcfabadcfcc"));
constraint
  (("dfccafcbbfafbab") ++ (F) ++ ("a") ++ (F) ++ ("bbde")) =
    (("dfccafcbbfafbabbea") ++ (F) ++ ("bbde"));
constraint
  (("effef") ++ (B) ++ ("ccffbcdbccbbbbedc")) =
    (("effefeeccffbcdbccbbbbedc") ++ (""));
constraint
  (("dca") ++ (B) ++ ("aaaecfdddfbcdebaccc")) =
    (("dcaeeaaaecfdddfbcdebaccc") ++ (""));
constraint
  (("ffbfaedbccbbacbcd") ++ (B) ++ ("bbfed")) =
    (("ffbfaedbccbbacbcd") ++ (B) ++ ("bbfed"));
constraint
  (("dddacafcfecdeefddaaffeda") ++ ("")) =
    (("dddacafcfecd") ++ (B) ++ ("fddaaffeda"));
constraint
  (("cc") ++ (F) ++ ("aabdbeadefecefbeabca")) =
    (("cc") ++ (F) ++ ("aabd") ++ (F) ++ ("adefecef") ++ (F) ++ ("abca"));
constraint
  (("baeacdfbfbeabcbaaccbdbfd") ++ ("")) =
    (("baeacdfbf") ++ (F) ++ ("abcbaaccbdbfd"));
solve satisfy;
