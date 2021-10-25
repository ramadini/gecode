var string: A;
var string: J;
constraint
  (("b") ++ (A) ++ ("aac") ++ (A) ++ ("abdbedefaeefafffecef") ++ (A) ++ ("b") ++ (A) ++ ("daafdafdeea"))
    = (("b") ++ (A) ++ ("aa") ++ (A) ++ (J) ++ ("fcbcdaafdafdeea"));
constraint
  (("eef") ++ (A) ++ ("cddd") ++ (A) ++ (A) ++ ("dadcfddfafbb") ++ (A) ++ ("adf") ++ (A) ++ ("c") ++ (A) ++ ("dffdddf") ++ (A) ++ (A) ++ ("ad"))
    =
    (("eef") ++ (A) ++ ("cddd") ++ (A) ++ (A) ++ ("dad") ++ (A) ++ ("fddfafbbcadf") ++ (A) ++ (A) ++ ("cdffdddf") ++ (A) ++ (A) ++ ("ad"));
constraint
  (("efebccdba") ++ (A) ++ ("fc") ++ (A) ++ ("defa") ++ (A) ++ ("ddffbbfaabadfeaef") ++ (A) ++ ("d") ++ (A) ++ ("bb"))
    =
    (("efebccdbacfccdefa") ++ (A) ++ ("ddffbbfaabadfeaef") ++ (A) ++ ("d") ++ (A) ++ ("bb"));
constraint ((str_len (J)) * (5)) >= (10);
solve satisfy;
