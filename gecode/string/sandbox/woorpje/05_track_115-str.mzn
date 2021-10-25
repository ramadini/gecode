var string: B;
var string: F;
var string: E;
constraint
  (("decfefba") ++ (F) ++ ("fdafae") ++ (F) ++ ("afb") ++ (F) ++ (B) ++ ("bc") ++ (B) ++ ("adebbcfacefc") ++ (B))
    =
    (("de") ++ (F) ++ ("fefbacfd") ++ (E) ++ (F) ++ (B) ++ ("b") ++ (F) ++ (B) ++ ("adebbcfa") ++ (F) ++ ("efcbb"));
constraint
  (("fbdebebbacdeaafebbbdeeaacadcededafaeeafb") ++ ("")) =
    (("fbdebe") ++ (B) ++ ("a") ++ (F) ++ ("deaafe") ++ (B) ++ ("bdeeaacadcededafaeeafb"));
constraint
  (("ea") ++ (F) ++ ("dfef") ++ (F) ++ ("dbb") ++ (F) ++ ("ffceeebebcfeffdfffbacffe") ++ (F) ++ ("acf"))
    =
    (("eacdfef") ++ (F) ++ ("d") ++ (B) ++ ("cffceeebebcfeffdfffba") ++ (F) ++ ("ffeca") ++ (F) ++ ("f"));
constraint ((str_len (F)) * (14)) >= (14);
constraint ((str_len (E)) * (14)) >= (56);
solve satisfy;
