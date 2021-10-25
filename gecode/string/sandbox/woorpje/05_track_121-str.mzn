var string: B;
var string: A;
var string: J;
constraint
  (("dfbffadb") ++ (B) ++ ("ebdcaedadabdabeb")) =
    (("dfbffadbfdfebf") ++ (J) ++ ("beb"));
constraint
  (("fabfddfdbdedfaeaeccecfadebc") ++ (A) ++ ("caeccedeadd")) =
    (("fabfddfdbdedfaeaeccecfadebccdcaeccedeadd") ++ (""));
constraint
  (("abbffebccbaaa") ++ (A) ++ ("babbadacebfeeedfdceaeadfa")) =
    (("abbffebccbaaa") ++ (A) ++ ("babbadacebfeeedfdceaeadfa"));
constraint ((str_len (A)) * (14)) >= (14);
constraint ((str_len (A)) * (3)) <= (15);
constraint ((str_len (J)) * (3)) >= (30);
constraint ((str_len (B)) * (14)) >= (14);
solve satisfy;
