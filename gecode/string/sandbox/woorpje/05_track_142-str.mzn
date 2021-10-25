var string: C;
var string: D;
var string: F;
constraint
  (("ec") ++ (D) ++ ("edcabbfddbbdeccde")) =
    (("eceddfdf") ++ (F) ++ ("bcbabdbedcabbfddbbdeccde"));
constraint
  (("facafbdbdfb") ++ (C) ++ ("cbefcaeeaffaeebdfdfddadac")) =
    (("facafbdbdfbdbcbcbefcaeeaffaeebdfdfddadac") ++ (""));
constraint
  (("cdacddeeeaecadbcbfbfbcdfcebeabfedeadcffc") ++ ("")) =
    (("cdacddeeeaeca") ++ (C) ++ ("fbfbcdfcebeabfedeadcffc"));
constraint ((str_len (D)) * (3)) >= (60);
constraint ((str_len (D)) * (2)) <= (42);
constraint ((str_len (F)) * (11)) >= (11);
solve satisfy;
