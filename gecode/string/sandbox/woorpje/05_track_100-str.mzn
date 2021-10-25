var string: H;
var string: B;
var string: E;
var string: A;
var string: C;
constraint
  (("dc") ++ (A) ++ ("a") ++ (E) ++ ("cdacfadbafafe")) =
    (("d") ++ (H) ++ ("e") ++ (B) ++ ("dbafafe"));
constraint
  (("afddecdebcc") ++ (E) ++ ("accffbecfcbaacfbbefdbedbeaa")) =
    (("afddecdebcccaaccffbecfcbaacfbbefdbedbeaa") ++ (""));
constraint
  (("afbfebdfccaceacea") ++ (C) ++ ("ffddbacfdcebdadbabcca")) =
    (("afbfebdfc") ++ (E) ++ ("ceaceaedffddbacfdcebdadbabc") ++ (E));
constraint ((str_len (B)) * (15)) >= (150);
constraint ((str_len (H)) * (6)) >= (54);
constraint ((str_len (H)) * (1)) <= (74);
constraint ((str_len (A)) * (20)) <= (2040);
constraint ((str_len (C)) * (8)) <= (24);
solve satisfy;
