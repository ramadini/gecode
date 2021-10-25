var string: C;
var string: A;
constraint
  (("bcecceddbd") ++ (A) ++ ("bddb") ++ (C)) =
    (("bcecceddbd") ++ (A) ++ ("bddbacaababdcdadcebbbbdcbbebbdceedebcceedacdadbb"));
solve satisfy;
