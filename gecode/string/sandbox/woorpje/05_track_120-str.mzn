var string: G;
var string: D;
var string: A;
constraint
  (("dfbadeebbbfadef") ++ (D) ++ (D) ++ ("c") ++ (D) ++ (G) ++ ("be") ++ (D) ++ ("eadfd") ++ (D) ++ (D) ++ ("fcfd"))
    = (("dfb") ++ (D) ++ (A) ++ ("d"));
constraint
  (("fecfbdfa") ++ (D) ++ ("cbec") ++ (D) ++ ("efeabbebeeefcaafaecdf") ++ (D) ++ ("dbfb"))
    =
    (("fecfbdf") ++ (D) ++ ("acbecaefe") ++ (D) ++ ("bbebeeefc") ++ (D) ++ ("af") ++ (D) ++ ("ecdf") ++ (D) ++ ("dbfb"));
constraint
  (("ddeacffede") ++ (D) ++ ("ee") ++ (D) ++ ("bcebbffc") ++ (D) ++ ("ddbfdabbefeeaefca"))
    =
    (("ddeacffede") ++ (D) ++ ("eeabcebbffcaddbfdabbefee") ++ (D) ++ ("efc") ++ (D));
constraint ((str_len (D)) * (2)) <= (2);
constraint ((str_len (A)) * (5)) >= (40);
constraint ((str_len (A)) * (5)) <= (765);
solve satisfy;
