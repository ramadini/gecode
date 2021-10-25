var string: H;
var string: E;
var string: A;
constraint
  (("fbc") ++ (E) ++ ("fcd") ++ (A) ++ ("edeedebdbedca")) =
    (("fbcbddfcdaabfbbffed") ++ (H) ++ ("bedca"));
constraint
  (("ddbaebfbaeebddbfbeffaceccdecbdddddcaaaea") ++ ("")) =
    (("ddbaebfbaeebddbfbeffaceccdec") ++ (E) ++ ("dddcaaaea"));
constraint
  (("bbadbbfcfabbbdddecbbfcdacafbcfbdedfeddef") ++ ("")) =
    (("bbadbbfcfabb") ++ (E) ++ ("decbbfcdacafbcfbdedfeddef"));
constraint ((str_len (H)) * (3)) >= (15);
constraint ((str_len (E)) * (15)) >= (30);
constraint ((str_len (E)) * (13)) <= (39);
constraint ((str_len (A)) * (19)) <= (3211);
solve satisfy;
