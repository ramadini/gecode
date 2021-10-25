var string: H;
var string: C;
var string: D;
constraint
  ((D) ++ ("beacddeedeededc") ++ (H) ++ ("fbafccfccffbecb")) =
    (("deebeacddee") ++ (D) ++ ("ded") ++ (C) ++ ("bdcaafbafccfccffbecb"));
constraint
  (("ecfaddccfceadbfcf") ++ (C) ++ ("edacfceeadebddffeffd")) =
    (("ecfaddccfceadbfcf") ++ (C) ++ ("edacfceeadebddffeffd"));
constraint
  (("bfecfbcedcfcccefbaeefbb") ++ (C) ++ ("dcfbaedebadebb")) =
    (("bfecfbcedcfcccefbaeefbb") ++ (C) ++ ("dcfbaedebadebb"));
constraint ((str_len (C)) * (6)) >= (18);
solve satisfy;
