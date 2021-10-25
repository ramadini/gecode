var string: B;
var string: E;
constraint (("dbd") ++ (B) ++ ("dabb")) = (("d") ++ (E));
constraint (("beccbade") ++ ("")) = (("beccb") ++ (B) ++ ("de"));
constraint (("ebb") ++ (B) ++ ("cbe") ++ (B)) = (("ebbacbea") ++ (""));
constraint
  (("edbb") ++ (B) ++ ("bd") ++ (B)) = (("edbb") ++ (B) ++ ("bd") ++ (B));
constraint
  (("c") ++ (B) ++ ("daaae") ++ (B)) =
    (("c") ++ (B) ++ ("d") ++ (B) ++ (B) ++ (B) ++ ("ea"));
constraint (("dee") ++ (B) ++ ("ebeb")) = (("deeaebeb") ++ (""));
constraint (("eeede") ++ (B) ++ (B) ++ ("a")) = (("eeedeaa") ++ (B));
constraint ((B) ++ ("cdbcdda") ++ ("")) = ((B) ++ ("cdbcdd") ++ (B));
constraint
  (("b") ++ (B) ++ ("abeb") ++ (B) ++ ("e")) =
    (("b") ++ (B) ++ ("abeb") ++ (B) ++ ("e"));
constraint (("bcbc") ++ (B) ++ ("cbe")) = (("bcbc") ++ (B) ++ ("cbe"));
constraint
  (("edcba") ++ (B) ++ ("ab")) = (("edcb") ++ (B) ++ (B) ++ (B) ++ ("b"));
constraint (("bceeacbc") ++ ("")) = (("bcee") ++ (B) ++ ("cbc"));
constraint (("eaa") ++ (B) ++ ("ddab")) = (("e") ++ (B) ++ ("aaddab"));
constraint (("cba") ++ (B) ++ ("bcbb")) = (("cb") ++ (B) ++ (B) ++ ("bcbb"));
constraint (("dcda") ++ (B) ++ ("ccd")) = (("dcd") ++ (B) ++ (B) ++ ("ccd"));
constraint (("edbed") ++ (B) ++ ("ac")) = (("edbeda") ++ (B) ++ ("c"));
constraint
  (("bb") ++ (B) ++ ("eed") ++ (B) ++ (B)) =
    (("bb") ++ (B) ++ ("eed") ++ (B) ++ (B));
constraint (("eeea") ++ (B) ++ ("bce")) = (("eeea") ++ (B) ++ ("bce"));
constraint (("ccabe") ++ (B) ++ ("cc")) = (("ccabe") ++ (B) ++ ("cc"));
constraint (("ccedccb") ++ (B)) = (("ccedccba") ++ (""));
constraint ((B) ++ ("eaabedb") ++ ("")) = ((B) ++ ("eaabedb") ++ (""));
constraint (("bcccdcc") ++ (B)) = (("bcccdcc") ++ (B));
constraint (("bee") ++ (B) ++ ("bcc") ++ (B)) = (("beeabcc") ++ (B));
constraint (("beabbedd") ++ ("")) = (("be") ++ (B) ++ ("bbedd"));
constraint (("baeeecdc") ++ ("")) = (("b") ++ (B) ++ ("eeecdc"));
constraint (("dcd") ++ (B) ++ ("ecbd")) = (("dcdaecbd") ++ (""));
constraint ((B) ++ ("ccdebae") ++ ("")) = ((B) ++ ("ccdeb") ++ (B) ++ ("e"));
constraint (("ccbbeec") ++ (B)) = (("ccbbeeca") ++ (""));
constraint
  (("dacea") ++ (B) ++ ("cb")) = (("d") ++ (B) ++ ("ce") ++ (B) ++ ("acb"));
constraint
  (("ccabaedd") ++ ("")) = (("cc") ++ (B) ++ ("b") ++ (B) ++ ("edd"));
constraint (("acccbcee") ++ ("")) = ((B) ++ ("cccbcee") ++ (""));
solve satisfy;
