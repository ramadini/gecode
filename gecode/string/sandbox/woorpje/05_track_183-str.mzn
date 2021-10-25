var string: I;
var string: F;
var string: E;
var string: D;
constraint
  (("fdcefceac") ++ (E) ++ ("c") ++ (E) ++ ("ffabb") ++ (E) ++ (D) ++ ("b") ++ (I) ++ (E) ++ ("e") ++ (E) ++ (E) ++ ("bc") ++ (E) ++ ("eccebb") ++ (E))
    =
    (("fdce") ++ (F) ++ (E) ++ ("ffabb") ++ (E) ++ ("aeffcb") ++ (I) ++ (E) ++ ("e") ++ (E) ++ (E) ++ ("bc") ++ (E) ++ ("eccebbd"));
constraint
  (("ebccafbe") ++ (E) ++ (E) ++ ("bfeacc") ++ (E) ++ ("aceedbfcedcffbbffbeacff"))
    =
    (("ebccafbe") ++ (E) ++ (E) ++ ("bfeacc") ++ (E) ++ ("aceedbfce") ++ (E) ++ ("cffbbffbeacff"));
constraint
  (("faace") ++ (E) ++ ("fcbeffcabaebaceffdbaabfe") ++ (I) ++ ("dabadfdc")) =
    ((I) ++ ("ace") ++ (E) ++ ("fcbeffcabaebaceff") ++ (E) ++ ("baabfefa") ++ (E) ++ ("aba") ++ (E) ++ ("fdc"));
constraint ((str_len (E)) * (12)) <= (12);
constraint ((str_len (F)) * (5)) >= (15);
constraint ((str_len (I)) * (9)) <= (81);
constraint ((str_len (D)) * (18)) >= (36);
solve satisfy;
