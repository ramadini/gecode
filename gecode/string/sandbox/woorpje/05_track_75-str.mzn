var string: I;
var string: B;
var string: F;
var string: A;
constraint
  (("cdfcafdc") ++ (B) ++ (F) ++ ("ffebe") ++ (A) ++ ("accabb") ++ (B) ++ ("ec"))
    = (("cdfcafdcbf") ++ (F) ++ ("ffebe") ++ (I) ++ ("bbcfadeaccabbbfec"));
constraint
  (("cadeaedaebaecdac") ++ (B) ++ ("ceedbbdeeafbbaecaf") ++ (B) ++ ("bc")) =
    (("cadeaedaebaecdacbfceedbbdeeafbbaecafbfbc") ++ (""));
constraint
  (("cdeceeacce") ++ (B) ++ ("cdfabebcbbaecababda") ++ (B) ++ ("cffbcae")) =
    (("cdeceeacce") ++ (B) ++ ("cdfabebcbbaecababda") ++ (B) ++ ("cffbcae"));
constraint ((str_len (I)) * (3)) >= (3);
constraint ((str_len (A)) * (9)) >= (90);
constraint ((str_len (A)) * (3)) <= (498);
constraint ((str_len (F)) * (19)) >= (19);
constraint ((str_len (F)) * (1)) <= (5);
solve satisfy;
