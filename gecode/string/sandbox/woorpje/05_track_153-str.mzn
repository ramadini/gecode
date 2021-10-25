var string: B;
var string: E;
var string: J;
var string: C;
constraint
  (("eed") ++ (J) ++ ("bebd") ++ (C) ++ ("caacf") ++ (B) ++ ("cedbabec")) =
    (("ee") ++ (C) ++ ("f") ++ (E) ++ ("e") ++ (C) ++ ("babec"));
constraint
  (("daabcccfabe") ++ (C) ++ (C) ++ ("bcc") ++ (C) ++ ("fdbcecaffafcbabdef") ++ (C) ++ ("faae"))
    =
    ((C) ++ ("aabcccfabe") ++ (C) ++ (C) ++ ("bccdfdbcecaffafcbab") ++ (C) ++ ("efdfaae"));
constraint
  (("ceacee") ++ (C) ++ ("bff") ++ (J) ++ ("bcbdbbcdace") ++ (C) ++ ("abeafabfbea") ++ (C) ++ ("fafa"))
    = (("ceaceedbff") ++ (J) ++ ("bcbdbbcdacedabeafabfbeadfafa"));
constraint ((str_len (E)) * (14)) <= (1330);
constraint ((str_len (B)) * (10)) >= (90);
constraint ((str_len (B)) * (18)) <= (2898);
constraint ((str_len (C)) * (20)) >= (20);
constraint ((str_len (C)) * (13)) <= (39);
solve satisfy;
