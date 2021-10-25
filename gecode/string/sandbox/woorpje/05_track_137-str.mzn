var string: F;
var string: J;
constraint
  (("ebfddfaafcdd") ++ (J) ++ ("cef") ++ (F) ++ ("ccfcfccc")) =
    (("e") ++ (J) ++ ("fddfaafcdd") ++ (J) ++ ("cefebccdeb") ++ (J) ++ ("daecbafdccfcfccc"));
constraint
  (("bfcf") ++ (J) ++ ("bcef") ++ (J) ++ ("d") ++ (J) ++ ("caceaddaff") ++ (J) ++ ("cdfcdff") ++ (J) ++ ("adda") ++ (J) ++ ("eaba"))
    =
    (("bfcfb") ++ (J) ++ ("cefbd") ++ (J) ++ ("caceaddaffbcdfcdffbadda") ++ (J) ++ ("ea") ++ (J) ++ ("a"));
constraint
  (("cfeaaceedcc") ++ (J) ++ ("f") ++ (J) ++ (J) ++ ("cce") ++ (J) ++ (J) ++ ("fcdef") ++ (J) ++ ("adaeafacdffeee"))
    =
    (("cfeaaceedcc") ++ (J) ++ ("f") ++ (J) ++ (J) ++ ("cceb") ++ (J) ++ ("fcdef") ++ (J) ++ ("adaeafacdffeee"));
constraint ((str_len (J)) * (2)) >= (2);
constraint ((str_len (J)) * (17)) <= (68);
constraint ((str_len (F)) * (2)) >= (30);
solve satisfy;
