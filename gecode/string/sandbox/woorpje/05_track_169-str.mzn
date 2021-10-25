var string: B;
var string: F;
var string: A;
var string: J;
constraint
  (("fbabfedbce") ++ (F) ++ ("ace") ++ (B) ++ ("dcddcabafffcdc")) =
    ((A) ++ ("bab") ++ (A) ++ ("edbce") ++ (F) ++ (J) ++ ("ace") ++ (A) ++ ("fcddcddcabafffcdc"));
constraint
  (("c") ++ (A) ++ ("eccdcacb") ++ (A) ++ ("abacacabaaaeded") ++ (A) ++ ("eecccbbbacdaf"))
    = (("cfeccdcacb") ++ (A) ++ ("abacacabaaaededfeecccbbbacdaf"));
constraint
  (("edcea") ++ (A) ++ ("da") ++ (A) ++ ("decda") ++ (A) ++ ("de") ++ (A) ++ ("aa") ++ (A) ++ ("caeddaabe") ++ (A) ++ ("aefcdebdc"))
    =
    (("edcea") ++ (A) ++ ("dafdecdafdefaafcaeddaabefae") ++ (A) ++ ("cdebdc"));
constraint ((str_len (F)) * (20)) <= (680);
constraint ((str_len (A)) * (18)) <= (270);
solve satisfy;
