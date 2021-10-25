var string: H;
var string: B;
var string: A;
var string: J;
constraint
  (("dceade") ++ (B) ++ ("ecfceadcecceedccabee")) =
    (("dceadebce") ++ (J) ++ (H) ++ ("ee"));
constraint
  (("abbbdffddc") ++ (A) ++ ("cdddfcacefdcbeefedefefcdef")) =
    (("abbbdffddc") ++ (A) ++ ("cdddfcacefdcbeefedefefcdef"));
constraint
  (("abfabfefeaefac") ++ (A) ++ ("afcfffabcfefafedefbace")) =
    (("abfabfefeaefacadebafcfffabcfefafedefbace") ++ (""));
constraint ((str_len (H)) * (7)) <= (1232);
constraint ((str_len (A)) * (2)) <= (28);
constraint ((str_len (B)) * (7)) >= (35);
solve satisfy;
