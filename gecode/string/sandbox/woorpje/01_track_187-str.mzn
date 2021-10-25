var string: H;
var string: C;
var string: F;
constraint
  (("icgai") ++ (H) ++ ("fh") ++ (C) ++ ("f")) =
    ((H) ++ ("cgaii") ++ (F) ++ ("hfcdc") ++ (H) ++ ("fifbcf"));
solve satisfy;
