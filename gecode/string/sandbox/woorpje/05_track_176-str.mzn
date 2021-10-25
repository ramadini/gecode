var string: H;
var string: F;
var string: A;
var string: C;
constraint
  (("fcdeebeaaeae") ++ (A) ++ ("fcfffafafddfea")) =
    (("fcdeebea") ++ (F) ++ ("fea"));
constraint
  (("cadcadc") ++ (H) ++ (A) ++ ("abe")) =
    (("cadcadc") ++ (H) ++ ("e") ++ (C) ++ ("feabe"));
constraint
  (("febdfcadbebcaacfdfefbeeccabbdcfcdedcafdd") ++ ("")) =
    (("febdfcadbebcaacfdfefbeec") ++ (H) ++ ("dcfcdedcafdd"));
constraint
  (("afefbaabeabcdadaedbdcbaeeccabbbcceeeaeda") ++ ("")) =
    (("afefbaabeabcdadaedbdcbaeec") ++ (H) ++ ("bcceeeaeda"));
constraint ((str_len (C)) * (15)) >= (15);
constraint ((str_len (H)) * (4)) >= (16);
constraint ((str_len (A)) * (19)) <= (4389);
solve satisfy;
