var string: H;
var string: G;
var string: F;
constraint
  (("cc") ++ (H) ++ ("edcffcea") ++ (F) ++ ("dcd")) =
    (("ccdeaadcaf") ++ (G) ++ ("cffcea") ++ (F) ++ ("dcd"));
constraint
  (("debfadceedccabcfcfdfdddadbccfafedacdcfdd") ++ ("")) =
    (("debfadceedccabcfcfdfdddadbccf") ++ (G) ++ ("acdcfdd"));
constraint
  (("bbbffafedcbeabefdfbccbeabceebeeffacedfcb") ++ ("")) =
    (("bbbff") ++ (G) ++ ("cbeabefdfbccbeabceebeeffacedfcb"));
constraint ((str_len (F)) * (10)) >= (30);
constraint ((str_len (F)) * (14)) <= (784);
constraint ((str_len (H)) * (7)) >= (21);
constraint ((str_len (G)) * (16)) >= (48);
solve satisfy;
