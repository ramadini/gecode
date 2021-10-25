var string: H;
var string: G;
var string: F;
var string: A;
var string: D;
constraint
  (("ab") ++ (D) ++ ("aaaabcaecb")) = (("abfaefaeeccaccdeef") ++ (G));
constraint
  (("fbcacc") ++ (F) ++ ("fabdddcefcebdcbeeffbacaedfbd")) =
    (("fbcaccaeeccafabdddcefcebdcbeeffbacaedfbd") ++ (""));
constraint
  ((A) ++ ("eceadcb") ++ (A) ++ ("a") ++ (A) ++ ("f") ++ (A) ++ ("cd") ++ (A) ++ ("cfeeaaedaddddadcfeddcaee"))
    = ((A) ++ ("eceadc") ++ (A) ++ (A) ++ (H) ++ (D) ++ ("dddadcfeddcaee"));
constraint
  (("ffdbfaccccfecdbacdfcde") ++ (A) ++ ("eecccc") ++ (A) ++ ("a") ++ (A) ++ ("ed") ++ (A) ++ ("cbece"))
    =
    (("ffdbfaccccfecd") ++ (A) ++ ("acdfcdebeeccccba") ++ (A) ++ ("ed") ++ (A) ++ ("c") ++ (A) ++ ("ece"));
constraint
  (("ecfcf") ++ (A) ++ ("aafcfcdecdbbacccdeffaf") ++ (A) ++ ("ddfecdeebda"))
    =
    (("ecfcf") ++ (A) ++ ("aafcfcdecd") ++ (A) ++ (A) ++ ("acccdeffaf") ++ (A) ++ ("ddfecdee") ++ (A) ++ ("da"));
constraint ((str_len (F)) * (1)) >= (16);
constraint ((str_len (F)) * (20)) <= (320);
constraint ((str_len (H)) * (5)) <= (100);
constraint ((str_len (G)) * (10)) >= (30);
constraint ((str_len (G)) * (14)) <= (56);
constraint ((str_len (D)) * (6)) <= (990);
constraint ((str_len (A)) * (4)) >= (4);
constraint ((str_len (A)) * (13)) <= (13);
solve satisfy;
