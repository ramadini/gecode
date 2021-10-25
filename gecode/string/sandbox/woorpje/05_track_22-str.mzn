var string: H;
var string: F;
var string: A;
constraint
  (("afdaaeaed") ++ (H) ++ ("dcbe")) =
    (("afdaae") ++ (F) ++ ("ccafbeefbcdccdcaeeccbdcbe"));
constraint
  (("df") ++ (A) ++ ("fddafddfbdbcbdeddfecddeeafdfcabad")) =
    (("dfbfbccfddafddfbdbcbdeddfecddeeafdfcabad") ++ (""));
constraint
  (("dcfdeeaceacfbebbebfbccbeebdbeeeaceabacfe") ++ ("")) =
    (("dcfdeeaceacfbebbe") ++ (A) ++ ("beebdbeeeaceabacfe"));
constraint ((str_len (A)) * (12)) <= (720);
constraint ((str_len (H)) * (5)) <= (660);
solve satisfy;
