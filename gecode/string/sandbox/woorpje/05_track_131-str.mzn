var string: B;
var string: E;
var string: C;
constraint
  (("eb") ++ (B) ++ ("ddfad")) =
    (("ebffcabedeaaf") ++ (E) ++ ("ffdf") ++ (C) ++ ("edcdaefddfad"));
constraint
  (("cecfaadfbdfdcceeefdaceadfaeaaafdfdefccce") ++ ("")) =
    (("cecf") ++ (C) ++ ("dfbdfdcceeefdaceadfaeaaafdfdefccce"));
constraint
  (("affbcddcdadfb") ++ (C) ++ ("beeefbfefdafdabdfecdecfdb")) =
    (("affbcddcdadfbaabeeefbfefdafdabdfecdecfdb") ++ (""));
constraint ((str_len (B)) * (16)) >= (48);
constraint ((str_len (B)) * (15)) <= (1410);
constraint ((str_len (C)) * (10)) <= (40);
constraint ((str_len (E)) * (2)) <= (18);
solve satisfy;
