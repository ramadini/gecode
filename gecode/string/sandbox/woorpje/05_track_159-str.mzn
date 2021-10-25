var string: I;
var string: B;
var string: C;
constraint
  (("effaccf") ++ (I) ++ ("fdbcfbcfbe") ++ (I) ++ ("abcebbecae") ++ (C) ++ (C) ++ ("eedea") ++ (I) ++ ("e"))
    =
    ((C) ++ ("f") ++ (I) ++ ("ccfafdbcf") ++ (B) ++ ("feede") ++ (I) ++ (I) ++ ("e"));
constraint
  (("ba") ++ (I) ++ ("bdfffdeab") ++ (I) ++ ("cfdddbbfce") ++ (I) ++ (I) ++ ("ddfccaebff") ++ (I) ++ ("fcfa"))
    =
    (("b") ++ (I) ++ ("abdfffde") ++ (I) ++ ("b") ++ (I) ++ ("cfdddbbfcea") ++ (I) ++ ("ddfcc") ++ (I) ++ ("ebffafcf") ++ (I));
constraint
  (("bcdefeeab") ++ (I) ++ ("abbcfcfebbadfdbbebbbbaccbeed") ++ (C)) =
    (("bcd") ++ (C) ++ ("eeab") ++ (I) ++ ("abbcfcfebb") ++ (I) ++ ("dfdbbebbbbaccbeed") ++ (C));
constraint ((str_len (B)) * (6)) <= (378);
constraint ((str_len (I)) * (20)) <= (40);
solve satisfy;
