var string: H;
var string: B;
var string: F;
var string: J;
constraint
  (("cebabedefab") ++ (B) ++ ("afbddcdfcd")) =
    (("ceba") ++ (H) ++ ("f") ++ (J) ++ ("a") ++ (F) ++ ("cffcfafbddcdfcd"));
constraint
  (("aecdaeddbbfeddbaaefdadcaedeecfcdbbcbbb") ++ (J)) =
    (("aecdaeddbbfeddbaaefdadcaedeecfcdbbcbbb") ++ (J));
constraint
  (("ffffaaebecdffddcbbbfafefaacedf") ++ (J) ++ ("edfbfddf")) =
    (("ffffaaebecdffddcbbbfafefaacedfabedfbfddf") ++ (""));
constraint ((str_len (J)) * (2)) >= (2);
constraint ((str_len (J)) * (8)) <= (96);
constraint ((str_len (B)) * (15)) >= (75);
constraint ((str_len (B)) * (13)) <= (1885);
constraint ((str_len (H)) * (20)) <= (140);
solve satisfy;
