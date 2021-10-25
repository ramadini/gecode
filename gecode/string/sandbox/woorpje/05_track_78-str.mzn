var string: I;
var string: C;
var string: F;
constraint
  (("c") ++ (F) ++ ("afcbfcbccccfcdebca")) = (("c") ++ (C) ++ ("a"));
constraint
  (("daefbddefaf") ++ (I) ++ ("aeecbafaecfeabffffacecdae")) =
    (("daefbddefafefbeaeecbafaecfeabffffacecdae") ++ (""));
constraint
  (("da") ++ (I) ++ ("dfbecebeacfafaccccfeedebeccbffefad")) =
    (("daefbedfbecebeacfafaccccfeedebeccbffefad") ++ (""));
constraint ((str_len (F)) * (13)) <= (3705);
constraint ((str_len (C)) * (15)) <= (2040);
constraint ((str_len (I)) * (3)) <= (57);
solve satisfy;
