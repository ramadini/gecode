var string: H;
var string: E;
var string: C;
var string: D;
constraint (("fedecccbfbfc") ++ (C) ++ ("aaaddbfec")) = (("fedecc") ++ (D));
constraint
  (("fccacffacaeceefab") ++ (H) ++ ("fdffeafedaffdceeadded")) =
    (("fccacffacaeceefabcbfdffeafedaffdceeadded") ++ (""));
constraint
  (("fbbfeaadf") ++ (E) ++ ("dfdea") ++ (H) ++ ("bdababbedbbbecfecceaf")) =
    (("fbbfeaadf") ++ (E) ++ ("dfdeacbbdababbedbbbecfecceaf"));
constraint ((str_len (H)) * (12)) <= (384);
constraint ((str_len (C)) * (14)) >= (168);
constraint ((str_len (C)) * (16)) <= (3920);
constraint ((str_len (D)) * (20)) >= (660);
constraint ((str_len (E)) * (11)) >= (33);
constraint ((str_len (E)) * (19)) <= (209);
solve satisfy;
