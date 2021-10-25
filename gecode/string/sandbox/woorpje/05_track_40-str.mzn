var string: B;
var string: D;
var string: F;
constraint
  (("bc") ++ (D) ++ ("aadfbe") ++ (B) ++ ("defacfbaeddbdaaedd")) =
    (("bcfafacecae") ++ (F) ++ ("baeddbdaaedd"));
constraint
  (("abbeabfdfdefffaf") ++ (B) ++ ("bfabbdbdfaeedfeebdbd")) =
    (("abbeabfdfdefffafadabbfabbdbdfaeedfeebdbd") ++ (""));
constraint
  (("aeaeecaedabbafbbcfebfebdedfcd") ++ (B) ++ ("bdefcdd")) =
    (("aeaeecaedabbafbbcfebfebdedfcd") ++ (B) ++ ("bdefcdd"));
constraint ((str_len (F)) * (15)) <= (960);
constraint ((str_len (B)) * (8)) >= (32);
constraint ((str_len (D)) * (8)) >= (32);
solve satisfy;
