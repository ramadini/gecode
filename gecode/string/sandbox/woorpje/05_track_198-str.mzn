var string: H;
var string: F;
var string: G;
var string: A;
var string: J;
constraint
  ((H) ++ ("bcbcc") ++ (F) ++ ("acdecebccdabb")) =
    (("bf") ++ (A) ++ ("f") ++ (J) ++ ("fdacacdecebccdabb"));
constraint
  (("dbdacbdbfbdc") ++ (G) ++ ("abaccecdcbacdddabcbaacbde")) =
    (("dbdacbdbfbdc") ++ (G) ++ ("abaccecdcbacdddabcbaacbde"));
constraint
  (("fbba") ++ (J) ++ ("ebfebaeadbeccdaddbfdadaeefcfbead")) =
    (("fbba") ++ (J) ++ ("ebfebaeadbeccdaddbfdadaeefcfbead"));
constraint ((str_len (A)) * (17)) >= (68);
constraint ((str_len (A)) * (10)) <= (670);
constraint ((str_len (J)) * (20)) >= (80);
constraint ((str_len (G)) * (11)) >= (11);
constraint ((str_len (F)) * (18)) >= (108);
constraint ((str_len (F)) * (15)) <= (2340);
constraint ((str_len (H)) * (17)) >= (17);
solve satisfy;
