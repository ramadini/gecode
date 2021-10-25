var string: B;
var string: G;
var string: F;
constraint
  (("af") ++ (G) ++ ("dfbb") ++ (F)) =
    (("afbffaed") ++ (B) ++ ("bf") ++ (F) ++ ("fdfbb") ++ (F));
constraint
  ((F) ++ ("bbfbee") ++ (F) ++ ("ffdebceeafdbaaccd") ++ (F) ++ ("de") ++ (F) ++ ("dabefcf") ++ (F) ++ ("fdf"))
    =
    (("cbbfbeecffdebceeafdbaac") ++ (F) ++ ("d") ++ (F) ++ ("decdabef") ++ (F) ++ ("fcfdf"));
constraint
  (("cecf") ++ (F) ++ ("fc") ++ (F) ++ ("dbfbbfca") ++ (F) ++ ("ddbefdfdebfabfb") ++ (F) ++ ("ede") ++ (F) ++ ("fae"))
    =
    (("ce") ++ (F) ++ ("f") ++ (F) ++ ("fc") ++ (F) ++ ("dbfbbf") ++ (F) ++ ("a") ++ (F) ++ ("ddbefdfdebfabfbcedecfae"));
constraint ((str_len (G)) * (13)) <= (533);
constraint ((str_len (F)) * (6)) >= (6);
solve satisfy;
