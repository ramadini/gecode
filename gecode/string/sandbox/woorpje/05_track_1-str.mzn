var string: H;
var string: B;
var string: A;
constraint
  (("aabbeefcbbfcdebbd") ++ (B) ++ ("efcacdcd")) =
    (("aabbeefc") ++ (H) ++ ("cac") ++ (A) ++ ("d"));
constraint
  (("affdaadccecabfaaddcdeecfdebcedaaabdbabca") ++ ("")) =
    (("affdaa") ++ (A) ++ ("cecabfaaddcdeecfdebcedaaabdbabca"));
constraint
  (("da") ++ (A) ++ ("aaffafffacceccacedefdeafcffdbafbcfaa")) =
    (("da") ++ (A) ++ ("aaffafffacceccacedefdeafcffdbafbcfaa"));
constraint ((str_len (A)) * (14)) <= (70);
constraint ((str_len (H)) * (14)) <= (364);
constraint ((str_len (B)) * (12)) <= (1404);
solve satisfy;
