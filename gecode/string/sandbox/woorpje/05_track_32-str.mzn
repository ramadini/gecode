var string: H;
var string: G;
var string: A;
var string: C;
constraint
  (("bb") ++ (A) ++ ("cdacb") ++ (G) ++ (C) ++ ("dfacbcabbdfaba")) =
    (("bbb") ++ (H) ++ ("adfacbcabbdfaba"));
constraint
  (("deccdcfebdebedcadcfdfeacaaafcfcaefebfafd") ++ ("")) =
    (("deccdcfebdebedcadcfdfeacaaafcfcaefe") ++ (A) ++ ("afd"));
constraint
  (("fccafaaaddaaaacacdebfabcbcbddecbdeedeafc") ++ ("")) =
    (("fccafaaaddaaaacacde") ++ (A) ++ ("abcbcbddecbdeedeafc"));
constraint ((str_len (C)) * (19)) >= (19);
constraint ((str_len (G)) * (6)) >= (48);
constraint ((str_len (G)) * (13)) <= (1534);
solve satisfy;
