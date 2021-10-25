var string: B;
var string: E;
var string: J;
var string: I;
var string: D;
constraint
  (("d") ++ (E) ++ ("febcbdcad") ++ (I) ++ ("aaeebffbcffebca")) =
    ((B) ++ ("ebc") ++ (D) ++ ("aeebffbcffebca"));
constraint
  (("bffbfc") ++ (J) ++ ("fcedcaea") ++ (I) ++ ("bff")) =
    (("bffbf") ++ (B) ++ ("bfa") ++ (D) ++ ("acabdddedbebcaedfabff"));
constraint
  (("dbeadadacfccabcdfedfddef") ++ (B) ++ ("cbcdffdaddeeec")) =
    (("dbeadadacfccabcdfedfddef") ++ (B) ++ ("cbcdffdaddeeec"));
constraint
  (("bdbedadefaccceb") ++ (B) ++ ("eaedffeafbafdefbddabbad")) =
    (("bdbedadefa") ++ (B) ++ ("ceb") ++ (B) ++ ("eaedffeafbafdefbddabbad"));
constraint ((str_len (D)) * (4)) >= (20);
constraint ((str_len (E)) * (19)) >= (95);
constraint ((str_len (E)) * (20)) <= (380);
constraint ((str_len (B)) * (10)) <= (150);
constraint ((str_len (I)) * (2)) <= (256);
solve satisfy;
