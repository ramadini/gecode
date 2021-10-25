var string: E;
var string: I;
var string: H;
var string: F;
var string: G;
var string: C;
var string: D;
constraint
  (("a") ++ (F) ++ ("bccceaddefebdebee")) =
    (("aabbfffcedeae") ++ (I) ++ ("ee"));
constraint
  (("bddfdeeabf") ++ (H) ++ ("bbeda")) =
    (("bddfdeeabfb") ++ (G) ++ ("dab") ++ (I) ++ ("a"));
constraint
  (("fc") ++ (C) ++ ("badebccbabafecadacebcbdcaeebdadc")) =
    (("fc") ++ (C) ++ ("badebccbabafecadacebcbdcaeebdadc"));
constraint
  (("abfdeeefba") ++ (D) ++ ("dccf") ++ (D) ++ ("fab") ++ (H) ++ ("ffdfdbccdccafa"))
    =
    (("a") ++ (D) ++ ("fdeeef") ++ (D) ++ ("a") ++ (D) ++ ("dccf") ++ (G) ++ ("ddcdcfdffdfdbccdccafa"));
constraint
  (("adceb") ++ (D) ++ ("dbedcdcedebdffcf") ++ (D) ++ ("bdeccffabe") ++ (E) ++ ("effaa"))
    =
    (("adcebbd") ++ (D) ++ ("edcdcede") ++ (D) ++ ("dffcfbbdeccffa") ++ (D) ++ ("ef") ++ (D) ++ ("effaa"));
constraint
  (("afbeaeaeb") ++ (D) ++ ("ffdcdeacf") ++ (D) ++ ("efde") ++ (D) ++ ("cbcfcecaeacadee"))
    =
    (("a") ++ (E) ++ ("eaeae") ++ (D) ++ (D) ++ ("ffdcdeacf") ++ (D) ++ ("efdebcbcfcecaeacadee"));
constraint ((str_len (H)) * (13)) >= (13);
constraint ((str_len (C)) * (2)) >= (16);
constraint ((str_len (F)) * (20)) >= (40);
constraint ((str_len (F)) * (16)) <= (3712);
constraint ((str_len (D)) * (10)) >= (10);
solve satisfy;
