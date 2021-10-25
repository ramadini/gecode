var string: B;
var string: A;
constraint
  (("dd") ++ (B) ++ ("c") ++ (B) ++ ("aad")) = (("ddec") ++ (B) ++ ("aad"));
constraint (("ce") ++ (B) ++ ("ddadd")) = (("c") ++ (B) ++ ("eddadd"));
constraint
  (("abc") ++ (B) ++ ("d") ++ (B) ++ ("cc")) =
    (("abc") ++ (B) ++ ("d") ++ (B) ++ ("cc"));
constraint
  (("dec") ++ (B) ++ ("decb")) = (("d") ++ (B) ++ ("c") ++ (B) ++ ("decb"));
constraint ((B) ++ ("aeedabd") ++ ("")) = ((B) ++ ("aeedabd") ++ (""));
constraint
  (("baaea") ++ (B) ++ ("ab")) = (("baa") ++ (B) ++ ("a") ++ (B) ++ ("ab"));
constraint (("dc") ++ (B) ++ ("eacdc")) = (("dce") ++ (B) ++ ("acdc"));
constraint
  (("caa") ++ (B) ++ ("cd") ++ (B) ++ ("d")) = (("caaecd") ++ (B) ++ ("d"));
constraint
  ((B) ++ ("b") ++ (B) ++ ("abebd")) = ((B) ++ ("beab") ++ (B) ++ ("bd"));
constraint
  (("c") ++ (B) ++ (B) ++ ("add") ++ (B) ++ (B)) =
    (("c") ++ (B) ++ ("eadd") ++ (B) ++ (B));
constraint (("ceeadabd") ++ ("")) = (("c") ++ (B) ++ (B) ++ ("adabd"));
constraint (("bcd") ++ (B) ++ ("cade")) = (("bcdecade") ++ (""));
constraint (("bacbad") ++ (B) ++ ("a")) = (("bacbad") ++ (B) ++ ("a"));
constraint (("bbadbc") ++ (B) ++ ("c")) = (("bbadbcec") ++ (""));
constraint ((B) ++ ("beabeca") ++ ("")) = ((B) ++ ("beab") ++ (B) ++ ("ca"));
constraint ((B) ++ ("adadbde") ++ ("")) = ((B) ++ ("adadbde") ++ (""));
constraint (("eadaaab") ++ (B)) = ((B) ++ ("adaaabe") ++ (""));
constraint ((B) ++ ("addaccc") ++ ("")) = ((B) ++ ("addaccc") ++ (""));
constraint
  (("bd") ++ (B) ++ ("ddd") ++ (B) ++ ("b")) =
    (("bd") ++ (B) ++ ("ddd") ++ (B) ++ ("b"));
constraint ((B) ++ ("bbadc") ++ (B) ++ ("b")) = ((B) ++ ("bbadceb") ++ (""));
constraint (("a") ++ (B) ++ ("cdabcb")) = (("aecdabcb") ++ (""));
constraint (("adcbe") ++ (B) ++ ("ce")) = (("adcb") ++ (B) ++ (B) ++ ("ce"));
constraint (("cdaaaecc") ++ ("")) = (("cdaaa") ++ (B) ++ ("cc"));
constraint (("abaaa") ++ (B) ++ ("db")) = (("abaaa") ++ (B) ++ ("db"));
constraint (("c") ++ (B) ++ ("acbcc") ++ (B)) = ((A) ++ ("cbcc") ++ (B));
constraint (("adbbceda") ++ ("")) = (("adbbc") ++ (B) ++ ("da"));
constraint (("bacceeac") ++ ("")) = (("bacce") ++ (B) ++ ("ac"));
constraint (("abadbc") ++ (B) ++ ("b")) = (("abadbceb") ++ (""));
constraint (("ecaedcba") ++ ("")) = (("eca") ++ (B) ++ ("dcba"));
solve satisfy;
