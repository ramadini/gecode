var string: H;
var string: D;
var string: E;
constraint
  (("fae") ++ (H) ++ ("ebecebeb")) =
    (("faedbacfecfbfaadcd") ++ (E) ++ ("ddcddfebecebeb"));
constraint
  (("adbecfdaeadffcadbb") ++ (D) ++ ("cbaabffeecfeffffaa")) =
    (("adbecfdaeadffcadbb") ++ (D) ++ ("cbaabffeecfeffffaa"));
constraint
  (("acbceadabc") ++ (D) ++ ("cefbdcbdbabdbbacbacdefbebb")) =
    (("acbceadabc") ++ (D) ++ ("cefbdcbdbabdbbacbacdefbebb"));
constraint ((str_len (E)) * (3)) >= (21);
constraint ((str_len (E)) * (10)) <= (750);
constraint ((str_len (H)) * (1)) >= (29);
solve satisfy;
