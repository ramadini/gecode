var string: I;
var string: B;
var string: G;
var string: E;
var string: A;
constraint
  (("bdfbade") ++ (A) ++ ("edfccedbebacc")) =
    (("bd") ++ (G) ++ ("fc") ++ (E) ++ ("ccedbebacc"));
constraint
  (("cebfcdefdbaffffefdcbbaeeecacfebfcddebcdb") ++ ("")) =
    (("cebfcdefdbaffffefdcbb") ++ (I) ++ (B) ++ ("ebfcddebcdb"));
constraint
  (("abdefcaaccdbfffdfccabecdf") ++ (B) ++ ("bababfbcfcfe")) =
    (("abdefcaaccdbfffdfccabecdf") ++ (B) ++ ("bababfbcfcfe"));
constraint ((str_len (G)) * (10)) <= (270);
constraint ((str_len (I)) * (12)) <= (252);
constraint ((str_len (A)) * (13)) <= (1898);
constraint ((str_len (B)) * (11)) <= (154);
solve satisfy;
