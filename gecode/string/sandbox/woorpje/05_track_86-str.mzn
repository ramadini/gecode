var string: H;
var string: B;
var string: D;
constraint
  (("c") ++ (H) ++ ("ebcedcbfdcbbfeabacd")) =
    (("ccdbbdcfd") ++ (D) ++ ("bbfeabacd"));
constraint
  (("afcfde") ++ (B) ++ ("cfbcffebbbdbaaab")) =
    (("afc") ++ (D) ++ ("bbbdbaaab"));
constraint
  (("badbbcddbebaabcbcaecbbefdefeaacbfdabdced") ++ ("")) =
    (("badbbcddbebaabcbcaecbbefdefeaa") ++ (H) ++ ("bdced"));
constraint
  (("caabfcccfcdafa") ++ (H) ++ ("abebeabcddcddacebbdac")) =
    (("caabfcccfcdafa") ++ (H) ++ ("abebeabcddcddacebbdac"));
constraint ((str_len (H)) * (7)) >= (14);
constraint ((str_len (H)) * (10)) <= (590);
constraint ((str_len (D)) * (13)) >= (65);
constraint ((str_len (D)) * (5)) <= (350);
constraint ((str_len (B)) * (3)) >= (39);
constraint ((str_len (B)) * (7)) <= (294);
solve satisfy;
