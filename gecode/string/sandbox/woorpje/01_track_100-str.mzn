var string: I;
var string: B;
var string: G;
var string: F;
constraint
  (("cefcdf") ++ (B) ++ ("bgcdfedb") ++ (G) ++ ("fgafb") ++ (G) ++ ("gefdgcbadf"))
    =
    ((G) ++ ("ef") ++ (I) ++ ("dcbbf") ++ (G) ++ ("f") ++ (G) ++ ("bbg") ++ (F) ++ ("gefdg") ++ (G) ++ ("badf"));
solve satisfy;
