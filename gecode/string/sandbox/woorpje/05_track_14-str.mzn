var string: H;
var string: B;
var string: E;
var string: C;
var string: D;
constraint
  (("ae") ++ (B) ++ ("f") ++ (H) ++ ("babf") ++ (E) ++ ("beddedf")) =
    (("a") ++ (D) ++ ("ecafc") ++ (H) ++ ("cfbaba") ++ (C) ++ ("fedfeadbeddedf"));
constraint
  (("aaedcbecedcdaababceabfbdabebcedfaff") ++ (H) ++ ("bfc")) =
    (("aaedcbecedcdaababceabfbdabebcedfaff") ++ (H) ++ ("bfc"));
constraint
  (("efefdafdbbdefebfacefffceadedeebfbdecdcbd") ++ ("")) =
    (("efefdafdbbdefebfacefffceaded") ++ (D) ++ ("bfbdecdcbd"));
constraint ((str_len (B)) * (5)) >= (15);
constraint ((str_len (B)) * (5)) <= (50);
constraint ((str_len (D)) * (16)) >= (16);
constraint ((str_len (D)) * (4)) <= (8);
constraint ((str_len (E)) * (7)) <= (1148);
solve satisfy;
