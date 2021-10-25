var string: B;
var string: F;
var string: E;
constraint
  (("dcceaeefaae") ++ (B) ++ ("eeacccfba")) =
    (("dcceaeefaaee") ++ (F) ++ ("ceebaeeacccfba"));
constraint
  (("cfaeaeccff") ++ (E) ++ ("cfcbbacddbbeefedddbcaeceb")) =
    (("cfaeaeccffededccfcbbacddbbeefedddbcaeceb") ++ (""));
constraint
  (("feffbebebfdeeeaeacdecabcae") ++ (E) ++ ("bbeafdfba")) =
    (("feffbebebfdeeeaeacdecabcaeededcbbeafdfba") ++ (""));
constraint ((str_len (B)) * (8)) >= (80);
constraint ((str_len (F)) * (20)) >= (80);
constraint ((str_len (F)) * (7)) <= (371);
solve satisfy;
