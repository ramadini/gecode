var string: G;
var string: F;
var string: A;
var string: C;
constraint
  (("babebf") ++ (C) ++ ("efbdbbcefdb") ++ (G) ++ ("ccafdfcbae")) =
    (("babebf") ++ (C) ++ ("efbdb") ++ (A) ++ ("e") ++ (F) ++ ("cafdfcbae"));
constraint
  (("dcadbfef") ++ (A) ++ ("acedccfefeaadaedcbd") ++ (C) ++ ("acbdcecd")) =
    (("dcadbfef") ++ (A) ++ ("acedccfefeaadaedcbdeafacbdcecd"));
constraint
  (("c") ++ (A) ++ ("fdedcaedddbda") ++ (A) ++ ("eefedeebfcaeaebffffbfc")) =
    (("c") ++ (A) ++ ("fdedcaedddbdabceefedeebfcaeaebffffbfc"));
constraint ((str_len (A)) * (4)) >= (4);
constraint ((str_len (A)) * (10)) <= (360);
constraint ((str_len (G)) * (8)) >= (32);
constraint ((str_len (F)) * (20)) >= (80);
constraint ((str_len (F)) * (7)) <= (861);
solve satisfy;
