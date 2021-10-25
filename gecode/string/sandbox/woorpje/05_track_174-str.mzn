var string: I;
var string: G;
var string: A;
constraint
  (("fb") ++ (G) ++ ("eccfdb")) = (("fbafbffefbceaabf") ++ (A) ++ ("fdb"));
constraint
  (("cebabd") ++ (I) ++ ("ccb") ++ (I) ++ ("c") ++ (I) ++ ("edcffedbeabca") ++ (I) ++ ("cbcfaa") ++ (I) ++ ("fdbbcb"))
    =
    (("cebabddccb") ++ (I) ++ ("cdedcffe") ++ (I) ++ ("beabcadcbcfaa") ++ (I) ++ ("f") ++ (I) ++ ("bbcb"));
constraint
  (("cdbc") ++ (I) ++ ("bdabffbceccb") ++ (I) ++ ("bfbadbdfcedc") ++ (I) ++ ("fabf") ++ (I) ++ ("adba"))
    =
    (("c") ++ (I) ++ ("bcdb") ++ (I) ++ ("abffbceccb") ++ (I) ++ ("bfbadbdfce") ++ (I) ++ ("cdfabfda") ++ (I) ++ ("ba"));
constraint ((str_len (G)) * (12)) >= (324);
constraint ((str_len (I)) * (10)) <= (90);
solve satisfy;
