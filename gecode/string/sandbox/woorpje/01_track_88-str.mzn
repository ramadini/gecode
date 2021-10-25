var string: M;
var string: B;
var string: E;
constraint
  (("c") ++ (B) ++ ("bdeb") ++ (E) ++ ("cbacbabadd")) =
    (("ceeecbcbbcebdebbbbee") ++ (M) ++ ("bacbabadd"));
solve satisfy;
