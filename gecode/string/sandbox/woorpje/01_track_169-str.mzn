var string: F;
var string: D;
var string: E;
var string: A;
constraint
  (("gb") ++ (D) ++ ("c") ++ (E) ++ ("h") ++ (A) ++ ("ggf")) =
    (("gbhgcegh") ++ (F) ++ ("hggf"));
solve satisfy;
