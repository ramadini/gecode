var string: I;
var string: B;
var string: G;
var string: F;
var string: D;
constraint
  (("debee") ++ (B) ++ ("bae") ++ (D) ++ ("e") ++ (B) ++ ("dbec") ++ (G) ++ (B) ++ ("ccdabefadadfaabcdf"))
    =
    (("debee") ++ (B) ++ (F) ++ (I) ++ ("e") ++ (B) ++ ("dbe") ++ (B) ++ (G) ++ (B) ++ ("ccdabefadadfaab") ++ (B) ++ ("df"));
constraint
  ((B) ++ (B) ++ ("cbeafcaafd") ++ (B) ++ ("bdcdffb") ++ (B) ++ ("afffae") ++ (F) ++ ("aaefdaedfaa"))
    =
    ((B) ++ ("c") ++ (B) ++ ("beafcaafdcbd") ++ (B) ++ ("dffb") ++ (B) ++ ("afffaebaaaefdaedfaa"));
constraint
  (("ffaebdcacbfbfeabdadadedaaaeefaf") ++ (B) ++ ("aaaeda") ++ (B) ++ ("f"))
    =
    (("ffaebdca") ++ (B) ++ ("bf") ++ (G) ++ ("eabdadadedaaaeefafcaaaedacf"));
constraint ((str_len (G)) * (20)) >= (40);
constraint ((str_len (B)) * (7)) <= (63);
constraint ((str_len (D)) * (10)) >= (10);
constraint ((str_len (I)) * (17)) >= (85);
solve satisfy;
