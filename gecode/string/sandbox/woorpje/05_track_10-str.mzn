var string: F;
var string: A;
constraint
  ((A) ++ ("cffcbdaa") ++ (A) ++ ("e") ++ (A) ++ (A) ++ ("fdceb") ++ (A) ++ (F) ++ ("ffdbaf") ++ (A) ++ ("e"))
    =
    ((A) ++ ("cffcbdaa") ++ (A) ++ ("e") ++ (A) ++ ("cfdceb") ++ (A) ++ ("fafe") ++ (A) ++ ("efeeefa") ++ (A) ++ ("ffdbafce"));
constraint
  (("cdbdfcdabd") ++ (A) ++ ("afaadaad") ++ (A) ++ ("aeacafddfdaebdb") ++ (A) ++ ("cbae"))
    =
    (("cdbdfcdabdcafaadaad") ++ (A) ++ ("aeacafddfdaebdb") ++ (A) ++ (A) ++ ("bae"));
constraint
  (("ababfdaeebcceaf") ++ (A) ++ ("ddb") ++ (A) ++ ("fbbfee") ++ (A) ++ ("caafceb") ++ (A) ++ ("cebeb"))
    = (("ababfdaeebcceaf") ++ (A) ++ ("ddbcfbbfeeccaafcebccebeb"));
constraint ((str_len (F)) * (16)) >= (176);
constraint ((str_len (A)) * (15)) >= (15);
constraint ((str_len (A)) * (20)) <= (40);
solve satisfy;
